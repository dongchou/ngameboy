#pragma once

#include "hardware.h"
#include "memory_operator.h"
#include "pixelmap.h"
#include <memory>
#include <vector>

namespace GB {

enum VideoMemoryRange {
  TileDataStart = 0x8000,
  TileDataEnd = 0x9800,

  BGWin0MapAddr = 0x9800,
  BGWin1MapAddr = 0x9C00,

  OAMDataStart = 0XFE00,
  OAMDataEnd = 0xFEA0,
};

enum LCDMode {
  Mode0 = 0,
  Mode1,
  Mode2,
  Mode3,
};

/*
  Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
  Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
  Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
  Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
  Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
  Bit 5 - Window Display Enable          (0=Off, 1=On)
  Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
  Bit 7 - LCD Display Enable             (0=Off, 1=On)
*/
enum LCDCtrlBits {
  GCF_BG_DISPLAY_ENABLED = 0,
  GCF_SPRITE_DISPLAY_ENABLED = 1,
  GCF_SPRITE_SIZE = 2,
  GCF_BG_TILE_MAP_DATA = 3,
  GCF_BG_WINDOW_TILE_DATA = 4,
  GCF_WINDOW_DISPLAY_ENABLED = 5,
  GCF_WINDOW_TILE_MAP = 6,
  GCF_LCD_DISPLAY_ENABLED = 7,
};
constexpr size_t GPU_VIDEO_MEMORY_SIZE = 0x2000;

constexpr int PIXEL_NUM_PER_TILE = 8;
constexpr int BYTE_NUM_PER_TILE = PIXEL_NUM_PER_TILE * 2;
constexpr int TILE_NUM_PER_BGWIN_SIDE = 32;
constexpr int PIXEL_NUM_OF_BGWIN_SIDE =
    TILE_NUM_PER_BGWIN_SIDE * PIXEL_NUM_PER_TILE;

constexpr int SCREEN_WIDTH_TILE_NUM = 20;
constexpr int SCREEN_HEIGTH_TILE_NUM = 18;

class Tile : public PixelMap {
public:
  Tile() : PixelMap(PIXEL_NUM_PER_TILE, PIXEL_NUM_PER_TILE) {}
};

class TileSelector final {
private:
  a16_t tile_addr(a16_t base_addr, uint8_t num) const {
    return base_addr + num * BYTE_NUM_PER_TILE;
  }

public:
  TileSelector(const byte *ram, bool td);
  Tile get_tile(a16_t addr) const;
  Tile select(uint8_t num) const {
    return get_tile(
        tile_addr(tile_data_start_addr_, uint8_t(num + tile_num_offset_)));
  }

private:
  const byte *ram_;
  a16_t tile_data_start_addr_;
  uint8_t tile_num_offset_;
};

class Sprite : public PixelMap {
public:
  Sprite(size_t width, size_t height, d8_t x, d8_t y, d8_t tile_num, d8_t flags,
         const TileSelector &selector);

  d8_t x() const { return x_; }
  d8_t y() const { return y_; }

  void draw() const;

private:
  d8_t x_; // 0 - 160
  d8_t y_; // 0 - 144
  d8_t flags_;
};

constexpr size_t OAM_RAM_LENGTH = 0xa0;
class OAM final : public MemoryOperator, public non_copyable {
public:
  OAM();

  void set(a16_t addr, byte data) override;
  byte get(a16_t addr) const override;

  std::vector<Sprite> collect_sprites(const TileSelector &selector,
                                      uint8_t lcd_ctrl) const;

private:
  byte ram_[OAM_RAM_LENGTH];
};

class Memory;
class CPU;
class GPU final : public MemoryOperator,
                  public IPortOperator,
                  public non_copyable {
public:
  GPU(CPU *cpu);
  ~GPU();

  void connect(Memory *memory);

  void set(a16_t addr, byte data) override;
  byte get(a16_t addr) const override;

  void set_reg(a16_t addr, byte data) override;
  byte get_reg(a16_t addr) const override;

  bool update(int clocks);

  PixelMap get_tile_map(a16_t base_addr) const;
  PixelMap get_current_background() const;
  PixelMap get_view() const;
  PixelMap get_all_tiles() const;

  void check_lyc() ;

private:
  const byte *addr(a16_t addr) const;

private:
  CPU *cpu_;
  std::shared_ptr<OAM> oam_;
  /* data */
  byte ram_[GPU_VIDEO_MEMORY_SIZE];

  uint8_t lcd_ctrl_;
  uint8_t lcd_status_;
  uint8_t scy_;
  uint8_t scx_;
  uint8_t lyc_;
  uint8_t bgp_;
  uint8_t bgp0_;
  uint8_t bgp1_;
  uint8_t wy_;
  uint8_t wx_;

  LCDMode mode_;
  int curr_mode_clocks_;
  int curr_lines_;
};

} // namespace GB
