#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#pragma GCC optimize(3, "Ofast", "inline")
using namespace std;

mutex mtx;

double dotProduct(double *f1, double *f2, long long len); //读文件的计算
double result(double f1, double f2, long double sum);
void branchProduct2(double *f1, double *f2, long long begin, long long end, long double &sum);

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

    //自己写的程序的计算
    //计算用时
    auto t1 = chrono::steady_clock::now();     //开始计时                                                                                                     //计算
    double sum = dotProduct(data, data, number); //...测试代码
    auto t2 = chrono::steady_clock::now();     //停止计时
    cout << "(time: " << chrono::duration<double, std::milli>(t2 - t1).count() << "ms)" << endl;

    //输出
    cout << "-> " /*<< std::fixed */ << sum << endl;

    delete data;
}

double dotProduct(double *f1, double *f2, long long len) //读文件的计算
{
    long double sum = 0;
    if (len >= 1000000)
    {
        long long p1e = len / 5;
        long long p2e = len / 5 * 2;
        long long p3e = len / 5 * 3;
        long long p4e = len / 5 * 4;
        long long p5e = len;

        thread first(branchProduct2, ref(f1), ref(f2), 0, ref(p1e), ref(sum));
        thread second(branchProduct2, ref(f1), ref(f2), ref(p1e), ref(p2e), ref(sum));
        thread third(branchProduct2, ref(f1), ref(f2), ref(p2e), ref(p3e), ref(sum));
        thread forth(branchProduct2, ref(f1), ref(f2), ref(p3e), ref(p4e), ref(sum));
        thread fifth(branchProduct2, ref(f1), ref(f2), ref(p4e), ref(p5e), ref(sum));

        first.join();
        second.join();
        third.join();
        forth.join();
        fifth.join();
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            sum = result(f1[i], f2[i], sum);
        }
    }

    return sum;
}

void branchProduct2(double *f1, double *f2, long long begin, long long end, long double &sum)
{
    long long i = begin;
    long double presum = 0;
    while (i < end)
    {
        presum += f1[i] * f2[i];
        i++;
    }

    mtx.lock();
    sum += presum;
    mtx.unlock();
}

double result(double f1, double f2, long double sum)
{
    sum += f1 * f2;
    return sum;
}
