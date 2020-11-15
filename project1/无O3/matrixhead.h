#pragma once

struct matrix1D
{
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
void initiaMatrix1D(matrix1D &A, matrix1D &B);
void initiaMatrix1D(matrix1D &A);
void initiaMatrix2D(matrix2D &A, matrix2D &B);
void resetMatrix1D(matrix1D &C);

//运算时间
double matrix1DTime(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_2(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_3_gap4(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_3_gap8(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_Tran(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1Dmm256Tran(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1Dpacking(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1Dpack_sse(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1Dmm256(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_Gap(matrix1D &A, matrix1D &B, matrix1D &C);
double matrix1DTime_Para(matrix1D &A, matrix1D &B, matrix1D &C);

double matrix2DTime(matrix2D &A, matrix2D &B, matrix2D &C);
double matrix2DTime_2(matrix2D &A, matrix2D &B, matrix2D &C);
double matrix2DTime_Gap(matrix2D &A, matrix2D &B, matrix2D &C);
double matrix2DTime_Para(matrix2D &A, matrix2D &B, matrix2D &C);

void transpose(matrix1D &c);
void showMatrix1D(matrix1D &c);
void multi4kernel(matrix1D &C, matrix1D &A, float** b,int brow,int row,int col);