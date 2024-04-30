#include "sim/utils.hpp"

#include <gtest/gtest.h>


using namespace fluids::sim;

TEST(Vector3DTest, Addition) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  Vector3D const result = vector1 + vector2;
  EXPECT_EQ(result.x, 5);
  EXPECT_EQ(result.y, 7);
  EXPECT_EQ(result.z, 9);
}

TEST(Vector3DTest, Subtraction) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  Vector3D const result = vector1 - vector2;
  EXPECT_EQ(result.x, -3);
  EXPECT_EQ(result.y, -3);
  EXPECT_EQ(result.z, -3);
}

TEST(Vector3DTest, ScalarMultiplication) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector(1, 2, 3);
  Vector3D const result = vector * 2;
  EXPECT_EQ(result.x, 2);
  EXPECT_EQ(result.y, 4);
  EXPECT_EQ(result.z, 6);
}

TEST(Vector3DTest, ScalarDivision) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector(2, 4, 6);
  Vector3D const result = vector / 2;
  EXPECT_EQ(result.x, 1);
  EXPECT_EQ(result.y, 2);
  EXPECT_EQ(result.z, 3);
}

TEST(Vector3DTest, DotProduct) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  double const result = vector1.dot(vector2);
  EXPECT_EQ(result, 32);
}

TEST(Vector3DTest, Normalization) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector(1, 2, 2);
  Vector3D const result = vector.normalized();
  EXPECT_EQ(result.x, 1.0 / 3);
  EXPECT_EQ(result.y, 2.0 / 3);
  EXPECT_EQ(result.z, 2.0 / 3);
}

TEST(Vector3DTest, Assignment) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  Vector3D const vector2 = vector1;
  EXPECT_EQ(vector2.x, 1);
  EXPECT_EQ(vector2.y, 2);
  EXPECT_EQ(vector2.z, 3);
}

TEST(Vector3DTest, UnaryMinus) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector(1, 2, 3);
  Vector3D const result = -vector;
  EXPECT_EQ(result.x, -1);
  EXPECT_EQ(result.y, -2);
  EXPECT_EQ(result.z, -3);
}

TEST(Vector3DTest, CompoundAddition) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  vector1 += vector2;
  EXPECT_EQ(vector1.x, 5);
  EXPECT_EQ(vector1.y, 7);
  EXPECT_EQ(vector1.z, 9);
}

TEST(Vector3DTest, CompoundSubtraction) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  vector1 -= vector2;
  EXPECT_EQ(vector1.x, -3);
  EXPECT_EQ(vector1.y, -3);
  EXPECT_EQ(vector1.z, -3);
}

TEST(Vector3DTest, CompoundMultiplication) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D vector(1, 2, 3);
  vector *= 2;
  EXPECT_EQ(vector.x, 2);
  EXPECT_EQ(vector.y, 4);
  EXPECT_EQ(vector.z, 6);
}

TEST(Vector3DTest, CompoundDivision) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D vector(2, 4, 6);
  vector /= 2;
  EXPECT_EQ(vector.x, 1);
  EXPECT_EQ(vector.y, 2);
  EXPECT_EQ(vector.z, 3);
}

TEST(Vector3DTest, Equality) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(1, 2, 3);
  EXPECT_TRUE(vector1 == vector2);
}

TEST(Vector3DTest, Inequality) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector1(1, 2, 3);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
  Vector3D const vector2(4, 5, 6);
  EXPECT_TRUE(vector1 != vector2);
}