#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

// 计算单个状态的SG值
int calculateSG(int n, const vector<int>& moves, vector<int>& sg) {
    if (sg[n] != -1) {
        return sg[n]; // 已经计算过，直接返回
    }

    unordered_set<int> successors; // 存储后继状态的SG值

    for (int move : moves) {
        if (n >= move) {
            successors.insert(calculateSG(n - move, moves, sg));
        }
    }

    // mex操作：找到最小的未出现的非负整数
    int mex = 0;
    while (successors.count(mex)) {
        mex++;
    }

    sg[n] = mex;
    return mex;
}

// 计算从0到max_n的所有SG值
vector<int> computeSGTable(int max_n, const vector<int>& moves) {
    vector<int> sg(max_n + 1, -1);
    sg[0] = 0; // 边界条件：0颗石子时SG值为0（必败态）

    for (int i = 1; i <= max_n; ++i) {
        calculateSG(i, moves, sg);
    }

    return sg;
}

// 打印SG表
void printSGTable(const vector<int>& sg) {
    cout << "SG Table:" << endl;
    for (int i = 0; i < sg.size(); ++i) {
        cout << "SG(" << i << ") = " << sg[i] << endl;
    }
}

// 判断当前玩家是否能赢（总SG值是否为0）
bool canWin(const vector<int>& piles, const vector<int>& sg) {
    int total = 0;
    for (int pile : piles) {
        total ^= sg[pile];
    }
    return total != 0;
}


int main() {
    // 定义每次可以取的石头数
    vector<int> moves = {1, 3, 4}; // 每次可以取1、3或4颗石子
    int max_n = 20; // 计算SG值到n=20

    // 计算SG表
    vector<int> sg = computeSGTable(max_n, moves);

    // 打印SG表
    printSGTable(sg);

    // 测试单个堆的胜负
    cout << "\nTest single pile:" << endl;
    for (int n = 0; n <= 10; ++n) {
        cout << "Pile size " << n << ": ";
        if (sg[n] != 0) {
            cout << "第 1 个玩家获得胜利 (SG = " << sg[n] << ")" << endl;
        } else {
            cout << "第 2 个玩家获得胜利 (SG = 0)" << endl;
        }
    }

    // 测试多个堆的胜负（异或和）
    cout << "\nTest multiple piles:" << endl;
    vector<vector<int>> testCases = {
        {5, 7},        // SG(5)=2, SG(7)=3 → 2^3=1≠0，先手胜
        {4, 4},        // SG(4)=0 → 0^0=0，后手胜
        {2, 3, 1},     // SG(2)=2, SG(3)=3, SG(1)=1 → 2^3^1=0，后手胜
        {10, 9}        // SG(10)=1, SG(9)=2 → 1^2=3≠0，先手胜
    };

    for (const auto& piles : testCases) {
        bool win = canWin(piles, sg);
        cout << "Piles [";
        for (int p : piles) cout << p << " ";
        cout << "]: ";
        if (win) {
            cout << "第 1 个玩家获得胜利." << endl;
        } else {
            cout << "第 2 个玩家获得胜利." << endl;
        }
    }

    return 0;
}