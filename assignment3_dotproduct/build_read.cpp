//string计算，1000000元素28s

#include <iostream>
#include <vector>
#include <windows.h>
#include <random>
#include <fstream>

using namespace std;

void prompt();
void help();
void initialVector(vector<float> &v, string &sf, char c, bool &iv); //初始化向量,不截取字符串再转换成vector，避免空格太多超出字符串长度
string dotProduct(vector<float> v[]);                               //点积运算
string dotProduct(float *f1, float *f2, int len);

void preprocess(string &prenum1, string &prenum2, string &num1, string &num2, const char *&numop1,
                const char *&numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2, int &afterpoint,
                int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2, int &minlen, int &maxlen,
                int &outlen, int *&outcome, bool &isresultsign); //预处理
string finalprocess(char operat, string &num1, string &num2, const char *numop1,
                    const char *numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2, int &afterpoint,
                    int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2, int &minlen, int &maxlen,
                    int &outlen, int *outcome, bool &isresultsign); //最后的处理
void multiply(string &num1, string &num2, const char *&numop1, const char *numop2, int &pposi1, int &pposi2,
              int &sign1posi, int &sign2posi, int &len1, int &len2, int &outlen, int *&outcome,
              bool &isresultsign); //乘法
void addition(const char *&numop1, const char *&numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2,
              int &afterpoint, int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2,
              int &minlen, int &maxlen, int &outlen, int *&outcome, bool &isresultsign);                   //将乘得的结果加到sum
int *substract(int *result, const char *num1, const char *num2, int len1, int len2, int sign1, int sign2); //整数减法
int *substract(int *result, const char *num1, const char *num2, int len1, int len2, int beforepoint, int afterpoint,
               int sign1, int sign2, int pposi1, int pposi2, bool isnum1afterlong, bool ispoint1, bool ispoint2); //小数减法

string deleteZorePadding(string s);            //消除零后缀
string scientificSum(string sum);              //最终结果以科学计数法显示，因为精度只有float
string result(float f1, float f2, string sum); //两向量点积的结果可能过大，需要大数计算

//判断是否合法
int isVaild(char c, char afterc, bool inexp); //初始化向量时判断字符是否为有效字符
bool isVaild_VaildChar(char c, char ac, bool inexp);
bool isVaild_InvaildChar(char c);
bool isVaild_NoEnter(vector<float> v);
bool isVaild_DifferentLen(vector<float> v1, vector<float> v2);
void warning_LosePrecision(string f);

void randomVector();

