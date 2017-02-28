#include "showtime/table.h"

namespace showtime {

Table::Table(ng::Vector2i position, ng::Vector2i size)
  : position_(position), size_(size) {
  shader_.initFromFiles("table", "shaders/table.vs", "shaders/table.fs");

  ng::MatrixXu indices(3, 2);
  indices.col(0) << 0, 1, 2;
  indices.col(1) << 2, 3, 0;

  ng::MatrixXu positions(2, 4);
  positions.col(0) << -1, -1;
  positions.col(1) << 1, -1;
  positions.col(2) << 1, 1;
  positions.col(3) << -1, 1;

  shader_.bind();
  shader_.uploadIndices(indices);
  shader_.uploadAttrib("position", positions);
  shader_.setUniform("intensity", 0.5f);
}

void Table::draw(ng::Vector2i window_size) {
  shader_.bind();

  ng::Matrix4f mvp;
  mvp.setIdentity();
  mvp.topLeftCorner<3, 3>() = ng::Matrix3f(Eigen::AngleAxisf((float)glfwGetTime(), ng::Vector3f::UnitZ())) * 0.25f;
  mvp.row(0) *= (float)window_size.y() / window_size.x();

  shader_.setUniform("mvp", mvp);
  shader_.drawIndexed(GL_TRIANGLES, 0, 2);
}

Table::~Table() {
  shader_.free();
}

} // namespace showtime
