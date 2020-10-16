#include <iostream>
#include <random>
#include <fstream>

using namespace std;

int main()
{
    string name;
    cout<<"Enter the file's name you want (end with \".dat\"): ";
    cin >> name;
    int number;
    cout<<"Enter the length of the vector: ";
    cin >> number;
    float *data = new float[number];

    random_device rd;
    mt19937 gen(rd());
    float k = __FLT_MAX__;
    uniform_real_distribution<double> time(-k, k);

    fstream out(name, ios::out | ios::binary);

    for (int n = 0; n < number; ++n)
    {
        data[n] = time(gen);
    }

    out.write(reinterpret_cast<char *>(data), sizeof(float) * number);

    out.close();
}