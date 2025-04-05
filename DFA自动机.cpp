#include <iostream>
#include <string>

using namespace std;

// 定义 DFA 的状态
enum State {
    A, // 偶数个 0，偶数个 1（初始态、接受态）
    B, // 奇数个 0，偶数个 1
    C, // 偶数个 0，奇数个 1
    D  // 奇数个 0，奇数个 1
};

// 定义 DFA 的转移函数
State transiton(State currentState, char input) {
    switch (currentState) {
        case A:
            if (input == '0') return B;
            else if (input == '1') return C;
            break;
        case B:
            if (input == '0') return A;
            else if (input == '1') return D;
            break;
        case C:
            if (input == '0') return D;
            else if (input == '1') return A;
            break;
        case D:
            if (input == '0') return C;
            else if (input == '1') return B;
            break;
    }
    // 如果输入不合法，返回一个当前态（或者抛出一个异常）
    return currentState;
}

// DFA 的验证函数
bool validate(string input) {
    State currentState = A; // 初始态
    for (char c : input) {
        currentState = transiton(currentState, c);
    }
    // 判断最终态是否是接受态
    return currentState == A;
}

// 测试函数
void testDFA() {
    // 测试用例
    string testCases[] = {
        "1100",
        "101",
        "000",
        "111111",
        "0101",
        "1001",
        "10010111010101",
        ""
    };
    // 运行测试用例
    for (string testCase : testCases) {
        bool accept = validate(testCase);
        cout << "输入 :" << testCase << " -> " << (accept ? "接受" : "拒绝") << endl;
    }
}

int main() {
    testDFA();  // 测试 DFA
    return 0;
}