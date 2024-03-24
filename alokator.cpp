#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include <cmath>

const float k = 107.36f;
const float a = 0.785f, d1=0.165f, d2=0.278f;

static Eigen::MatrixXd M;

class Allocator
{
    public:
    Allocator()
    {
        Eigen::MatrixXd K(6, 6);
        K.fill(0.f);

        for(int i = 0; i < 6; i++)
            K(i, i) = k;

        Eigen::MatrixXd Kinv = K.inverse();

        Eigen::MatrixXd t1(5,1);
        Eigen::MatrixXd t2(5,1);
        Eigen::MatrixXd t3(5,1);
        Eigen::MatrixXd t4(5,1);
        Eigen::MatrixXd t5(5,1);
        Eigen::MatrixXd t6(5,1);

        t1 << cos(a), sin(a), 0, 0, d2;
        t2 << cos(a), -sin(a), 0, 0, -d2;
        t3 << -cos(a), -sin(a), 0, 0, d2;
        t4 << -cos(a), sin(a), 0, 0, -d2;
        t5 << 0, 0, 1, d1, 0;
        t6 << 0, 0, 1, -d1, 0;

        Eigen::MatrixXd T(5,6);
        T << t1, t2, t3, t4, t5, t6;

        Eigen::MatrixXd Ttrans = T.transpose();

        M = Ttrans*(T*Ttrans).inverse();
    }
    Eigen::VectorXd Allocate(Eigen::VectorXd F)
        {
            return M * F;
        }
};

int main()
{
    Allocator Object;

    Eigen::VectorXd F(5, 1);
    F << 0.f, 0.f, 0.f, 0.f, 1.f;
    std::cout << Object.Allocate(F) << std::endl;
}