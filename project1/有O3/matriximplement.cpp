#include <iostream>
#include <chrono>
#include <immintrin.h>
#include <omp.h>
#include "matrixhead.h"
#pragma GCC optimize(3, "Ofast", "inline")
using namespace std;

//初始化矩阵
void initiaMatrix1D(matrix1D &A, matrix1D &B)
{
    for (int i = 0; i < A.row * A.colnum; i++)
    {
        A.data[i] = 1;
    }

    for (int i = 0; i < B.row * B.colnum; i++)
    {
        B.data[i] = 1;
    }
}
void initiaMatrix1D(matrix1D &A)
{
    for (int i = 0; i < A.row * A.colnum; i++)
    {
        A.data[i] = 1;
    }
}
void initiaMatrix2D(matrix2D &A, matrix2D &B)
{
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.colnum; j++)
        {
            A.data[i][j] = 1;
        }
    }

    for (int i = 0; i < B.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            B.data[i][j] = 1;
        }
    }
}
void resetMatrix1D(matrix1D &C)
{
    for (int i = 0; i < C.colnum * C.row; i++)
    {
        C.data[i] = 0;
    }
}

//运算时间
double matrix1DTime(matrix1D &A, matrix1D &B, matrix1D &C)
{
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            for (int k = 0; k < B.row; k++)
            {
                C.data[i * B.colnum + j] += A.data[i * A.colnum + k] * B.data[k * B.colnum + j];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_2(matrix1D &A, matrix1D &B, matrix1D &C) //A的读取一直连续，B先遍历一遍再循环，C的存储是一行一行
{
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier = A.data[i * A.colnum + j];
            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_3_gap4(matrix1D &A, matrix1D &B, matrix1D &C)
{
    auto t1 = chrono::steady_clock::now();
    int error = A.row % 4;
#pragma omp parallel for
    for (int i = 0; i < A.row - error; i += 4)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier1 = A.data[i * A.colnum + j];
            float multiplier2 = A.data[(i + 1) * A.colnum + j];
            float multiplier3 = A.data[(i + 2) * A.colnum + j];
            float multiplier4 = A.data[(i + 3) * A.colnum + j];

            // register float multiplier1 = A.data[i * A.colnum + j];
            // register float multiplier2 = A.data[(i + 1) * A.colnum + j];
            // register float multiplier3 = A.data[(i + 2) * A.colnum + j];
            // register float multiplier4 = A.data[(i + 3) * A.colnum + j];

            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 1) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 2) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 3) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
            }
        }
    }

