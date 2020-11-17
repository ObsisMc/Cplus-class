#pragma once

struct matrix1D
{
private:
    int k = 0;

public:
    int row = 1;
    int colnum = 1;
    float *data;
    matrix1D()
    {
        data = new float[row * colnum]();
    }
    matrix1D(int r, int c)
    {
        row = r;
        colnum = c;
        data = new float[row * colnum]();
    }

    int operator+(matrix1D a);
};
struct matrix2D
{
    int row = 1;
    int colnum = 1;
    float **data;

    matrix2D()
    {
        data = new float *[row];
        for (int i = 0; i < row; i++)
        {
            data[i] = new float[colnum]();
        }
    }
    matrix2D(int r, int c)
    {
        row = r;
        colnum = c;
        data = new float *[row];
        for (int i = 0; i < row; i++)
        {
            data[i] = new float[colnum]();
        }
    }
};

//初始化矩阵
void initiaMatrix1D(matrix1D &A, matrix1D &B);//初始化矩阵元素为1
void initiaMatrix1D(matrix1D &A);
void initiaMatrix2D(matrix2D &A, matrix2D &B);//初始化矩阵元素为1
void resetMatrix1D(matrix1D &C);

//运算时间
double matrix1DTime(matrix1D &A, matrix1D &B, matrix1D &C);//一维C中元素一个一个计算
double matrix1DTime_2(matrix1D &A, matrix1D &B, matrix1D &C);//一维C中元素一行一行计算
double matrix1DTime_3_gap4(matrix1D &A, matrix1D &B, matrix1D &C);//一维C中元素一行一行计算，循环展开4，omp
double matrix1DTime_3_gap8(matrix1D &A, matrix1D &B, matrix1D &C);//一维C中元素一行一行计算，循环展开8，omp
double matrix1DTime_Tran(matrix1D &A, matrix1D &B, matrix1D &C);//一维B转置再相乘
double matrix1Dmm256Tran(matrix1D &A, matrix1D &B, matrix1D &C);//一维B转置再相乘，用sse指令
double matrix1Dpacking(matrix1D &A, matrix1D &B, matrix1D &C);//一维4x4打包计算，omp
double matrix1Dpack_sse(matrix1D &A, matrix1D &B, matrix1D &C);//一维sse指令计算且打包，未实现
double matrix1Dmm256(matrix1D &A, matrix1D &B, matrix1D &C);//一维sse指令计算
double matrix1DTime_Gap(matrix1D &A, matrix1D &B, matrix1D &C);//一维循环展开8
double matrix1DTime_Para(matrix1D &A, matrix1D &B, matrix1D &C);//一维omp

double matrix2DTime(matrix2D &A, matrix2D &B, matrix2D &C);//二维C中元素一个一个计算
double matrix2DTime_2(matrix2D &A, matrix2D &B, matrix2D &C);//二维C中元素一行一行计算
double matrix2DTime_Gap(matrix2D &A, matrix2D &B, matrix2D &C);//二维循环展开8
double matrix2DTime_Para(matrix2D &A, matrix2D &B, matrix2D &C);//二维omp

void transpose(matrix1D &c);//转置
void showMatrix1D(matrix1D &c);//输出矩阵
void multi4kernel(matrix1D &C, matrix1D &A, float **b, int brow, int row, int col);//4x4打包