//不能嵌套符号如--2,---3，但可以处理带一个负号的数和不带符号的正数如-2+1,1--2

#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <cmath>

using namespace std;

int precedence(char);                                                                                        //将操作符根据优先级量化
string operation(string, string, char);                                                                      //计算
int *substract(int *, const char *, const char *, int, int, int, int, int, int, int, int, bool, bool, bool); //小数减法
int *substract(int *, const char *, const char *, int, int, int, int);                                       //整数减法
double divide(const char *, const char *);                                                                   //除法
int mode(string);                                                                                            //判断是数值运算还是参数赋值
bool isvalidmode2(char);                                                                                     //是否符合参数赋值的要求，参数名只能为英文和数字
string finalresult(string, vector<string>, vector<string>);                                                  //数字计算主体
string rmblankstr(string);                                                                                   //去除字符串中空白字符

string pow(string, string); //幂运算
int findcommaposi(string);  //找最外层pow函数的逗号
string abs(string);         //绝对值
int whichfunction(string);

int main()
{
    //prompt
    cout << "This is a calculator, enter \"quit\" to exit.(enter \"-help\" to get help)\n";
    cout << "-------------------------------------------------------------------------------------------------------\n";

    vector<string> parameters(0);
    vector<string> values(0);

    do
    {
        //输入字符串
        string preprestr;

        getline(cin, preprestr);

        //输入quit结束程序
        if (preprestr == "quit")
            break;

        //查看已定义参数
        if (preprestr == "-para")
        {
            cout << "----------\n";
            if (parameters.size() == 0)
            {
                cout << "No parameter.\n";
                cout << "----------\n";
            }
            else
            {
                for (int i = 0; i < parameters.size(); i++)
                {
                    cout << parameters[i] << " -> " << values[i] << endl;
                }
                cout << "----------\n";
            }

            continue;
        }

        //帮助
        if (preprestr == "-help")
        {
            cout << "------------------------------------------------------------------------------------------------------------------\n";
            cout << "*You can only enter '+' , '-' , '*' , '/' , '=' , ',' , '(' , ')' , numbers and letters in computation expression.\n";
            cout << "*Parameter: \n";
            cout << "     **Set parameters: there is must a '=', on the left of that is name of parameter, on the right is a number or \n";
            cout << "                       a expression.\n";
            cout << "     **Check parameter: enter \"-para\" \n";
            cout << "     **Parameter's name: only containing letters and numbers.\n";
            cout << "*Functions: \n";
            cout << "     **pow( [number] , [expression] ): compute power, [number] is the base, [expression] is the exponent.\n";
            cout << "     **abs( [number] ): get the absolute value of [number].\n";
            cout << "------------------------------------------------------------------------------------------------------------------\n";
            continue;
        }

        //预处理输入，将字符串中的空格删除
        string prestr = rmblankstr(preprestr);

        //判断模式并开始运行
        int cas = mode(prestr);
        if (cas == 0) //数值计算
        {
            cout << finalresult(prestr, parameters, values) << endl;
        }
        else if (cas > 0) //参数的赋值
        {
            string preprevalue(prestr, cas + 1, prestr.length() - cas - 1);
            string preparameter(prestr, 0, cas);

            string prevalue = rmblankstr(preprevalue);                //去除表达式空格
            string value = finalresult(prevalue, parameters, values); //计算表达式

            string parameter = rmblankstr(preparameter); //去除参数名空格
            int hasit = false;
            if (value == "Error: At least one parameter doesn't exist or there are letters mixed with numbers.")
            {

                cout << "Error: fail to set the parameter \"" << parameter << "\".(the expression is wrong)" << endl;
            }
            else
            {
                for (int i = 0; i < parameters.size(); i++)
                {
                    if (parameters[i] == parameter)
                    {
                        values[i] = value;
                        hasit = true;
                    }
                }
                if (!hasit)
                {
                    values.push_back(value);
                    parameters.push_back(parameter);
                }
                cout << "Parameter \"" << parameter << "\" has been set to " << value << ".\n";
            }
        }
        else if (cas == -1) //一些错误提示
        {
            cout << "Error: The format of the assignment of parameters is wrong.\n";
        }
        else if (cas == -2)
        {
            cout << "Error: There are invalid characters. (valid: letters, numbers and '*' , '/' , '-' , '+' , '.' , '(' , ')')\n ";
        }

    } while (true);
}

