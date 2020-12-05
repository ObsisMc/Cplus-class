#include <iostream>
using namespace std;

class Matrix
{
private:
    string name;
    string fromname;
    float *data;
    int row;
    int column;
    int *refcount;
    static int matrixnumber;

public:
    Matrix();
    Matrix(string s);
    Matrix(int row,int column);
    Matrix(int row, int column, string s);
    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);
    Matrix operator*(const Matrix &m) const;
    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    friend Matrix operator*(float f, Matrix &m);
    friend Matrix operator*(Matrix &m, float f);
    friend ostream &operator<<(ostream &os, const Matrix &m);
    friend Matrix &operator>>(Matrix &m1, Matrix &m2);
    friend void operator>>(istream &is, Matrix &m);
    string getName();
    string getFromname();
    int getRow();
    int getColumn();
    float *getData();
    int getMatrixnumber();
    void show();
    ~Matrix();
};