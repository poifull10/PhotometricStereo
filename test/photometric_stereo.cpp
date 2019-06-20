#include <iostream>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "photometric_stereo.hpp"
#include "generate_data.hpp"

class PhotometricStereoTest : public ::testing::Test{};

TEST_F(PhotometricStereoTest, AddImage)
{
  PhotometricStereo ps;
  cv::Mat a;
  EXPECT_EQ(1, ps.AddImage(a));
}

TEST_F(PhotometricStereoTest, ConvertToMat)
{
  PhotometricStereo ps;
  cv::Vec3f light1(1, 0, 1);
  cv::Vec3f light2(0, 1, 1);
  cv::Vec3f light3(1, 1, 1);
  std::vector<cv::Vec3f> lights;
  lights.push_back(light1);
  lights.push_back(light2);
  lights.push_back(light3);
  auto mat = ps.ConvertToMat(lights);

  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(0, 0)));
  EXPECT_FLOAT_EQ(0, mat.at<float>(cv::Point(1, 0)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(2, 0)));
  EXPECT_FLOAT_EQ(0, mat.at<float>(cv::Point(0, 1)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(1, 1)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(2, 1)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(0, 2)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(1, 2)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(2, 2)));

  std::vector<uint8_t> intensities = {2, 1, 0};
  mat = ps.ConvertToMat(intensities);
  EXPECT_FLOAT_EQ(2, mat.at<float>(cv::Point(0, 0)));
  EXPECT_FLOAT_EQ(1, mat.at<float>(cv::Point(0, 1)));
  EXPECT_FLOAT_EQ(0, mat.at<float>(cv::Point(0, 2)));
}

TEST_F(PhotometricStereoTest, EstimateNormal)
{
  cv::Vec3f light1(0.1, -0.1, 1);
  cv::Vec3f light2(-0.1, -0.1, 1);
  cv::Vec3f light3(0, 0, 1);
  std::vector<cv::Vec3f> lights;
  lights.push_back(light1);
  lights.push_back(light2);
  lights.push_back(light3);
  
  SphereObserver so(200, 200);
  so.Config(40, 100);

  std::vector<uint8_t> intensities;

  for (const auto & light : lights){
    auto _light = light / cv::norm(light);
    auto img = so.Generate(_light);
    intensities.push_back(static_cast<uint8_t>(img.at<cv::Vec3b>(cv::Point(100, 100))[0]));
    //cv::imwrite("test.png", img);
  }

  PhotometricStereo ps;
  auto [normal, rho] = ps.EstimateNormal(intensities, lights);

  EXPECT_NEAR(0.f, normal[0], 0.1);
  EXPECT_NEAR(0.f, normal[1], 0.1);
  EXPECT_NEAR(-1.f, normal[2], 0.1);
  EXPECT_NEAR(255.f, rho, 1);
}