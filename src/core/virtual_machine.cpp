#include "virtual_machine.h"
#include "joypad.h"
#include "special_registers.h"
#include <algorithm>
#include <cassert>
#include <cstring>

namespace GB {

VirtualMachine::VirtualMachine(SPtr<BootstrapROM> bsr, CartridgePtr cartridge,
                               SPtr<LCDDisplayer> displayer, bool debug_mode)
    : bootstrap_rom_(bsr), cartridge_(cartridge), displayer_(displayer) {
  memory_ = UPtr<Memory>(new Memory());
  cpu_ = UPtr<CPU>(new CPU(memory_.get(), debug_mode));
  gpu_ = UPtr<GPU>(new GPU(cpu_.get()));
  joypad_ = UPtr<Joypad>(new Joypad());
  turnoff_bootstrap_ =
      UPtr<IPortOperator>(new SR_TurnOffBootstrap(memory_.get()));
  port_dma_ = UPtr<IPortOperator>(new SR_DMA(memory_.get()));
}

VirtualMachine::~VirtualMachine() {}

void VirtualMachine::connect_all_components() {
  gpu_->connect(memory_.get());
  memory_->map_port(MappedIOPorts::REG_JOYPAD, joypad_.get());
  memory_->map_port(MappedIOPorts::REG_TURN_OFF_ROM, turnoff_bootstrap_.get());
  memory_->map_port(MappedIOPorts::REG_DMA, port_dma_.get());

  memory_->load_boot_rom(bootstrap_rom_.get());
  memory_->load_cartridge(cartridge_.get());

  if (!displayer_->prepare(joypad_.get())) {
    return;
  }
}

void VirtualMachine::run() {
  auto last_clock = std::chrono::high_resolution_clock::now();
  auto total_clocks = 0;
  while (true) {
    auto cost_clocks = cpu_->update();
    total_clocks += cost_clocks;
    if (!gpu_->update(cost_clocks)) {
      continue;
    }
    //now render the current frame
    auto now_clock = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now_clock - last_clock)
                       .count();
    if (elapsed < 15) {
      std::this_thread::sleep_for(std::chrono::milliseconds(15 - elapsed));
    }
    last_clock = std::chrono::high_resolution_clock::now();
    total_clocks = 0;

    cpu_->request_interrupt(CPUInterrupts::INT_V_BLANK);
    displayer_->push_frame(gpu_->get_view());
  }
}

} // namespace GB