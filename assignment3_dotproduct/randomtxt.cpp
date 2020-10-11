#include <iostream>
#include <random>
#include <fstream>

using namespace std;

int main()
{
    int number;
    cin >> number;
    // float data[number];

    // random_device rd;
    // mt19937 gen(4);
    // float k=__FLT_MAX__;
    // float k=1000000.0;
    // uniform_real_distribution<double> time(-k, k);

    fstream out("test.txt", ios::out);

    int i = 1;
    int datai[number];

    for (int n = 0; n < number; ++n)
    {
        out << i;
    }
    // for (int j = 0; j < number; j++)
    // {
    //     out << datai[j];
    // }

    out.close();
}