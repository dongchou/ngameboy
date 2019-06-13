#include "cartridge.h"
#include "common.h"
#include <cassert>
#include <fstream>

namespace GB {

CartridgeHeader::CartridgeHeader()
    : cgb_flag_(0), rom_size_(0), ram_size_(0), ram_banks_num_(0),
      destination_code_(0), version_number_(0), header_checksum_(0),
      global_checksum_(0) {}

int CartridgeHeader::load(const byte *raw_data) {
  auto fill_str = [raw_data](std::string &str, size_t begin, size_t end) {
    const byte *title_begin = raw_data + begin;
    const byte *title_end = raw_data + end;
    while (title_begin <= title_end && *title_begin) {
      str.push_back(*title_begin++);
    }
  };

  fill_str(title_, 0x134, 0x13e);
  fill_str(manufacturer_code_, 0x13f, 0x142);
  if (raw_data[0x14b] == 0x33) {
    fill_str(license_code_, 0x144, 0x145);
  } else {
    fill_str(license_code_, 0x14b, 0x14b);
  }

  cgb_flag_ = raw_data[0x143];
  cartridge_type_ = static_cast<CartridgeType>(raw_data[0x147]);
  auto rom_size_flag = raw_data[0x148];
  /*
    00h -  32KByte (no ROM banking)
    01h -  64KByte (4 banks)
    02h - 128KByte (8 banks)
    03h - 256KByte (16 banks)
    04h - 512KByte (32 banks)
    05h -   1MByte (64 banks)  - only 63 banks used by MBC1
    06h -   2MByte (128 banks) - only 125 banks used by MBC1
    07h -   4MByte (256 banks)
    52h - 1.1MByte (72 banks)
    53h - 1.2MByte (80 banks)
    54h - 1.5MByte (96 banks)
  */
  switch (rom_size_flag) {
  case 0x0:
    rom_size_ = 32 * 1024;
    rom_banks_num_ = 0;
    break;
  case 0x1:
    rom_banks_num_ = 4;
    break;
  case 0x2:
    rom_banks_num_ = 8;
    break;
  case 0x3:
    rom_banks_num_ = 16;
    break;
  case 0x4:
    rom_banks_num_ = 32;
    break;
  case 0x5:
    rom_banks_num_ = 64;
    break;
  case 0x6:
    rom_banks_num_ = 128;
    break;
  case 0x7:
    rom_banks_num_ = 256;
    break;
  case 0x52:
    rom_banks_num_ = 72;
    break;
  case 0x53:
    rom_banks_num_ = 80;
    break;
  case 0x54:
    rom_banks_num_ = 96;
    break;
  }
  if (rom_banks_num_) {
    rom_size_ = rom_banks_num_ * 16 * 1024;
  }

  auto ram_size_flag = raw_data[0x149];
  /*
  00h - None
  01h - 2 KBytes
  02h - 8 Kbytes
  03h - 32 KBytes (4 banks of 8KBytes each)
  */
  switch (ram_size_flag) {
  case 0x0:
    ram_size_ = 0;
    break;
  case 0x1:
    ram_size_ = 2 * 1024;
    break;
  case 0x2:
    ram_size_ = 8 * 1024;
    break;
  case 0x3:
    ram_size_ = 32 * 1024;
    ram_banks_num_ = 4;
    break;
  }

  destination_code_ = raw_data[0x14a];
  version_number_ = raw_data[0x14c];
  header_checksum_ = raw_data[0x14d];
  global_checksum_ = raw_data[0x14e] << 8 | raw_data[0x14f];
  return 0;
}

std::string CartridgeHeader::description() const {
  char buf[1024];
  std::string type_name;
  switch (cartridge_type_) {
  case CartridgeType::ROM_ONLY:
    type_name = "ROMONLY";
    break;
  case CartridgeType::MBC1:
    type_name = "MBC1";
    break;
  case CartridgeType::MBC1_RAM:
    type_name = "MBC1+RAM";
    break;
  case CartridgeType::MBC1_RAM_BATTERY:
    type_name = "MBC1+RAM+BATTERY";
    break;
  case CartridgeType::MBC2:
    type_name = "MBC2";
    break;
  case CartridgeType::MBC2_BATTERY:
    type_name = "MBC2+BATTERY";
    break;
  case CartridgeType::ROM_RAM:
    type_name = "ROM+RAM";
    break;
  case CartridgeType::ROM_RAM_BATTERY:
    type_name = "ROM+RAM+BATTERY";
    break;
  case CartridgeType::MMM01:
    type_name = "MMM01";
    break;
  case CartridgeType::MMM01_RAM:
    type_name = "MMM01+RAM";
    break;
  case CartridgeType::MMM01_RAM_BATTERY:
    type_name = "MMM01+RAM+BATTERY";
    break;
  case CartridgeType::MBC3_TIMER_BATTERY:
    type_name = "MBC3+TIMER+BATTERY";
    break;
  case CartridgeType::MBC3_TIMER_RAM_BATTERY:
    type_name = "MBC3+TIMER+RAM+BATTERY";
    break;
  case CartridgeType::MBC3:
    type_name = "MBC3";
    break;
  case CartridgeType::MBC3_RAM:
    type_name = "MBC3+RAM";
    break;
  case CartridgeType::MBC3_RAM_BATTERY:
    type_name = "MBC3+RAM+BATTERY";
    break;
  case CartridgeType::MBC4:
    type_name = "MBC4";
    break;
  case CartridgeType::MBC4_RAM:
    type_name = "MBC4+RAM";
    break;
  case CartridgeType::MBC4_RAM_BATTERY:
    type_name = "MBC4+RAM+BATTERY";
    break;
  case CartridgeType::MBC5:
    type_name = "MBC5";
    break;
  case CartridgeType::MBC5_RAM:
    type_name = "MBC5+RAM";
    break;
  case CartridgeType::MBC5_RAM_BATTERY:
    type_name = "MBC5+RAM+BATTERY";
    break;
  case CartridgeType::MBC5_RUMBLE:
    type_name = "MBC5+RUMBLE";
    break;
  case CartridgeType::MBC5_RUMBLE_RAM:
    type_name = "MBC5+RUMBLE+RAM";
    break;
  case CartridgeType::MBC5_RUMBLE_RAM_BATTERY:
    type_name = "MBC5+RUMBLE+RAM+BATTERY";
    break;
  case CartridgeType::POCKET_CAMERA:
    type_name = "POCKETCAMERA";
    break;
  case CartridgeType::BANDAI_TAMA5:
    type_name = "BANDAITAMA5";
    break;
  case CartridgeType::HuC3:
    type_name = "HuC3";
    break;
  case CartridgeType::HuC1_RAM_BATTERY:
    type_name = "HuC1+RAM+BATTERY";
    break;
  }

  snprintf(buf, sizeof(buf),
           "Title:%s,Manufacturer:%s,License:%s,Type:%s,ROM:%dKB,ROMBanks:%d,"
           "RAM:%dKB,DestCode:%d,Version:%d,HC:%d,GC:%d",
           title_.c_str(), manufacturer_code_.c_str(), license_code_.c_str(),
           type_name.c_str(), (int)rom_size_ / 1024, (int)rom_banks_num_,
           (int)ram_size_ / 1024, (int)destination_code_, (int)version_number_,
           (int)header_checksum_, (int)global_checksum_);
  return std::string(buf);
}

//=============================cartridge=============================
Cartridge::Cartridge(CartridgeHeaderSP header, ByteArraySP rom_data)
    : header_(header), rom_data_(std::move(rom_data)) {
  if (header_->ram_size() > 0) {
    ram_data_ = ByteArraySP(new byte[header_->ram_size()]);
  }
}
//==========================MBC implementation========================
class ROMOnlyCartridge : public Cartridge {
public:
  ROMOnlyCartridge(CartridgeHeaderSP header, ByteArraySP rom_data)
      : Cartridge(header, std::move(rom_data)) {}

