#include <iostream>
#include <fstream>

using namespace std;

int main()

{
    int n = 1;
    float data[n];
    string name;
    cin >> name;
    // float data2[n];
    ifstream in(name, ios::in | ios::binary);
    if (!in)
    {
        printf("error\n");
    }
    else
    {
        in.read(reinterpret_cast<char *>(data), sizeof(data));
        // in.read(reinterpret_cast<char *>(data2), sizeof(data2));
    }

    in.close();
    for (int i = 0; i < n; i++)
    {
        cout << data[i] << endl;
    }
    // for (int i = 0; i < n; i++)
    // {
    //     cout << data2[i] << endl;
    // }
}