#include <iostream>
#include <random>

using namespace std;

int main()
{
    int number;
    cin >> number;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> time(-__FLT_MAX__, __FLT_MAX__);

    for (int n = 0; n < number; ++n)
    {
        cout << fixed << time(gen);
        if (n != number - 1)
            cout << ",";
    }

    cout << '\n';
}