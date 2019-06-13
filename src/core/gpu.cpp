#include "gpu.h"
#include "common.h"
#include "cpu.h"
#include "memory.h"
#include <cassert>
#include <cstring>

namespace GB {
TileSelector::TileSelector(const byte *ram, bool td) : ram_(ram) {
  if (td) {
    tile_data_start_addr_ = 0x8000;
    tile_num_offset_ = 0;
  } else {
    tile_data_start_addr_ = 0x8800;
    tile_num_offset_ = 128;
  }
}

Tile TileSelector::get_tile(a16_t addr) const {
  assert(addr >= VideoMemoryRange::TileDataStart &&
         addr < VideoMemoryRange::TileDataEnd && (addr & 0xF) == 0);
  addr -= 0x8000;

  const byte *bmp = &ram_[addr];
  Tile t;

  auto i = 0;
  while (i < BYTE_NUM_PER_TILE) {
    auto fb = bmp[i];
    auto sb = bmp[i + 1];

    for (auto j = 0; j < PIXEL_NUM_PER_TILE; ++j) {
      auto p = ((sb & 1) << 1) | (fb & 1);
      t.pixel(PIXEL_NUM_PER_TILE - 1 - j, i / 2, p);
      fb >>= 1;
      sb >>= 1;
    }
    i += 2;
  }
  return t;
}

Sprite::Sprite(size_t width, size_t height, d8_t x, d8_t y, d8_t tile_num,
               d8_t flags, const TileSelector &selector)
    : PixelMap(width, height), x_(x), y_(y), flags_(flags) {
  if (height == PIXEL_NUM_PER_TILE) {
    auto t = selector.select(tile_num);
    merge_from(t, 0, 0);
  } else if (height == PIXEL_NUM_PER_TILE * 2) {
    auto upper_tile = selector.select(tile_num & 0xFE);
    auto lower_tile = selector.select(tile_num | 0x01);
    merge_from(upper_tile, 0, 0);
    merge_from(lower_tile, 0, PIXEL_NUM_PER_TILE);
  } else {
    assert(0);
  }

  if (flags_ & (1 << 6)) {
    vflip();
  }

  if (flags_ & (1 << 5)) {
    hflip();
  }

  // TODO: priority check
}

OAM::OAM() { memset(ram_, 0, sizeof(ram_)); }

void OAM::set(a16_t addr, byte data) {
  assert(addr >= 0xFE00 && addr <= 0xFE9F);
  ram_[addr - 0xFE00] = data;
}

byte OAM::get(a16_t addr) const {
  assert(addr >= 0xFE00 && addr <= 0xFE9F);
  return ram_[addr - 0xFE00];
}

std::vector<Sprite> OAM::collect_sprites(const TileSelector &selector,
                                         uint8_t lcd_ctrl) const {
  size_t width = PIXEL_NUM_PER_TILE;
  size_t height = PIXEL_NUM_PER_TILE;

  if (IS_BIT_SET(lcd_ctrl, LCDCtrlBits::GCF_SPRITE_SIZE)) {
    height *= 2;
  }

  std::vector<Sprite> list;
  for (a16_t start = 0; start < OAM_RAM_LENGTH; start += 4) {
    auto y = ram_[start];
    auto x = ram_[start + 1];
    auto tile_num = ram_[start + 2];
    auto flags = ram_[start + 3];

    if (y < 16 || y >= 160 || x < 8 || x >= 168) {
      continue;
    }

    list.push_back(
        Sprite(width, height, x - 8, y - 16, tile_num, flags, selector));
  }
  return list;
}

GPU::GPU(CPU *cpu)
    : cpu_(cpu), oam_(new OAM()), lcd_ctrl_(0), lcd_status_(0), scy_(0),
      scx_(0), lyc_(0), bgp_(0), bgp0_(0), bgp1_(0), wy_(0), wx_(0),
      mode_(LCDMode::Mode2), curr_mode_clocks_(0), curr_lines_(0) {
  memset(ram_, 0, sizeof(ram_));
}
GPU::~GPU() {}

void GPU::connect(Memory *memory) {
  memory->map_port(MappedIOPorts::REG_LCD_CTRL, this);
  memory->map_port(MappedIOPorts::REG_LCD_STATUS, this);
  memory->map_port(MappedIOPorts::REG_SCY, this);
  memory->map_port(MappedIOPorts::REG_SCX, this);
  memory->map_port(MappedIOPorts::REG_LY, this);
  memory->map_port(MappedIOPorts::REG_LYC, this);
  memory->map_port(MappedIOPorts::REG_BGP, this);
  memory->map_port(MappedIOPorts::REG_BGP0, this);
  memory->map_port(MappedIOPorts::REG_BGP1, this);
  memory->map_port(MappedIOPorts::REG_WY, this);
  memory->map_port(MappedIOPorts::REG_WX, this);
  memory->connect_gpu(this);
  memory->connect_oam(oam_.get());
}

void GPU::set(a16_t addr, byte data) {
  assert(addr >= 0x8000 && addr <= 0x9fff);
  ram_[addr - 0x8000] = data;
}

byte GPU::get(a16_t addr) const {
  assert(addr >= 0x8000 && addr <= 0x9fff);
  return ram_[addr - 0x8000];
}

const byte *GPU::addr(a16_t addr) const {
  assert(addr >= 0x8000 && addr <= 0x9fff);
  return &ram_[addr - 0x8000];
}

void GPU::set_reg(a16_t addr, byte data) {
  switch (addr) {
  case MappedIOPorts::REG_LCD_CTRL:
    lcd_ctrl_ = data;
    return;
  case MappedIOPorts::REG_LCD_STATUS:
    lcd_status_ = data;
    return;
  case MappedIOPorts::REG_SCY:
    scy_ = data;
    return;
  case MappedIOPorts::REG_SCX:
    scx_ = data;
    return;
  case MappedIOPorts::REG_LY:
    curr_lines_ = data;
    check_lyc();
    return;
  case MappedIOPorts::REG_LYC:
    lyc_ = data;
    check_lyc();
    return;
  case MappedIOPorts::REG_BGP:
    bgp_ = data;
    return;
  case MappedIOPorts::REG_BGP0:
    bgp0_ = data;
    return;
  case MappedIOPorts::REG_BGP1:
    bgp1_ = data;
    return;
  case MappedIOPorts::REG_WY:
    wy_ = data;
    return;
  case MappedIOPorts::REG_WX:
    wx_ = data;
    return;
  default:
    assert(0);
  }
}

byte GPU::get_reg(a16_t addr) const {
  switch (addr) {
  case MappedIOPorts::REG_LCD_CTRL:
    return lcd_ctrl_;
  case MappedIOPorts::REG_LCD_STATUS:
    return lcd_status_;
  case MappedIOPorts::REG_SCY:
    return scy_;
  case MappedIOPorts::REG_SCX:
    return scx_;
  case MappedIOPorts::REG_LY:
    return curr_lines_;
  case MappedIOPorts::REG_LYC:
    return lyc_;
  case MappedIOPorts::REG_BGP:
    return bgp_;
  case MappedIOPorts::REG_BGP0:
    return bgp0_;
  case MappedIOPorts::REG_BGP1:
    return bgp1_;
  case MappedIOPorts::REG_WY:
    return wy_;
  case MappedIOPorts::REG_WX:
    return wx_;
  default:
    assert(0);
  }
}

PixelMap GPU::get_tile_map(a16_t base_addr) const {
  constexpr auto bgwin_size = TILE_NUM_PER_BGWIN_SIDE * PIXEL_NUM_PER_TILE;
  PixelMap pm(bgwin_size, bgwin_size);

  TileSelector selector(
      ram_, IS_BIT_SET(lcd_ctrl_, LCDCtrlBits::GCF_BG_WINDOW_TILE_DATA));

  const byte *tile_nums = addr(base_addr);
  for (auto i = 0; i < TILE_NUM_PER_BGWIN_SIDE * TILE_NUM_PER_BGWIN_SIDE; ++i) {
    auto y = i / TILE_NUM_PER_BGWIN_SIDE * PIXEL_NUM_PER_TILE;
    auto x = i % TILE_NUM_PER_BGWIN_SIDE * PIXEL_NUM_PER_TILE;
    pm.merge_from(selector.select(tile_nums[i]), x, y);
  }

  return pm;
}

constexpr static a16_t TileMapBaseAddr[] = {VideoMemoryRange::BGWin0MapAddr,
                                            VideoMemoryRange::BGWin1MapAddr};

PixelMap GPU::get_current_background() const {
  PixelMap view(SCREEN_WIDTH_TILE_NUM * PIXEL_NUM_PER_TILE,
                SCREEN_HEIGTH_TILE_NUM * PIXEL_NUM_PER_TILE);
  auto bg_map_base_addr =
      TileMapBaseAddr[(lcd_ctrl_ >> LCDCtrlBits::GCF_BG_TILE_MAP_DATA) & 0x1];
  auto full_bg = get_tile_map(bg_map_base_addr);
  auto screen_bg = full_bg.cut(scx_, scy_, view.width(), view.height());
  view.merge_from(screen_bg, 0, 0);
  // wrap the upper side
  if (screen_bg.height() < view.height()) {
    view.merge_from(full_bg, scx_, screen_bg.height());
  }
  // wrap the right side
  if (screen_bg.width() < view.width()) {
    view.merge_from(full_bg, screen_bg.width(), scy_);
  }
  // TODO left,right,upper,lower side must be handled

  return view;
}

PixelMap GPU::get_view() const {
  PixelMap view(SCREEN_WIDTH_TILE_NUM * PIXEL_NUM_PER_TILE,
                SCREEN_HEIGTH_TILE_NUM * PIXEL_NUM_PER_TILE);

  if (IS_BIT_SET(lcd_ctrl_, LCDCtrlBits::GCF_BG_DISPLAY_ENABLED)) {
    view.merge_from(get_current_background(), 0, 0);
  }

  if (IS_BIT_SET(lcd_ctrl_, LCDCtrlBits::GCF_WINDOW_DISPLAY_ENABLED)) {
    assert(wy_ < 144 && wx_ >= 7 && wx_ <= 166);
    auto win_map_base_addr =
        TileMapBaseAddr[(lcd_ctrl_ >> LCDCtrlBits::GCF_WINDOW_TILE_MAP) & 0x1];
    auto full_win = get_tile_map(win_map_base_addr);
    auto screen_win = full_win.cut(wx_ - 7, wy_, view.width(), view.height());
    view.merge_from(screen_win, 0, 0);
  }

  // collect pixels of sprits
  if (IS_BIT_SET(lcd_ctrl_, LCDCtrlBits::GCF_SPRITE_DISPLAY_ENABLED)) {
    auto sprite_list =
        oam_->collect_sprites(TileSelector(ram_, true), lcd_ctrl_);
    for (const auto &sprite : sprite_list) {
      view.merge_from(sprite, sprite.x(), sprite.y());
    }
  }

  return view;
}

PixelMap GPU::get_all_tiles() const {
  TileSelector selector(
      ram_, IS_BIT_SET(lcd_ctrl_, LCDCtrlBits::GCF_BG_WINDOW_TILE_DATA));

  PixelMap pm(128, 192);
  int num = 0;
  for (int i = VideoMemoryRange::TileDataStart;
       i < VideoMemoryRange::TileDataEnd; i += BYTE_NUM_PER_TILE, ++num) {
    pm.merge_from(selector.get_tile(i), (num & 0xF) << 3, (num >> 4) << 3);
  }
  return pm;
}

void GPU::check_lyc() {
  if (lyc_ != curr_lines_) {
    CLEAR_BIT(lcd_status_, 2);
    return;
  }
  SET_BIT(lcd_status_, 2);

  if (IS_BIT_SET(lcd_status_, 6)) {
    cpu_->request_interrupt(CPUInterrupts::INT_LCD_STAT);
  }
}

bool GPU::update(int clocks) {
  auto refresh = false;
  curr_mode_clocks_ += clocks;
  if (mode_ == LCDMode::Mode0) {
    if (curr_mode_clocks_ >= 204) {
      if (curr_lines_ >= 144) {
        mode_ = LCDMode::Mode1;
        refresh = true;
      } else {
        ++curr_lines_;
        mode_ = LCDMode::Mode2;
        curr_mode_clocks_ -= 204;
        check_lyc();
      }
    }
  } else if (mode_ == LCDMode::Mode2) {
    if (curr_mode_clocks_ >= 80) {
      mode_ = LCDMode::Mode3;
      curr_mode_clocks_ -= 80;
    }
  } else if (mode_ == LCDMode::Mode3) {
    if (curr_mode_clocks_ >= 172) {
      mode_ = LCDMode::Mode0;
      curr_mode_clocks_ -= 172;
    }
  } else if (mode_ == LCDMode::Mode1) {
    if (curr_mode_clocks_ >= 456) {
      curr_mode_clocks_ -= 456;
      if (++curr_lines_ >= 154) {
        mode_ = LCDMode::Mode2;
        curr_lines_ = 0;
      }
      check_lyc();
    }
  }

  lcd_status_ = (lcd_status_ & (~0x3)) | static_cast<uint8_t>(mode_);

  return refresh;
}

} // namespace GB
