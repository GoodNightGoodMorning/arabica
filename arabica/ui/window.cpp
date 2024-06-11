#include <arabica/ui/window.hpp>
#include <arabica/driver/keypad.hpp>
#include <fmt/core.h>

namespace arabica {

Uint32 _ontick(Uint32 interval, void* userdata) {
  return (static_cast<Window*>(userdata))->ontick(interval, userdata);
}

Window::Window(const std::string& title, const int width, const int height) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    fmt::print("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _window = SDL_CreateWindow(title.c_str(),           //
                             SDL_WINDOWPOS_UNDEFINED, //
                             SDL_WINDOWPOS_UNDEFINED, //
                             width,                   //
                             height,                  //
                             SDL_WINDOW_SHOWN);       //
  if (_window == nullptr) {
    fmt::print("Window could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (_renderer == nullptr) {
    fmt::print("Renderer could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _timer_id = SDL_AddTimer(2, _ontick, this);
}

Window::~Window() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::execute() {
  _running = true;
  while (_running) {
    while (SDL_PollEvent(&_event)) {
      switch (_event.type) {
        case SDL_QUIT: {
          SDL_RemoveTimer(_timer_id);
          _running = false;
        } break;
        case SDL_WINDOWEVENT: {
          if (_event.window.event == SDL_WINDOWEVENT_CLOSE) {
            SDL_RemoveTimer(_timer_id);
          }
        } break;
        case SDL_KEYDOWN: on_keyboard(_event.key.keysym.sym); break;
        default: break;
      }
    }
    on_render();
  }
}

void Window::on_keyboard(const SDL_Keycode keycode) {
  switch (keycode) {
    case SDLK_0: emulator.keypad.keydown_code = 0x0; break;
    case SDLK_1: emulator.keypad.keydown_code = 0x1; break;
    case SDLK_2: emulator.keypad.keydown_code = 0x2; break;
    case SDLK_3: emulator.keypad.keydown_code = 0x3; break;
    case SDLK_4: emulator.keypad.keydown_code = 0x4; break;
    case SDLK_5: emulator.keypad.keydown_code = 0x5; break;
    case SDLK_6: emulator.keypad.keydown_code = 0x6; break;
    case SDLK_7: emulator.keypad.keydown_code = 0x7; break;
    case SDLK_8: emulator.keypad.keydown_code = 0x8; break;
    case SDLK_9: emulator.keypad.keydown_code = 0x9; break;
    case SDLK_a: emulator.keypad.keydown_code = 0xA; break;
    case SDLK_b: emulator.keypad.keydown_code = 0xB; break;
    case SDLK_c: emulator.keypad.keydown_code = 0xC; break;
    case SDLK_d: emulator.keypad.keydown_code = 0xD; break;
    case SDLK_e: emulator.keypad.keydown_code = 0xE; break;
    case SDLK_f: emulator.keypad.keydown_code = 0xF; break;
    default: break;
  }
}

void Window::on_render() {
  SDL_RenderClear(_renderer);
  SDL_SetRenderDrawColor(_renderer, 0x33, 0x99, 0x66, 0xFF);
  SDL_RenderPresent(_renderer);
}

Uint32 Window::ontick(Uint32 interval, void* userdata) {
  //
  // TODO: Implementation
  //
  // Note, this happens on a separate thread
  //
  // 1. Check if there were any keystroke (which could happen on the UI thread at the same time, synchronization
  //    required), if so, tell the CPU that's the case
  // 2. Execute a batch of CPU instructions (Potentially just one)
  // 3. Check if there are update to display, if so, tell SDL to redraw (how - remember this is a separate thread)
  // 4. Handles any other CPU/Window interactions.
  //
  // This design should work even if this is executed on the UI thread, but this is not the case for SDL.
  //
  emulator.run();
  return interval;
}

} // namespace arabica
