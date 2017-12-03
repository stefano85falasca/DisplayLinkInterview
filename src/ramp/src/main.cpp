#include "common/assert.h"
#include "display/colours.h"
#include "display/display.h"
#include "testscreens/testscreens.h"
#include <iostream>

int main(int ac, const char *av[]) {
  {
    using namespace Assert;
    Handler::setHandler(Handler::availableHandlers::throwException);
  }

  auto display = Display{};
  const auto size = [&display]() {
    int width = 0;
    int height = 0;
    display.get_size(width, height);
    return std::pair<int, int>{width, height};
  }();
  const auto tl = DisplayNS::RGB565{0xffff};
  const auto tr = DisplayNS::RGB565{0x001f};
  const auto bl = DisplayNS::RGB565{0x07e0};
  const auto br = DisplayNS::RGB565{0xf800};

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
  std::cout << DisplayNS::RGB565{0xffff} << std::endl;

  return 1;
}