  void set(a16_t addr, byte data) override {
    // nothing to do
  }

  byte get(a16_t addr) const override {
    assert(addr < header_->rom_size());
    return static_cast<byte>(rom_data_[addr]);
  }
};

class MBC1Cartridge : public Cartridge {
  enum BankingMode {
    ROMMode,
    RAMMode,
  };

public:
  MBC1Cartridge(CartridgeHeaderSP header, ByteArraySP rom_data)
      : Cartridge(header, std::move(rom_data)), ram_enabled_(false),
        lower_data_(1), upper_data_(0), banking_mode_(BankingMode::ROMMode) {}

  void set(a16_t addr, byte data) override {
    if (addr < 0x2000) {
      ////0xA means enabling RAM
      ram_enabled_ = ((data & 0xf) == 0xA);
    } else if (addr < 0x4000) {
      // select rom bank number(lower 5 bits)
      lower_data_ = data & 0x1f;
      if (lower_data_ == 0) {
        lower_data_ = 1;
      }
    } else if (addr < 0x6000) {
      // ram bank number or rom bank number - 2 bits
      upper_data_ = data & 0x3;
    } else if (addr < 0x8000) {
      banking_mode_ =
          IS_BIT_SET(data, 0) ? BankingMode::RAMMode : BankingMode::ROMMode;
    } else if (addr >= 0xA000 && addr < 0xC000) {
      ram_data_[get_ram_addr(addr)] = data;
    } else {
      assert(0);
    }
  }

