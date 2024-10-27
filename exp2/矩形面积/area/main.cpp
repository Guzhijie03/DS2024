#include <iostream>
#include <vector>
#include <random>


class Queue {
private:
    std::vector<int> data; // 用一个 vector 来存储队列中的元素
    int front; // 队首元素的下标
    int rear; // 队尾元素的下一个下标
    int size; // 队列中元素的个数
    int capacity; // 队列的容量

public:
    // 构造函数，初始化队列
    Queue(int cap) {
        data.resize(cap); // 分配空间
        front = 0; // 队首下标为 0
        rear = 0; // 队尾下标为 0
        size = 0; // 队列为空
        capacity = cap; // 记录容量
    }

    // 判断队列是否为空
    bool isEmpty() {
        return size == 0;
    }

    // 判断队列是否已满
    bool isFull() {
        return size == capacity;
    }

    // 入队操作，将元素加入队尾
    void enqueue(int x) {
        if (isFull()) { // 如果队列已满，抛出异常
            throw std::runtime_error("Queue is full");
        }
        data[rear] = x; // 将元素存入队尾位置
        rear = (rear + 1) % capacity; // 更新队尾下标，如果到达数组末尾，循环到数组开头
        size++; // 更新队列大小
    }

    // 出队操作，将队首元素移除并返回
    int dequeue() {
        if (isEmpty()) { // 如果队列为空，抛出异常
            throw std::runtime_error("Queue is empty");
        }
        int x = data[front]; // 取出队首元素
        front = (front + 1) % capacity; // 更新队首下标，如果到达数组末尾，循环到数组开头
        size--; // 更新队列大小
        return x; // 返回队首元素
    }

    // 获取队首元素，但不移除
    int peek() {
        if (isEmpty()) { // 如果队列为空，抛出异常
            throw std::runtime_error("Queue is empty");
        }
        return data[front]; // 返回队首元素
    }
};


int largestRectangleArea(std::vector<int>& heights) {
    int n = heights.size(); // 获取高度数组的长度
    int maxArea = 0; // 初始化最大面积为 0
    Queue stack(n); // 创建一个队列作为单调递增栈
    for (int i = 0; i < n; i++) { // 遍历高度数组
        while (!stack.isEmpty() && heights[i] < heights[stack.peek()]) { // 如果栈不为空且当前高度小于栈顶高度
            int h = heights[stack.dequeue()]; // 取出栈顶高度
            int w = stack.isEmpty() ? i : i - stack.peek() - 1; // 计算宽度
            maxArea = std::max(maxArea, h * w); // 更新最大面积
        }
        stack.enqueue(i); // 将当前下标入栈
    }
    while (!stack.isEmpty()) { // 如果栈不为空
        int h = heights[stack.dequeue()]; // 取出栈顶高度
        int w = stack.isEmpty() ? n : n - stack.peek() - 1; // 计算宽度
        maxArea = std::max(maxArea, h * w); // 更新最大面积
    }
    return maxArea; // 返回最大面积
}

// 随机生成 10 组数据进行测试（1 <= heights.length <=105，0 <= heights[i] <= 104）
// 使用 C++ 标准库中的 random 库来生成随机数
std::random_device rd; // 随机数设备
std::mt19937 gen(rd()); // 随机数生成器
std::uniform_int_distribution<> dis1(1, 105); // 均匀分布 [1, 105]
std::uniform_int_distribution<> dis2(0, 104); // 均匀分布 [0, 104]


int main()
{
    for (int i = 0; i < 10; i++) { // 循环 10 次
        int len = dis1(gen); // 随机生成高度数组的长度
        std::vector<int> heights(len); // 创建一个长度为 len 的 vector
        for (int j = 0; j < len; j++) { // 遍历 vector
            heights[j] = dis2(gen); // 随机生成每个元素的值
        }
        std::cout << "测试 " << i + 1 << ": " << std::endl; // 输出测试用例编号
        std::cout << "Heights: "; // 输出高度数组
        for (int h : heights) {
            std::cout << h << " ";
        }
        std::cout << std::endl;
        std::cout << "最大面积: " << largestRectangleArea(heights) << std::endl; // 输出最大矩形面积
    }
    return 0;
}