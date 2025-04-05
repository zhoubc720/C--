#include <iostream>
#include <vector>
#include <algorithm> // 用于std::max_element

using namespace std;

void radixSort(int arr[], int n) {
    if (n <= 0) return;

    // 找到数组中的最大值，确定最高位
    int max_val = *max_element(arr, arr + n);

    // 从最低位到最高位依次进行排序
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        vector< vector<int> > buckets(10); // 10个桶，分别对应0-9

        // 将元素分配到对应的桶中
        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10; // 计算当前位的数字
            buckets[digit].push_back(arr[i]);
        }

        // 将桶中的元素按顺序收集回原数组
        int index = 0;
        for (vector<int> &bucket : buckets) {
            for (int num : bucket) {
                arr[index++] = num;
            }
        }
    }
}

int main() {
    // 测试用例
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = sizeof(arr) / sizeof(arr[0]);

    radixSort(arr, n);

    cout << "排序后的数组：";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
