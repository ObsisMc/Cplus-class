#include <iostream>
#include <vector>

using namespace std;

void prompt();
void initialvector(vector<float> v, string pv);
string addition(float newelement, string sum); //两向量点积的结果可能过大，需要大数计算

int main()
{
    //获取两个向量字符串
    string prevector1;
    string prevector2;
    cout<<"vector 1 :";
    getline(cin, prevector1);
    cout<<"vector 2 :";
    getline(cin, prevector2);

    //转换成vector类型
    vector<float> vector1(1000000000);
    vector<float> vector2(1000000000);

}

void prompt()
{
    cout<<"Vectors calculator\n";
    cout<<"--------------------------------\n";
    cout<<"--------------------------------\n";
}
void initialvector(vector<float> v, string pv)
{
    
    for(int i=0;i<pv.length();i++)
    {

    }
}
string addition(float newelement, string sum); //两向量点积的结果可能过大，需要大数计算