  byte get(a16_t addr) const override {
    if (addr < 0x4000) {
      return rom_data_[addr];
    } else if (addr < 0x8000) {
      return rom_data_[(addr - 0x4000) + get_rom_banks_num() * 0x4000];
    } else if (addr >= 0xA000 && addr < 0xC000) {
      return ram_data_[get_ram_addr(addr)];
    }
    assert(0);
    return 0;
  }

protected:
  size_t get_rom_banks_num() const {
    if (banking_mode_ == BankingMode::ROMMode) {
      return (upper_data_ << 5) | lower_data_;
    } else {
      return lower_data_;
    }
  }

  size_t get_ram_addr(a16_t addr) const {
    addr -= 0xA000;
    assert(ram_enabled_ && addr < header_->ram_size());

    if (header_->ram_banks_num() > 0) {
      auto bn = 0;
      if (banking_mode_ == BankingMode::RAMMode) {
        bn = upper_data_;
      }
      return addr + bn * 0x2000;
    } else {
      return addr;
    }
  }

protected:
  bool ram_enabled_;
  uint8_t lower_data_;
  uint8_t upper_data_;
  BankingMode banking_mode_;
};

CartridgePtr CartridgeLoader::load(std::string rom_path) {
  std::ifstream is(rom_path.c_str(), std::ifstream::binary);
  if (!is) {
    return CartridgePtr();
  }

  // get length of file:
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);

  ByteArraySP buffer(new byte[length]);
  is.read(reinterpret_cast<char *>(buffer.get()), length);
  is.close();

  // parse the cartridge header
  auto header = new CartridgeHeader();
  CartridgeHeaderSP header_sp(header);
  if (header->load(buffer.get())) {
    return CartridgePtr();
  }

  switch (header_sp->type()) {
  case CartridgeType::ROM_ONLY:
    return CartridgePtr(new ROMOnlyCartridge(header_sp, std::move(buffer)));
  case CartridgeType::MBC1:
  case CartridgeType::MBC1_RAM:
  case CartridgeType::MBC1_RAM_BATTERY:
    return CartridgePtr(new MBC1Cartridge(header_sp, std::move(buffer)));
  default:
    return CartridgePtr();
  }
  return CartridgePtr();
}

} // namespace GB