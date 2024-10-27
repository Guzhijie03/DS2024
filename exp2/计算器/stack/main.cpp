#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <cmath>

// 定义一个函数来计算两个数和一个运算符的结果
double calculate(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    case '^': return pow(a, b);
    default: return 0;
    }
}

// 定义一个函数来实现字符串计算器
double string_calculator(const std::string& expression) {
    // 定义两个栈分别存储运算符和运算数
    std::stack<char> op_stack;
    std::stack<double> num_stack;

    // 定义一个优先级表
    std::map<char, int> priority = {
      {'+', 1},
      {'-', 1},
      {'*', 2},
      {'/', 2},
      {'^', 3}
    };

    // 定义一个字符串流对象来读取表达式中的字符或数字
    std::istringstream iss(expression);

    // 定义一个字符变量和一个数字变量
    char ch;
    double num;

    // 遍历表达式中的每个字符
    while (iss >> ch) {
        // 如果是数字，就读取完整的数字，并将其压入运算数栈中
        if (std::isdigit(ch)) {
            iss.putback(ch); // 将字符放回字符串流中
            iss >> num; // 读取完整的数字
            num_stack.push(num); // 将数字压入运算数栈中
        }
        // 如果是左括号，就将其压入运算符栈中
        else if (ch == '(') {
            op_stack.push(ch);
        }
        // 如果是右括号，就不断弹出运算符和运算数，并进行计算，直到遇到左括号为止
        else if (ch == ')') {
            while (!op_stack.empty() && op_stack.top() != '(') {
                char op = op_stack.top(); // 弹出运算符
                op_stack.pop();
                double b = num_stack.top(); // 弹出右操作数
                num_stack.pop();
                double a = num_stack.top(); // 弹出左操作数
                num_stack.pop();
                double res = calculate(a, b, op); // 计算结果
                num_stack.push(res); // 将结果压入运算数栈中
            }
            if (!op_stack.empty() && op_stack.top() == '(') {
                op_stack.pop(); // 弹出左括号
            }
        }
        // 如果是其他运算符，就比较它和栈顶运算符的优先级，并进行相应的操作
        else {
            while (!op_stack.empty() && priority[ch] <= priority[op_stack.top()]) {
                char op = op_stack.top(); // 弹出运算符
                op_stack.pop();
                double b = num_stack.top(); // 弹出右操作数
                num_stack.pop();
                double a = num_stack.top(); // 弹出左操作数
                num_stack.pop();
                double res = calculate(a, b, op); // 计算结果
                num_stack.push(res); // 将结果压入运算数栈中
            }
            op_stack.push(ch); // 将当前运算符压入运算符栈中
        }
    }

    // 当遍历完表达式后，如果运算符栈不为空，就继续弹出运算符和运算数，并进行计算，直到运算符栈为空为止
    while (!op_stack.empty()) {
        char op = op_stack.top(); // 弹出运算符
        op_stack.pop();
        double b = num_stack.top(); // 弹出右操作数
        num_stack.pop();
        double a = num_stack.top(); // 弹出左操作数
        num_stack.pop();
        double res = calculate(a, b, op); // 计算结果
        num_stack.push(res); // 将结果压入运算数栈中
    }

    // 此时，运算数栈中只剩下一个元素，就是最终的计算结果，返回它即可
    return num_stack.top();
}

int main() {
    std::string expression; // 定义一个字符串变量来存储表达式
    std::cout << "请输入一个算式："; // 输出提示信息
    std::cin >> expression; // 从标准输入读取表达式
    double result = string_calculator(expression); // 调用函数计算结果
    std::cout << "算式计算结果是：" << result << "\n"; // 输出结果
    return 0; // 返回0表示程序正常结束
}