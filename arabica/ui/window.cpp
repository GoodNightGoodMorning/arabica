#include <arabica/ui/window.hpp>
#include <arabica/log/log.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace arabica {

Uint32 _on_tick(const Uint32 interval, void* userdata) {
  return (static_cast<Window*>(userdata))->on_tick(interval, userdata);
}

Window::Window(const std::string& title, const int width, const int height, const std::string& rom) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
    ARABICA_LOG_INFO("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  if (!emulator.init()) {
    ARABICA_LOG_INFO("{}\n", "Failed to initialize emulator");
    std::exit(1);
  }

  if (!emulator.load(rom)) {
    ARABICA_LOG_INFO("{}\n", "Failed to load rom");
    std::exit(1);
  }

  emulator.display.init(width, height, 10);

  _width  = width;
  _height = height;

  _window = SDL_CreateWindow(title.c_str(),           //
                             SDL_WINDOWPOS_UNDEFINED, //
                             SDL_WINDOWPOS_UNDEFINED, //
                             width,                   //
                             height,                  //
                             SDL_WINDOW_SHOWN);       //
  if (_window == nullptr) {
    ARABICA_LOG_INFO("Window could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (_renderer == nullptr) {
    ARABICA_LOG_INFO("Renderer could not be created! SDL_Error: {}\n", SDL_GetError());
    std::exit(1);
  }

  _texture = SDL_CreateTexture(_renderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STATIC,
                               emulator.display.window_width,
                               emulator.display.window_height);

  _timer_id = SDL_AddTimer(emulator.milliseconds_per_frame, //
                           _on_tick,                        // every `t` milliseonds will execute `_on_tick`
                           this);                           //
}

Window::~Window() {
  SDL_DestroyTexture(_texture);
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::execute() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(
    [](void* arg) {
      static_cast<Window*>(arg)->polling();
    },
    this,
    0,
    true);
#else
  _running = true;
  while (_running) {
    polling();
  }
#endif
}

void Window::polling() {
  while (SDL_PollEvent(&_event)) {
    switch (_event.type) {
      case SDL_QUIT: _running = false; break;
      case SDL_WINDOWEVENT: {
        if (_event.window.event == SDL_WINDOWEVENT_CLOSE) {
          if (SDL_FALSE == SDL_RemoveTimer(_timer_id)) {
            ARABICA_LOG_INFO("Failed to remove timer! SDL_Error: {}\n", SDL_GetError());
            std::exit(1);
          }
        }
      } break;
      case SDL_KEYDOWN: on_keyboard(_event.key.keysym.sym, true); break;
      case SDL_KEYUP: on_keyboard(_event.key.keysym.sym, false); break;
      default: break;
    }
  }
  on_render();
}

void Window::on_keyboard(const SDL_Keycode keycode, const bool is_pressed) {
  int chip8_keycode = -1;
  switch (keycode) {
    case SDLK_0: chip8_keycode = 0x00; break;
    case SDLK_1: chip8_keycode = 0x01; break;
    case SDLK_2: chip8_keycode = 0x02; break;
    case SDLK_3: chip8_keycode = 0x03; break;
    case SDLK_4: chip8_keycode = 0x04; break;
    case SDLK_5: chip8_keycode = 0x05; break;
    case SDLK_6: chip8_keycode = 0x06; break;
    case SDLK_7: chip8_keycode = 0x07; break;
    case SDLK_8: chip8_keycode = 0x08; break;
    case SDLK_9: chip8_keycode = 0x09; break;
    case SDLK_a: chip8_keycode = 0x0A; break;
    case SDLK_b: chip8_keycode = 0x0B; break;
    case SDLK_c: chip8_keycode = 0x0C; break;
    case SDLK_d: chip8_keycode = 0x0D; break;
    case SDLK_e: chip8_keycode = 0x0E; break;
    case SDLK_f: chip8_keycode = 0x0F; break;
    default: break;
  }

  if (chip8_keycode != -1) {
    if (is_pressed) {
      emulator.keypad.on_keydown(chip8_keycode);
    } else {
      emulator.keypad.on_keyup(chip8_keycode);
    }
  }
}

void Window::on_render() {
  if (emulator.display.is_refresh) {
    SDL_RenderClear(_renderer);
    SDL_UpdateTexture(_texture, nullptr, emulator.display.pixels, emulator.display.window_width * sizeof(uint32_t));
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    emulator.display.is_refresh = false;
  }
}

Uint32 Window::on_tick(const Uint32 interval, void* userdata) {
  emulator.execute();
  return interval;
}

} // namespace arabica
