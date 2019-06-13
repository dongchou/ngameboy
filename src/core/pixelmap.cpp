#include "pixelmap.h"
#include <algorithm>
#include <cstring>
#include <sstream>

namespace GB {

PixelMap::PixelMap() : width_(0), height_(0), pixels_(nullptr) {}

PixelMap::PixelMap(size_t width, size_t height)
    : width_(width), height_(height) {
  auto len = width * height;
  pixels_ = new pixel_t[len];
  memset(pixels_, 0, len);
}

PixelMap::PixelMap(const pixel_t *pixels, size_t width, size_t height)
    : width_(width), height_(height) {
  auto len = width * height;
  pixels_ = new pixel_t[len];
  memcpy(pixels_, pixels, len);
}

PixelMap::~PixelMap() {
  if (pixels_) {
    delete[] pixels_;
  }
}

PixelMap::PixelMap(PixelMap &&other) noexcept {
  width_ = other.width_;
  height_ = other.height_;
  pixels_ = other.pixels_;

  other.width_ = 0;
  other.height_ = 0;
  other.pixels_ = nullptr;
}

PixelMap &PixelMap::operator=(PixelMap &&other) {
  if (pixels_) {
    delete[] pixels_;
  }

  width_ = other.width_;
  height_ = other.height_;
  pixels_ = other.pixels_;

  other.width_ = 0;
  other.height_ = 0;
  other.pixels_ = nullptr;

  return *this;
}

void PixelMap::merge_from(const PixelMap &pm, size_t x, size_t y) {
  if (x >= width_ || y >= height_) {
    assert(0);
    return;
  }
  size_t len = std::min(pm.width(), width_ - x);
  if (len < 1) {
    return;
  }
  size_t pheight = std::min(pm.height(), height_ - y);
  for (size_t i = 0; i < pheight; ++i, ++y) {
    memcpy(&pixels_[y * width_ + x], &pm.pixels_[i * pm.width()], len);
  }
}

void PixelMap::vflip() {
  pixel_t *tmp = new pixel_t[width_];
  for (size_t y = 0; y < height_ / 2; ++y) {
    // swap the head and the tail
    auto head = y * width_;
    auto tail = (height_ - y - 1) * width_;
    memcpy(tmp, &pixels_[head], width_);
    memcpy(&pixels_[head], &pixels_[tail], width_);
    memcpy(&pixels_[tail], tmp, width_);
  }
  delete[] tmp;
}

void PixelMap::hflip() {
  for (size_t y = 0; y < height_; ++y) {
    for (size_t x = 0; x < width_ / 2; ++x) {
      auto head = y * width_ + x;
      auto tail = y * width_ + width_ - x - 1;
      std::swap(pixels_[head], pixels_[tail]);
    }
  }
}

PixelMap PixelMap::clone() const { return PixelMap(pixels_, width_, height_); }

PixelMap PixelMap::cut(size_t x, size_t y, size_t width, size_t height) const {
  assert(x < width_ && y < height_);
  auto new_width = std::min(width_ - x, width);
  auto new_height = std::min(height_ - y, height);
  PixelMap new_pm(new_width, new_height);

  for (size_t ny = 0; ny < new_height; ++ny, ++y) {
    memcpy(&new_pm.pixels_[ny * new_width], &pixels_[y * width_ + x],
           new_width);
  }

  return new_pm;
}

PixelMap PixelMap::magnify(size_t ratio) const {
  assert(ratio > 0);
  auto new_width = width_ * ratio;
  auto new_height = height_ * ratio;
  PixelMap new_pm(new_width, new_height);

  for (size_t x = 0; x < width_; ++x) {
    for (size_t y = 0; y < height_; ++y) {
      auto v = pixel(x, y);
      for (size_t i = 0; i < ratio; ++i) {
        for (size_t j = 0; j < ratio; ++j) {
          new_pm.pixel(x * ratio + i, y * ratio + j, v);
        }
      }
    }
  }
  return new_pm;
}

std::string PixelMap::debug_info() const {
  std::stringstream ss;
  for (size_t x = 0; x < width_; ++x) {
    ss << "=";
  }
  ss << std::endl;

  for (size_t y = 0; y < height_; ++y) {
    for (size_t x = 0; x < width_; ++x) {
      auto v = (unsigned)pixel(x, y);
      if (v) {
        // ss << "■";
        ss << "1";
      } else {
        // ss << "□";
        ss << "0";
      }
    }
    ss << std::endl;
  }

  for (size_t x = 0; x < width_; ++x) {
    ss << "=";
  }
  ss << std::endl;
  return ss.str();
}
} // namespace GB
