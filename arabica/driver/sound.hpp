#pragma once

#include <SDL2/SDL.h>
#include <cmath>

namespace arabica {

class Sound {
public:
  Sound()
    : rate(0)
    , _device(0) {
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
      SDL_Init(SDL_INIT_AUDIO);
    }

    // The following comments source from: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.5
    //
    // Chip-8 provides a sound timer.
    //
    // The sound timer is active whenever the sound timer register (ST) is non-zero.
    // This timer also decrements at a rate of 60Hz, however,
    // as long as ST's value is greater than zero, the Chip-8 buzzer will sound.
    // When ST reaches zero, the sound timer deactivates.
    //
    // The sound produced by the Chip-8 interpreter has only one tone.
    // The frequency of this tone is decided by the author of the interpreter.
    //
    SDL_AudioSpec desired_spec;
    {
      SDL_zero(desired_spec);
      desired_spec.freq     = 60;           // decrements at a rate of 60Hz
      desired_spec.format   = AUDIO_F32SYS; // 32-bit float format
      desired_spec.channels = 1;            // mono
      desired_spec.samples  = 4096;         // Buffer size in samples per channel
      desired_spec.callback = []([[maybe_unused]] void* userdata, Uint8* stream, int len) {
        auto* const fstream        = reinterpret_cast<float*>(stream);
        const float amplitude      = 0.5f;
        const float sample_rate    = 1 / 60; // f = 1 / T = 1 / 60 (hz)
        const float tone_frequency = 440.0f; // A4 note, https://en.wikipedia.org/wiki/A440_(pitch_standard)
        for (int i = 0; i < len / sizeof(float); ++i) {
          const auto omega = 2.0f * M_PI * tone_frequency; // angular speed
          const auto scale = (i / sample_rate);
          fstream[i]       = amplitude * sinf(omega * scale);
        }
      };
    }
    //
    // _device = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, &_spec, 0);
    // if (_device == 0) {
    //   SDL_Log("Failed to open audio: %s", SDL_GetError());
    //   SDL_Quit();
    // }
  }

  ~Sound() {
    if (_device != 0) {
      SDL_CloseAudioDevice(_device);
    }
    SDL_Quit();
  }

  void start_beep() {
    if (_device != 0) {
      SDL_PauseAudioDevice(_device, 0);
    }
  }

  void stop_beep() {
    if (_device != 0) {
      SDL_PauseAudioDevice(_device, 1);
    }
  }

  int rate = 0;

private:
  SDL_AudioDeviceID _device{0};
  SDL_AudioSpec     _spec{};
};

} // namespace arabica