#pragma once

#include "hardware.h"
#include <cassert>
#include <string>

namespace GB {

typedef uint8_t pixel_t;

class PixelMap : public non_copyable {
public:
  PixelMap();
  PixelMap(size_t width, size_t height);
  PixelMap(const pixel_t *pixels, size_t width, size_t height);
  virtual ~PixelMap();

  size_t width() const { return width_; }
  size_t height() const { return height_; }
  PixelMap(PixelMap &&other) noexcept;
  PixelMap &operator=(PixelMap &&other);

  // merge pixelmap to self starting at (x,y) point
  void merge_from(const PixelMap &pm, size_t x, size_t y);
  void vflip(); // vertically flip
  void hflip(); // horizontally flip

  PixelMap clone() const;

  void pixel(size_t x, size_t y, pixel_t p) {
    assert(x < width_);
    assert(y < height_);
    pixels_[y * width_ + x] = p;
  }

  pixel_t pixel(size_t x, size_t y) const {
    assert(x < width_);
    assert(y < height_);
    return pixels_[y * width_ + x];
  }

  PixelMap cut(size_t x, size_t y, size_t width, size_t height) const;
  PixelMap magnify(size_t ratio) const;

  std::string debug_info() const;

  bool draw() const;

private:
  size_t width_;
  size_t height_;
  pixel_t *pixels_;
};

} // namespace GB