//数值运算
string finalresult(string prestr, vector<string> parameters, vector<string> values)
{
    string str = prestr + '#'; //字符串末尾添加停止标志

    vector<string> operands(0);
    vector<char> operators(0);

    bool befoisnum = false;
    bool beforeisrightbracket = false; //上一个字符是否为右括号，防止误将操作符‘-’当成负号
    bool beforeispara = false;         //上一个读取的是否为参数
    bool beforeisfunction = false;     //上一个是否为函数，函数有最高优先级，函数被看作数字
    bool isbracketinfun = false;       //函数里有无括号
    int bracketinfunc = -1;            //函数里有几个括号，当函数里的括号为0，且遇到')'则停止截取，开始计算函数
    int tagform = 0;
    int taglatt = 0;

    //数值运算
    for (int i = 0; i < str.length(); i++)
    {
        //是数字则继续读取直到读到非数字,然后将该数字字符串push入vector
        if (char(str[i]) >= 48 && char(str[i]) <= 57 || str[i] == '-' && !befoisnum && !beforeisrightbracket ||
            str[i] == '+' && !befoisnum && !beforeisrightbracket || str[i] == '.' || str[i] >= 65 && str[i] <= 90 ||
            str[i] >= 97 && str[i] <= 122 || beforeispara && str[i] == '(' || !isbracketinfun && beforeisfunction && str[i] != ')' ||
            beforeisfunction && isbracketinfun && ((bracketinfunc != 0 ^ str[i] != ')') || bracketinfunc != 0 && str[i] != ')'))
        {
            if (befoisnum)
            {
                taglatt = i;
            }
            else
            {
                tagform = i;
                taglatt = i;
            }

            befoisnum = true;
            if (!beforeisfunction && (str[i] >= 65 && str[i] <= 90 || str[i] >= 97 && str[i] <= 122))
            {
                beforeispara = true;
            }
            if (beforeispara && str[i] == '(')
            {
                beforeisfunction = true;
                beforeispara = false;
            }

            //函数里的括号数
            if (beforeisfunction && str[i] == '(')
            {
                isbracketinfun = true;
                bracketinfunc++;
            }
            if (beforeisfunction && str[i] == ')')
            {
                bracketinfunc--;
            }
        }
        //遇到非数字，看优先级，若优先级高则push，否则先运算再push
        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '(' || str[i] == ')' || str[i] == '/' || str[i] == '#')
        {
            //将字符串化的数字push进操作数vector中,注意两个操作符在一起的情况
            if (befoisnum)
            {
                string num;
                num = str.substr(tagform, taglatt - tagform + 1);
                if (beforeispara) //若截取的的是参数则执行，否则跳过
                {
                    bool hasthepar = false;
                    for (int i = 0; i < parameters.size(); i++) //没做异常处理（当参数不存在时）
                    {
                        if (parameters[i] == num)
                        {
                            num = values[i];
                            hasthepar = true;
                            break;
                        }
                    }
                    if (!hasthepar)
                        return "Error: At least one parameter doesn't exist or there are letters mixed with numbers.";
                }
                if (beforeisfunction && str[i] == ')') //截取的是函数，且碰到右括号
                {
                    int leftbracketposi = num.find('('); //第一个左括号位置

                    string funcname = num.substr(0, leftbracketposi);

                    int which = whichfunction(funcname);
                    if (which == 1) //pow函数运算
                    {
                        int commaposi = findcommaposi(num);
                        string numoffunc = num.substr(leftbracketposi + 1, commaposi - leftbracketposi - 1);
                        string preexponent = num.substr(commaposi + 1, i - commaposi - 1);

                        string num1 = finalresult(numoffunc, parameters, values);
                        string exponent = finalresult(preexponent, parameters, values);
                        num = pow(num1, exponent);
                    }
                    else if (which == 2) //abs函数运算
                    {
                        string numoffunc = num.substr(leftbracketposi + 1, i - leftbracketposi - 1);

                        num = abs(finalresult(numoffunc, parameters, values));
                    }
                }
                beforeispara = false;

                operands.push_back(num);
            }

            //操作符
            int funcerror = beforeisfunction ? 1 : 0; //因为函数最后有一个')'，但这不是操作符，根据我的栈的计算原理，i要再加一来取得下一个真正的操作符
            if (operators.empty())
            {
                operators.push_back(str[i + funcerror]);
            }
            else if (precedence(str[i + funcerror]) > precedence(operators.back()))
            {
                operators.push_back(str[i + funcerror]);
            }
            else
            {
                do //for循环不能保证所有数都操作过。当遇到‘#’开始while循环直到operators栈为空
                {
                    //遇到括号不操作
                    if (operators.back() == '(')
                    {
                        break;
                    }

                    string num2 = operands.back();
                    operands.pop_back();
                    string num1 = operands.back();
                    operands.pop_back(); //会不会出现pop后vector长度为负数？
                    char operat = operators.back();
                    operators.pop_back();

                    string outcome = operation(num1, num2, operat); //计算完成
                    operands.push_back(outcome);                    //计算结果push入operands
                } while (str[i + funcerror] == ')' && operators[operators.size() - 1] != '(' || str[i + funcerror] == '#' && !operators.empty());

                operators.push_back(str[i + funcerror]); //将刚才未push进operators的push进来

                if (operators.back() == ')' && operators[operators.size() - 2] == '(') //左括号与右括号间无操作符则pop它们
                {
                    operators.pop_back();
                    operators.pop_back();
                    beforeisrightbracket = true;
                }
                else
                {
                    beforeisrightbracket = false;
                }
            }

            befoisnum = false;
            if (beforeisfunction)
            {
                bool isbracketinfun = false;
                bracketinfunc = -1;
                beforeisfunction = false;
                i++; //不加一将导致多循环一次
            }

            if (str[i] != ')')
                beforeisrightbracket = false;
        }
    }
    return operands.back();
}

