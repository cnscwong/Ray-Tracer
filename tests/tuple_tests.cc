#include <gtest/gtest.h>
#include "Tuple.h"
#include "common.h"

TEST(TupleTest, BasicTest){
  Tuple a(4.3, -4.2, 3.1, 1.0);
  EXPECT_NEAR(a.x, 4.3, EPSILON);
  EXPECT_NEAR(a.y, -4.2, EPSILON);
  EXPECT_NEAR(a.z, 3.1, EPSILON);
  EXPECT_NEAR(a.point, 1.0, EPSILON);

  Tuple b(4.3, -4.2, 3.1, 0.0);
  EXPECT_NEAR(b.x, 4.3, EPSILON);
  EXPECT_NEAR(b.y, -4.2, EPSILON);
  EXPECT_NEAR(b.z, 3.1, EPSILON);
  EXPECT_NEAR(b.point, 0.0, EPSILON);
}

TEST(PointTest, TestingIsPoint){
  Point a(4.3, -4.2, 3.1);
  EXPECT_NEAR(a.x, 4.3, EPSILON);
  EXPECT_NEAR(a.y, -4.2, EPSILON);
  EXPECT_NEAR(a.z, 3.1, EPSILON);
  EXPECT_NEAR(a.point, 1.0, EPSILON);
}

TEST(VectorTest, TestingIsNotPoint){
  Vector a(4.3, -4.2, 3.1);
  EXPECT_NEAR(a.x, 4.3, EPSILON);
  EXPECT_NEAR(a.y, -4.2, EPSILON);
  EXPECT_NEAR(a.z, 3.1, EPSILON);
  EXPECT_NEAR(a.point, 0.0, EPSILON);
}

TEST(TupleOperations, TupleEquality){
  Tuple a(4.3, -4.2, 3.1, 1.0);
  Tuple b(4.3, -4.2, 3.1, 1.0);

  EXPECT_TRUE(a.isEqual(b));

  b = Tuple(0, -4.2, 3.1, 1.0);
  EXPECT_FALSE(a.isEqual(b));

  b = Tuple(4.3, 0, 3.1, 1.0);
  EXPECT_FALSE(a.isEqual(b));

  b = Tuple(4.3, -4.2, 0, 1.0);
  EXPECT_FALSE(a.isEqual(b));

  b = Tuple(4.3, -4.2, 3.1, 0);
  EXPECT_FALSE(a.isEqual(b));
}

TEST(TupleOperations, TupleAddition){
  Point a(4.3, -4.2, 3.1);
  Vector b(4.3, -4.2, 3.1);

  EXPECT_TRUE((a + b).isEqual(Point(8.6, -8.4, 6.2)));

  Vector c(6.2, 2.1, -5.8);

  EXPECT_TRUE((b + c).isEqual(Vector(10.5, -2.1, -2.7)));

  Tuple d(3, -2, 5, 1);
  Tuple e(-2, 3, 1, 0);
  EXPECT_TRUE((d + e).isEqual(Tuple(1, 1, 6, 1)));
}

TEST(TupleOperations, TupleSubtraction){
  Point a(3, 2, 1);
  Point b(5, 6, 7);

  EXPECT_TRUE((a - b).isEqual(Vector(-2, -4, -6)));

  Vector c(5, 6, 7);
  EXPECT_TRUE((a - c).isEqual(Point(-2, -4, -6)));

  Vector d(8, 9, 2);
  EXPECT_TRUE((c - d).isEqual(Vector(-3, -3, 5)));
}

TEST(TupleOperations, TupleNegation){
  Tuple a(1, -2, 3, -4);

  EXPECT_TRUE(a.negateTuple().isEqual(Tuple(-1, 2, -3, 4)));
}

TEST(TupleOperations, TupleMultiplyDivide){
  Tuple a(1, -2, 3, -4);

  EXPECT_TRUE((a*3.5).isEqual(Tuple(3.5, -7, 10.5, -14)));
  EXPECT_TRUE((a*0.5).isEqual(Tuple(0.5, -1, 1.5, -2)));
  EXPECT_TRUE((a/2).isEqual(Tuple(0.5, -1, 1.5, -2)));
}

TEST(VectorOperations, VectorMagnitude){
  EXPECT_NEAR(Vector(0, 0, 0).magnitude(), 0, EPSILON);
  EXPECT_NEAR(Vector(1, 0, 0).magnitude(), 1, EPSILON);
  EXPECT_NEAR(Vector(0, 1, 0).magnitude(), 1, EPSILON);
  EXPECT_NEAR(Vector(0, 0, 1).magnitude(), 1, EPSILON);
  EXPECT_NEAR(Vector(1, 2, 3).magnitude(), sqrt(14), EPSILON);
  EXPECT_NEAR(Vector(-1, -2, -3).magnitude(), sqrt(14), EPSILON);
}

TEST(VectorOperations, VectorNormalize){
  Vector a(4, 0, 0);
  a.normalize();
  EXPECT_TRUE(a.isEqual(Vector(1, 0, 0)));

  Vector b(1, 2, 3);
  b.normalize();
  EXPECT_TRUE(b.isEqual(Vector(0.26726, 0.53452, 0.80178)));
  EXPECT_NEAR(b.magnitude(), 1, EPSILON);
}

TEST(VectorOperations, VectorDotProduct){
  EXPECT_NEAR(dotProduct(Vector(1,2,3), Vector(2,3,4)), 20, EPSILON);
}

TEST(VectorOperations, VectorCrossProduct){
  Vector a(1, 2, 3);
  Vector b(2, 3, 4);
  EXPECT_TRUE(crossProduct(a, b).isEqual(Vector(-1, 2, -1)));
  EXPECT_TRUE(crossProduct(b, a).isEqual(Vector(1, -2, 1)));
}