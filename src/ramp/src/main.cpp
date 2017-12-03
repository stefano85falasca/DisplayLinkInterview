#include "common/assert.h"
#include "display/display.h"
#include "testscreens/colours.h"
#include "testscreens/testscreens.h"
#include <boost/spirit/home/qi.hpp>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

template <typename Iterator>
struct ParametersGrammar
    : boost::spirit::qi::grammar<Iterator, std::vector<uint16_t>,
                                 boost::spirit::ascii::space_type> {
  ParametersGrammar() : ParametersGrammar::base_type(params_) {
    using boost::spirit::qi::eoi;
    using boost::spirit::qi::lit;
    using boost::spirit::lexeme;
    number_ = lexeme[(lit("0X") | lit("0x")) >>
                     boost::spirit::qi::uint_parser<uint16_t, 16>{}] |
              boost::spirit::qi::uint_parser<uint16_t>{};
    params_ = number_ >> number_ >> -number_ >> -number_ >> eoi;
  }

private:
  boost::spirit::qi::rule<Iterator, uint16_t, boost::spirit::ascii::space_type>
      number_;
  boost::spirit::qi::rule<Iterator, std::vector<uint16_t>,
                          boost::spirit::ascii::space_type>
      params_;
};

namespace {
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
  throw 1;
}
#if 0
void help() { std::cout << "usage" << std::endl; }
#endif
}

int main(int ac, const char *av[]) {
  {
    using namespace Assert;
    Handler::setHandler(Handler::availableHandlers::throwException);
  }
  std::stringstream parameters;
  for (auto i = 1; i < ac; ++i) {
    parameters << av[i] << ' ';
  }
  auto corners = parse(parameters.str());
  for (const auto &corner : corners) {
    std::cout << corner << ", ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  assert_normal((corners.size() >= 2) && (corners.size() <= 4));
  const auto tl = TestScreens::RGB565{corners[0]};
  const auto tr = TestScreens::RGB565{corners[1]};
  const auto bl =
      TestScreens::RGB565{corners.size() > 2 ? corners[2] : corners[0]};
  const auto br =
      TestScreens::RGB565{corners.size() > 3 ? corners[3] : corners[1]};

  auto display = Display{};
  const auto size = [&display]() {
    int width = 0;
    int height = 0;
    display.get_size(width, height);
    return std::pair<int, int>{width, height};
  }();

  for (auto y = 0; y < size.second; ++y) {
    const auto l = TestScreens::colour(tl, bl, size.second, y);
    const auto r = TestScreens::colour(tr, br, size.second, y);
    for (auto x = 0; x < size.first; ++x) {
      const auto colour = TestScreens::colour(l, r, size.first, x).value;
      display.draw_raster(x, y, &colour, 1);
      std::cout << TestScreens::colour(l, r, size.first, x);
    }
    std::cout << std::endl;
  }
  std::cout << TestScreens::RGB565{0xffff} << std::endl;

  return 1;
}