#pragma omp parallel for
    for (int i = A.row - error; i < A.row; i++)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier = A.data[i * A.colnum + j];
            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_3_gap8(matrix1D &A, matrix1D &B, matrix1D &C) //没gap4快
{
    if (A.row % 8 != 0)
        return -1;
    auto t1 = chrono::steady_clock::now();
#pragma omp parallel for
    for (int i = 0; i < A.row; i += 8)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier1 = A.data[i * A.colnum + j];
            float multiplier2 = A.data[(i + 1) * A.colnum + j];
            float multiplier3 = A.data[(i + 2) * A.colnum + j];
            float multiplier4 = A.data[(i + 3) * A.colnum + j];
            float multiplier5 = A.data[(i + 4) * A.colnum + j];
            float multiplier6 = A.data[(i + 5) * A.colnum + j];
            float multiplier7 = A.data[(i + 6) * A.colnum + j];
            float multiplier8 = A.data[(i + 7) * A.colnum + j];

            // register float multiplier1 = A.data[i * A.colnum + j];
            // register float multiplier2 = A.data[(i + 1) * A.colnum + j];
            // register float multiplier3 = A.data[(i + 2) * A.colnum + j];
            // register float multiplier4 = A.data[(i + 3) * A.colnum + j];
            // register float multiplier5 = A.data[(i+4) * A.colnum + j];
            // register float multiplier6 = A.data[(i + 5) * A.colnum + j];
            // register float multiplier7 = A.data[(i + 6) * A.colnum + j];
            // register float multiplier8 = A.data[(i + 7) * A.colnum + j];

            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 1) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 2) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 3) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 4) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 5) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 6) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
                C.data[(i + 7) * B.colnum + k] += multiplier1 * B.data[j * B.colnum + k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_Tran(matrix1D &A, matrix1D &B, matrix1D &C) //没换顺序快
{
    // transpose(B);
    auto t1 = chrono::steady_clock::now();
    transpose(B);
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            for (int k = 0; k < B.row; k++)
            {
                C.data[i * C.colnum + j] += A.data[i * A.colnum + k] * B.data[j * B.colnum + k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1Dmm256Tran(matrix1D &A, matrix1D &B, matrix1D &C)
{
    transpose(B);
    if (B.row % 8 != 0)
        return -1;
    float *pa = A.data;
    float *pb = B.data;
    __m256 a, b;
    __m256 c = _mm256_setzero_ps();
    float res[8] = {0};

    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            // float multiplier = A.data[i * A.colnum + j];
            for (int k = 0; k < B.row; k += 8)
            {
                cout << res[0] << res[1];
                //  C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
                b = _mm256_loadu_ps(pb + k);
                a = _mm256_loadu_ps(pa + k);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
                _mm256_storeu_ps(res, c);

                C.data[i * C.colnum + j] += res[0] + res[1] + res[2] + res[3] + res[4] + res[5] + res[6] + res[7];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1Dpacking(matrix1D &A, matrix1D &B, matrix1D &C)
{
    auto t1 = chrono::steady_clock::now();
    int brow = B.row;
    int arow = A.row;
    int bcol = B.colnum;

    float *tr[4];
    int i(0);
    do
    {
        tr[i++] = new float[B.row];
    } while (i < 4);
#pragma omp parallel for
    for (int j = 0; j < bcol; j += 4)
    {
        i = 0;
        do
        {
            tr[0][i] = B.data[i * bcol + j]; //packing过程，
            tr[1][i] = B.data[i * bcol + j + 1];
            tr[2][i] = B.data[i * bcol + j + 2];
            tr[3][i] = B.data[i * bcol + j + 3];
        } while ((++i) < brow);
        i = 0;
        do
        {
            multi4kernel(C, A, tr, brow, i, j);
            i += 4;
        } while (i < arow);
    }

    free(tr[0]);
    free(tr[1]);

    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1Dmm256(matrix1D &A, matrix1D &B, matrix1D &C)
{
    if (B.row % 8 != 0)
        return -1;
    float *pa = A.data;
    float *pb = B.data;
    __m256 a, b;
    __m256 c = _mm256_setzero_ps();
    register float res[8] = {0};

    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i += 8)
    {
        for (int j = 0; j < B.row; j++)
        {

            a = _mm256_set_ps(A.data[i * A.row + j], A.data[(i + 1) * A.row + j], A.data[(i + 2) * A.row + j], A.data[(i + 3) * A.row + j],
                              A.data[(i + 4) * A.row + j], A.data[(i + 5) * A.row + j], A.data[(i + 6) * A.row + j], A.data[(i + 7) * A.row + j]);
            for (int k = 0; k < B.colnum; k++)
            {
                // cout << res[0] << res[1];
                // float multiplier = A.data[i * A.colnum + j];
                //  C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
                b = _mm256_set1_ps(B.data[j * B.colnum + k]);
                // a = _mm256_loadu_ps(pa + k);

                _mm256_storeu_ps(res, _mm256_mul_ps(a, b));

                // C.data[i * C.colnum + j] += res[0] + res[1] + res[2] + res[3] + res[4] + res[5] + res[6] + res[7];
                C.data[i * C.colnum + k] += res[0];
                C.data[(i + 1) * C.colnum + k] += res[1];
                C.data[(i + 2) * C.colnum + k] += res[2];
                C.data[(i + 3) * C.colnum + k] += res[3];
                C.data[(i + 4) * C.colnum + k] += res[4];
                C.data[(i + 5) * C.colnum + k] += res[5];
                C.data[(i + 6) * C.colnum + k] += res[6];
                C.data[(i + 7) * C.colnum + k] += res[7];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_Gap(matrix1D &A, matrix1D &B, matrix1D &C)
{
    auto t1 = chrono::steady_clock::now();
    int gap = 8;
    int error = B.row % gap;
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier = A.data[i * A.colnum + j];
            for (int k = 0; k < B.colnum; k += 8)
            {
                C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
                C.data[i * B.colnum + k + 1] += multiplier * B.data[j * B.colnum + k + 1];
                C.data[i * B.colnum + k + 2] += multiplier * B.data[j * B.colnum + k + 2];
                C.data[i * B.colnum + k + 3] += multiplier * B.data[j * B.colnum + k + 3];
                C.data[i * B.colnum + k + 4] += multiplier * B.data[j * B.colnum + k + 4];
                C.data[i * B.colnum + k + 5] += multiplier * B.data[j * B.colnum + k + 5];
                C.data[i * B.colnum + k + 6] += multiplier * B.data[j * B.colnum + k + 6];
                C.data[i * B.colnum + k + 7] += multiplier * B.data[j * B.colnum + k + 7];
            }
            for (int m = B.row - error + 1; m < B.colnum; m++)
            {
                C.data[i * B.colnum + m] += multiplier * B.data[j * B.colnum + m];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix1DTime_Para(matrix1D &A, matrix1D &B, matrix1D &C)
{
    auto t1 = chrono::steady_clock::now();
#pragma omp parallel for
    for (int i = 0; i < A.row; i++)
    {

        for (int j = 0; j < B.row; j++)
        {
            float multiplier = A.data[i * A.colnum + j];

            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i * B.colnum + k] += multiplier * B.data[j * B.colnum + k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}

double matrix2DTime(matrix2D &A, matrix2D &B, matrix2D &C)
{
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            for (int k = 0; k < B.row; k++)
            {
                C.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix2DTime_2(matrix2D &A, matrix2D &B, matrix2D &C)
{
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.row; j++)
        {
            float multiplier = A.data[i][j];
            for (int k = 0; k < B.colnum; k++)
            {
                C.data[i][k] += multiplier * B.data[j][k];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix2DTime_Gap(matrix2D &A, matrix2D &B, matrix2D &C)
{
    int gap = 8;
    int error = B.row % gap;
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            for (int k = 0; k < B.row - error; k += 8) //小心越界
            {
                C.data[i][j] += A.data[i][k] * B.data[k][j];
                C.data[i][j] += A.data[i][k + 1] * B.data[k + 1][j];
                C.data[i][j] += A.data[i][k + 2] * B.data[k + 2][j];
                C.data[i][j] += A.data[i][k + 3] * B.data[k + 3][j];
                C.data[i][j] += A.data[i][k + 4] * B.data[k + 4][j];
                C.data[i][j] += A.data[i][k + 5] * B.data[k + 5][j];
                C.data[i][j] += A.data[i][k + 6] * B.data[k + 6][j];
                C.data[i][j] += A.data[i][k + 7] * B.data[k + 7][j];
            }
            for (int m = B.row - error + 1; m < B.row; m++)
            {
                C.data[i][j] += A.data[i][m] * B.data[m][j];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}
double matrix2DTime_Para(matrix2D &A, matrix2D &B, matrix2D &C)
{

    auto t1 = chrono::steady_clock::now();
#pragma omp parallel for
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.colnum; j++)
        {
            for (int k = 0; k < B.row; k++)
            {
                C.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    auto t2 = chrono::steady_clock::now();
    auto diff = chrono::duration<double, std::milli>(t2 - t1).count();
    return diff;
}

void transpose(matrix1D &c)
{
    int row = c.row;
    int col = c.colnum;
    float *tran = new float[col * row];

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            tran[j * row + i] = c.data[i * col + j];
        }
    }
    float *tem = c.data;
    c.data = tran;
    delete tem;
}
void showMatrix1D(matrix1D &c)
{
    for (int i = 0; i < c.colnum * c.row; i++)
    {
        printf("%f ", c.data[i]);
    }
}
void multi4kernel(matrix1D &C, matrix1D &A, float **b, int brow, int row, int col)
{
    float *abegin = A.data + A.colnum * row;
    // float* bbegin=B.data+col;
    register float t0(0), t1(0), t2(0), t3(0), t4(0), t5(0), t6(0), t7(0),
        t8(0), t9(0), t10(0), t11(0), t12(0), t13(0), t14(0), t15(0);
    float *a0(abegin), *a1(abegin + A.colnum), *a2(abegin + A.colnum * 2), *a3(abegin + A.colnum * 3),
        *b0(b[0]), *b1(b[1]), *b2(b[2]), *b3(b[3]), *end = b0 + brow;
    do
    {
        t0 += *(a0) * *(b0);
        t1 += *(a0) * *(b1);
        t2 += *(a0) * *(b2);
        t3 += *(a0++) * *(b3);
        t4 += *(a1) * *(b0);
        t5 += *(a1) * *(b1);
        t6 += *(a1) * *(b2);
        t7 += *(a1++) * *(b3);
        t8 += *(a2) * *(b0);
        t9 += *(a2) * *(b1);
        t10 += *(a2) * *(b2);
        t11 += *(a2++) * *(b3);
        t12 += *(a3) * *(b0++); //优势：连续，要是不转置就不连续
        t13 += *(a3) * *(b1++);
        t14 += *(a3) * *(b2++);
        t15 += *(a3++) * *(b3++);
    } while (b0 != end);
    C.data[col + row * C.colnum] = t0;
    C.data[col + 1 + row * C.colnum] = t1;
    C.data[col + 2 + row * C.colnum] = t2;
    C.data[col + 3 + row * C.colnum] = t3;
    C.data[col + (row + 1) * C.colnum] = t4;
    C.data[col + 1 + (row + 1) * C.colnum] = t5;
    C.data[col + 2 + (row + 1) * C.colnum] = t6;
    C.data[col + 3 + (row + 1) * C.colnum] = t7;
    C.data[col + (row + 2) * C.colnum] = t8;
    C.data[col + 1 + (row + 2) * C.colnum] = t9;
    C.data[col + 2 + (row + 2) * C.colnum] = t10;
    C.data[col + 3 + (row + 2) * C.colnum] = t11;
    C.data[col + (row + 3) * C.colnum] = t12;
    C.data[col + 1 + (row + 3) * C.colnum] = t13;
    C.data[col + 2 + (row + 3) * C.colnum] = t14;
    C.data[col + 3 + (row + 3) * C.colnum] = t15;
}
