#pragma once

#include "display/colours.h"

namespace TestScreens {
DisplayNS::RGB565 colour(DisplayNS::RGB565 first, DisplayNS::RGB565 last,
                         int total, int coordinate);
}
