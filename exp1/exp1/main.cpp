#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

// 定义一个复数类
class Complex {
public:
    // 构造函数，可以接受实部和虚部，也可以接受一个std::complex对象
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
    Complex(const std::complex<double>& c) : real(c.real()), imag(c.imag()) {}

    // 获取实部和虚部的方法
    double get_real() const { return real; }
    double get_imag() const { return imag; }

    // 重载运算符，实现复数类之间的加减乘除等操作
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
            real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        return Complex((real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator);
    }

    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

private:
    // 实部和虚部
    double real;
    double imag;
};

// 重载输出流运算符，方便打印复数类对象
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << "(" << c.get_real() << ", " << c.get_imag() << ")";
    return os;
}

// 随机生成一个无序的复数向量（有重复项）
std::vector<Complex> generate_random_complex_vector(int size) {
    std::vector<Complex> result;
    result.reserve(size); // 预分配空间，提高效率
    std::random_device rd; // 随机数生成器
    std::uniform_real_distribution<double> dist(-10.0, 10.0); // 均匀分布
    for (int i = 0; i < size; i++) {
        // 随机生成实部和虚部，构造一个复数对象，添加到向量中
        result.emplace_back(dist(rd), dist(rd));
    }
    return result;
}

// 定义一个比较函数，用于比较两个复数的模（模相同的情况下，以实部为基准）
bool compare_complex(const Complex& a, const Complex& b) {
    // 计算两个复数的模
    double mod_a = std::sqrt(a.get_real() * a.get_real() + a.get_imag() * a.get_imag());
    double mod_b = std::sqrt(b.get_real() * b.get_real() + b.get_imag() * b.get_imag());
    // 如果模相同，就比较实部
    if (mod_a == mod_b) {
        return a.get_real() < b.get_real();
    }
    // 否则就比较模
    return mod_a < mod_b;
}


// 定义一个起泡排序函数，用于对复数向量进行排序，使用比较函数作为参数
void bubble_sort(std::vector<Complex>& v, bool (*cmp)(const Complex&, const Complex&)) {
    int n = v.size(); // 获取向量的大小
    bool swapped = true; // 定义一个标志，表示是否发生了交换
    while (swapped) { // 如果发生了交换，继续循环
        swapped = false; // 重置标志为false
        for (int i = 1; i < n; i++) { // 遍历向量的元素，从第二个开始
            if (cmp(v[i], v[i - 1])) { // 如果当前元素比前一个元素小，根据比较函数的定义
                std::swap(v[i], v[i - 1]); // 交换两个元素的位置
                swapped = true; // 设置标志为true，表示发生了交换
            }
        }
        n--; // 减少遍历的范围，因为最后一个元素已经是最大的了
    }
}

