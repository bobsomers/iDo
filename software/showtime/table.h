#pragma once

#include "nanogui/nanogui.h"

namespace showtime {

namespace ng = nanogui;

class Table {
public:
  Table(ng::Vector2i position, ng::Vector2i size);
  ~Table();

  void draw();

private:
  ng::Vector2i position_;
  ng::Vector2i size_;
  ng::GLShader shader_;
};

} // namespace showtime
