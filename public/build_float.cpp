//直接float计算


#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

using namespace std;

void prompt();
void help();
void initialVector(vector<float> &v, string &sf, char c, bool &iv); //初始化向量,不截取字符串再转换成vector，避免空格太多超出字符串长度
long double dotProduct(vector<float> v[]);                          //点积运算
long double dotProduct(float *f1, float *f2, int len);              //读文件的计算
long double result(float f1, float f2, long double sum);

//判断是否合法
int isVaild(char c, char afterc, bool inexp); //初始化向量时判断字符是否为有效字符
bool isVaild_VaildChar(char c, char ac, bool inexp);
bool isVaild_InvaildChar(char c);
bool isVaild_NoEnter(vector<float> v);
bool isVaild_DifferentLen(vector<float> v1, vector<float> v2);

int main()
{
    // cout.setf(ios_base::fixed, ios_base::floatfield);
    //提示
    prompt();

    do
    {
        rewind(stdin);
        //询问执行什么
        cout << "*Please choose mode(press Enter to calculate dot product): ";
        string mode;
        getline(cin, mode);
        if (mode == "quit") //退出
            break;
        else if (mode == "help") //帮助
            help();
        else if (mode == "\0" || mode == "read") //计算
        {
            bool issamelen = false;
            string sum;
            if (mode == "\0")
            {
                //用vector存储
                vector<float> vector[2];
                vector[0].reserve(1000);
                vector[1].reserve(1000);

                //初始化vector
                string strfloat = ""; //存浮点数
                char c;               //获得的每个字符
                bool isvaild = true;  //判断是否合法

                do
                {
                    strfloat = "";
                    vector[0].clear();
                    cout << "vector 1 : ";
                    // randomVector(); //生成随机数
                    initialVector(vector[0], strfloat, c, isvaild);
                    rewind(stdin); //除去换行符
                } while (!isvaild);

                do
                {
                    strfloat = "";
                    vector[1].clear();
                    cout << "vector 2 : ";
                    // randomVector(); //生成随机数
                    initialVector(vector[1], strfloat, c, isvaild);
                    rewind(stdin); //除去换行符
                } while (!isvaild);

                //检查向量长度是否一致
                issamelen = isVaild_DifferentLen(vector[0], vector[1]);

                if (issamelen)
                {
                    auto t1 = chrono::steady_clock::now();
                    long double sum = dotProduct(vector); //...测试代码
                    auto t2 = chrono::steady_clock::now();
                    cout << "(time: " << chrono::duration<double, std::milli>(t2 - t1).count() << "ms)" << endl;

                    //输出
                    cout << "-> " << sum << endl;
                }
            }
            else //读入二进制文件
            {
                string name1;
                string name2;
                int n = 0;

                cout << "File name for vector 1: ";
                cin >> name1;
                ifstream r1(name1, ios::in | ios::binary);

                if (!r1)
                {
                    cout << "**Error: No such file.\n";
                }
                else
                {
                    cout << "File name for vector 2: ";
                    cin >> name2;
                    ifstream r2(name2, ios::in | ios::binary);
                    if (!r2)
                    {
                        cout << "**Error: No such file.\n";
                    }
                    else
                    {
                        cout << "Number of elements: ";
                        cin >> n;
                        if (cin.fail())
                        {
                            cout << "**Error: the number isn't a integer.\n";
                            cin.clear();
                            cin.ignore();
                        }

                        float **f = new float *[2];
                        f[0] = new float[n];
                        f[1] = new float[n];
                        long long byteslen = sizeof(float) * n;

                        r1.read(reinterpret_cast<char *>(f[0]), byteslen);
                        r2.read(reinterpret_cast<char *>(f[1]), byteslen);
                        r1.close();
                        r2.close();

                        auto t1 = chrono::steady_clock::now();
                        long double sum = dotProduct(f[0], f[1], n); //...测试代码
                        auto t2 = chrono::steady_clock::now();
                        cout << "(time: " << chrono::duration<double, std::milli>(t2 - t1).count() << "ms)" << endl;

                        //输出
                        cout << "-> " << sum << endl;
                        delete[] f[0];
                        delete[] f[1];
                        delete[] f;
                    }
                }
            }
        }
        else
        {
            cout << "**Error: Please enter \"help\", \"quit\" or press Enter.\n";
        }
    } while (true);
}