// 定义一个归并函数，用于将两个有序的复数向量合并为一个有序的复数向量，使用比较函数作为参数
void merge(std::vector<Complex>& v, int left, int mid, int right, bool (*cmp)(const Complex&, const Complex&)) {
    int n1 = mid - left + 1; // 计算左半部分的大小
    int n2 = right - mid; // 计算右半部分的大小

    std::vector<Complex> L(n1); // 创建一个临时向量，用于存储左半部分的元素
    std::vector<Complex> R(n2); // 创建一个临时向量，用于存储右半部分的元素

    for (int i = 0; i < n1; i++) { // 复制左半部分的元素到L中
        L[i] = v[left + i];
    }
    for (int j = 0; j < n2; j++) { // 复制右半部分的元素到R中
        R[j] = v[mid + 1 + j];
    }

    int i = 0; // 定义一个指针，指向L的第一个元素
    int j = 0; // 定义一个指针，指向R的第一个元素
    int k = left; // 定义一个指针，指向v的左边界

    while (i < n1 && j < n2) { // 当L和R都有剩余元素时，循环比较并合并
        if (cmp(L[i], R[j])) { // 如果L的当前元素比R的当前元素小，根据比较函数的定义
            v[k] = L[i]; // 将L的当前元素复制到v中
            i++; // 移动L的指针到下一个位置
        }
        else { // 如果L的当前元素不比R的当前元素小
            v[k] = R[j]; // 将R的当前元素复制到v中
            j++; // 移动R的指针到下一个位置
        }
        k++; // 移动v的指针到下一个位置
    }

    while (i < n1) { // 当L还有剩余元素时，循环复制到v中
        v[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) { // 当R还有剩余元素时，循环复制到v中
        v[k] = R[j];
        j++;
        k++;
    }
}

// 定义一个归并排序函数，用于对复数向量进行排序，使用比较函数作为参数
void merge_sort(std::vector<Complex>& v, int left, int right, bool (*cmp)(const Complex&, const Complex&)) {
    if (left < right) { // 如果左边界小于右边界，说明还可以继续分割
        int mid = (left + right) / 2; // 计算中间位置
        merge_sort(v, left, mid, cmp); // 对左半部分进行归并排序
        merge_sort(v, mid + 1, right, cmp); // 对右半部分进行归并排序
        merge(v, left, mid, right, cmp); // 将两个有序的部分合并为一个有序的部分
    }
}


// 定义一个打印函数，用于打印复数向量的内容
void print_vector(const std::vector<Complex>& v) {
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n";
}

// 定义一个区间查找函数，用于在顺序的复数向量中，查找出模介于[m1,m2) 的所有元素，按序存于一个子向量中作为返回值
std::vector<Complex> range_search(const std::vector<Complex>& v, double m1, double m2) {
    // 创建一个空的子向量，用于存储结果
    std::vector<Complex> result;
    // 遍历原始向量的每个元素
    for (const auto& c : v) {
        // 计算当前元素的模
        double mod = std::sqrt(c.get_real() * c.get_real() + c.get_imag() * c.get_imag());
        // 如果模介于[m1,m2)，就将当前元素添加到子向量中
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    // 返回子向量
    return result;
}

// 在主函数中测试无序向量的置乱、查找（实部和虚部均相同）、插入、删除和唯一化的操作
int main() {
    // 生成一个大小为10的随机复数向量
    std::vector<Complex> v = generate_random_complex_vector(10);

    // 打印原始向量
    std::cout << "原始向量：\n";
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n\n";

    // 置乱向量
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(v.begin(), v.end(), gen);

    // 打印置乱后的向量
    std::cout << "置乱后的向量：\n";
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n\n";

    // 查找指定的复数（实部和虚部均相同）
    Complex target(1.0, 2.0); // 目标复数
    auto it = std::find(v.begin(), v.end(), target); // 在向量中查找
    if (it != v.end()) { // 如果找到了
        std::cout << "找到了目标复数 " << target << "，它的位置是 " << it - v.begin() << "\n";
    }
    else { // 如果没找到
        std::cout << "没有找到目标复数 " << target << "\n";
    }

    std::cout << "\n";

    // 插入一个新的复数到向量的末尾
    Complex new_complex(3.0, 4.0); // 新的复数
    v.push_back(new_complex); // 插入到向量的末尾

    // 打印插入后的向量
    std::cout << "插入后的向量：\n";
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n\n";

    // 删除一个指定的复数（实部和虚部均相同）
    Complex to_delete(3.0, 4.0); // 要删除的复数
    auto it2 = std::remove(v.begin(), v.end(), to_delete); // 删除向量中所有等于该复数的元素，并返回新的末尾迭代器
    v.erase(it2, v.end()); // 删除多余的元素

    // 打印删除后的向量
    std::cout << "删除后的向量：\n";
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n\n";

    // 唯一化向量，即删除所有重复的元素，只保留一个
    auto it3 = std::unique(v.begin(), v.end()); // 唯一化向量，并返回新的末尾迭代器
    v.erase(it3, v.end()); // 删除多余的元素

    // 打印唯一化后的向量
    std::cout << "唯一化后的向量：\n";
    for (const auto& c : v) {
        std::cout << c << " ";
    }
    std::cout << "\n\n";

    // 比较顺序、乱序、逆序的情况下，起泡排序和归并排序的效率（clock()函数记时）

    // 生成一个大小为1000的复数向量
    std::vector<Complex> v1 = generate_random_complex_vector(1000);

    // 复制一份，用于归并排序
    std::vector<Complex> v2 = v1;

    // 对v1进行起泡排序，并计时
    clock_t start1 = clock(); // 开始计时
    bubble_sort(v1, compare_complex); // 起泡排序
    clock_t end1 = clock(); // 结束计时
    double elapsed1 = (double)(end1 - start1) / CLOCKS_PER_SEC; // 计算耗时

    // 对v2进行归并排序，并计时
    clock_t start2 = clock(); // 开始计时
    merge_sort(v2, 0, v2.size() - 1, compare_complex); // 归并排序
    clock_t end2 = clock(); // 结束计时
    double elapsed2 = (double)(end2 - start2) / CLOCKS_PER_SEC; // 计算耗时

    // 打印结果
    std::cout << "顺序向量的起泡排序耗时: " << elapsed1 << " seconds\n"; // 打印起泡排序的耗时
    std::cout << "顺序向量的归并排序耗时: " << elapsed2 << " seconds\n"; // 打印归并排序的耗时

    std::cout << "\n";

    // 对v1进行置乱，并计时
    clock_t start3 = clock(); // 开始计时
    std::shuffle(v1.begin(), v1.end(), gen); // 置乱
    clock_t end3 = clock(); // 结束计时
    double elapsed3 = (double)(end3 - start3) / CLOCKS_PER_SEC; // 计算耗时

    // 对v2进行置乱，并计时
    clock_t start4 = clock(); // 开始计时
    std::shuffle(v2.begin(), v2.end(), gen); // 置乱
    clock_t end4 = clock(); // 结束计时
    double elapsed4 = (double)(end4 - start4) / CLOCKS_PER_SEC; // 计算耗时

    std::cout << "\n";

    // 对v1进行起泡排序，并计时
    clock_t start5 = clock(); // 开始计时
    bubble_sort(v1, compare_complex); // 起泡排序
    clock_t end5 = clock(); // 结束计时
    double elapsed5 = (double)(end5 - start5) / CLOCKS_PER_SEC; // 计算耗时

    // 对v2进行归并排序，并计时
    clock_t start6 = clock(); // 开始计时
    merge_sort(v2, 0, v2.size() - 1, compare_complex); // 归并排序
    clock_t end6 = clock(); // 结束计时
    double elapsed6 = (double)(end6 - start6) / CLOCKS_PER_SEC; // 计算耗时

    // 打印结果
    std::cout << "置乱向量的起泡排序耗时: " << elapsed5 << " seconds\n"; // 打印起泡排序的耗时
    std::cout << "置乱向量的归并排序耗时: " << elapsed6 << " seconds\n"; // 打印归并排序的耗时
    std::cout << "\n";

    // 对v1进行逆序，并计时
    clock_t start7 = clock(); // 开始计时
    std::reverse(v1.begin(), v1.end()); // 逆序
    clock_t end7 = clock(); // 结束计时
    double elapsed7 = (double)(end7 - start7) / CLOCKS_PER_SEC; // 计算耗时

    // 对v2进行逆序，并计时
    clock_t start8 = clock(); // 开始计时
    std::reverse(v2.begin(), v2.end()); // 逆序
    clock_t end8 = clock(); // 结束计时
    double elapsed8 = (double)(end8 - start8) / CLOCKS_PER_SEC; // 计算耗时

    std::cout << "\n";

    // 对v1进行起泡排序，并计时
    clock_t start9 = clock(); // 开始计时
    bubble_sort(v1, compare_complex); // 起泡排序
    clock_t end9 = clock(); // 结束计时
    double elapsed9 = (double)(end9 - start9) / CLOCKS_PER_SEC; // 计算耗时

    // 对v2进行归并排序，并计时
    clock_t start10 = clock(); // 开始计时
    merge_sort(v2, 0, v2.size() - 1, compare_complex); // 归并排序
    clock_t end10 = clock(); // 结束计时
    double elapsed10 = (double)(end10 - start10) / CLOCKS_PER_SEC; // 计算耗时

    // 打印结果
    std::cout << "逆序向量的起泡排序耗时: " << elapsed9 << " seconds\n"; // 打印起泡排序的耗时
    std::cout << "逆序向量的归并排序耗时: " << elapsed10 << " seconds\n"; // 打印归并排序的耗时

    std::cout << "\n";

    // 生成一个大小为1000的随机复数向量
    std::vector<Complex> v3 = generate_random_complex_vector(10);

    // 对原始向量进行排序，得到顺序向量
    std::sort(v.begin(), v.end(), compare_complex);

    // 打印顺序向量
    std::cout << "顺序向量：\n";
    print_vector(v);
    std::cout << "\n";

    // 定义两个模的下限和上限
    double m1 = 5.0;
    double m2 = 10.0;

    // 调用区间查找函数，得到子向量
    std::vector<Complex> sub_v = range_search(v, m1, m2);

    // 打印子向量
    std::cout << "模介于[" << m1 << ", " << m2 << ") 的复数有：\n";
    print_vector(sub_v);

    return 0;
}
