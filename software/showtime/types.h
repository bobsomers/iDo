#pragma once

#include <array>
#include <cstdint>

namespace showtime {

struct Color {
  Color() : r(0.0f), g(0.0f), b(0.0f) {}
  Color(float r, float g, float b) : r(r), g(g), b(b) {}

  float r;
  float g;
  float b;
};

struct ByteColor {
  ByteColor() : r(0), g(0), b(0) {}
  explicit ByteColor(Color c)
    : r(c.r * 255.0f), g(c.g * 255.0f), b(c.b * 255.0f) {}

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

constexpr int kNumChannels = 26;
using ColorChannels = std::array<Color, kNumChannels>;
using ByteColorChannels = std::array<ByteColor, kNumChannels>;

inline ByteColorChannels quantizeChannels(ColorChannels channels) {
  ByteColorChannels bytes;
  static_assert(channels.size() == bytes.size(),
      "Color and ByteColor have different number of channels!");
  for (std::size_t i = 0; i < channels.size(); ++i) {
    bytes[i] = ByteColor(channels[i]);
  }
  return bytes;
}

} // namespace showtime
