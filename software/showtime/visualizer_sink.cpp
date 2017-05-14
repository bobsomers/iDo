#include "showtime/visualizer_sink.h"

#include <Eigen/Dense>

namespace showtime {

namespace {

constexpr float kTableWidth = 0.35f;
constexpr float kTableHeight = 0.3f;
constexpr int kNumTables = 13;
constexpr float kLightWidth = 0.05f;
constexpr float kLightHeight = 0.1f;
constexpr int kNumLights = 26;

} // namespace

namespace ng = nanogui;

VisualizerSink::VisualizerSink() {
  makeTables();
  makeLights();
}

VisualizerSink::~VisualizerSink() {
  table_shader_.free();
  light_shader_.free();
}

void VisualizerSink::sink(ColorChannels channels) {
  ng::MatrixXf colors(3, 4 * kNumLights);
  for (int i = 0; i < kNumLights; ++i) {
    Color c = channels[i];
    colors.col(4 * i + 0) << c.r, c.g, c.b;
    colors.col(4 * i + 1) << c.r, c.g, c.b;
    colors.col(4 * i + 2) << c.r, c.g, c.b;
    colors.col(4 * i + 3) << c.r, c.g, c.b;
  }

  table_shader_.bind();
  table_shader_.drawIndexed(GL_TRIANGLES, 0, 2 * kNumTables);
  light_shader_.bind();
  light_shader_.uploadAttrib("vertex_color", colors);
  light_shader_.drawIndexed(GL_TRIANGLES, 0, 2 * kNumLights);
}

void VisualizerSink::makeTables() {
  if (!table_shader_.initFromFiles("color_rect",
                              "shaders/color_rect.vs",
                              "shaders/color_rect.fs")) {
    std::cout << "Failied to load color_rect shader!" << std::endl;
  }

  ng::MatrixXf verts(2, 4 * kNumTables);
  ng::MatrixXf colors(3, 4 * kNumTables);
  ng::MatrixXu indices(3, 2 * kNumTables);
  for (int i = 0; i < kNumTables; ++ i) {
    int row = i % 4;
    int col = i / 4;
    float x1 = 0.6f - (col * (kTableWidth + 0.05f));
    float y1 = 0.6f - (row * (kTableHeight + 0.15f));
    float x2 = x1 + kTableWidth;
    float y2 = y1 + kTableHeight;

    verts.col(4 * i + 0) << x1, y1;
    verts.col(4 * i + 1) << x2, y1;
    verts.col(4 * i + 2) << x2, y2;
    verts.col(4 * i + 3) << x1, y2;

    colors.col(4 * i + 0) << 0.5412f, 0.3529f, 0.1843f;
    colors.col(4 * i + 1) << 0.6412f, 0.4529f, 0.2843f;
    colors.col(4 * i + 2) << 0.6412f, 0.4529f, 0.2843f;
    colors.col(4 * i + 3) << 0.5412f, 0.3529f, 0.1843f;

    indices.col(2 * i + 0) << (4 * i + 0), (4 * i + 1), (4 * i + 2);
    indices.col(2 * i + 1) << (4 * i + 2), (4 * i + 3), (4 * i + 0);
  }

  table_shader_.bind();
  table_shader_.uploadIndices(indices);
  table_shader_.uploadAttrib("vertex_position", verts);
  table_shader_.uploadAttrib("vertex_color", colors);
}

void VisualizerSink::makeLights() {
  if (!light_shader_.initFromFiles("color_rect",
                              "shaders/color_rect.vs",
                              "shaders/color_rect.fs")) {
    std::cout << "Failied to load color_rect shader!" << std::endl;
  }

  ng::MatrixXf verts(2, 4 * kNumLights);
  ng::MatrixXu indices(3, 2 * kNumLights);
  for (int i = 0; i < kNumLights; ++i) {
    int row = (i / 2) % 4;
    int col = (i / 2) / 4;
    float x1 = 0.6f + (0.67f * kTableWidth) - (col * (kTableWidth + 0.05f));
    float y1 = 0.6f + (0.34f * kTableHeight) - (row * (kTableHeight + 0.15f));
    float x2 = x1 + kLightWidth;
    float y2 = y1 + kLightHeight;

    verts.col(4 * i + 0) << x1, y1;
    verts.col(4 * i + 1) << x2, y1;
    verts.col(4 * i + 2) << x2, y2;
    verts.col(4 * i + 3) << x1, y2;

    indices.col(2 * i + 0) << (4 * i + 0), (4 * i + 1), (4 * i + 2);
    indices.col(2 * i + 1) << (4 * i + 2), (4 * i + 3), (4 * i + 0);

    ++i;

    x1 = x1 - (kTableWidth * 0.5f);
    x2 = x1 + kLightWidth;
    y2 = y1 + kLightHeight;

    verts.col(4 * i + 0) << x1, y1;
    verts.col(4 * i + 1) << x2, y1;
    verts.col(4 * i + 2) << x2, y2;
    verts.col(4 * i + 3) << x1, y2;

    indices.col(2 * i + 0) << (4 * i + 0), (4 * i + 1), (4 * i + 2);
    indices.col(2 * i + 1) << (4 * i + 2), (4 * i + 3), (4 * i + 0);
  }

  light_shader_.bind();
  light_shader_.uploadIndices(indices);
  light_shader_.uploadAttrib("vertex_position", verts);
}

} // namespace showtime
