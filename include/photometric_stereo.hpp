#pragma once
#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>
#include <vector>

class PhotometricStereo {
    FRIEND_TEST(PhotometricStereoTest, ConvertToMat);
    public:
        PhotometricStereo() = default;
        ~PhotometricStereo() = default;

        size_t AddImage(const cv::Mat &);
        const std::tuple<cv::Vec3f, float> EstimateNormal(std::vector<uint8_t> &, const std::vector<cv::Vec3f> &) const;
    
    private:
        std::vector<cv::Mat> observerdImages;
        const cv::Mat ConvertToMat(const std::vector<uint8_t> &vec) const;
        const cv::Mat ConvertToMat(const std::vector<cv::Vec3f> &vec) const;
};