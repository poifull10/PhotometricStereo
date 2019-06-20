#include <iostream>
#include <opencv2/opencv.hpp>
#include "generate_data.hpp"
#include "photometric_stereo.hpp"
#include <omp.h>

int main(int argc, char** argv){
  cv::Vec3f light1(1, -0.5, 1);
  cv::Vec3f light2(0.5, 1, 1);
  cv::Vec3f light3(0, 0, 1);
  std::vector<cv::Vec3f> lights;
  cv::Mat normal_map(cv::Size(200, 200), CV_8UC3);
  lights.push_back(light1);
  lights.push_back(light2);
  lights.push_back(light3);
  
  SphereObserver so(200, 200);
  so.Config(40, 100);

  std::vector<cv::Mat> images;

  for (int i=0;i<3;i++){
    auto light = lights[i];
    auto _light = light / cv::norm(light);
    auto img = so.Generate(_light);
    images.push_back(img);
    cv::imwrite("input" + std::to_string(i) + ".png", img);
  }

  #pragma omp parallel for
  for (int iy=0; iy<200; iy++){
    for (int ix=0; ix<200; ix++){  
      std::vector<uint8_t> intensities;
      
      for (int i=0;i<3;i++)
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

  cv::imwrite("estimated.png", normal_map);
}