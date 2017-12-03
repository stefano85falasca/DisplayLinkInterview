#include "display/display.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>

#define W 100
#define H 40

static unsigned short frame_buffer[W * H];

Display::Display() { memset(frame_buffer, 0, sizeof(frame_buffer)); }

Display::~Display() {
#if 0
  unsigned short *pix = frame_buffer;
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      if (x > 0) {
        printf(" ");
      }
      printf("%04X", *pix++);
    }
    printf("\n");
  }
#endif
}

bool Display::connect(const char * /*display_name*/) { return true; }

void Display::get_size(int &width, int &height) {
  width = W;
  height = H;
}

void Display::draw_raster(int x, int y, const unsigned short *pixels,
                          int width) {
  memcpy(&frame_buffer[y * W + x], pixels, width * sizeof(unsigned short));
}
