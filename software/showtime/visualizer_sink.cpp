#include "showtime/visualizer_sink.h"

#include <Eigen/Dense>

namespace showtime {

namespace ng = nanogui;

VisualizerSink::VisualizerSink() {
  makeTables();
}

VisualizerSink::~VisualizerSink() {
  table_shader_.free();
}

void VisualizerSink::sink(ColorChannels channels) {
  table_shader_.bind();
  table_shader_.drawIndexed(GL_TRIANGLES, 0, 2 * 13);
}

void VisualizerSink::makeTables() {
  if (!table_shader_.initFromFiles("color_rect",
                              "shaders/color_rect.vs",
                              "shaders/color_rect.fs")) {
    std::cout << "Failied to load color_rect shader!" << std::endl;
  }

  constexpr float table_width = 0.35f;
  constexpr float table_height = 0.3f;
  constexpr int num_tables = 13;

  ng::MatrixXf verts(2, 4 * num_tables);
  ng::MatrixXf colors(3, 4 * num_tables);
  ng::MatrixXu indices(3, 2 * num_tables);
  for (int i = 0; i < num_tables; ++ i) {
    int row = i % 4;
    int col = i / 4;
    float x1 = 0.6f - (col * (table_width + 0.05f));
    float y1 = 0.6f - (row * (table_height + 0.15f));
    float x2 = x1 + table_width;
    float y2 = y1 + table_height;

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

} // namespace showtime