void prompt()
{
    cout << "Vectors calculator\n";
    cout << "-----------------------------------------------------------------------------\n";
}
void help()
{
    cout << "help:\n";
    cout << "    choose mode status:\n";
    cout << "        \"help\": get help\n";
    cout << "        \"quit\": quit calculator\n";
    cout << "        press Enter: dot product\n";
    cout << "    dot product:\n";
    cout << "        use comma to split each element\n";
    cout << "        press Enter to input another vector\n";
}

//检查合法的各种方法
int isVaild(char c, char afterc, bool inexp)
{

    if (isVaild_InvaildChar(c))
    {
        if (isVaild_VaildChar(c, afterc, inexp))
        {
            return 0;
        }
        else
            return 2;
    }

    return 1;
}

bool isVaild_VaildChar(char c, char ac, bool inexp)
{
    if (c <= 57 && c >= 48)
    {
        if (ac == '-' || ac == '+' || (inexp && (ac == 'e' || ac == 'E')))
        {
            return false;
        }
    }
    else if (c == '-' || c == '+')
    {
        if (ac == '-' || ac == '+' || ac == ',' || ac == '.' || ac == '\n' || ac == 'e' || ac == 'E')
            return false;
    }
    else if (c == ',' || c == '.')
    {
        if (ac == '\n' || ac == ',' || ac == '.' || ac == 'e' || ac == 'E')
            return false;
    }
    else if (c == 'e' || c == 'E')
    {
        if (!(ac == '+' || ac == '-') && !(ac >= 48 && ac <= 57))
            return false;
    }

    return true;
}
bool isVaild_InvaildChar(char c)
{
    if (c >= 48 && c <= 57 || c == ' ' || c == '-' || c == '+' || c == ',' || c == '.' || c == '\n' ||
        c == 'e' || c == 'E')
        return true;
    return false;
}

bool isVaild_DifferentLen(vector<float> v1, vector<float> v2)
{
    if (v1.size() != v2.size())
    {
        cout << "**Error: The two vectors' lengths are different and dot product need the same lengths.\n";
        return false;
    }
    return true;
}
bool isVaild_NoEnter(char c)
{
    if (c == '\n')
    {
        cout << "**Error: The vector has no element. Please enter again.\n";
        return false;
    }
    return true;
}
//初始化向量
void initialVector(vector<float> &v, string &sf, char c, bool &iv)
{
    char tem = getchar();
    bool inexp = false;

    if (v.empty())
    {
        if (!isVaild_NoEnter(tem))
        {
            iv = false;
            return;
        }
    }

    if (!(tem >= 48 && tem <= 57) && tem != '-' && tem != '+' && tem != ' ')
    {
        cout << "**Error: The first character should be number.Please enter again.\n";
        iv = false;
        return;
    }

    do
    {
        c = tem;
        char afterc = '\0';
        if (c != '\n')
        {
            afterc = getchar(); //取得下一个字符，跟c一起才能判断这个输入是否合法
            tem = afterc;       //由于上面已经getchar了，下一次循环将无法取到这个，因此用tem存
        }

        int vaild = isVaild(c, afterc, inexp);
        if (vaild == 0)
        {
            if (c != ' ')
            {
                if (c != ',' && c != '\n')
                {
                    sf = sf + c;
                    if (c == 'e' || c == 'E')
                    {
                        inexp = true;
                    }
                }
                else
                {
                    double e = atof(sf.c_str());
                    if (e > __FLT_MAX__ || e < -__FLT_MAX__)
                    {
                        cout << "Error: There are non-float numbers. Please enter again.\n";
                        iv = false;
                        return;
                    }

                    v.push_back(e);
                    sf.clear();
                    inexp = false;
                }
            }
        }
        else if (vaild == 1)
        {
            cout << "**Error: There are invaild characters. Please enter again.\n";
            iv = false;
            return;
        }
        else if (vaild == 2)
        {
            cout << "**Error: All characters are vaild but there are invaild arrangements. Please enter again.\n";
            iv = false;
            return;
        }

    } while (c != '\n');
    iv = true;
}

//运算
long double dotProduct(vector<float> v[])
{
    long double sum = 0;
    for (int i = 0; i < v[0].size(); i++)
    {
        sum = result(v[0][i], v[1][i], sum);
    }

    return sum;
}
long double dotProduct(float *f1, float *f2, int len) //读文件的计算
{
    long double sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum = result(f1[i], f2[i], sum);
    }

    return sum;
}

long double result(float f1, float f2, long double sum)
{
    sum += f1 * f2;
    return sum;
}
