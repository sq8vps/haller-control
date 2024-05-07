#pragma once

#include "../libs/eigen/Eigen/Dense"
#include <array>

class Allocator
{
private:
    Eigen::MatrixXd M;
    float K{1.f};
public:
    Allocator(float a = 0.785f, float K = 1.f);
    Eigen::VectorXd allocate(Eigen::VectorXd &F);
    std::array<float, 6> allocate(std::array<float, 5> &forces);
};
