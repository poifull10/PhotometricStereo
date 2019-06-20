#include "generate_data.hpp"

const std::tuple<float, float> SphereObserver::Config(float r, float d){
    R = r;
    D = d;
    return {R, D};
}

const cv::Mat SphereObserver::Generate(const cv::Vec3f &s) const {
    cv::Mat out = cv::Mat::zeros(cv::Size(WIDTH, HEIGHT), CV_8UC3);

    for (int iy = 0; iy < HEIGHT; iy++){
        for (int ix = 0; ix < WIDTH; ix++){
            cv::Vec3f ray(ix - WIDTH/2, iy - HEIGHT/2, std::max(WIDTH, HEIGHT));
            cv::Vec3f pos(0,0,0); 
            if (Exist(ray, pos))
            {
                auto brightness = CalcBrightness(ray, pos, s);
                cv::Vec3b pix;
                pix[0] = 255 * brightness;
                pix[1] = 255 * brightness;
                pix[2] = 255 * brightness;
                out.at<cv::Vec3b>(cv::Point(ix, iy)) = pix;
            }
        }
    }

    return out;
}

float SphereObserver::CalcD4(const cv::Vec3f &ray, const cv::Vec3f &pos) const {
    auto dx = ray[0], dy = ray[1], dz = ray[2];
    auto px = pos[0], py = pos[1], pz = pos[2];

    auto b_ = dx*px+dy*py+dz*pz-D*dz;
    auto a = dx*dx+dy*dy+dz*dz;
    auto c = px*px+py*py+pz*pz+D*D-2*pz*D-R*R;
    return (b_*b_ - a*c);
}

bool SphereObserver::Exist(const cv::Vec3f &ray, const cv::Vec3f &pos) const {
    return CalcD4(ray, pos) >= 0;
}

const std::tuple<cv::Vec3f, cv::Vec3f> SphereObserver::CalcNormal(const cv::Vec3f &ray, const cv::Vec3f &pos)
const {
    cv::Vec3f normal, cross_point;
    auto dx = ray[0], dy = ray[1], dz = ray[2];
    auto px = pos[0], py = pos[1], pz = pos[2];

    auto b_ = dx*px+dy*py+dz*pz-D*dz;
    auto a = dx*dx+dy*dy+dz*dz;
    auto c = px*px+py*py+pz*pz+D*D-2*pz*D-R*R;

    auto k = - b_ - std::sqrt(CalcD4(ray, pos));
    k = k / a;

    cross_point = k * ray + pos;

    normal = cross_point - cv::Vec3f(0, 0, D); 
    normal /= R;

    return {normal, cross_point};
}


float SphereObserver::CalcBrightness(const cv::Vec3f& ray, const cv::Vec3f& pos, const cv::Vec3f& light) const
{
    if (Exist(ray, pos)){
        auto [normal, position] = CalcNormal(ray, pos);
        auto inner_product = normal.dot(light);
        if (inner_product > 0) return 0;
        return std::abs(inner_product) * rho;
    }
    return 0;
}