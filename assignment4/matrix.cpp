#include <iostream>
#include <random>
#include "matrix.h"
using namespace std;

int Matrix::matrixnumber = 0;
Matrix::Matrix()
{
    name = "unamed" + to_string(++matrixnumber);
    fromname = "none";
    row = 5;
    column = 5;
    data = new float[row * column]();
    refcount = new int(1);
    printf("Create %s\n", name.c_str());
};
Matrix::Matrix(string s)
{
    name = s;
    matrixnumber++;
    fromname = "none";
    row = 5;
    column = 5;
    data = new float[row * column]();
    refcount = new int(1);
    printf("Create %s\n", name.c_str());
};
Matrix::Matrix(int row, int column)
{
    name = "unamed" + to_string(++matrixnumber);
    fromname = "none";
    this->row = row;
    this->column = column;
    data = new float[row * column]();
    refcount = new int(1);
    printf("Create %s\n", name.c_str());
};
Matrix::Matrix(int row, int column, string s)
{
    name = s;
    matrixnumber++;
    fromname = "none";
    this->row = row;
    this->column = column;
    data = new float[row * column]();
    refcount = new int(1);
    printf("Create %s\n", name.c_str());
};
Matrix::Matrix(const Matrix &m) //Matrix A=B+C会跳过该方法
{
    name = "unamed" + to_string(++matrixnumber);
    fromname = m.name;
    row = m.row;
    column = m.column;
    data = m.data;
    refcount = m.refcount;
    (*refcount)++;
    printf("Create %s\n", name.c_str());
};
Matrix &Matrix::operator=(const Matrix &m) //不加const不能用
{
    if (this == &m)
        return *this; //函数后面不能加const，否则不能返回*this
    else
    {
        this->~Matrix();
        fromname = m.name;
        row = m.row;
        column = m.column;
        refcount = m.refcount;
        data = m.data;
        (*refcount)++;
    }
    return *this;
};
Matrix Matrix::operator*(const Matrix &m) const //好像有点问题
{
    Matrix newmatrix(row, m.column);
    int error = row % 4;

    for (int i = 0; i < row - error; i += 4)
    {
        for (int j = 0; j < m.row; j++)
        {
            float multiplier1 = data[i * column + j];
            float multiplier2 = data[(i + 1) * column + j];
            float multiplier3 = data[(i + 2) * column + j];
            float multiplier4 = data[(i + 3) * column + j];

            for (int k = 0; k < m.column; k++)
            {
                newmatrix.data[i * m.column + k] += multiplier1 * m.data[j * m.column + k];
                newmatrix.data[(i + 1) * m.column + k] += multiplier1 * m.data[j * m.column + k];
                newmatrix.data[(i + 2) * m.column + k] += multiplier1 * m.data[j * m.column + k];
                newmatrix.data[(i + 3) * m.column + k] += multiplier1 * m.data[j * m.column + k];
            }
        }
    }

    for (int i = row - error; i < row; i++)
    {
        for (int j = 0; j < m.row; j++)
        {
            float multiplier = data[i * column + j];
            for (int k = 0; k < m.column; k++)
            {
                newmatrix.data[i * m.column + k] += multiplier * m.data[j * m.column + k];
            }
        }
    }

    return newmatrix;
};
Matrix Matrix::operator+(const Matrix &m) const
{
    Matrix newm(row, column);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            newm.getData()[i * column + j] = data[i * column + j] + m.data[i * column + j];
        }
    }
    return newm;
};
Matrix Matrix::operator-(const Matrix &m) const
{
    Matrix newm(row, column);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            newm.getData()[i * column + j] = data[i * column + j] - m.data[i * column + j];
        }
    }
    return newm;
};
Matrix operator*(float f, Matrix &m)
{
    Matrix newm(m.row, m.column);

    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.column; j++)
        {
            newm.getData()[i * m.column + j] = m.data[i * m.column + j] * f;
        }
    }
    return newm;
};
Matrix operator*(Matrix &m, float f)
{
    return f*m; 
};
ostream &operator<<(ostream &os, const Matrix &m)
{
    os << m.name << "(from " << m.fromname << ", used: " << *m.refcount << "):" << endl;
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.column; j++)
        {
            os << m.data[i * m.column + j] << " ";
        }
        os << endl;
    }
    return os;
};
Matrix &operator>>(Matrix &m1, Matrix &m2)
{
    if (&m2 == &m1)
        return m2;
    else
    {
        m2.~Matrix();
        m2.fromname = m1.name;
        m2.row = m1.row;
        m2.column = m1.column;
        m2.data = m1.data;
        m2.refcount = m1.refcount;
        (*m2.refcount)++;
    }
    return m2;
};
void operator>>(istream &is, Matrix &m)
{
    random_device rd;
    mt19937 gen(rd());
    float k = __FLT_MAX__;
    uniform_real_distribution<double> time(-k, k);
    for (int i = 0; i < m.column * m.row; i++)
    {
        m.data[i]=time(gen);
    }
};
string Matrix::getName()
{
    return name;
}
int Matrix::getRow()
{
    return row;
};
int Matrix::getColumn()
{
    return column;
};
float *Matrix::getData()
{
    return data;
};
int Matrix::getMatrixnumber()
{
    return matrixnumber;
    ;
};
void Matrix::show()
{
    cout << name << "(from " << fromname << ", used: " << *refcount << "):" << endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            cout << data[i * column + j] << " ";
        }
        cout << endl;
    }
};
Matrix::~Matrix()
{
    --(*refcount);
    if (*refcount > 0)
    {
        printf("[Times of being used of %s(from %s)'s data decrease to %d]\n",
               name.c_str(), fromname.c_str(), *refcount);
    }
    else if (*refcount == 0)
    {
        printf("[delete %s(from %s)'s data]\n", name.c_str(), fromname.c_str());
        delete data;
        delete refcount;
    }
    else
    {
        printf("[%s(from %s) has been delete]\n", name.c_str(), fromname.c_str());
    }
}