#include "common/assert.h"
#include "testscreens/colours.h"
#include "testscreens/ramp.h"
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/spirit/home/qi.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
template <typename Iterator>
struct ParametersGrammar
    : boost::spirit::qi::grammar<Iterator, std::vector<uint16_t>,
                                 boost::spirit::ascii::space_type> {
  ParametersGrammar() : ParametersGrammar::base_type(params_) {
    using boost::spirit::qi::eoi;
    using boost::spirit::qi::lit;
    using boost::spirit::qi::char_;
    using boost::spirit::lexeme;
    number_ = lexeme[(lit("0X") | lit("0x")) >>
                     boost::spirit::qi::uint_parser<uint16_t, 16>{}] |
              boost::spirit::qi::uint_parser<uint16_t>{};
    params_ = boost::spirit::qi::repeat(4, 6)[number_] >> eoi;
  }

private:
  boost::spirit::qi::rule<Iterator, uint16_t, boost::spirit::ascii::space_type>
      number_;
  boost::spirit::qi::rule<Iterator, std::vector<uint16_t>,
                          boost::spirit::ascii::space_type>
      params_;
};

std::vector<uint16_t> parse(const std::string &source) {
  using iterator_type = std::string::const_iterator;
  static ParametersGrammar<iterator_type> grammar{};
  auto begin = std::begin(source);
  auto end = std::end(source);

  std::vector<uint16_t> parsingResult;
  using boost::spirit::ascii::space;
  auto success = boost::spirit::qi::phrase_parse(begin, end, grammar, space,
                                                 parsingResult);

  if (success) {
    return parsingResult;
  }
  std::stringstream error;
  error << R"(Invalid parameters: ")" << source << '"';
  throw std::runtime_error{error.str()};
}

void help() {
  std::cout << "Usage: truecolor width height tl tr [bl] [br]" << std::endl;
}
} // namespace

int main(int ac, const char *av[]) {
  {
    using namespace Assert;
    Handler::setHandler(Handler::availableHandlers::abort);
  }

  try {
    std::stringstream parameters;
    for (auto i = 1; i < ac; ++i) {
      parameters << av[i] << ' ';
    }
    const auto parsedParameters = parse(parameters.str());
    assert_normal((parsedParameters.size() >= 4) &&
                  (parsedParameters.size() <= 6));
    const auto tl = TestScreens::RGB565{parsedParameters[2]};
    const auto tr = TestScreens::RGB565{parsedParameters[3]};
    const auto bl =
        TestScreens::RGB565{parsedParameters.size() > 4 ? parsedParameters[4]
                                                        : parsedParameters[2]};
    const auto br =
        TestScreens::RGB565{parsedParameters.size() > 5 ? parsedParameters[5]
                                                        : parsedParameters[3]};

    const auto size =
        std::pair<uint16_t, uint16_t>{parsedParameters[0], parsedParameters[1]};

    for (auto y = 0u; y < size.second; ++y) {
      const auto l = TestScreens::ramp(tl, bl, size.second, y);
      const auto r = TestScreens::ramp(tr, br, size.second, y);
      for (auto x = 0u; x < size.first; ++x) {
        std::cout << TestScreens::ramp(l, r, size.first, x);
      }
      std::cout << TestScreens::RGB565(0x0) << std::endl;
    }

    return 0;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    help();
  } catch (std::exception &e) {
    std::cout << "An unhandled error occurred:\n\t" << e.what()
              << "\nterminating" << std::endl;
  }
  return 1;
}
