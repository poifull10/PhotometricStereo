#pragma once
#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>
#include <tuple>

class SphereObserver{
    FRIEND_TEST(GenerateDataTest, Exist);
    FRIEND_TEST(GenerateDataTest, CalcNormal);
    FRIEND_TEST(GenerateDataTest, CalcBrightness);

public:
    SphereObserver() = default;
    SphereObserver(uint width, uint height) : WIDTH(width), HEIGHT(height) {}
    ~SphereObserver() = default;
    const std::tuple<float, float> Config(float, float);
    const cv::Mat Generate(const cv::Vec3f &s) const;



private:
    float SphereRadius = 0.f;
    int WIDTH = 100;
    int HEIGHT = 100;
    float R = 100;
    float D = 100;
    float rho = 1.0;

    float CalcD4(const cv::Vec3f&, const cv::Vec3f &) const;
    bool Exist(const cv::Vec3f&, const cv::Vec3f &) const; 
    const std::tuple<cv::Vec3f, cv::Vec3f> CalcNormal(const cv::Vec3f&, const cv::Vec3f &) const;
    float CalcBrightness(const cv::Vec3f&, const cv::Vec3f&, const cv::Vec3f&) const;
};