#include <iostream>
#include <chrono>
#include <omp.h>
#pragma GCC optimize(3, "Ofast", "inline")
using namespace std;

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
double matrix1DTime_3_gap4(matrix1D &A, matrix1D &B, matrix1D &C);
void showMatrix1D(matrix1D &c);

int main()
{
    int row = 1;
    int interr = 1;
    int col = 1;
    cout << "Input matrix:\n";
    cout << "Row of 1st matrix: ";
    cin >> row;
    if (row <= 0)
    {
        cout << "The number must be large than 0.\n";
    }
    else if (cin.fail())
    {
        cout << "Please input float number.\n";
    }
    else
    {
        cout << "Colnum of 1st matrix or row of 2nd matrix: ";
        cin >> interr;
        if (interr <= 0)
        {
            cout << "The number must be large than 0.\n";
        }
        else if (cin.fail())
        {
            cout << "Please input float number.\n";
        }
        else
        {
            cout << "Colnum of 2nd matrix: ";
            cin >> col;
            if (col <= 0)
            {
                cout << "The number must be large than 0.\n";
            }
            else if (cin.fail())
            {
                cout << "Please input float number.\n";
            }
            else if (row * col > 2e9)
            {
                cout << "The number of elements in the result matrix shouldn't more than 2e9.\n";
            }
            else
            {
                matrix1D A(row, interr);
                matrix1D B(interr, col);
                bool isfail = false;
                cout << "Please input the first matrix:\n";
                for (int i = 0; i < A.colnum * A.row; i++)
                {
                    cin >> A.data[i];
                    if (cin.fail())
                    {
                        cout << "Please input float number.\n";
                        isfail = true;
                        break;
                    }
                }
                if (!isfail)
                {
                    cout<<"Please input the first matrix:\n";
                    for (int i = 0; i < B.colnum * B.row; i++)
                    {
                        cin >> B.data[i];
                        if (cin.fail())
                        {
                            cout << "Please input float number.\n";
                            isfail = true;
                            break;
                        }
                    }
                    if (!isfail)
                    {
                        matrix1D C(row, col);
                        double time=matrix1DTime_3_gap4(A,B,C);
                        showMatrix1D(C);
                        cout<<"(time: "<<time<<"ms)"<<endl;
                    }
                }
            }
        }
    }
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

void showMatrix1D(matrix1D &c)
{
    for (int i = 0; i < c.colnum * c.row; i++)
    {
        printf("%f ", c.data[i]);
        if ((i+1) %c.colnum==0)
            cout << endl;
    }
}