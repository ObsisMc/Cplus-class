#include <iostream>
#include <random>
#include <fstream>

using namespace std;

int main()
{
    int number;
    cin >> number;
    float *data = new float[number];

    // random_device rd;
    mt19937 gen(1);
    float k = __FLT_MAX__;
    uniform_real_distribution<double> time(-k, k);

    fstream out("test1000000.dat", ios::out | ios::binary);

    // int i = 1;
    // int datai[number];

    for (int n = 0; n < number; ++n)
    {
        data[n] = time(gen);
    }

    out.write(reinterpret_cast<char *>(data), sizeof(float)*number);

    out.close();
}