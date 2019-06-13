#pragma once

#include "bootstrap_rom.h"
#include "cartridge.h"
#include "cpu.h"
#include "gpu.h"
#include "lcd_displayer.h"
#include "memory.h"
#include <memory>

namespace GB {
class VirtualMachine final : public non_copyable {
public:
  VirtualMachine(SPtr<BootstrapROM> bsr, CartridgePtr cartridge,
                 SPtr<LCDDisplayer> displayer, bool debug_mode);
  ~VirtualMachine();

  void connect_all_components();
  void run();

private:
  SPtr<BootstrapROM> bootstrap_rom_;
  CartridgePtr cartridge_;
  SPtr<LCDDisplayer> displayer_;

  UPtr<Memory> memory_;
  UPtr<CPU> cpu_;
  UPtr<GPU> gpu_;
  UPtr<Joypad> joypad_;

  UPtr<IPortOperator> turnoff_bootstrap_;
  UPtr<IPortOperator> port_dma_;
};
} // namespace GB