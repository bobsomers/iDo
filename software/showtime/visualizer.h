#pragma once

#include <cstdint>
#include <vector>

#include <nanogui/nanogui.h>

#include "showtime/types.h"

namespace showtime {

namespace ng = nanogui;

class TableId {
public:
  explicit TableId(std::size_t index) : index_(index) {}
  std::size_t index() const { return index_; }

private:
  std::size_t index_;
};

class Visualizer {
public:
  Visualizer();
  ~Visualizer();

  TableId addTable(Vec2f position, Vec2f size);
  void setColor(TableId table, Vec3f color);
  void draw(float aspect_ratio);

private:
  ng::GLShader shader_;
  std::vector<Vec2f> positions_;
  std::vector<Vec2f> sizes_;
  std::vector<Vec3f> colors_;
};

} // namespace showtime
