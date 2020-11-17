#include <iostream>
#include "matrixhead.h"
using namespace std;

int main()
{
    int row = 1;
    int interr = 1;
    int col = 1;
    cout << "Input matrix:\n";
    cout << "Row of 1st matrix: ";
    cin >> row;
    cout << "Colnum of 1st matrix or row of 2nd matrix: ";
    cin >> interr;
    cout << "Colnum of 2nd matrix: ";
    cin >> col;

    cout << endl;
    //创建和初始化矩阵
    matrix1D A1(row, interr);
    matrix1D B1(interr, col);
    matrix1D C1(row, col);
    matrix2D A2(row, interr);
    matrix2D B2(interr, col);
    matrix2D C2(row, col);

    initiaMatrix1D(A1, B1);
    initiaMatrix2D(A2, B2);

    //计算时间
    double time1_pack = matrix1Dpacking(A1, A1, C1);//一维4x4打包计算，omp
    cout << "O3-pack (1D-array matrix): " << time1_pack << endl;
    cout << endl;

    double time1 = matrix1DTime(A1, B1, C1);//一维C中元素一个一个计算
    cout << "O3 (1D-array matrix): " << time1 << endl;
    cout << endl;
    // showMatrix1D(C1);
    // cout<<endl;
    resetMatrix1D(C1);

    double time1_2 = matrix1DTime_2(A1, B1, C1);//一维C中元素一行一行计算
    cout << "O3-Change order of calc (1D-array matrix): " << time1_2 << endl;
    cout << endl;
    // showMatrix1D(C1);
    // cout<<endl;
    resetMatrix1D(C1);

    double time1_3 = matrix1DTime_3_gap4(A1, A1, C1);//一维C中元素一行一行计算，循环展开4，omp
    cout << "O3-Change order of calc-Gap4 (1D-array matrix): " << time1_3 << endl;
    cout << endl;
    // showMatrix1D(C1);
    // cout<<endl;
    resetMatrix1D(C1);

    double time1_3_gap8 = matrix1DTime_3_gap8(A1, B1, C1);//一维C中元素一行一行计算，循环展开8，omp
    cout << "O3-change order-gap8 (1D-array matrix): " << time1_3_gap8 << endl;
    cout << endl;

    double time1_tran = matrix1DTime_Tran(A1, B1, C1);//一维B转置再相乘
    cout << "O3-transpose (1D-array matrix): " << time1_tran << endl;
    // showMatrix1D(C1);
    // cout<<endl;
    cout << endl;
    resetMatrix1D(C1);

    double time1_mm256 = matrix1Dmm256(A1, B1, C1);//一维sse指令计算
    cout << "O3-mm256 (1D-array matrix): " << time1_mm256 << endl;
    cout << endl;

    double time1_gap = matrix1DTime_Gap(A1, B1, C1);//一维循环展开8
    cout << "O3-Gap 8 (2D-array matrix): " << time1_gap << endl;
    cout << endl;
    resetMatrix1D(C1);

    double time1_para = matrix1DTime_Para(A1, B1, C1);//一维omp
    cout << "O3-parallel (1D-array matrix): " << time1_para << endl;
    cout << endl;
    // showMatrix1D(C1);
    // cout<<endl;
    resetMatrix1D(C1);

    double time2 = matrix2DTime(A2, B2, C2);//二维C中元素一个一个计算
    cout << "O3 (2D-array matrix): " << time2 << endl;
    cout << endl;

    double time2_2 = matrix2DTime_2(A2, B2, C2);//二维C中元素一行一行计算
    cout << "O3-Change order of calc (2D-array matrix): " << time2_2 << endl;
    cout << endl;

    double time2_gap = matrix2DTime_Gap(A2, B2, C2);//二维循环展开8
    cout << "O3-Gap 8 (2D-array matrix): " << time2_gap << endl;
}
