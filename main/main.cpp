#include <iostream>
#include <opencv2/opencv.hpp>
#include "generate_data.hpp"
#include "photometric_stereo.hpp"

int main(int argc, char** argv){
  std::vector lights = {
    cv::Vec3f(0.8, -0.2, 1),
    cv::Vec3f(-0.32, -0.7, 1),
    cv::Vec3f(0.3, -0.5, 1),
    cv::Vec3f(0.1, 0.8, 1),
    cv::Vec3f(-0.1, -0.8, 1),
    cv::Vec3f(0.8, -0.4, 0.1),
    cv::Vec3f(1, 0, 0),
    cv::Vec3f(-1, 0, 0),
    cv::Vec3f(0, 1, 0),
    cv::Vec3f(0, -1, 0),
  };
  cv::Mat normal_map(cv::Size(200, 200), CV_8UC3);
  
  SphereObserver so(200, 200);
  so.Config(40, 100);

  std::vector<cv::Mat> images;

  for (int i=0;i<lights.size();i++){
    auto light = lights[i];
    auto _light = light / cv::norm(light);
    auto img = so.Generate(_light);
    images.push_back(img);
    cv::imwrite("input" + std::to_string(i) + ".png", img);
  }

  for (int iy=0; iy<200; iy++){
    for (int ix=0; ix<200; ix++){  
      std::vector<uint8_t> intensities;
      
      for (int i=0;i<lights.size();i++)
        intensities.push_back(static_cast<uint8_t>(images[i].at<cv::Vec3b>(cv::Point(ix, iy))[0]));

      PhotometricStereo ps;
      auto [normal, rho] = ps.EstimateNormal(intensities, lights);
      cv::Vec3b n;
      n[0] = 128 + normal[0] * 128;
      n[1] = 128 + normal[1] * 128;
      n[2] = 128 + normal[2] * 128;
      normal_map.at<cv::Vec3b>(cv::Point(ix, iy)) = n;
    }
  }
  {
    std::vector<uint8_t> intensities;
    for (int i=0;i<lights.size();i++)
      intensities.push_back(static_cast<uint8_t>(images[i].at<cv::Vec3b>(cv::Point(100, 100))[0]));

    PhotometricStereo ps;
    auto [normal, rho] = ps.EstimateNormal(intensities, lights);
    std::cout <<  normal << std::endl;
  }
  cv::imwrite("estimated.png", normal_map);
}