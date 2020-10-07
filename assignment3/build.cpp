#include <iostream>
#include <vector>

using namespace std;

void prompt();
bool isVaild(char c);                                              //初始化向量时判断字符是否为有效字符
void initialVector(vector<float> &v, string &sf, char c, bool iv); //初始化向量,不截取字符串再转换成vector，避免空格太多超出字符串长度
string addition(float newelement, string sum);                     //两向量点积的结果可能过大，需要大数计算

int main()
{
    //提示
    prompt();

    //用vector存储
    vector<float> vector[2];
    vector[0].reserve(1000);
    vector[1].reserve(1000);

    //初始化vector
    string strfloat = ""; //存浮点数
    char c;               //获得的每个字符
    bool isvaild = true;  //判断是否为有效字符

    cout << "vector 1 :";
    initialVector(vector[0], strfloat, c, isvaild);

    rewind(stdin);
    strfloat = " ";
    cout << "vector 2 :";
    initialVector(vector[1], strfloat, c, isvaild);
    cout << vector[0].size();
    cout << vector[1].size();

    // for (int i = 0; i < 1; i++)
    // {

    //     for (int j = 0; j < vector[i].size(); j++)
    //     {
    //         cout << vector[i][j];
    //     }
    // }
}

void prompt()
{
    cout << "Vectors calculator\n";
    cout << "--------------------------------\n";
    cout << "--------------------------------\n";
}
bool isVaild(char c)
{
    if (c >= 48 && c <= 57 || c == ' ' || c == ',' || c == '.' || c == '\n')
        return true;
    return false;
}
void initialVector(vector<float> &v, string &sf, char c, bool iv)
{
    do
    {
        c = getchar();
        if (isVaild(c))
        {
            if (c != ' ')
            {
                if (c != ',' && c != '\n')
                    sf = sf + c;
                else
                {
                    const char *element = sf.c_str();
                    v.push_back(atof(element));
                }
            }
        }

    } while (c != '\n');
}
string addition(float newelement, string sum); //两向量点积的结果可能过大，需要大数计算