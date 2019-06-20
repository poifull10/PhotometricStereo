#include <iostream>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "generate_data.hpp"

class GenerateDataTest : public ::testing::Test{};

TEST_F(GenerateDataTest, Config)
{
  SphereObserver so(100, 200); 
  auto [r, d] = so.Config(40, 50);
  auto out = so.Generate(cv::Vec3f(0, 0, 0));
  EXPECT_EQ(100, out.cols);
  EXPECT_EQ(200, out.rows);
  EXPECT_EQ(40, r);
  EXPECT_EQ(50, d);
}

TEST_F(GenerateDataTest, Generate)
{ 
  SphereObserver so(400, 400);
  auto [r, d] = so.Config(30, 100);
  auto light = cv::Vec3f(0, 0.3, 1);
  light /= cv::norm(light);
  auto out = so.Generate(light); 
  //cv::imwrite(std::string("test.png"), out);
}

TEST_F(GenerateDataTest, Exist)
{
  SphereObserver so(200, 200);
  auto [r, d] = so.Config(10, 100);

  cv::Vec3f ray(0, 0, 1);
  cv::Vec3f pos(0, 0, 0);
  EXPECT_TRUE(so.Exist(ray, pos));

  // 接する方向ギリギリ内側
  ray = cv::Vec3f(1, 0,  9.9498743872+0.01);
  pos = cv::Vec3f(0, 0, 0);
  EXPECT_TRUE(so.Exist(ray, pos));

  // 接する方向ギリギリ外側
  ray = cv::Vec3f(1, 0,  9.9498743872-0.01);
  pos = cv::Vec3f(0, 0, 0);
  EXPECT_FALSE(so.Exist(ray, pos));
}

TEST_F(GenerateDataTest, CalcNormal)
{
  SphereObserver so(200, 200);
  auto [r, d] = so.Config(10, 100);

  cv::Vec3f ray(0, 0, 1);
  cv::Vec3f pos(0, 0, 0);
  auto [normal, position] = so.CalcNormal(ray, pos);
  
  EXPECT_FLOAT_EQ(0.f, normal[0]);
  EXPECT_FLOAT_EQ(0.f, normal[1]);
  EXPECT_FLOAT_EQ(-1.f, normal[2]);
  EXPECT_FLOAT_EQ(0.f, position[0]);
  EXPECT_FLOAT_EQ(0.f, position[1]);
  EXPECT_FLOAT_EQ(d-r, position[2]);
}

TEST_F(GenerateDataTest, CalcBrightness){
  SphereObserver so(200, 200);
  auto [r, d] = so.Config(10, 100);

  cv::Vec3f ray(0, 0, 1);
  cv::Vec3f pos(0, 0, 0);
  cv::Vec3f light(0, 0, 1);

  EXPECT_FLOAT_EQ(1.f, so.CalcBrightness(ray, pos, light));
}