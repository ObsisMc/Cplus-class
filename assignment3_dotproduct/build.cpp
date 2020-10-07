//单个vector长度有限，向量多长依然不行（sum用string存储同，但已经足够大）
//未完成等效减法的加法

#include <iostream>
#include <vector>

using namespace std;

void prompt();
bool isVaild(char c);                                              //初始化向量时判断字符是否为有效字符
void initialVector(vector<float> &v, string &sf, char c, bool iv); //初始化向量,不截取字符串再转换成vector，避免空格太多超出字符串长度
string dotProduct(vector<float> v[]);                              //点积运算

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
              int &minlen, int &maxlen, int &outlen, int *&outcome, bool &isresultsign); //将乘得的结果加到sum
string deleteZorePadding(string s);                                                      //消除零后缀
string result(float f1, float f2, string sum);                                           //两向量点积的结果可能过大，需要大数计算

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

    rewind(stdin); //除去换行符
    strfloat = " ";
    cout << "vector 2 :";
    initialVector(vector[1], strfloat, c, isvaild);

    //计算
    string sum = dotProduct(vector);

    //输出
    cout << sum;
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
                    sf.clear();
                }
            }
        }

    } while (c != '\n');
}
string dotProduct(vector<float> v[])
{
    string sum = "0";
    for (int i = 0; i < v[0].size(); i++)
    {
        sum = result(v[0][i], v[1][i], sum);
        sum = deleteZorePadding(sum);
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

    //结果与sum相加
    preprocess(result, sum, num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint,
               beforepoint, sign1posi, sign2posi, len1, len2, minlen, maxlen, outlen, outcome, isresultsign);
    addition(numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint, beforepoint, sign1posi, sign2posi,
             len1, len2, minlen, maxlen, outlen, outcome, isresultsign);
    sum = finalprocess('+', num1, num2, numop1, numop2, pposi1, pposi2, ispoint1, ispoint2, afterpoint,
                       beforepoint, sign1posi, sign2posi, len1, len2, minlen, maxlen, outlen, outcome,
                       isresultsign);
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

string deleteZorePadding(string s)
{
    int i = s.length() - 1;
    while (s[i] == '0')
    {
        i--;
    }
    if (s[i] == '.')
        i--;

    return s.substr(0, i + 1);
}
