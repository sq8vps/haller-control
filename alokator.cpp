#include <iostream>
#include <vector>
#include <Eigen\Dense>
#include <cmath>
using namespace std;

const float k = 107.36;
const float a = 0.785, d1=0.165, d2=0.278;

Eigen::MatrixXd alokator(float F)
{
    Eigen::MatrixXd K(6,6);
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            if(j==i)
            {
                K(i,j) = k;
            }
            else
                K(i,j) = 0;
        }
    }

    Eigen::MatrixXd K_inverse = K.inverse();

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

    Eigen::MatrixXd T_transponowana = T.transpose();

    Eigen::MatrixXd Force(6,1);
    Force << F, F, F, F, F, F;

    Eigen::MatrixXd TT = T_transponowana*T;

    Eigen::MatrixXd TT_inverse = TT.inverse();

    Eigen::MatrixXd T_plus(6,6);
    T_plus = TT_inverse*T_transponowana;

    Eigen::MatrixXd U(6,6);
    U = K_inverse*T_plus*T*Force;

    return U;
}

int main()
{
    float F=50;
    cout<<alokator(F)<<endl;
}