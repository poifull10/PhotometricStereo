#include "photometric_stereo.hpp"
#include <iostream>

size_t PhotometricStereo::AddImage(const cv::Mat &img){
    observerdImages.push_back(img);
    return observerdImages.size();
}

const std::tuple<cv::Vec3f, float> PhotometricStereo::EstimateNormal(std::vector<uint8_t> &intensities, const std::vector<cv::Vec3f> &lights)
const {
    cv::Mat I = ConvertToMat(intensities);
    cv::Mat Light = ConvertToMat(lights);
    
    cv::Mat out = (-Light).inv(cv::DECOMP_SVD);
    auto n_tilda = out * I;

    float rho = cv::norm(n_tilda);
    cv::Mat normal = cv::Mat(n_tilda / rho);

    cv::Vec3f normal_;
    normal_[0] = normal.at<float>(cv::Point(0, 0));
    normal_[1] = normal.at<float>(cv::Point(0, 1));
    normal_[2] = normal.at<float>(cv::Point(0, 2));

    return {normal_, rho};
}

const cv::Mat PhotometricStereo::ConvertToMat(const std::vector<uint8_t> &vec) const
{
    cv::Mat out = cv::Mat::zeros(cv::Size(1, vec.size()), CV_32F);

    for (int ix = 0; ix < vec.size(); ix++){
        out.at<float>(cv::Point(0, ix)) = vec[ix];
    }

    return out;
}

const cv::Mat PhotometricStereo::ConvertToMat(const std::vector<cv::Vec3f> &vec) const 
{
    cv::Mat out = cv::Mat::zeros(cv::Size(3, vec.size()), CV_32F);

    for (int ix = 0; ix < vec.size(); ix++){
        for (int i=0; i<3; i++){
            out.at<float>(cv::Point(i, ix)) = vec[ix][i];
        }
    }

    return out;
}