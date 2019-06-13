#pragma once

#include "hardware.h"
#include "memory_operator.h"
#include <cassert>
#include <memory>
#include <string>

namespace GB {

/*
  00h  ROM ONLY                 13h  MBC3+RAM+BATTERY
  01h  MBC1                     15h  MBC4
  02h  MBC1+RAM                 16h  MBC4+RAM
  03h  MBC1+RAM+BATTERY         17h  MBC4+RAM+BATTERY
  05h  MBC2                     19h  MBC5
  06h  MBC2+BATTERY             1Ah  MBC5+RAM
  08h  ROM+RAM                  1Bh  MBC5+RAM+BATTERY
  09h  ROM+RAM+BATTERY          1Ch  MBC5+RUMBLE
  0Bh  MMM01                    1Dh  MBC5+RUMBLE+RAM
  0Ch  MMM01+RAM                1Eh  MBC5+RUMBLE+RAM+BATTERY
  0Dh  MMM01+RAM+BATTERY        FCh  POCKET CAMERA
  0Fh  MBC3+TIMER+BATTERY       FDh  BANDAI TAMA5
  10h  MBC3+TIMER+RAM+BATTERY   FEh  HuC3
  11h  MBC3                     FFh  HuC1+RAM+BATTERY
  12h  MBC3+RAM*/
enum CartridgeType {
  ROM_ONLY = 0x00,
  MBC1 = 0x01,
  MBC1_RAM = 0x02,
  MBC1_RAM_BATTERY = 0x03,
  MBC2 = 0x05,
  MBC2_BATTERY = 0x6,
  ROM_RAM = 0x08,
  ROM_RAM_BATTERY = 0x9,
  MMM01 = 0xB,
  MMM01_RAM = 0xC,
  MMM01_RAM_BATTERY = 0xD,
  MBC3_TIMER_BATTERY = 0xF,
  MBC3_TIMER_RAM_BATTERY = 0x10,
  MBC3 = 0x11,
  MBC3_RAM = 0x12,
  MBC3_RAM_BATTERY = 0x13,
  MBC4 = 0x15,
  MBC4_RAM = 0x16,
  MBC4_RAM_BATTERY = 0x17,
  MBC5 = 0x19,
  MBC5_RAM = 0x1A,
  MBC5_RAM_BATTERY = 0x1B,
  MBC5_RUMBLE = 0x1C,
  MBC5_RUMBLE_RAM = 0x1D,
  MBC5_RUMBLE_RAM_BATTERY = 0x1E,
  POCKET_CAMERA = 0xFC,
  BANDAI_TAMA5 = 0xFD,
  HuC3 = 0xFE,
  HuC1_RAM_BATTERY = 0xFF,
};

class MemoryBank final {
public:
  MemoryBank();

  void set(a16_t addr, byte data);
  byte get(a16_t addr) const;

private:
  byte *data_;
  size_t size_;
};

class CartridgeHeader final {
public:
  CartridgeHeader();
  ~CartridgeHeader() {}

  int load(const byte *raw_data);

  std::string title() const { return title_; }
  std::string manufacturer() const { return manufacturer_code_; }
  std::string license() const { return license_code_; }
  CartridgeType type() const { return cartridge_type_; }
  size_t rom_size() const { return rom_size_; }
  size_t rom_banks_num() const { return rom_banks_num_; }
  size_t ram_size() const { return ram_size_; }
  size_t ram_banks_num() const { return ram_banks_num_; }
  uint8_t destination_code() const { return destination_code_; }
  uint8_t version() const { return version_number_; }

  std::string description() const;

private:
  std::string title_;
  std::string manufacturer_code_;
  uint8_t cgb_flag_; // TODO implement CGB mode
  std::string license_code_;
  CartridgeType cartridge_type_;
  size_t rom_size_;
  size_t rom_banks_num_;
  size_t ram_size_;
  size_t ram_banks_num_;
  uint8_t destination_code_;
  uint8_t version_number_;
  uint8_t header_checksum_;
  uint16_t global_checksum_;
};

typedef std::shared_ptr<const CartridgeHeader> CartridgeHeaderSP;
typedef std::unique_ptr<byte[]> ByteArraySP;

class Cartridge : public MemoryOperator {
public:
  Cartridge(CartridgeHeaderSP header, ByteArraySP rom_data);
  ~Cartridge() = default;

  CartridgeHeaderSP header() const { return header_; }

protected:
  CartridgeHeaderSP header_;
  ByteArraySP rom_data_;
  ByteArraySP ram_data_;
};

typedef std::shared_ptr<Cartridge> CartridgePtr;

class CartridgeLoader final {
public:
  static CartridgePtr load(std::string rom_path);
};

} // namespace GB