int main()
{
    cout.setf(ios_base::fixed, ios_base::floatfield);
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
                    double time = 0;
                    double counts = 0;

                    //计算用时
                    LARGE_INTEGER nFreq;
                    LARGE_INTEGER nBeginTime;
                    LARGE_INTEGER nEndTime;
                    QueryPerformanceFrequency(&nFreq);
                    QueryPerformanceCounter(&nBeginTime);                                              //开始计时
                                                                                                       //计算
                    string sum = dotProduct(vector);                                                   //...测试代码
                    QueryPerformanceCounter(&nEndTime);                                                //停止计时
                    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart; //计算程序执行时间单位为s
                    cout << "(time: " << time * 1000 << "ms)" << endl;

                    warning_LosePrecision(sum); //是否丢失精度
                    //输出
                    cout << "-> " << scientificSum(sum) << endl;
                }
            }
            else //读入二进制文件
            {
                string name;
                int n = 0;

                cout << "File name: ";
                cin >> name;
                cout << "Number of elements: ";
                cin >> n;
                if (cin.fail())
                {
                    cout << "**Error: the number isn't a integer.\n";
                    cin.clear();
                    cin.ignore();
                }
                else
                {
                    ifstream r(name, ios::in | ios::binary);
                    if (!r)
                    {
                        cout << "**Error: No such file.\n";
                    }
                    else
                    {
                        float **f = new float *[2];
                        f[0] = new float[n];
                        f[1] = new float[n];
                        long long byteslen = sizeof(float) * n;

                        r.read(reinterpret_cast<char *>(f[0]), byteslen);
                        r.read(reinterpret_cast<char *>(f[1]), byteslen);
                        r.close();

                        double time = 0;
                        double counts = 0;

                        //计算用时
                        LARGE_INTEGER nFreq;
                        LARGE_INTEGER nBeginTime;
                        LARGE_INTEGER nEndTime;
                        QueryPerformanceFrequency(&nFreq);
                        QueryPerformanceCounter(&nBeginTime);                                              //开始计时
                                                                                                           //计算
                        string sum = dotProduct(f[0], f[1], n);                                            //...测试代码
                        QueryPerformanceCounter(&nEndTime);                                                //停止计时
                        time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart; //计算程序执行时间单位为s
                        cout << "(time: " << time * 1000 << "ms)" << endl;

                        warning_LosePrecision(sum); //是否丢失精度
                        //输出
                        cout << "-> " << scientificSum(sum) << endl;
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

void warning_LosePrecision(string f)
{
    if (f.size() > 7)
    {
        cout << "(**Warning: Result has been round to at most seven places because of precision.)\n";
    }
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
string dotProduct(vector<float> v[])
{
    string sum = "0";
    for (int i = 0; i < v[0].size(); i++)
    {
        sum = result(v[0][i], v[1][i], sum);
    }

    return sum;
}

string dotProduct(float *f1, float *f2, int len)
{
    string sum = "0";
    for (int i = 0; i < len; i++)
    {
        sum = result(f1[i], f2[i], sum);
    }

    return sum;
}

string result(float f1, float f2, string sum)
{
    //先将两个浮点数乘起来
    string prenum1 = to_string(f1);
    string prenum2 = to_string(f2);

    //预处理所需变量

    string num1;
    string num2;
    //字符串数字转字符数组
    const char *numop1;
    const char *numop2;
    //找出每个数字小数点位置，（index从零开始）从后向前数(有点繁琐)
    int pposi1;
    int pposi2;

    bool ispoint1;
    bool ispoint2;
    int afterpoint;  //小数点后的最长位数
    int beforepoint; //小数点前的最长位数

    //确定是否有符号
    int sign1posi;
    int sign2posi;

    //去符号的长度；
    int len1;
    int len2;
    int minlen;
    int maxlen;
    int outlen; //可能的最大长度
    int *outcome;
    bool isresultsign;

    //向量里的元素做乘法
    preprocess(prenum1, prenum2, num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint,
               beforepoint, sign1posi, sign2posi, len1, len2, minlen, maxlen, outlen, outcome, isresultsign);
    multiply(num1, num2, numop1, numop2, pposi1, pposi2, sign1posi, sign2posi, len1,
             len2, outlen, outcome, isresultsign);
    string result = finalprocess('*', num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint,
                                 beforepoint, sign1posi, sign2posi, len1, len2, minlen, maxlen, outlen, outcome,
                                 isresultsign);
    string resultdz = deleteZorePadding(result);

    //结果与sum相加
    preprocess(result, sum, num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint,
               beforepoint, sign1posi, sign2posi, len1, len2, minlen, maxlen, outlen, outcome, isresultsign);
    addition(numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint, beforepoint, sign1posi, sign2posi,
             len1, len2, minlen, maxlen, outlen, outcome, isresultsign);
    sum = deleteZorePadding(finalprocess('+', num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2,
                                         afterpoint, beforepoint, sign1posi, sign2posi, len1, len2, minlen,
                                         maxlen, outlen, outcome, isresultsign));

    return sum;
}

void multiply(string &num1, string &num2, const char *&numop1, const char *numop2, int &pposi1, int &pposi2,
              int &sign1posi, int &sign2posi, int &len1, int &len2, int &outlen, int *&outcome, bool &isresultsign)
{
    int back1 = pposi1 == num1.size() ? 0 : 1; //消除小数点的位
    int back2 = pposi2 == num2.size() ? 0 : 1;

    outlen = len1 + len2 - back1 - back2;
    outcome = new int[outlen]();
    for (int i = len2 + sign2posi - 1; i >= sign2posi; i--)
    {
        if (numop2[i] == '.')
            continue;
        for (int j = len1 + sign1posi - 1; j >= sign1posi; j--)
        {
            if (numop1[j] == '.')
                continue;

            int pmove1 = (pposi1 != num1.size() && j > len1 - pposi1 - 1) ? 1 : 0;
            int pmove2 = (pposi2 != num2.size() && i > len2 - pposi2 - 1) ? 1 : 0;

            outcome[outlen + sign1posi + sign2posi - i - j - 2 + pmove1 + pmove2] += (numop1[j] - 48) * (numop2[i] - 48); //待确定
        }
    }

    if (sign1posi == 1 ^ sign2posi == 1)
        isresultsign = true;
}

void addition(const char *&numop1, const char *&numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2,
              int &afterpoint, int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2,
              int &minlen, int &maxlen, int &outlen, int *&outcome, bool &isresultsign)
{
    if (sign1posi == sign2posi) //加法
    {
        if (ispoint2 || ispoint1)
        {
            bool isnum1afterlong = true;                              //num1小数点前的位数前是否是最长的
            bool isnum1beforelong = true;                             //num1小数点后的位数前是否是最长的
            if (pposi1 > pposi2 && ispoint2 || ispoint1 && !ispoint2) //计算小数点后的最长长度
            {
                afterpoint = pposi1;
            }
            else
            {
                afterpoint = pposi2;
                isnum1afterlong = false;
            }
            if (len1 - (ispoint1 ? pposi1 + 1 : 0) > len2 - (ispoint2 ? pposi2 + 1 : 0)) //小数点前的最长位数
            {
                beforepoint = len1 - (ispoint1 ? pposi1 + 1 : 0);
            }
            else
            {
                beforepoint = len2 - (ispoint2 ? pposi2 + 1 : 0);
                isnum1beforelong = false;
            }

            //确定长度
            outlen = afterpoint + beforepoint + 1;
            //创建数组
            outcome = new int[outlen]();

            int firstpause; //循环中第一个停止点
            if (pposi1 > pposi2 && ispoint2 || ispoint1 && !ispoint2)
                firstpause = pposi1 - (ispoint2 ? pposi2 : 0);
            else
                firstpause = pposi2 - (ispoint1 ? pposi1 : 0);

            for (int i = 0; i < outlen; i++) //开始计算
            {
                if (i < firstpause) //小数点后，较长位数的数字与较短位数的数字  没有  重叠的区域
                {
                    if (isnum1afterlong)
                    {
                        int index = len1 + sign1posi - i - 1;
                        outcome[i] = numop1[index] - 48;
                    }
                    else
                    {
                        int index = len2 + sign2posi - i - 1;
                        outcome[i] = numop2[index] - 48;
                    }
                }
                else if (i < afterpoint) //小数点后，较长位数的数字与较短位数的数字  有  重叠的区域
                {
                    if (isnum1afterlong)
                    {
                        int index1 = len1 + sign1posi - i - 1;
                        int index2 = len2 + sign2posi - (i - pposi1 + pposi2) - 1;
                        outcome[i] = numop1[index1] + numop2[index2] - 96;
                    }
                    else
                    {
                        int index1 = len2 + sign2posi - i - 1;
                        int index2 = len1 + sign1posi - (i - pposi2 + pposi1) - 1;
                        outcome[i] = numop2[index1] + numop1[index2] - 96;
                    }
                }
                else if (i > afterpoint) //小数点前
                {
                    if (isnum1beforelong) //第一个数小数点前的位数较大
                    {
                        if (i < afterpoint + len2 - (ispoint2 ? pposi2 : -1)) //小数点前的位数中，两数字长度相等的地方
                        {
                            if (isnum1afterlong)
                            {
                                int index1 = len1 + sign1posi - i - 1;
                                int index2 = len2 + sign2posi - (i - pposi1 + (ispoint2 ? pposi2 : -1)) - 1;
                                outcome[i - 1] = numop1[index1] + numop2[index2] - 96;
                            }
                            else
                            {
                                int index1 = len2 + sign2posi - i - 1;
                                int index2 = len1 + sign1posi - (i - pposi2 + (ispoint1 ? pposi1 : -1)) - 1;
                                outcome[i - 1] = numop2[index1] + numop1[index2] - 96;
                            }
                        }
                        else //小数点前的位数中，两较长位数的数字与较短位数的数字  没有 重叠的区域
                        {
                            if (isnum1afterlong) //在第一个数小数点后的位数较大的情况下,第一个数小数点前的位数较大
                            {
                                int index = len1 + sign1posi - i - 1;
                                outcome[i - 1] = numop1[index] - 48;
                            }
                            else //在第二个数小数点后的位数较大的情况下,第一个数小数点前的位数较大
                            {
                                int index = len1 + sign1posi - (i - pposi2 + (ispoint1 ? pposi1 : -1)) - 1;
                                outcome[i - 1] = numop1[index] - 48;
                            }
                        }
                    }
                    else //第一个数小数点前的位数较大
                    {
                        if (i < afterpoint + len1 - (ispoint1 ? pposi1 : -1))
                        {
                            if (isnum1afterlong)
                            {
                                int index1 = len1 + sign1posi - i - 1;
                                int index2 = len2 + sign2posi - (i - pposi1 + (ispoint2 ? pposi2 : -1)) - 1;
                                outcome[i - 1] = numop1[index1] + numop2[index2] - 96;
                            }
                            else
                            {
                                int index1 = len2 + sign2posi - i - 1;
                                int index2 = len1 + sign1posi - (i - pposi2 + (ispoint1 ? pposi1 : -1)) - 1;
                                outcome[i - 1] = numop2[index1] + numop1[index2] - 96;
                            }
                        }
                        else //小数点前的位数中，两较长位数的数字与较短位数的数字  没有 重叠的区域
                        {
                            if (isnum1afterlong)
                            {
                                int index = len2 + sign2posi - (i - pposi1 + (ispoint2 ? pposi2 : -1)) - 1;
                                outcome[i - 1] = numop2[index] - 48;
                            }
                            else
                            {
                                int index = len2 + sign2posi - i - 1;
                                outcome[i - 1] = numop2[index] - 48;
                            }
                        }
                    }
                }
            }
        }
        else //没有小数的加法
        {
            //确定长度
            outlen = maxlen + 1;
            //创建数组
            outcome = new int[outlen]();

            for (int i = 0; i < maxlen; i++)
            {
                if (i <= minlen - 1)
                {
                    int value = numop1[len1 + sign1posi - 1 - i] + numop2[len2 + sign2posi - 1 - i] - 96;
                    outcome[i] = value;
                }
                else
                {
                    if (minlen == len1)
                    {
                        outcome[i] = numop2[len2 + sign2posi - 1 - i] - 48;
                    }
                    else
                    {
                        outcome[i] = numop1[len1 + sign1posi - 1 - i] - 48;
                    }
                }
            }
        }

        if (sign1posi == 1)
            isresultsign = true;
    }
    else //等效减法
    {
        bool isnum1afterlong = true; //num1小数点前的位数前是否是最长的
        if (ispoint1 || ispoint2)    //有小数点
        {
            if (ispoint1 && !ispoint2 || pposi1 >= pposi2 && ispoint1 && ispoint2) //找到小数点后最大位数
            {
                afterpoint = pposi1;
            }
            else
            {
                afterpoint = pposi2;
                isnum1afterlong = false;
            }

            int beforep1 = len1 - (ispoint1 ? pposi1 : -1) - 1; //第一个数小数点前有几位
            int beforep2 = len2 - (ispoint2 ? pposi2 : -1) - 1; //第二个数小数点前有几位
            beforepoint = beforep1 > beforep2 ? beforep1 : beforep2;

            outlen = afterpoint + beforepoint;
            outcome = new int[outlen];
        }
        else //无小数点
        {
            outlen = maxlen;
            outcome = new int[outlen]();
        }

        //开始计算
        if (len1 - (ispoint1 ? pposi1 + 1 : 0) > len2 - (ispoint2 ? pposi2 + 1 : 0)) //第一个数小数点前的位数大
        {
            if (ispoint1 || ispoint2)
            {
                outcome = substract(outcome, numop1, numop2, len1, len2, beforepoint, afterpoint, sign1posi, sign2posi, pposi1, pposi2, isnum1afterlong, ispoint1, ispoint2);
            }
            else
            {
                outcome = substract(outcome, numop1, numop2, len1, len2, sign1posi, sign2posi);
            }
            isresultsign = false;
        }
        else if (len1 - (ispoint1 ? pposi1 + 1 : 0) == len2 - (ispoint2 ? pposi2 + 1 : 0)) //两个数的小数点前的位数一样
        {
            int tag1 = INT_MAX;
            int tag2 = INT_MAX;
            for (int i = 0; i < len1; i++)
            {
                if (i > len2 - 1)
                    break;
                if (numop1[i + sign1posi] > numop2[i + sign2posi])
                    tag1 = i;
                else if (numop1[i + sign1posi] < numop2[i + sign2posi])
                    tag2 = i;
                if (tag1 != INT_MAX && tag2 != INT_MAX)
                    break;
            }
            if (tag1 > tag2 || tag1 == tag2 && len1 < len2) //第2个数大
            {
                if (ispoint1 || ispoint2)
                {
                    if (pposi2 < pposi1 && ispoint2 && ispoint1 || !ispoint2 && ispoint1)
                        isnum1afterlong = false;
                    else
                        isnum1afterlong = true;
                    outcome = substract(outcome, numop2, numop1, len2, len1, beforepoint, afterpoint, sign2posi, sign1posi, pposi2, pposi1, isnum1afterlong, ispoint2, ispoint1);
                }
                else
                    outcome = substract(outcome, numop2, numop1, len2, len1, sign2posi, sign1posi);
                isresultsign = true;
            }
            else if (tag1 < tag2 || tag1 == tag2 && len1 > len2) //第1个数大
            {
                if (ispoint1 || ispoint2)
                    outcome = substract(outcome, numop1, numop2, len1, len2, beforepoint, afterpoint, sign1posi, sign2posi, pposi1, pposi2, isnum1afterlong, ispoint1, ispoint2);
                else
                    outcome = substract(outcome, numop1, numop2, len1, len2, sign1posi, sign2posi);
                isresultsign = false;
            }
            else //两数相等
            {
                for (int i = 0; i < outlen; i++)
                {
                    outcome[i] = 0;
                }
                ispoint1 = false;
                ispoint2 = false;
                isresultsign = false;
            }
        }
        else //第二个数小数点前的位数大
        {
            if (ispoint1 || ispoint2)
            {
                if (pposi2 < pposi1 && ispoint2 && ispoint1 || !ispoint2 && ispoint1)
                    isnum1afterlong = false;
                else
                    isnum1afterlong = true;
                outcome = substract(outcome, numop2, numop1, len2, len1, beforepoint, afterpoint, sign2posi, sign1posi, pposi2, pposi1, isnum1afterlong, ispoint2, ispoint1);
            }
            else
            {
                outcome = substract(outcome, numop2, numop1, len2, len1, sign2posi, sign1posi);
            }
            isresultsign = true;
        }
    }
}

void preprocess(string &prenum1, string &prenum2, string &num1, string &num2, const char *&numop1,
                const char *&numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2, int &afterpoint,
                int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2, int &minlen, int &maxlen,
                int &outlen, int *&outcome, bool &isresultsign)
{
    //除去正号
    if (prenum1[0] == '+')
    {
        num1 = prenum1.substr(1);
    }
    else
    {
        num1 = prenum1;
    }

    if (prenum2[0] == '+')
    {
        num2 = prenum2.substr(1);
    }
    else
    {
        num2 = prenum2;
    }

    //字符串数字转字符数组
    numop1 = num1.c_str();
    numop2 = num2.c_str();

    //找出每个数字小数点位置，（index从零开始）从后向前数(有点繁琐)
    pposi1 = num1.size();
    pposi2 = num2.size();
    for (int i = 0; i < num1.size(); i++)
    {
        if (numop1[i] == '.')
        {
            pposi1 = num1.size() - i - 1;
            break;
        }
    }
    for (int i = 0; i < num2.size(); i++)
    {
        if (numop2[i] == '.')
        {
            pposi2 = num2.size() - i - 1;
            break;
        }
    }
    ispoint1 = pposi1 != num1.size() ? true : false;
    ispoint2 = pposi2 != num2.size() ? true : false;
    afterpoint = 0;  //小数点后的最长位数
    beforepoint = 0; //小数点前的最长位数

    //确定是否有符号
    sign1posi = 0;
    sign2posi = 0;
    if (numop1[0] == '-')
        sign1posi = 1;
    if (numop2[0] == '-')
        sign2posi = 1;

    //去符号的长度；
    len1 = num1.size() - sign1posi;
    len2 = num2.size() - sign2posi;
    minlen = len1 < len2 ? len1 : len2;
    maxlen = len1 > len2 ? len1 : len2;
    outlen = 0; //可能的最大长度

    isresultsign = false;
}

string finalprocess(char operat, string &num1, string &num2, const char *numop1,
                    const char *numop2, int &pposi1, int &pposi2, bool &ispoint1, bool &ispoint2, int &afterpoint,
                    int &beforepoint, int &sign1posi, int &sign2posi, int &len1, int &len2, int &minlen, int &maxlen,
                    int &outlen, int *outcome, bool &isresultsign)
{
    //计算进位
    if (operat == '-' && sign1posi == sign2posi || operat == '+' && !(sign1posi == sign2posi))
    {
    }
    else
    {
        for (int i = 0; i < outlen; i++)
        {
            int k = outcome[i] / 10;
            int m = outcome[i] % 10;
            outcome[i] = m;
            outcome[i + 1] += k;
        }
    }

    //新建一个char数组接收outcome数组以便除去前导零，同时还要加上符号
    //确定新数组长度

    int resultlen = 0;
    int zreolen = 0;

    if (operat == '-' && !(sign2posi == 0 ^ sign1posi == 0) || operat == '+' && !(sign1posi == sign2posi))
    {
        int reduce = 0;
        for (int i = outlen - 1; i >= 0; i--)
        {
            if ((ispoint1 || ispoint2) && i < afterpoint + 1)
                break;
            if (outcome[i] != 0)
            {
                reduce = outlen - i - 1;
                break;
            }
            else
            {
                zreolen++;
            }
        }
        if (ispoint1 || ispoint2)
        {
            resultlen = outlen - zreolen + 1;
        }
        else
        {
            if (zreolen == outlen)
            {
                resultlen = 2;
            }
            else
            {
                resultlen = outlen - reduce + 1;
            }
        }
    }
    else
    {
        if (outcome[outlen - 1] == 0)
        {
            resultlen = outlen;
        }
        else
        {
            resultlen = outlen + 1;
        }
    }

    if (sign1posi == 1 && operat == '+' && sign2posi == 0)
    {
        isresultsign = !isresultsign;
    }
    if (isresultsign)
        resultlen++;

    //将outcome数组元素传给新数组result,未加入小数点
    char *result = new char[resultlen]();
    int error = isresultsign ? 1 : 0;           //由于符号导致的移位
    for (int i = error; i < resultlen - 1; i++) //result最后一位添加'\0'，第一位加符号
    {
        result[resultlen - 2 - i + error] = outcome[i - error] + 48; //outcome从i-error即0开始
    }

    if (isresultsign) //加上符号和空字符
        result[0] = '-';
    result[resultlen - 1] = '\0';

    //先建resultstr（最终输出结果）
    string resultstr;
    resultstr = result; //将没有小数的结果先存起来

    //再建数组pointresult存含小数点的数

    int pointresultlen = resultlen + 1;
    char *pointresult = new char[pointresultlen]();
    int ppositot = 0;         //小数点从后往前数的位置
    if (ispoint1 || ispoint2) //否有小数点
    {
        if (operat == '*') //如果是乘法
        {
            ppositot = (ispoint1 ? pposi1 : 0) + (ispoint2 ? pposi2 : 0); //确定小数点位置

            for (int i = pointresultlen - 1; i >= 0; i--)
            {
                if (i == pointresultlen - 2 - ppositot)
                    pointresult[i] = '.';
                else
                    pointresult[i] = result[i - (i > pointresultlen - 2 - ppositot ? 1 : 0)];
            }

            resultstr = pointresult;
        }
        else if (operat == '+' || operat == '-') //如果是加法或减法
        {
            ppositot = afterpoint;
            for (int i = pointresultlen - 1; i >= 0; i--)
            {
                if (i == pointresultlen - 2 - ppositot)
                    pointresult[i] = '.';
                else
                    pointresult[i] = result[i - (i > pointresultlen - 2 - ppositot ? 1 : 0)];
            }
            resultstr = pointresult;
        }
    }

    delete[] outcome;
    delete[] result;
    delete[] pointresult;

    return resultstr;
}

int *substract(int *result, const char *num1, const char *num2, int len1, int len2, int sign1, int sign2) //无小数点
{
    bool bit = false; //是否借位

    for (int i = 0; i < len1; i++)
    {
        if (i < len2)
        {
            if (num1[len1 - 1 - i + sign1] - (bit ? 1 : 0) >= num2[len2 - 1 - i + sign2])
            {
                result[i] = num1[len1 - 1 - i + sign1] - (bit ? 1 : 0) - num2[len2 - 1 - i + sign2];
                bit = false;
            }
            else
            {
                result[i] = num1[len1 - 1 - i + sign1] - (bit ? 1 : 0) - num2[len2 - 1 - i + sign2] + 10;
                bit = true;
            }
        }
        else
        {
            if (num1[len1 - 1 - i + sign1] - 48 - (bit ? 1 : 0) >= 0)
            {
                result[i] = num1[len1 - 1 - i + sign1] - (bit ? 1 : 0) - 48;
                bit = false;
            }
            else
            {
                result[i] = num1[len1 - 1 - i + sign1] - (bit ? 1 : 0) + 10 - 48;
                bit = true;
            }
        }
    }

    return result;
}

int *substract(int *result, const char *num1, const char *num2, int len1, int len2, int beforepoint, int afterpoint,
               int sign1, int sign2, int pposi1, int pposi2, bool isnum1afterlong, bool ispoint1, bool ispoint2)
{
    int outlen = beforepoint + afterpoint;

    if (!ispoint1)
        pposi1 = 0;
    if (!ispoint2)
        pposi2 = 0;

    int firstpause; //小数点之后两数的重叠区域
    if (pposi1 > pposi2 && ispoint2 || ispoint1 && !ispoint2)
    {
        firstpause = pposi1 - pposi2;
    }
    else
    {
        firstpause = pposi2 - pposi1;
    }

    int secondpause; //小数点前两数的重叠区域
    if (isnum1afterlong)
    {
        secondpause = len2 - (ispoint2 ? pposi2 : -1) + afterpoint;
    }
    else
    {
        secondpause = len2;
    }

    bool bit = false; //是否借位
    for (int i = 0; i < outlen + 1; i++)
    {
        if (i < firstpause) //小数点之后重叠的部分
        {
            if (isnum1afterlong) //若被减数的小数点之后的位数大于减数的
            {
                int ans = num1[len1 + sign1 - i - 1] - 48;
                result[i] = ans;
                bit = false;
            }
            else //若被减数的小数点之后的位数小于减数的
            {
                if (num2[len2 + sign2 - 1 - i] > 48)
                {
                    if (!bit)
                    {
                        int ans = 10 - (num2[len2 + sign2 - 1 - i] - 48);
                        result[i] = ans;
                        bit = true;
                    }
                    else
                    {
                        int ans = 9 - (num2[len2 + sign2 - 1 - i] - 48);
                        result[i] = ans;
                    }
                }
                else
                {
                    if (bit)
                    {
                        result[i] = 9;
                    }
                    else
                    {
                        result[i] = 0;
                    }
                }
            }
        }
        else if (i < afterpoint)
        {
            int index1;
            int index2;
            if (isnum1afterlong)
            {
                index1 = len1 + sign1 - i - 1;
                if (pposi1 == pposi2)
                    index2 = len2 + sign2 - (i - (pposi1 - (ispoint2 ? pposi2 : -1))) - 1;
                else
                    index2 = len2 + sign2 - (i - (pposi1 - (ispoint2 ? pposi2 : -1))) - 1;
            }
            else
            {
                index1 = len1 + sign1 - (i - (pposi2 - (ispoint1 ? pposi1 : -1))) - 1;
                index2 = len2 + sign2 - i - 1;
            }

            if (num1[index1] - num2[index2] - (bit ? 1 : 0) >= 0)
            {
                result[i] = num1[index1] - num2[index2] - (bit ? 1 : 0);
                bit = false;
            }
            else
            {
                result[i] = num1[index1] - num2[index2] - (bit ? 1 : 0) + 10;
                bit = true;
            }
        }

        else if (i == afterpoint)
        {
        }
        else if (i < secondpause)
        {
            int index1;
            int index2;
            if (isnum1afterlong) //被减数小数点后位数长
            {
                index1 = len1 + sign1 - 1 - i;
                index2 = len2 + sign2 - 1 - (i - (ispoint2 ? pposi1 - pposi2 : pposi1 + 1));
            }
            else //减数小数点后位数长
            {
                index1 = len1 + sign1 - 1 - (i - (ispoint1 ? pposi2 - pposi1 : pposi2 + 1));
                index2 = len2 + sign2 - 1 - i;
            }

            if (num1[index1] - num2[index2] - (bit ? 1 : 0) >= 0)
            {
                int ans = num1[index1] - num2[index2] - (bit ? 1 : 0);
                result[i - 1] = ans;
                bit = false;
            }
            else
            {
                int ans = 10 + num1[index1] - num2[index2] - (bit ? 1 : 0);
                result[i - 1] = ans;
                bit = true;
            }
        }
        else
        {
            int index;
            if (isnum1afterlong)
            {
                index = len1 + sign1 - 1 - i;
            }
            else
            {
                index = len1 + sign1 - 1 - (i - (ispoint1 ? pposi2 - pposi1 : pposi2 + 1));
            }

            if (num1[index] - 48 - (bit ? 1 : 0) >= 0)
            {
                result[i - 1] = num1[index] - 48 - (bit ? 1 : 0);
            }
            else
            {
                result[i - 1] = num1[index] - 48 - (bit ? 1 : 0) + 10;
            }
        }
    }
    return result;
}

string deleteZorePadding(string s)
{
    // int j = s.length() - 1;
    // while (s[j] != '.' && j >= 0)
    // {
    //     j--;
    // }
    // if (j = -1)
    //     return s;
    int i = s.length() - 1;
    while (s[i] == '0')
    {
        i--;
    }
    if (s[i] == '.')
        i--;

    return s.substr(0, i + 1);
}

string scientificSum(string sum)
{
    string result;
    int pointposi = sum.length() - 1; //小数点位置
    int exp = 0;                      //10的指数大小
    int issigned = (sum[0] == '-' ? 1 : 0);
    char *re;
    while (sum[pointposi] != '.' && pointposi >= 0)
    {
        pointposi--;
    }
    if (pointposi == -1)
    {
        exp = sum.length() - 1 - issigned;
    }
    else
    {
        exp = pointposi - 1 - issigned;
    }

    if (pointposi == -1 && sum.size() <= 7 + issigned || pointposi != -1 && sum.size() <= 8 + issigned)
    {
        return sum;
    }
    else
    {
        string scienstr;
        int start = (sum[0] == '-' || sum[0] == '+') ? 1 : 0;
        int lastplace = 0;

        if (pointposi == -1)
        {
            scienstr = sum.substr(start, 1) + "." + sum.substr(1 + start, 5);
            lastplace = start + 6;
        }
        else
        {
            if (pointposi > 6 + start)
            {
                scienstr = sum.substr(start, 1) + "." + sum.substr(1 + start, 5);
                lastplace = start + 6; //最后一位数
            }
            else
            {
                scienstr = sum.substr(start, 1) + "." + sum.substr(1 + start, pointposi - 1 - start) + sum.substr(pointposi + 1, 6 - (pointposi - start));
                lastplace = 6 + start + 1;
            }
        }

        int afterlast = (pointposi == lastplace + 1 ? lastplace + 2 : lastplace + 1);
        if (atoi(to_string(sum[afterlast]).c_str()) - 48 < 5)
        {
            scienstr += sum.substr(lastplace, 1);
        }
        else
        {
            int last = atoi(sum.substr(lastplace, 1).c_str()) + 1;
            if (last < 10) //未进位
            {
                scienstr += to_string(last);
            }
            else //进位
            {
                const char *preresult = scienstr.c_str();
                int len = scienstr.length();
                int r[len] = {0};
                int m = 1;
                for (int i = len - 1; i >= 0; i--)
                {
                    if (i != 1)
                    {
                        int value = scienstr[i] - 48;
                        int k = (value + m) % 10;
                        m = (value + m) / 10;
                        r[i + (i > 1 ? 0 : 1)] = k;
                    }
                }
                if (m == 1)
                    r[0] = 1;

                int startp = 0;
                if (r[0] == 0)
                {
                    startp = 1;
                }
                if (startp == 1)
                {
                    re = new char[len + 1];
                    for (int i = 0; i < len; i++)
                    {
                        if (i == 1)
                        {
                            re[1] = '.';
                        }
                        else
                        {
                            int a = r[i + startp - ((i > 1) ? 1 : 0)];
                            re[i] = a + 48;
                        }
                    }
                    re[len - 1] = r[len - 1] + 48;
                    re[len] = '\0';
                }
                else
                {
                    re = new char[2];
                    re[0] = '1';
                    re[1] = '\0';
                    exp++;
                }

                scienstr = re;
            }
        }

        result = (issigned == 1 ? "-" : "") + scienstr + "E+" + to_string(exp);
    }
    delete[] re;
    return result;
}

void randomVector()
{
    int number = 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> time(-__FLT_MAX__, __FLT_MAX__);

    for (int n = 0; n < number; ++n)
    {
        cout << time(gen);
        if (n != number - 1)
            cout << ",";
    }

    cout << "\n";
}