//优先级
int precedence(char operat)
{
    switch (operat)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case '(':
        return 3;
    case ')':
        return 0;
    case '#':
        return 0;
    }
    return 0;
}

//计算
string operation(string prenum1, string prenum2, char operat)
{
    string num1;
    string num2;

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
    const char *numop1 = num1.c_str();
    const char *numop2 = num2.c_str();

    //找出每个数字小数点位置，（index从零开始）从后向前数(有点繁琐)
    int pposi1 = num1.size();
    int pposi2 = num2.size();
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
    bool ispoint1 = pposi1 != num1.size() ? true : false;
    bool ispoint2 = pposi2 != num2.size() ? true : false;
    int afterpoint = 0;  //小数点后的最长位数
    int beforepoint = 0; //小数点前的最长位数

    //确定是否有符号
    int sign1posi = 0;
    int sign2posi = 0;
    if (numop1[0] == '-')
        sign1posi = 1;
    if (numop2[0] == '-')
        sign2posi = 1;

    //去符号的长度；
    int len1 = num1.size() - sign1posi;
    int len2 = num2.size() - sign2posi;
    int minlen = len1 < len2 ? len1 : len2;
    int maxlen = len1 > len2 ? len1 : len2;
    int outlen = 0; //可能的最大长度
    int *outcome;
    bool isresultsign = false;

    //去除符号的计算

    //加法
    if (operat == '+' && sign1posi == sign2posi || operat == '-' && !(sign2posi == sign1posi))
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
    else if (operat == '*') //乘法
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
    else if (operat == '-' || operat == '+' && !(sign1posi == sign2posi)) //减法
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
    else if (operat == '/') //除法
    {
        double ans = divide(numop1, numop2);
        string ansstr = to_string(ans);
        return ansstr;
    }

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
    else if (sign1posi == 1 && operat == '-' && sign2posi == 1 && !(resultlen == 2 && outcome[0] == 0))
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

    //对于有小数点的数加入小数点，无则跳过此步骤
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

//减法的方法，第一个数字总是较大的那个
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
                index = len1 - sign1 - 1 - i;
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

int mode(string prestr)
{
    int equaln = 0;
    int equalposi = INT_MAX;
    bool ismode2valid = true;
    bool hasletter = false;

    for (int i = 0; i < prestr.length(); i++)
    {
        if (prestr[i] == '=')
        {
            if (!hasletter) //参数是否含字母
                return -1;

            equaln++;
            if (equaln == 2)
                return -1;

            equalposi = i;
            if (equalposi + 1 >= prestr.length())
                return -1;
        }
        if (i < equalposi && !isvalidmode2(prestr[i]))
            ismode2valid = false;

        if (prestr[i] >= 65 && prestr[i] <= 90 || prestr[i] >= 97 && prestr[i] <= 122)
            hasletter = true;

        else if (!(prestr[i] >= 48 && prestr[i] <= 57 || prestr[i] == 46 || prestr[i] == 45 || prestr[i] == 40 || prestr[i] == ',' ||
                   prestr[i] == 41 || prestr[i] == 47 || prestr[i] == 42 || prestr[i] == 43 || prestr[i] == ' ' || prestr[i] == '='))
        {
            return -2;
        }
    }

    if (equaln == 0)
        return 0;
    if (equaln == 1 && ismode2valid && hasletter)
        return equalposi;

    return -1;
}

bool isvalidmode2(char c)
{
    if (c >= 48 && c <= 57 || c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 61)
        return true;
    return false;
}

string rmblankstr(string preprestr)
{
    string prestr = "";
    for (int i = 0; i < preprestr.length(); i++)
    {
        if (preprestr[i] != ' ')
            prestr = prestr.append(1, preprestr[i]);
    }

    return prestr;
}

//幂函数，当指数为整数时结果可以大数；否则调用cmath（结果的数字范围在double内）
string pow(string num, string preexponent)
{
    bool isinteg = true;
    string result = "1";

    for (int i = 0; i < preexponent.length(); i++)
    {
        if (preexponent[i] == '.' || preexponent[i] == '-')
        {
            isinteg = false;
            break;
        }
    }

    if (isinteg)
    {
        const char *str = preexponent.c_str();
        long exponent = atol(str);
        while (exponent >= 1)
        {
            if (exponent % 2 == 1)
            {
                exponent--;
                result = operation(result, num, '*');
            }
            exponent /= 2;
            num = operation(num, num, '*');
        }
    }
    else
    {
        double preans = pow(atof(num.c_str()), atof(preexponent.c_str()));
        result = to_string(preans);
    }

    return result;
}

string abs(string num)
{
    int len = 0;
    bool issign = false;
    int error;
    if (num[0] == '-')
    {
        len = num.length();
        issign = true;
        error = issign ? 1 : 0;
    }
    else
    {
        return num;
    }

    char *result = new char[len];
    for (int i = error; i < num.length(); i++)
    {
        result[i - error] = num[i];
    }
    result[len - 1] = '\0';

    return result;
}

int whichfunction(string str)
{
    if (str == "pow")
        return 1;
    else if (str == "abs")
        return 2;
    return 0;
}

int findcommaposi(string str)
{
    int brackectn = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '(')
            brackectn++;
        if (str[i] == ')')
            brackectn--;
        if (str[i] == ',' && brackectn == 0)
            return i;
    }

    return str.find(',');
}

double divide(const char *num1, const char *num2)
{

    double number1 = atof(num1);
    double number2 = atof(num2);

    return number1 / number2;
}