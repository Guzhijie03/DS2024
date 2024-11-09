#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

// 定义二叉树节点类 BinNode
class BinNode {
public:
    char data; // 存储字符
    int freq; // 存储频率
    BinNode* left; // 左孩子指针
    BinNode* right; // 右孩子指针

    // 构造函数
    BinNode(char data, int freq) {
        this->data = data;
        this->freq = freq;
        this->left = nullptr;
        this->right = nullptr;
    }
};

// 定义比较函数，用于优先队列中比较两个节点的频率
struct compare {
    bool operator()(BinNode* l, BinNode* r) {
        return l->freq > r->freq;
    }
};

// 定义二叉树类 BinTree
class BinTree {
public:
    BinNode* root; // 根节点指针

    // 构造函数，根据给定的字符数组和频率数组构建 Huffman 树
    BinTree(char data[], int freq[], int size) {
        priority_queue<BinNode*, vector<BinNode*>, compare> Q; // 定义一个优先队列，用于存储节点
        for (int i = 0; i < size; i++) { // 将每个字符作为一个叶子节点加入队列
            Q.push(new BinNode(data[i], freq[i]));
        }
        while (Q.size() > 1) { // 当队列中还有多于一个节点时，重复以下步骤
            BinNode* left = Q.top(); // 取出频率最小的节点作为左孩子
            Q.pop();
            BinNode* right = Q.top(); // 取出频率次小的节点作为右孩子
            Q.pop();
            BinNode* parent = new BinNode('$', left->freq + right->freq); // 创建一个新的内部节点，频率为左右孩子之和，字符为'$'
            parent->left = left; // 将左孩子连接到父节点
            parent->right = right; // 将右孩子连接到父节点
            Q.push(parent); // 将父节点加入队列
        }
        root = Q.top(); // 最后剩下的节点就是根节点
    }

    // 辅助函数，用于打印 Huffman 编码
    void printCodes(BinNode* root, string str) {
        if (!root) return; // 如果根节点为空，直接返回
        if (root->data != '$') { // 如果根节点不是内部节点，打印其字符和编码
            cout << root->data << ": " << str << "\n";
        }
        printCodes(root->left, str + "0"); // 递归地打印左子树的编码，左边加上"0"
        printCodes(root->right, str + "1"); // 递归地打印右子树的编码，右边加上"1"
    }

    // 主函数，用于打印 Huffman 编码
    void printCodes() {
        printCodes(root, ""); // 调用辅助函数，初始编码为空字符串
    }

    // 辅助函数，用于生成 Huffman 编码表
    void generateCodes(BinNode* root, string str, unordered_map<char, string>& codes) {
        if (!root) return; // 如果根节点为空，直接返回
        if (root->data != '$') { // 如果根节点不是内部节点，将其字符和编码存入哈希表中
            codes[root->data] = str;
        }
        generateCodes(root->left, str + "0", codes); // 递归地生成左子树的编码，左边加上"0"
        generateCodes(root->right, str + "1", codes); // 递归地生成右子树的编码，右边加上"1"
    }

    // 主函数，用于生成 Huffman 编码表，并返回哈希表的引用
    unordered_map<char, string>& generateCodes() {
        static unordered_map<char, string> codes; // 定义一个静态的哈希表，用于存储字符和编码的映射关系
        generateCodes(root, "", codes); // 调用辅助函数，初始编码为空字符串
        return codes; // 返回哈希表的引用
    }
};

// 定义位图类 Bitmap，用于存储二进制位序列
class Bitmap {
public:
    vector<bool> bits; // 存储位序列的向量

    // 构造函数，根据给定的字符串生成位序列
    Bitmap(string s) {
        for (char c : s) { // 遍历字符串中的每个字符
            bitset<8> b(c); // 将字符转换为8位二进制数
            for (int i = 7; i >= 0; i--) { // 将二进制数的每一位加入位序列向量中，从高位到低位
                bits.push_back(b[i]);
            }
        }
    }

    // 打印位序列的函数
    void printBits() {
        for (bool b : bits) { // 遍历位序列向量中的每一位
            cout << b; // 打印该位的值（0或1）
        }
        cout << "\n";
    }
};

// 定义 Huffman 编码串类型 HuffCode，继承自 Bitmap 类，并添加了长度属性和构造函数重载等功能
class HuffCode : public Bitmap {
public:
    int length; // 记录编码串的长度

    // 构造函数重载，根据给定的字符串生成位序列，并记录长度（注意：这里的字符串只包含0和1）
    HuffCode(string s) : Bitmap(s) {
        length = s.length(); // 长度等于字符串的长度（不是字节数）
    }

    // 构造函数重载，根据给定的字符数组和频率数组生成 Huffman 编码串，并记录长度（注意：这里假设字符数组和频率数组已经按照频率从小到大排序）
    HuffCode(char data[], int freq[], int size) : Bitmap("") {
        length = 0; // 初始长度为0
        BinTree tree(data, freq, size); // 根据字符数组和频率数组构建 Huffman 树
        unordered_map<char, string>& codes = tree.generateCodes(); // 根据 Huffman 树生成 Huffman 编码表，并获取其引用（注意：这里使用了引用而不是拷贝，可以节省空间和时间）
        for (int i = 0; i < size; i++) { // 遍历字符数组中的每个字符（按照频率从小到大）
            string code = codes[data[i]]; // 根据哈希表查找该字符对应的编码串（注意：这里使用了哈希表而不是遍历 Huffman 树，可以节省时间）
            length += code.length() * freq[i]; // 累加该字符
            for (char c : code) { // 遍历该字符对应的编码串中的每个字符（0或1）
                bitset<8> b(c); // 将字符转换为8位二进制数
                for (int j = 7; j >= 0; j--) { // 将二进制数的每一位加入位序列向量中，从高位到低位
                    bits.push_back(b[j]);
                }
            }
        }
    }

    // 打印编码串的长度的函数
    void printLength() {
        cout << "The length of the Huffman code is " << length << " bits.\n";
    }
};

// 主函数，用于测试
int main() {
    // 读取马丁路德金的演讲原文《I have a dream》
    ifstream fin("I_have_a_dream.txt");
    if (!fin) {
        cout << "File not found.\n";
        return -1;
    }
    string text;
    char c;
    while (fin.get(c)) {
        text += c;
    }
    fin.close();

    // 统计原文中每个字符出现的频率，并存入哈希表中
    unordered_map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }

    // 将哈希表中的键值对转换为两个向量，分别存储字符和频率，并按照频率从小到大排序
    vector<pair<char, int>> vec(freq.begin(), freq.end());
    sort(vec.begin(), vec.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
        return a.second < b.second;
        });
    vector<char> data;
    vector<int> freqs;
    for (auto p : vec) {
        data.push_back(p.first);
        freqs.push_back(p.second);
    }

    // 根据字符向量和频率向量构建 Huffman 编码串，并打印其长度和位序列
    HuffCode hc(&data[0], &freqs[0], data.size());
    hc.printLength();
    hc.printBits();

    // 对 dream 以及其他单词进行编码，并打印其编码串
    string words[] = { "dream", "freedom", "justice", "nation", "ring" };
    for (string word : words) {
        cout << word << ": ";
        Bitmap bm(word);
        bm.printBits();
    }

    return 0;
}
