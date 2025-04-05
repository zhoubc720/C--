#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// AC自动机节点类
class ACAutomaton {
private:
    // Trie节点结构
    struct TrieNode {
        unordered_map<char, TrieNode*> children;  // 子节点映射表
        TrieNode* fail;                           // 失败指针
        vector<int> output;                       // 输出列表（存储模式串的索引）
        bool isEnd;                               // 是否是某个模式串的结尾

        TrieNode() : fail(nullptr), isEnd(false) {}
    };

    TrieNode* root;  // 根节点
    vector<string> patterns;  // 存储所有模式串

public:
    // 构造函数
    ACAutomaton() {
        root = new TrieNode();
    }

    // 析构函数
    ~ACAutomaton() {
        clear(root);
    }

    // 递归删除所有节点
    void clear(TrieNode* node) {
        if (!node) return;
        for (auto& child : node->children) {
            clear(child.second);
        }
        delete node;
    }

    // 插入模式串
    void insert(const string& pattern, int index) {
        TrieNode* node = root;
        for (char c : pattern) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
        node->output.push_back(index);  // 存储模式串的索引
    }

    // 构建AC自动机（包括失败指针）
    void build() {
        queue<TrieNode*> q;
        
        // 第一层节点的失败指针都指向root
        for (auto& child : root->children) {
            child.second->fail = root;
            q.push(child.second);
        }

        // BFS构建失败指针
        while (!q.empty()) {
            TrieNode* curr = q.front();
            q.pop();

            // 遍历当前节点的所有子节点
            for (auto& child : curr->children) {
                char c = child.first;
                TrieNode* childNode = child.second;
                
                // 失败指针初始指向父节点的失败指针
                TrieNode* failTo = curr->fail;
                
                // 如果failTo没有c子节点，继续向上跳转失败指针
                while (failTo != root && failTo->children.find(c) == failTo->children.end()) {
                    failTo = failTo->fail;
                }
                
                // 如果failTo有c子节点，则设置失败指针
                if (failTo->children.find(c) != failTo->children.end()) {
                    childNode->fail = failTo->children[c];
                } else {
                    childNode->fail = root;
                }
                
                // 合并输出（重要！）
                childNode->output.insert(childNode->output.end(), 
                                         childNode->fail->output.begin(),
                                         childNode->fail->output.end());
                
                q.push(childNode);
            }
        }
    }

    // 查询文本中所有模式串出现的位置
    vector<pair<int, string>> search(const string& text) {
        vector<pair<int, string>> result;
        TrieNode* curr = root;

        for (int i = 0; i < text.size(); ++i) {
            char c = text[i];
            
            // 如果当前节点没有c子节点，跳转失败指针
            while (curr != root && curr->children.find(c) == curr->children.end()) {
                curr = curr->fail;
            }
            
            // 如果找到c子节点，则进入该节点
            if (curr->children.find(c) != curr->children.end()) {
                curr = curr->children[c];
            } else {
                continue;  // 没找到，保持在root
            }
            
            // 检查当前节点是否有输出
            for (int idx : curr->output) {
                int startPos = i - patterns[idx].size() + 1;
                result.emplace_back(startPos, patterns[idx]);
            }
        }

        return result;
    }

    // 构建AC自动机（外部接口）
    void buildAC(const vector<string>& patternList) {
        patterns = patternList;
        for (int i = 0; i < patterns.size(); ++i) {
            insert(patterns[i], i);
        }
        build();
    }
};

// 测试函数
void testACAutomaton() {
    ACAutomaton ac;
    
    // 测试用例1：简单模式匹配
    {
        cout << "测试用例1：简单模式匹配" << endl;
        vector<string> patterns = {"a", "ab", "bab"};
        string text = "abab";
        
        ac.buildAC(patterns);
        auto results = ac.search(text);
        
        cout << "在文本 \"" << text << "\" 中找到以下模式：" << endl;
        for (auto& res : results) {
            cout << "位置 " << res.first << ": " << res.second << endl;
        }
        cout << "------------------------" << endl;
    }
    
    // 测试用例2：重叠模式
    {
        cout << "测试用例2：重叠模式" << endl;
        vector<string> patterns = {"he", "she", "his", "hers"};
        string text = "ushershehis";
        
        ac.buildAC(patterns);
        auto results = ac.search(text);
        
        cout << "在文本 \"" << text << "\" 中找到以下模式：" << endl;
        for (auto& res : results) {
            cout << "位置 " << res.first << ": " << res.second << endl;
        }
        cout << "------------------------" << endl;
    }
    
    // 测试用例3：无匹配情况
    {
        cout << "测试用例3：无匹配情况" << endl;
        vector<string> patterns = {"apple", "orange"};
        string text = "banana";
        
        ac.buildAC(patterns);
        auto results = ac.search(text);
        
        if (results.empty()) {
            cout << "在文本 \"" << text << "\" 中没有找到任何模式" << endl;
        }
        cout << "------------------------" << endl;
    }
    
    // 测试用例4：中文匹配
    {
        cout << "测试用例4：中文匹配" << endl;
        vector<string> patterns = {"中国", "中国人", "人民"};
        string text = "中国人民万岁";
        
        ac.buildAC(patterns);
        auto results = ac.search(text);
        
        cout << "在文本 \"" << text << "\" 中找到以下模式：" << endl;
        for (auto& res : results) {
            cout << "位置 " << res.first << ": " << res.second << endl;
        }
        cout << "------------------------" << endl;
    }
}

int main() {
    testACAutomaton();
    return 0;
}