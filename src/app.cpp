#include "bootstrap_rom.h"
#include "cartridge.h"
#include "common.h"
#include "hardware.h"
#include "lcd_displayer.h"
#include "virtual_machine.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include "lcd_displayer.h"

using namespace GB;

static byte bootstrap_rom_data[] = {
    0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb,
    0x21, 0x26, 0xff, 0x0e, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
    0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x11, 0x04, 0x01,
    0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
    0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22,
    0x23, 0x05, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
    0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20, 0xf9, 0x2e, 0x0f, 0x18,
    0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
    0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20,
    0xf7, 0x1d, 0x20, 0xf2, 0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
    0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06, 0x7b, 0xe2, 0x0c, 0x3e,
    0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
    0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17,
    0xc1, 0xcb, 0x11, 0x17, 0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,
    0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83,
    0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
    0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63,
    0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,
    0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x21, 0x04, 0x01, 0x11,
    0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
    0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x20, 0xfe,
    0x3e, 0x01, 0xe0, 0x50};

constexpr size_t WINDOW_WIDTH = 700;
constexpr size_t WINDOW_HEIGHT = 800;

struct Color
{
public:
  Color() : red(0xff), green(0xff), blue(0xff) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class SpinLock
{
  std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
  void lock()
  {
    while (locked.test_and_set(std::memory_order_acquire))
    {
      ;
    }
  }
  void unlock() { locked.clear(std::memory_order_release); }
};

class GLDisplayer : public LCDDisplayer
{
public:
  GLDisplayer(size_t width, size_t height) : width_(width), height_(height), window_(nullptr), joypad_(nullptr)
  {
    new (&palettes_[0]) Color(0xd0, 0xf8, 0xe0);
    new (&palettes_[1]) Color(0x70, 0xc0, 0x88);
    new (&palettes_[2]) Color(0x56, 0x68, 0x34);
    new (&palettes_[3]) Color(0x20, 0x18, 0x08);
  }
  ~GLDisplayer() { glfwTerminate(); }

  void add_color(size_t index, Color c) {}

  virtual bool prepare(Joypad *joypad)
  {
    assert(window_ == nullptr);
    assert(joypad != nullptr);

    if (!glfwInit())
    {
      debug_log("failed to glfwInit");
      return false;
    }

    window_ = glfwCreateWindow(width_, height_, "GameBoy", NULL, NULL);
    if (!window_)
    {
      glfwTerminate();
      debug_log("failed to glfwCreateWindow");
      return false;
    }

    joypad_ = joypad;

    glfwMakeContextCurrent(window_);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, GLDisplayer::key_callback);

    return true;
  }

  virtual void push_frame(const PixelMap &frame)
  {
    std::lock_guard<SpinLock> guard(frame_mutex_);
    frame_list_.push_back(UPtr<PixelMap>(new PixelMap(frame.clone())));
  }

  void run()
  {
    debug_log("now ui thread is running!");
    auto last_clock = std::chrono::high_resolution_clock::now();
    auto draw_times = 0;
    while (true)
    {
      UPtr<PixelMap> pm;
      {
        std::lock_guard<SpinLock> guard(frame_mutex_);
        if (frame_list_.empty())
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(5));
          continue;
        }

        pm = std::move(frame_list_.front());
        frame_list_.pop_front();
      }
      render(*pm);
      ++draw_times;
      auto now_clock = std::chrono::high_resolution_clock::now();
      auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
          now_clock - last_clock);
      if (time_span.count() >= 1)
      {
        debug_log("=========================:%d times,%d", draw_times);
        draw_times = 0;
        last_clock = std::chrono::high_resolution_clock::now();
      }
    }
    debug_log("now ui thread is over!");
  }

private:
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods)
  {
    GLDisplayer *displayer =
        reinterpret_cast<GLDisplayer *>(glfwGetWindowUserPointer(window));
    JoypadKey jpkey = JoypadKey::KEY_NONE;
    switch (key)
    {
    case GLFW_KEY_UP:
      jpkey = JoypadKey::KEY_UP;
      break;
    case GLFW_KEY_DOWN:
      jpkey = JoypadKey::KEY_DOWN;
      break;
    case GLFW_KEY_LEFT:
      jpkey = JoypadKey::KEY_LEFT;
      break;
    case GLFW_KEY_RIGHT:
      jpkey = JoypadKey::KEY_RIGHT;
      break;
    case GLFW_KEY_A:
      jpkey = JoypadKey::KEY_BUTTON_A;
      break;
    case GLFW_KEY_S:
      jpkey = JoypadKey::KEY_BUTTON_B;
      break;
    case GLFW_KEY_Z:
      jpkey = JoypadKey::KEY_SELECT;
      break;
    case GLFW_KEY_X:
      jpkey = JoypadKey::KEY_START;
      break;
    default:
      return;
    }
    displayer->joypad_->update_key(jpkey,
                                   action == GLFW_PRESS || action == GLFW_REPEAT);
  }

  void render(const PixelMap &frame)
  {
    auto pm = frame.magnify(4);
    GLubyte *screenbmp = (GLubyte *)malloc(pm.width() * pm.height() * 3);
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t x = 0; x < pm.width(); ++x)
    {
      for (size_t y = 0; y < pm.height(); ++y)
      {
        auto pixel = pm.pixel(x, y);
        assert(pixel < 4);
        auto color = palettes_[pixel];
        auto real_y = pm.height() - 1 - y;
        screenbmp[real_y * pm.width() * 3 + x * 3 + 0] = color.red;
        screenbmp[real_y * pm.width() * 3 + x * 3 + 1] = color.green;
        screenbmp[real_y * pm.width() * 3 + x * 3 + 2] = color.blue;
      }
    }
    glDrawPixels(pm.width(), pm.height(), GL_RGB, GL_UNSIGNED_BYTE, screenbmp);
    free(screenbmp);
    glFlush();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

private:
  size_t width_;
  size_t height_;

  GLFWwindow *window_;
  Joypad *joypad_;
  Color palettes_[4];

  // std::mutex frame_mutex_;
  SpinLock frame_mutex_;
  std::list<UPtr<PixelMap>> frame_list_;
};

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "invalid file path args" << std::endl;
    return -2;
  }

  auto debug_mode = true;
  for (auto i = 1; i < argc; ++i)
  {
    if (!strcmp(argv[i], "-n"))
    {
      debug_mode = false;
    }
  }

  SPtr<BootstrapROM> bsr(new BootstrapROM(bootstrap_rom_data));

  std::string rom_path(argv[1]);
  auto cartridge = CartridgeLoader::load(rom_path);
  if (!cartridge)
  {
    std::cerr << "failed to open (" << rom_path.c_str() << ") " << std::endl;
    return -1;
  }

  debug_log("Cartridge Header:%s", cartridge->header()->description().c_str());

  SPtr<LCDDisplayer> displayer(new GLDisplayer(WINDOW_WIDTH, WINDOW_HEIGHT));

  VirtualMachine vm(bsr, cartridge, displayer, debug_mode);
  vm.connect_all_components();
  // vm.run();
  std::thread t(&VirtualMachine::run, &vm);
  displayer->run();
  t.join();

  return 0;
}