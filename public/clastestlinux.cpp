#include <iostream>
#include <cblas.h>
#include <chrono>
#include <random>

using namespace std;

int main()
{
    long long number;
    cin >> number;
    double *data = new double[number];

    // random_device rd;
    mt19937 gen(12);
    float k = __FLT_MAX__;
    uniform_real_distribution<double> timed(-k, k);

    for (int n = 0; n < number; ++n)
    {
        data[n] = timed(gen);
    }

    //openblas计算
    //计算用时
    auto t1 = chrono::steady_clock::now();
    double sum = cblas_ddot(number, data, 1, data, 1); //...测试代码
    auto t2 = chrono::steady_clock::now();
    cout << "(time: " << chrono::duration<double, std::milli>(t2 - t1).count() << "ms)" << endl;
    cout << "-> " /*<< std::fixed */ << sum << endl;

    delete data;
}
