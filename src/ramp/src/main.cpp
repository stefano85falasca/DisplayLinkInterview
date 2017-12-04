#include "common/assert.h"
#include "display/display.h"
#include "testscreens/colours.h"
#include "testscreens/ramp.h"
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/spirit/home/qi.hpp>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

namespace {
template <typename Iterator>
struct ParametersGrammar
    : boost::spirit::qi::grammar<Iterator,
                                 std::tuple<std::string, std::vector<uint16_t>>,
                                 boost::spirit::ascii::space_type> {
  ParametersGrammar() : ParametersGrammar::base_type(params_) {
    using boost::spirit::qi::eoi;
    using boost::spirit::qi::lit;
    using boost::spirit::qi::char_;
    using boost::spirit::lexeme;
    number_ = lexeme[(lit("0X") | lit("0x")) >>
                     boost::spirit::qi::uint_parser<uint16_t, 16>{}] |
              boost::spirit::qi::uint_parser<uint16_t>{};
    corners_ = boost::spirit::qi::repeat(2, 4)[number_];
    params_ = lexeme[+(char_ - boost::spirit::ascii::space_type{})] >>
              corners_ >> eoi;
  }

private:
  boost::spirit::qi::rule<Iterator, uint16_t, boost::spirit::ascii::space_type>
      number_;
  boost::spirit::qi::rule<Iterator, std::vector<uint16_t>,
                          boost::spirit::ascii::space_type>
      corners_;
  boost::spirit::qi::rule<Iterator,
                          std::tuple<std::string, std::vector<uint16_t>>,
                          boost::spirit::ascii::space_type>
      params_;
};

std::tuple<std::string, std::vector<uint16_t>>
parse(const std::string &source) {
  using iterator_type = std::string::const_iterator;
  static ParametersGrammar<iterator_type> grammar{};
  auto begin = std::begin(source);
  auto end = std::end(source);

  std::tuple<std::string, std::vector<uint16_t>> parsingResult;
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

void help() { std::cout << "Usage: ramp display tl tr [bl] [br]" << std::endl; }
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
    const auto &corners = std::get<1>(parsedParameters);
    assert_normal((corners.size() >= 2) && (corners.size() <= 4));
    const auto tl = TestScreens::RGB565{corners[0]};
    const auto tr = TestScreens::RGB565{corners[1]};
    const auto bl =
        TestScreens::RGB565{corners.size() > 2 ? corners[2] : corners[0]};
    const auto br =
        TestScreens::RGB565{corners.size() > 3 ? corners[3] : corners[1]};

    auto display = Display{};
    const auto &displayName = std::get<0>(parsedParameters);
    if (!display.connect(displayName.c_str())) {
      std::stringstream ss;
      ss << R"(Cannot connect to the display named ")" << displayName << '"';
      throw std::runtime_error{ss.str()};
    }
    const auto size = [&display]() -> std::pair<int, int> {
      int width = 0;
      int height = 0;
      display.get_size(width, height);
      assert_opt(width > 0);
      assert_opt(height > 0);
      return {width, height};
    }();

    for (auto y = 0; y < size.second; ++y) {
      const auto l = TestScreens::ramp(tl, bl, size.second, y);
      const auto r = TestScreens::ramp(tr, br, size.second, y);
      for (auto x = 0; x < size.first; ++x) {
        const auto colour = TestScreens::ramp(l, r, size.first, x).value;
        display.draw_raster(x, y, &colour, 1);
        // std::cout << TestScreens::ramp(l, r, size.first, x);
      }
      // std::cout << TestScreens::RGB565(0x0) << std::endl;
    }

    return 1;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    help();
  } catch (std::exception &e) {
    std::cout << "An unhandled error occurred:\n\t" << e.what()
              << "\nterminating" << std::endl;
  }
  return 0;
}
