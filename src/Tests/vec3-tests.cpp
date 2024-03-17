#include <gtest/gtest.h>
#include "../RayOn/Vec3.h"

TEST(Vec3Test, Constructor) {
    RN::vec3 v(1.5f, -2.5f, 3.5f);
    ASSERT_NEAR(v.getX(), 1.5, 1e-6);
    ASSERT_NEAR(v.getY(), -2.5, 1e-6);
    ASSERT_NEAR(v.getZ(), 3.5, 1e-6);

    RN::vec3 v2(RN::ToFixed(1), RN::ToFixed(5), RN::ToFixed(8));
    ASSERT_NEAR(v2.getX(), 1, 1e-6);
    ASSERT_NEAR(v2.getY(), 5, 1e-6);
    ASSERT_NEAR(v2.getZ(), 8, 1e-6);

    RN::vec3 v3(v2);
    ASSERT_NEAR(v3.getX(), 1, 1e-6);
    ASSERT_NEAR(v3.getY(), 5, 1e-6);
    ASSERT_NEAR(v3.getZ(), 8, 1e-6);
}

TEST(Vec3Test, AdditionSubtraction) {
    RN::vec3 v1(1.0f, 2.0, 3.0);
    RN::vec3 v2(0.5f, -0.5, 1.0);

    RN::vec3 result = v1 + v2;
    ASSERT_NEAR(result.getX(), 1.5, 1e-6);
    ASSERT_NEAR(result.getY(), 1.5, 1e-6);
    ASSERT_NEAR(result.getZ(), 4.0, 1e-6);

    result = v1 - v2;
    ASSERT_NEAR(result.getX(), 0.5, 1e-6);
    ASSERT_NEAR(result.getY(), 2.5, 1e-6);
    ASSERT_NEAR(result.getZ(), 2.0, 1e-6);

    result = v1 + 0.5;
    ASSERT_NEAR(result.getX(), 1.5, 1e-6);
    ASSERT_NEAR(result.getY(), 2.5, 1e-6);
    ASSERT_NEAR(result.getZ(), 3.5, 1e-6);

    result = result - 0.5;
    ASSERT_NEAR(result.getX(), 1.0, 1e-6);
    ASSERT_NEAR(result.getY(), 2.0, 1e-6);
    ASSERT_NEAR(result.getZ(), 3.0, 1e-6);
}

TEST(Vec3Test, MultiplicationDivision) {
    RN::vec3 v(1.0f, -2.0, 0.5);

    RN::vec3 multiplied = v * 2.0f;
    ASSERT_NEAR(multiplied.getX(), 2.0, 1e-6);
    ASSERT_NEAR(multiplied.getY(), -4.0, 1e-6);
    ASSERT_NEAR(multiplied.getZ(), 1.0, 1e-6);

    multiplied = v * 2ll;
    ASSERT_NEAR(multiplied.getX(), 2.0, 1e-6);
    ASSERT_NEAR(multiplied.getY(), -4.0, 1e-6);
    ASSERT_NEAR(multiplied.getZ(), 1.0, 1e-6);

    RN::vec3 divided = multiplied / 2.0f;
    ASSERT_NEAR(divided.getX(), 1.0, 1e-6);
    ASSERT_NEAR(divided.getY(), -2.0, 1e-6);
    ASSERT_NEAR(divided.getZ(), 0.5, 1e-6);

    divided = multiplied / 2ll;
    ASSERT_NEAR(divided.getX(), 1.0, 1e-6);
    ASSERT_NEAR(divided.getY(), -2.0, 1e-6);
    ASSERT_NEAR(divided.getZ(), 0.5, 1e-6);

    v.multiply(2.0f);
    ASSERT_NEAR(v.getX(), 2.0, 1e-6);
    ASSERT_NEAR(v.getY(), -4.0, 1e-6);
    ASSERT_NEAR(v.getZ(), 1.0, 1e-6);

    v.multiply(2LL);
    ASSERT_NEAR(v.getX(), 4.0, 1e-6);
    ASSERT_NEAR(v.getY(), -8.0, 1e-6);
    ASSERT_NEAR(v.getZ(), 2.0, 1e-6);
}

TEST(Vec3Test, DotProductAndLength) {
    RN::vec3 v1(1.0f, 0.0, 0.0);
    RN::vec3 v2(0.0f, 1.0f, 0.0f);
    float dot = RN::vec3::dot(v1, v2);
    ASSERT_NEAR(dot, 0.0, 1e-6);

    float dotFixed = RN::FromFixed(RN::vec3::dotFix(v1, v2));
    ASSERT_NEAR(dot, 0.0, 1e-6);

    float length = v1.length();
    ASSERT_NEAR(length, 1.0, 1e-6);
}

TEST(Vec3Test, Distance) {
    RN::vec3 v1(1.0f, 2.0, 3.0);
    RN::vec3 v2(4.0f, 5.0, 6.0);
    float distance = RN::vec3::distance(v1, v2);
    ASSERT_NEAR(distance, std::sqrt(27.0), 1e-6);
}
