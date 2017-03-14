#include "showtime/visualizer.h"

#include <iostream>

namespace showtime {

Visualizer::Visualizer() {
  shader_.initFromFiles("table", "shaders/table.vs", "shaders/table.fs");

  ng::MatrixXu indices(3, 2);
  indices.col(0) << 0, 1, 2;
  indices.col(1) << 2, 3, 0;

  ng::MatrixXf verts(2, 4);
  verts.col(0) << -0.5f, -0.5f;
  verts.col(1) << 0.5f, -0.5f;
  verts.col(2) << 0.5f, 0.5f;
  verts.col(3) << -0.5f, 0.5f;

  shader_.bind();
  shader_.uploadIndices(indices);
  shader_.uploadAttrib("position", verts);
}

Visualizer::~Visualizer() {
  shader_.free();
}

TableId Visualizer::addTable(Vec2f position, Vec2f size) {
  positions_.push_back(position);
  sizes_.push_back(size);
  colors_.push_back(Vec3f(1.0f, 1.0f, 0.0f));
  return TableId(positions_.size() - 1);
}

void Visualizer::draw(float aspect_ratio) {
  std::cout << "aspect_ratio = " << aspect_ratio << "\n";
  shader_.bind();

  for (std::size_t i =  0; i < positions_.size(); ++i) {
    Eigen::Affine3f t;
    t = Eigen::Scaling(0.5f);

    //ng::Matrix4f mvp;
    //mvp.setIdentity();
    //mvp.row(0) *= aspect_ratio;

    shader_.setUniform("mvp", t.matrix());
    shader_.setUniform("clr", colors_[i].eigen());
    shader_.drawIndexed(GL_TRIANGLES, 0, 2);
  }
}

} // namespace showtime
