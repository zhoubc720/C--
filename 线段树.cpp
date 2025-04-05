#include <iostream>
#include <vector>
#include <climits>
#include <cassert>

using namespace std;

class SegmentTree {
private:
    struct Node {
        int start, end;
        int sum;        // 区间和
        int max_val;    // 区间最大值
        int lazy;       // 懒惰标记
        
        Node() : start(0), end(0), sum(0), max_val(0), lazy(0) {}
        Node(int s, int e) : start(s), end(e), sum(0), max_val(0), lazy(0) {}
    };
    
    vector<Node> tree;
    vector<int> nums;
    int n;
    
    // 计算左子节点索引
    int left(int idx) { return 2 * idx + 1; }
    
    // 计算右子节点索引
    int right(int idx) { return 2 * idx + 2; }
    
    // 下放懒惰标记
    void pushDown(int idx) {
        if (tree[idx].lazy != 0) {
            int l = left(idx), r = right(idx);
            
            // 更新左子节点
            if (l < tree.size()) {
                tree[l].sum += tree[idx].lazy * (tree[l].end - tree[l].start + 1);
                tree[l].max_val += tree[idx].lazy;
                tree[l].lazy += tree[idx].lazy;
            }
            
            // 更新右子节点
            if (r < tree.size()) {
                tree[r].sum += tree[idx].lazy * (tree[r].end - tree[r].start + 1);
                tree[r].max_val += tree[idx].lazy;
                tree[r].lazy += tree[idx].lazy;
            }
            
            tree[idx].lazy = 0;
        }
    }
    
    // 构建线段树
    void build(int idx, int start, int end) {
        tree[idx].start = start;
        tree[idx].end = end;
        
        if (start == end) {
            tree[idx].sum = nums[start];
            tree[idx].max_val = nums[start];
            return;
        }
        
        int mid = start + (end - start) / 2;
        build(left(idx), start, mid);
        build(right(idx), mid + 1, end);
        
        // 合并子节点信息
        tree[idx].sum = tree[left(idx)].sum + tree[right(idx)].sum;
        tree[idx].max_val = max(tree[left(idx)].max_val, tree[right(idx)].max_val);
    }
    
    // 区间更新
    void updateRange(int idx, int l, int r, int val) {
        // 当前节点区间与更新区间无交集
        if (tree[idx].end < l || tree[idx].start > r) {
            return;
        }
        
        // 当前节点区间完全包含在更新区间内
        if (l <= tree[idx].start && tree[idx].end <= r) {
            tree[idx].sum += val * (tree[idx].end - tree[idx].start + 1);
            tree[idx].max_val += val;
            tree[idx].lazy += val;
            return;
        }
        
        // 部分重叠，需要下放懒惰标记
        pushDown(idx);
        
        // 更新左右子树
        updateRange(left(idx), l, r, val);
        updateRange(right(idx), l, r, val);
        
        // 更新当前节点
        tree[idx].sum = tree[left(idx)].sum + tree[right(idx)].sum;
        tree[idx].max_val = max(tree[left(idx)].max_val, tree[right(idx)].max_val);
    }
    
    // 区间求和查询
    int querySum(int idx, int l, int r) {
        // 当前节点区间与查询区间无交集
        if (tree[idx].end < l || tree[idx].start > r) {
            return 0;
        }
        
        // 当前节点区间完全包含在查询区间内
        if (l <= tree[idx].start && tree[idx].end <= r) {
            return tree[idx].sum;
        }
        
        // 部分重叠，需要下放懒惰标记
        pushDown(idx);
        
        // 查询左右子树
        return querySum(left(idx), l, r) + querySum(right(idx), l, r);
    }
    
    // 区间最大值查询
    int queryMax(int idx, int l, int r) {
        // 当前节点区间与查询区间无交集
        if (tree[idx].end < l || tree[idx].start > r) {
            return INT_MIN;
        }
        
        // 当前节点区间完全包含在查询区间内
        if (l <= tree[idx].start && tree[idx].end <= r) {
            return tree[idx].max_val;
        }
        
        // 部分重叠，需要下放懒惰标记
        pushDown(idx);
        
        // 查询左右子树
        return max(queryMax(left(idx), l, r), queryMax(right(idx), l, r));
    }

public:
    SegmentTree(const vector<int>& nums) : nums(nums) {
        n = nums.size();
        if (n == 0) return;
        
        // 计算线段树大小
        int size = 1;
        while (size < n) {
            size <<= 1;
        }
        tree.resize(2 * size - 1);
        
        build(0, 0, n - 1);
    }
    
    // 区间加法更新
    void rangeAdd(int l, int r, int val) {
        if (l > r || l < 0 || r >= n) return;
        updateRange(0, l, r, val);
    }
    
