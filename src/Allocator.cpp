#include "Allocator.hpp"
#include "../libs/eigen/Eigen/Dense"
#include <cmath>

Allocator::Allocator(float a, float K)
{
    this->K = K;
    Eigen::MatrixXd t1(5,1);
    Eigen::MatrixXd t2(5,1);
    Eigen::MatrixXd t3(5,1);
    Eigen::MatrixXd t4(5,1);
    Eigen::MatrixXd t5(5,1);
    Eigen::MatrixXd t6(5,1);

    t1 << cos(a), sin(a), 0, 0, 1;
    t2 << cos(a), -sin(a), 0, 0, -1;
    t3 << -cos(a), -sin(a), 0, 0, 1;
    t4 << -cos(a), sin(a), 0, 0, -1;
    t5 << 0, 0, 1, 1, 0;
    t6 << 0, 0, 1, -1, 0;

    Eigen::MatrixXd T(5,6);
    T << t1, t2, t3, t4, t5, t6;

    Eigen::MatrixXd Ttrans = T.transpose();

    M = K*Ttrans*(T*Ttrans).inverse();
}

Eigen::VectorXd Allocator::allocate(Eigen::VectorXd &F)
{
   return M * F;
}

std::array<float, 6> Allocator::allocate(std::array<float, 5> &forces)
{
    Eigen::VectorXd forcesVector(5);
    for(int i = 0; i < 5; i++)
        forcesVector[i] = forces[i];

    Eigen::VectorXd powerVector = allocate(forcesVector);

    std::array<float, 6> power;
    for(int i = 0; i < 6; i++)
        power[i] = powerVector[i];

    return power;
}
