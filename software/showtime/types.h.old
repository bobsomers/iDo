#pragma once

#include <Eigen/Dense>

namespace showtime {

template <typename T>
struct Vec2 {
  Vec2(T x, T y) : x(x), y(y) {}
  Eigen::Matrix<T, 2, 1> eigen() { return Eigen::Matrix<T, 2, 1>(x, y); }
  T x;
  T y;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

template <typename T>
struct Vec3 {
  Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
  Eigen::Matrix<T, 3, 1> eigen() { return Eigen::Matrix<T, 3, 1>(x, y, z); }
  T x;
  T y;
  T z;
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;

template <typename T>
struct Vec4 {
  Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
  Eigen::Matrix<T, 4, 1> eigen() { return Eigen::Matrix<T, 4, 1>(x, y, z, w); }
  T x;
  T y;
  T z;
  T w;
};

using Vec4i = Vec4<int>;
using Vec4f = Vec4<float>;

} // namespace
