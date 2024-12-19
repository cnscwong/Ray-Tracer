#include <gtest/gtest.h>
#include "Tuple.h"

const float EPSILON = 0.0001f;

TEST(TupleTest, BasicTest){
  Tuple a = Tuple(4.3, -4.2, 3.1, 1.0);
  EXPECT_NEAR(a.x, 4.3, EPSILON);
  EXPECT_NEAR(a.y, -4.2, EPSILON);
  EXPECT_NEAR(a.z, 3.1, EPSILON);
  EXPECT_NEAR(a.isPoint(), 1.0, EPSILON);

  Tuple b = Tuple(4.3, -4.2, 3.1, 0.0);
  EXPECT_NEAR(b.x, 4.3, EPSILON);
  EXPECT_NEAR(b.y, -4.2, EPSILON);
  EXPECT_NEAR(b.z, 3.1, EPSILON);
  EXPECT_NEAR(b.point, 0.0, EPSILON);
}

TEST(PointTest, TestingIsPoint){
  Point a = Point(4.3, -4.2, 3.1);
  EXPECT_NEAR(a.isPoint(), 1.0, EPSILON);
}

TEST(VectorTest, TestingIsNotPoint){
  Vector a = Vector(4.3, -4.2, 3.1);
  EXPECT_NEAR(a.isPoint(), 0.0, EPSILON);
}