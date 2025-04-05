#include <iostream>
#include <vector>
#include <algorithm>  // std::sort

using namespace std;

// 桶排序
std::vector<int> bucketSort(const std::vector<int> &arr, int bucketSize) {
    if (arr.empty()) {
        return arr;
    }

    // 1. 确定数据的最小值和最大值
    int minValue = *std::min_element(arr.begin(), arr.end());
    int maxValue = *std::max_element(arr.begin(), arr.end());

    // 2. 计算桶的数量
    int bucketCount = (maxValue - minValue) / bucketSize + 1;
    
    // 3. 初始化桶
    std::vector< std::vector<int> > buckets(bucketCount);

    // 4. 将数据分配到桶中
    for (int num : arr) {
        int bucketIndex = (num - minValue) / bucketSize;
        buckets[bucketIndex].push_back(num);
    }
    
    // 5. 对每个桶进行排序，合并桶
    std::vector<int> sortedArr;
    for (auto &bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
        sortedArr.insert(sortedArr.end(), bucket.begin(), bucket.end());
    }

    return sortedArr;
}


int main() {
    std::vector<int> arr = {43, 60, 141, 28, 59, 4, 7, 867, 29, 130};
    int bucketSize = 5;
    std::vector<int> sortedArr = bucketSort(arr, bucketSize);
    // 输出排序后的数组
    std::cout << "排序后的数组为：" << std::endl;
    for (int num : sortedArr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    return 0;
}
