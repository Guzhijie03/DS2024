using Rank = unsigned int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）
template <typename T> class Vector { //向量模板类
protected:
    Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
    void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank lo, Rank hi); //扫描交换
    void bubbleSort(Rank lo, Rank hi); //起泡排序算法
    Rank maxItem(Rank lo, Rank hi); //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序算法
    void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法
    void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
    // 构造函数
    Vector(Rank c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
    } //s<=c
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
    // 析构函数
    ~Vector() { delete[] _elem; } //释放内部空间
    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank search(T const& e) const //有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
    // 可写访问接口
    T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] (Rank r) const; //仅限于做右值的重载版本
    Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
    T remove(Rank r); //删除秩为r的元素
    Rank remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
    Rank insert(Rank r, T const& e); //插入元素
    Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort() { unsort(0, _size); } //整体置乱
    Rank deduplicate(); //无序去重
    Rank uniquify(); //有序去重
    // 遍历
    void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
}; //Vector

template < typename T> //T为基本类型，或已重载赋值操作符'='
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = max<Rank>(DEFAULT_CAPACITY, 2 * (hi - lo))]; //分配空间
    for (_size = 0; lo < hi; _size++, lo++) //A[lo, hi)内的元素逐一
        _elem[_size] = A[lo]; //复制至_elem[0, hi - lo)
} //用const修饰，保证A中的元素不致被篡改；运行时间 = O(hi-lo)

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //深复制
    if (_elem) delete[] _elem; //释放原有内容
    copyFrom(V._elem, 0, V.size()); //整体复制
    return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
    if (_size < _capacity) return; //尚未满员时，不必扩容
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //不低于最小容量
    T* oldElem = _elem;  _elem = new T[_capacity <<= 1]; //容量加倍
    for (Rank i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete[] oldElem; //释放原空间
}

template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩向量所占空间
    if (_capacity < DEFAULT_CAPACITY << 1) return; //不致收缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity) return; //以25%为界
    T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //容量减半
    for (Rank i = 0; i < _size; i++) _elem[i] = oldElem[i]; //复制原向量内容
    delete[] oldElem; //释放原空间
}

template <typename T> T& Vector<T>::operator[] (Rank r) //重载下标操作符
{
    return _elem[r];
} // assert: 0 <= r < _size
template <typename T> const T& Vector<T>::operator[] (Rank r) const //仅限于做右值
{
    return _elem[r];
} // assert: 0 <= r < _size

template <typename T> void permute(Vector<T>& V) { //随机置乱向量，使各元素等概率出现于各位置
    for (int i = V.size(); i > 0; i--) //自后向前
        swap(V[i - 1], V[rand() % i]); //V[i - 1]与V[0, i)中某一随机元素交换
}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { //随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; 1 < i; --i) //自后向前
        swap(V[i - 1], V[rand() % i]); //将V[i - 1]与V[0, i)中某一元素随机交换
}

template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } // less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } // less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); } // equal
template <typename T> static bool eq(T& a, T& b) { return a == b; } // equal

template < typename T> //在无序向量中顺序查找e：成功则返回最靠后的出现位置，否则返回lo-1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const { //0 <= lo < hi <= _size
    while ((lo < hi--) && (e != _elem[hi])); //从后向前，顺序查找
    return hi; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template < typename T> //将e插入至[r]
Rank Vector<T>::insert(Rank r, T const& e) { //0 <= r <= size
    expand(); //如必要，先扩容
    for (Rank i = _size; r < i; i--) //自后向前，后继元素
        _elem[i] = _elem[i - 1]; //顺次后移一个单元
    _elem[r] = e; _size++; //置入新元素并更新容量
    return r; //返回秩
}

template <typename T> Rank Vector<T>::remove(Rank lo, Rank hi) { //0 <= lo <= hi <= n
    if (lo == hi) return 0; //出于效率考虑，单独处理退化情况
    while (hi < _size) _elem[lo++] = _elem[hi++]; //后缀[hi, _size)顺次前移hi-lo位
    _size = lo; shrink(); //更新规模，lo=_size之后的内容无需清零；如必要，则缩容
    //若有必要，则缩容
    return hi - lo; //返回被删除元素的数目
}

template <typename T> T Vector<T>::remove(Rank r) { //删除向量中秩为r的元素，0 <= r < size
    T e = _elem[r]; //备份被删除元素
    remove(r, r + 1); //调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; //返回被删除元素
}

template <typename T> Rank Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
    Rank oldSize = _size; //记录原规模
    for (Rank i = 1; i < _size; ) //自前向后逐个考查_elem[1,_size)
        if (-1 == find(_elem[i], 0, i)) //在前缀[0,i)中寻找与[i]雷同者（至多一个），O(i)
            i++; //若无雷同，则继续考查其后继
        else
            remove(i); //否则删除[i]，O(_size-i)
    return oldSize - _size; //被删除元素总数
}

template <typename T> void Vector<T>::traverse(void (*visit)(T&)) //借助函数指针机制
{
    for (Rank i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量
template <typename T> template < typename VST> //元素类型、操作器
void Vector<T>::traverse(VST& visit) //借助函数对象机制
{
    for (Rank i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量

template <typename T> struct Increase //函数对象：递增一个T类对象
{
    virtual void operator()(T& e) { e++; }
}; //假设T可直接递增或已重载++

template <typename T> void increase(Vector<T>& V) //统一递增向量中的各元素
{
    V.traverse(Increase<T>());
} //以Increase<T>()为基本操作进行遍历

template <typename T> Rank Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
    Rank i = 0, j = 0; //各对互异“相邻”元素的秩
    while (++j < _size) //逐一扫描，直至末元素
        if (_elem[i] != _elem[j]) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i; shrink(); //直接截除尾部多余元素
    return j - i; //向量规模变化量，即被删除元素总数
}

template < typename T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const { // 0 <= lo < hi <= _size
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
} //等概率地随机使用二分查找、Fibonacci查找

//二分查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> static Rank binSearch(T* S, T const& e, Rank lo, Rank hi) {
    while (lo < hi) { //每步迭代可能要做两次比较判断，有三个分支
        Rank mi = (lo + hi) >> 1; //以中点为轴点（区间宽度折半，等效于其数值表示的右移一位）
        if (e < S[mi]) hi = mi; //深入前半段[lo, mi)继续查找
        else if (S[mi] < e) lo = mi + 1; //深入后半段(mi, hi)继续查找
        else                 \
            return mi; //在mi处命中
    } //成功查找可以提前终止
    return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置

