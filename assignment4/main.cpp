#include <iostream>
#include "matrix.h"
using namespace std;

int main()
{
    //create basic matrix
    Matrix A;
    Matrix B("B");
    Matrix C(5, 7, "C");
    A.show();
    cout << B << C;

    cout << "-----------initialize matrix----------\n";
    for (int i = 0; i < A.getColumn() * A.getRow(); i++)
    {
        A.getData()[i] = i;
    }
    for (int i = 0; i < B.getColumn() * B.getRow(); i++)
    {
        B.getData()[i] = float(i) / 2;
    }
    cin >> C;
    cout << A << B<< C;
    cout<<"------------shallow copy-------------\n";
    //先创建一个矩阵，再用"="拷贝
    Matrix E1("E1");
    E1.show();
    E1 = A;
    E1.show();
    //创建矩阵的同时用"="拷贝
    Matrix E2=B;
    E2.show();
    //用copy constructor拷贝
    Matrix F(C);
    F.show();
    //用">>"拷贝
    A>>F;
    F.show();

    cout<<"----------operator overloading----------\n";
    Matrix G("G");
    G = A*A;
    G.show();

    G=A+B;
    G.show();

    G=B-A;
    G.show();

    G=6*B;
    G.show();

    G=B*6;
    G.show();

    //内存错误释放
    // cout<<"-----------erroneous free-------------\n";
    // Matrix K=A;
    // K.~Matrix();//模仿不使用refcount变量进行复制，将refount减回1
    // cout<<A;
    // K.~Matrix();//K被释放，此时A的data指向的内存被释放
    // cout<<A;
};