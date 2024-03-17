#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../RayOn/Vec3.h"

    TEST(Vec3Test, Constructor) {
        RN::vec2 v(1.5f, -2.5f);
        ASSERT_NEAR(v.getX(), 1.5, 1e-6);
        ASSERT_NEAR(v.getY(), -2.5, 1e-6);

        RN::vec2 v2(RN::ToFixed(1), RN::ToFixed(5));
        ASSERT_NEAR(v2.getX(), 1, 1e-6);
        ASSERT_NEAR(v2.getY(), 5, 1e-6);

        RN::vec2 v3(v2);
        ASSERT_NEAR(v3.getX(), 1, 1e-6);
        ASSERT_NEAR(v3.getY(), 5, 1e-6);
    }

    TEST(Vec2Test, AdditionSubtraction) {
        RN::vec2 v1(1.0f, 2.0);
        RN::vec2 v2(0.5f, -0.5);

        RN::vec2 result = v1 + v2;
        ASSERT_NEAR(result.getX(), 1.5, 1e-6);
        ASSERT_NEAR(result.getY(), 1.5, 1e-6);

        result = v1 - v2;
        ASSERT_NEAR(result.getX(), 0.5, 1e-6);
        ASSERT_NEAR(result.getY(), 2.5, 1e-6);

        result = v1 + 0.5;
        ASSERT_NEAR(result.getX(), 1.5, 1e-6);
        ASSERT_NEAR(result.getY(), 2.5, 1e-6);

        result = result - 0.5;
        ASSERT_NEAR(result.getX(), 1.0, 1e-6);
        ASSERT_NEAR(result.getY(), 2.0, 1e-6);
    }

    TEST(Vec2Test, MultiplicationDivision) {
        RN::vec2 v(1.0f, -2.0);

        RN::vec2 multiplied = v * 2.0f;
        ASSERT_NEAR(multiplied.getX(), 2.0, 1e-6);
        ASSERT_NEAR(multiplied.getY(), -4.0, 1e-6);

        multiplied = v * 2ll;
        ASSERT_NEAR(multiplied.getX(), 2.0, 1e-6);
        ASSERT_NEAR(multiplied.getY(), -4.0, 1e-6);

        RN::vec2 divided = multiplied / 2.0f;
        ASSERT_NEAR(divided.getX(), 1.0, 1e-6);
        ASSERT_NEAR(divided.getY(), -2.0, 1e-6);

        divided = multiplied / 2ll;
        ASSERT_NEAR(divided.getX(), 1.0, 1e-6);
        ASSERT_NEAR(divided.getY(), -2.0, 1e-6);

        v.multiply(2.0f);
        ASSERT_NEAR(v.getX(), 2.0, 1e-6);
        ASSERT_NEAR(v.getY(), -4.0, 1e-6);

        v.multiply(2LL);
        ASSERT_NEAR(v.getX(), 4.0, 1e-6);
        ASSERT_NEAR(v.getY(), -8.0, 1e-6);
    }

    TEST(Vec2Test, DotProductAndLength) {
        RN::vec2 v1(1.0f, 0.0);
        RN::vec2 v2(0.0f, 1.0f);
        float dot = RN::vec2::dot(v1, v2);
        ASSERT_NEAR(dot, 0.0, 1e-6);

        RN::vec2 v3(1.0f, 0.0);
        RN::vec2 v4(0.5f, 1.0f);
        dot = RN::vec2::dot(v3, v4);
        ASSERT_NEAR(dot, 0.5, 1e-6);

        float dotFixed = RN::FromFixed(RN::vec2::dotFix(v1, v2));
        ASSERT_NEAR(dotFixed, 0.0, 1e-6);

        dot = RN::vec2::dot({0.0f, 3.4f}, {1.2f, 2.1f});//: 7.14
        ASSERT_NEAR(dot, 7.14, 1e-6);

        RN::vec2 dv1(-10.0f,3.4f);
        RN::vec2 dv2(-1.2f, 3.9f); //25.26
        dot =dv1.dot(dv2);
        ASSERT_NEAR(dot, 25.26, 1e-6);

        RN::vec2 dv3(0.1f,-0.4f);
        RN::vec2 dv4(12.2f, -2.8f); //2.34
        dot = dv3.dot(dv4);
        ASSERT_NEAR(dot, 2.34, 1e-6);

        RN::vec2 dv5(0.7f,3.4f);
        RN::vec2 dv6(1.32f, -23.1f); //-77.616
        dot = dv5.dot(dv6);
        ASSERT_NEAR(dot, -77.616, 1e-6);

        RN::vec2 dvl1(0.0f, 3.4f);
        RN::vec2 dvl2(1.2f, 2.1f);
        float distance = RN::vec2::distance(dvl1, dvl2);
        ASSERT_NEAR(distance, 1.77, 1e-6);

        RN::vec2 dvl3(-10.0f, 3.4f);
        RN::vec2 dvl4(-1.2f, 3.9f);
        distance = RN::vec2::distance(dvl3, dvl4);
        ASSERT_NEAR(distance, 8.81, 1e-6);

        RN::vec2 dvl5(0.1f, -0.4f);
        RN::vec2 dvl6(12.2f, -2.8f);
        distance = dvl5.distance(dvl6);
        ASSERT_NEAR(distance, 12.34, 1e-6);

        RN::vec2 dvl7(0.7f, 3.4f);
        RN::vec2 dvl8(1.32f, -23.1f);
        distance = dvl7.distance(dvl8);
        ASSERT_NEAR(distance, 26.51, 1e-6);

        float length = v1.length();
        ASSERT_NEAR(length, 1.0, 1e-6);

        RN::vec2 vl1(1.2f, 2.1f);
        length = vl1.length();
        ASSERT_NEAR(length, 2.42, 1e-6);

        RN::vec2 vl2(-1.2f, 3.9f);
        length = vl2.length();
        ASSERT_NEAR(length, 4.08, 1e-6);

        RN::vec2 vl3(0.1f, -2.8f);
        length = vl3.length();
        ASSERT_NEAR(length, 2.80, 1e-6);

        RN::vec2 vl4(0.7f, -23.1f);
        length = vl4.length();
        ASSERT_NEAR(length, 23.11, 1e-6);
    }

TEST(Vec2Test, Distance) {
RN::vec2 v1(1.0f, 2.0);
RN::vec2 v2(1.0f, 5.0);
float distance = RN::vec2::distance(v1, v2);
ASSERT_NEAR(distance, (3.0), 1e-6);
}
