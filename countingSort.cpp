#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void countingSort(vector<int> &nums)
{
    auto maxNum = max_element(nums.begin(), nums.end());//找到最大数值
    vector<int> count(*maxNum + 1, 0);//计数数组

    for (auto i : nums)
    {
        count[i]++;
    }

    for (int i = 1; i < count.size(); i++)//前缀和，目的是得到位置范围
    {
        count[i] += count[i - 1];
    }

    vector<int> ans(nums.size());

    for (int i = nums.size() - 1; i >= 0; i--)
    {
        ans[count[nums[i]] - 1] = nums[i];//以原数组值为下标找到计数数组的值，值-1即是位置，直接放入该数即可
        count[nums[i]]--;
    }
    nums = ans;
}

int main()
{
    vector<int> nums{2, 6, 5, 4, 1, 2, 3, 6, 9, 8, 7, 5, 6, 3, 2, 1, 4, 5, 6, 5, 4, 1, 2, 1};
    countingSort(nums);
    for (int num : nums)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}