    // 单点更新
    void pointUpdate(int index, int val) {
        if (index < 0 || index >= n) return;
        int delta = val - nums[index];
        nums[index] = val;
        updateRange(0, index, index, delta);
    }
    
    // 查询区间和
    int rangeSum(int l, int r) {
        if (l > r || l < 0 || r >= n) return 0;
        return querySum(0, l, r);
    }
    
    // 查询区间最大值
    int rangeMax(int l, int r) {
        if (l > r || l < 0 || r >= n) return INT_MIN;
        return queryMax(0, l, r);
    }
    
    // 打印线段树结构（调试用）
    void printTree() {
        cout << "Segment Tree Structure:" << endl;
        for (int i = 0; i < tree.size(); ++i) {
            if (tree[i].end == 0 && tree[i].start == 0 && i >= n) continue;
            cout << "Node " << i << ": [" << tree[i].start << ", " << tree[i].end 
                 << "], sum=" << tree[i].sum << ", max=" << tree[i].max_val 
                 << ", lazy=" << tree[i].lazy << endl;
        }
    }
};


void testSegmentTree() {
    // 测试用例1: 基本功能测试
    {
        cout << "=== 测试用例1: 基本功能测试 ===" << endl;
        vector<int> nums = {1, 3, 5, 7, 9, 11};
        SegmentTree st(nums);
        
        // 初始状态验证
        assert(st.rangeSum(0, 5) == 36);
        assert(st.rangeMax(0, 5) == 11);
        assert(st.rangeSum(2, 4) == 21);
        assert(st.rangeMax(2, 4) == 9);
        
        // 单点更新
        st.pointUpdate(3, 10);  // nums[3]从7改为10
        assert(st.rangeSum(0, 5) == 39);  // 36-7+10=39
        assert(st.rangeMax(2, 4) == 10);  // max(5,10,9)=10
        
        // 区间更新
        st.rangeAdd(1, 3, 2);  // nums[1],nums[2],nums[3]各加2
        assert(st.rangeSum(1, 3) == 3+2 + 5+2 + 10+2);  // 24
        assert(st.rangeMax(0, 5) == 12);  // nums[3]现在是12
        
        cout << "Test Case 1 Passed!" << endl << endl;
    }
    
    // 测试用例2: 边界测试
    {
        cout << "=== 测试用例2: 边界测试 ===" << endl;
        vector<int> nums = {5};
        SegmentTree st(nums);
        
        assert(st.rangeSum(0, 0) == 5);
        assert(st.rangeMax(0, 0) == 5);
        
        st.pointUpdate(0, -3);
        assert(st.rangeSum(0, 0) == -3);
        assert(st.rangeMax(0, 0) == -3);
        
        st.rangeAdd(0, 0, 10);
        assert(st.rangeSum(0, 0) == 7);
        
        cout << "Test Case 2 Passed!" << endl << endl;
    }
    
    // 测试用例3: 懒惰传播测试
    {
        cout << "=== 测试用例3: 懒惰传播测试 ===" << endl;
        vector<int> nums = {1, 2, 3, 4, 5};
        SegmentTree st(nums);
        
        // 初始最大值是5
        assert(st.rangeMax(0, 4) == 5);
        
        // 区间[0,2]加3
        st.rangeAdd(0, 2, 3);
        // 此时应该是[4,5,6,4,5]
        assert(st.rangeSum(0, 2) == 4+5+6);
        assert(st.rangeMax(0, 4) == 6);
        
        // 查询[1,3]应该触发懒惰传播
        assert(st.rangeSum(1, 3) == 5+6+4);
        
        // 再次区间更新[1,3]减2
        st.rangeAdd(1, 3, -2);
        // 此时应该是[4,3,4,2,5]
        assert(st.rangeSum(1, 3) == 3+4+2);
        assert(st.rangeMax(0, 4) == 5);  // max(4,3,4,2,5)
        
        cout << "Test Case 3 Passed!" << endl << endl;
    }
    
    // 测试用例4: 大区间测试
    {
        cout << "=== 测试用例4: 大区间测试 ===" << endl;
        vector<int> nums(1000, 1);  // 1000个1
        SegmentTree st(nums);
        
        assert(st.rangeSum(0, 999) == 1000);
        assert(st.rangeMax(0, 999) == 1);
        
        // 前500个元素加2
        st.rangeAdd(0, 499, 2);
        assert(st.rangeSum(0, 499) == 3 * 500);
        assert(st.rangeSum(500, 999) == 500);
        assert(st.rangeMax(0, 999) == 3);
        
        // 后500个元素减1
        st.rangeAdd(500, 999, -1);
        assert(st.rangeSum(500, 999) == 0);
        assert(st.rangeMax(500, 999) == 0);
        
        cout << "Test Case 4 Passed!" << endl << endl;
    }
    
    cout << "测试用例全部通过!" << endl;
}


int main() {
    testSegmentTree();
    return 0;
}