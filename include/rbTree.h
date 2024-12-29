#ifndef LIBRARYMANAGEMENT_RBTREE_H
#define LIBRARYMANAGEMENT_RBTREE_H

#include <iostream>
#include <utility>
using namespace std;

// 定义颜色类型，使用 bool 类型表示节点的颜色
typedef bool Color;
// 定义红色节点的颜色为 false
const Color Red = false;
// 定义黑色节点的颜色为 true
const Color Black = true;

// 红黑树的节点结构
template <class Value>
struct Node {
    // 定义一个指向节点的指针类型
    typedef Node<Value> *NodePtr;

    Color color;     // 节点颜色，红色为 false，黑色为 true
    NodePtr parent;  // 父节点指针
    NodePtr left;    // 左子节点指针
    NodePtr right;   // 右子节点指针
    Value value;     // 节点存储的值

    // 构造函数，初始化节点时设置颜色为红色，父节点和子节点为空，值为传入的参数
    explicit Node(const Value &v = Value())
            : color(Red),  // 默认节点颜色为红色
              parent(nullptr),  // 父节点为空
              left(nullptr),    // 左子节点为空
              right(nullptr),   // 右子节点为空
              value(v) {}  // 节点的值为传入的参数值

    // 求取最小节点
    static NodePtr minimum(NodePtr x) {
        // 一直走到最左边，找到极小值
        while (x->left != nullptr) {
            x = x->left;
        }
        return x;
    }

    // 求取最大节点
    static NodePtr maximum(NodePtr x) {
        // 一直走到最右边，找到极大值
        while (x->right != nullptr) {
            x = x->right;
        }
        return x;
    }
};

// 迭代器
// Value：节点存储的数据类型
// Ref：对 Value 的引用类型，用于支持对值的修改或只读访问
// Ptr：指向 Value 的指针类型
template <class Value, class Ref, class Ptr>
struct Iterator {
    // 定义普通迭代器和常量迭代器类型
    typedef Iterator<Value, Value &, Value *> iterator;
    typedef Iterator<Value, const Value &, const Value *> constIterator;
    typedef Iterator<Value, Ref, Ptr> Self;
    typedef Node<Value> *NodePtr;  // 定义节点指针类型

    NodePtr node;  // 当前迭代器指向的节点

    // 默认构造函数
    Iterator() {}

    // 构造函数，初始化迭代器并将 node 指向给定节点
    Iterator(NodePtr x) { node = x; }

    // 复制构造函数，从另一个迭代器复制节点
    Iterator(const iterator &it) { node = it.node; }

    // 解引用运算符，返回当前节点的值
    Ref operator*() const { return node->value; }

    // 箭头运算符，返回当前节点值的指针
    Ptr operator->() const { return &(operator*()); }

    // 寻找后继节点（大于当前节点的最小节点）
    void increment() {
        if (node->right != nullptr) {  // 如果有右子节点
            node = node->right;        // 向右走
            // 然后一直往左子节点走到底，找到后继节点
            while (node->left != nullptr) {
                node = node->left;
            }
        } else {  // 没有右子节点
            NodePtr y = node->parent;  // 找出父节点
            // 如果当前节点是右子节点，则一直上溯，直到不为右子节点
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y) {  // 如果此时的右子节点不等于父节点
                node = y;            // 则此时的父节点为后继节点
            }
        }
    }

    // 寻找前驱节点（小于当前节点的最大节点）
    void decrement() {
        if (node->color == Red && node->parent->parent == node) {  // 如果是红色节点且父节点的父节点是当前节点(处理是否为Header节点的情况)
            node = node->right;  // 此时的右子节点为前驱节点
        } else if (node->left != nullptr) {  // 如果有左子节点
            NodePtr y = node->left;          // 让 y 指向左子节点
            // 一直往右子节点走到底，找到前驱节点
            while (y->right != nullptr) {
                y = y->right;
            }
            node = y;  // y 即为前驱节点
        } else {  // 如果既没有右子节点，也没有左子节点
            NodePtr y = node->parent;  // 找出父节点
            // 如果当前节点是左子节点，则一直上溯，直到不为左子节点
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;  // 当前节点的父节点即为前驱节点
        }
    }

    // 后置前置自增运算符
    Self &operator++() {
        increment();
        return *this;
    }

    // 前置自增运算符
    Self operator++(int) {
        Self tmp = *this;
        increment();
        return tmp;
    }

    // 后置前置自减运算符
    Self &operator--() {
        decrement();
        return *this;
    }

    // 前置自减运算符
    Self operator--(int) {
        Self tmp = *this;
        decrement();
        return tmp;
    }
};

// 运算符重载
template <class Value, class Ref, class Ptr>
bool operator==(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node == y.node;
}
// 运算符重载
template <class Value, class Ref, class Ptr>
bool operator!=(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node != y.node;
}

// 红黑树
template <class Key, class Value, class KeyOfValue, class Compare>
class RbTree {
public:
    // 类型定义部分
    typedef Node<Value> *NodePtr;
    typedef Node<Value> Node;
    typedef Value *Ptr;
    typedef const Value *constPtr;
    typedef Value &Ref;
    typedef const Value &constRef;
    typedef Iterator<Value, Ref, Ptr> iterator;
    typedef Iterator<Value, constRef, constPtr> constIterator;

private:
    // 内部成员变量
    size_t nodeCount;      // 树的节点总数
    Compare keyCompare;    // 键值比较函数对象
    NodePtr header;        // header 节点，简化边界处理
    // header 的作用：
    // 1. header->parent 指向根节点
    // 2. header->left 指向最小节点
    // 3. header->right 指向最大节点
    // 辅助函数，访问树的关键节点
    NodePtr &root() const { return header->parent; }
    NodePtr &leftmost() const { return header->left; }
    NodePtr &rightmost() const { return header->right; }

    // 静态函数，用于快速访问节点的属性
    static NodePtr &left(NodePtr x) { return x->left; }
    static NodePtr &right(NodePtr x) { return x->right; }
    static NodePtr &parent(NodePtr x) { return x->parent; }
    static Ref value(NodePtr x) { return x->value; }
    static const Key &key(NodePtr x) { return KeyOfValue()(value(x)); }
    static Color &color(NodePtr x) { return x->color; }
    static NodePtr minimum(NodePtr x) { return Node::minimum(x); }
    static NodePtr maximum(NodePtr x) { return Node::maximum(x); }

    // 树结构调整
    // 左旋（x 是旋转点, root 是根节点）
    static void RotateLeft(NodePtr x, NodePtr &root);
    // 右旋（x 是旋转点, root 是根节点）
    static void RotateRight(NodePtr x, NodePtr &root);
    // 重新令红黑树平衡 (x 是新节点, root 是根节点)
    static void Rebalance(NodePtr x, NodePtr &root);
    // 移除节点后重新令树平衡
    // z：要移除的节点, root：是根节点, leftmost：是最左节点, rightmost：是最右节点
    // 返回移除的节点
    static NodePtr RebalanceForErase(NodePtr z, NodePtr &root, NodePtr &leftmost, NodePtr &rightmost);

    // 内部操作
    // 插入实现
    iterator _insert(NodePtr x, NodePtr y, const Value &v);
    // 移除以 x 为根节点的整棵子树, 不进行平衡操作
    void _erase(NodePtr x);
    // 复制一个节点的值和颜色
    NodePtr _cloneNode(NodePtr x);
    // 复制以 x 为根节点的子树至另一棵树的节点 p 下
    // 返回复制子树的根节点 (父节点为 p)
    NodePtr _copy(NodePtr x, NodePtr p);
    // 空树的初始化
    void _emptyInitialize() {
        header = new Node();  // 构造 header 节点
        color(header) =  Red;// 令 header 为红色（虚拟节点）
        root() = 0;      // 此时没有根节点
        leftmost() = header;   // 令 header 的左子节点为自己
        rightmost() = header;  // 令 header 的右子节点为自己
    }

    // 调试和验证
    // 返回 node节点到根节点路径上的黑色节点个数
    int _blackCount(NodePtr node, NodePtr root);
    // 判断红黑树是否正确
    bool _rb_verify() const;

public:
    // 构造函数与析构函数
    explicit RbTree(const Compare &comp = Compare()) : nodeCount(0), keyCompare(comp) { _emptyInitialize(); }
    // 复制构造函数
    RbTree(const RbTree<Key, Value, KeyOfValue, Compare> &t)
            : nodeCount(0), keyCompare(t.keyCompare) {
        if (t.root() == 0) {     // 如果 t 是空树
            _emptyInitialize();  // 则初始化一棵空树
        } else {                 // 如果 t 非空树则进行复制
            color(header) = Red;
            root() = _copy(t.root(), header);  // 复制
            leftmost() = minimum(root());      // 设定最左节点
            rightmost() = maximum(root());     // 设定最右节点
        }
        nodeCount = t.nodeCount;  // 复制树的大小
    }
    // 析构函数
    ~RbTree() {
        clear();
        delete header;
    }

    // 运算符重载
    RbTree &operator=(const RbTree &x);

    // 树的基本信息
    bool empty() const { return nodeCount == 0; }
    size_t size() const { return nodeCount; }
    void clear();

    // 迭代器
    // 返回指向起始的迭代器 (最左节点)
    iterator begin() { return leftmost(); }
    constIterator begin() const { return leftmost(); }
    // 返回指向末尾的迭代器（最右节点的后一个节点）
    iterator end() { return header; }
    constIterator end() const { return header; }

    // 插入操作
    // 插入新值, 节点键值不允许重复, 若重复则插入无效
    // 返回值是 pair, 第一个元素是指向新增节点 (或重复键值节点) 的迭代器；第二个元素表示插入是否成功
    pair<iterator, bool> insertUnique(const Value &v);
    // 插入新值, 节点键值允许重复
    // 返回指向新增节点的迭代器
    iterator insertEqual(const Value &v);
    // 在指定位置插入新值, 节点键值不允许重复, 若重复则插入无效
    // 先判断插入位置是否正确, 正确则直接插入；错误则会先寻找到正确的位置再插入
    // 返回指向新增节点 (或重复键值节点) 的迭代器
    iterator insertUnique(iterator position, const Value &v);
    // 在指定位置插入新值, 节点键值允许重复
    // 返回指向新增节点的迭代器
    iterator insertEqual(iterator position, const Value &v);

    // 删除操作
    // 移除指定位置的节点
    void erase(iterator position);

    // 查询操作
    // 寻找键值为 k 的节点的迭代器
    iterator find(const Key &k);
    // 返回键值为 k 的节点区间
    // 返回 pair, 第一个元素是首个 >= k 的节点的迭代器；第二个元素是首个 > k 的节点的迭代器
    pair<iterator, iterator> equalRange(const Key &k);
    // 返回首个 >= k 的节点的迭代器
    iterator lowerBound(const Key &k);
    // 返回首个 > k 的节点的迭代器
    iterator upperBound(const Key &k);

    // 中序遍历函数
    void inOrderTraversal() const;
    void inOrderTraversal(NodePtr root) const;

    // 删除最右节点的函数
    void removeRightmost();

    // 获取根节点
    NodePtr rootNode() const {
        return root();
    }
};

#include "rbTree.h"

// 红黑树的中序遍历函数定义
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::inOrderTraversal() const {
    inOrderTraversal(root());
}

// 辅助函数的定义
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::inOrderTraversal(NodePtr root) const {
    if (root == nullptr) return;

    // 递归遍历左子树
    inOrderTraversal(root->left);

    // 输出节点的键值和颜色
    cout << "Node ID: " << key(root) << ", Color: "
         << (root->color == Red ? "RED" : "BLACK") << endl;

    // 递归遍历右子树
    inOrderTraversal(root->right);
}

// 删除最右节点函数定义
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::removeRightmost() {
    if (empty()) {
        cout << "The tree is empty." << endl;
        return;
    }
    NodePtr maxNode = maximum(root()); // 获取最右节点

    // 输出删除节点的颜色
    cout << "Before removing, Rightmost Node ID: " << key(maxNode)
         << ", Color: " << (maxNode->color == Red ? "RED" : "BLACK") << endl;

    erase(iterator(maxNode));  // 删除节点
    cout << "Removed the rightmost node: " << key(maxNode) << endl;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::_insert(NodePtr x, NodePtr y,
                                                 const Value &v) {
    // x 是新值的插入点, y 是插入点的父节点, v 是新值
    NodePtr z;

    // 如果 y 是 header 或者 x 不为 null（意味着找到插入点），并且新值小于父节点
    // 则将新节点作为左子节点插入
    if (y == header || x != 0 || keyCompare(KeyOfValue()(v), key(y))) {
        z = new Node(v);  // 创建新的节点 z
        left(y) = z;      // 将父节点 y 的左子节点指向新节点 z
        // 如果 y 是 header，说明树为空，插入的节点成为根节点
        if (y == header) {
            root() = z;   // 设置根节点为新节点 z
            rightmost() = z;  // 此时新节点既是最左节点，也是最右节点
        } else if (y == leftmost()) {  // 如果 y 为最左节点
            leftmost() = z;  // 更新 leftmost 为新节点 z
        }
    } else {  // 如果新值不小于父节点，则插入右子树
        z = new Node(v);  // 创建新的节点 z
        right(y) = z;     // 将父节点 y 的右子节点指向新节点 z
        if (y == rightmost()) {  // 如果 y 为最右节点
            rightmost() = z;  // 更新 rightmost 为新节点 z
        }
    }
    parent(z) = y;  // 设置新节点 z 的父节点为 y
    left(z) = 0;    // 设置新节点的左子节点为 nullptr
    right(z) = 0;   // 设置新节点的右子节点为 nullptr

    // 重新平衡树，保持红黑树的性质
    Rebalance(z, header->parent);  // 新节点的颜色在平衡过程中设定

    ++nodeCount;         // 更新树的节点计数
    return iterator(z);  // 返回一个指向新节点的迭代器
}

// 移除以 x 为根节点的整棵子树, 不进行平衡操作
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::_erase(NodePtr x) {
    while (x != 0) {
        _erase(right(x));  // 递归实现
        NodePtr y = left(x);
        delete x;
        x = y;
    }
}

// 克隆一个节点，并返回新创建的节点指针
template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::_cloneNode(NodePtr x) {
    // 创建一个新的节点 tmp，值与原节点 x 相同
    NodePtr tmp = new Node(x->value);
    // 将新节点的颜色设置为原节点 x 的颜色
    tmp->color = x->color;
    // 初始化新节点的左右子节点为 null（即没有子节点）
    tmp->left = 0;
    tmp->right = 0;
    // 返回新创建的节点指针
    return tmp;
}

// 递归复制一棵子树，并返回复制后的子树根节点指针
template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::_copy(NodePtr x, NodePtr p) {
    // 克隆当前节点 x，作为新树的根节点
    NodePtr top = _cloneNode(x);
    top->parent = p;  // 设置当前节点 top 的父节点为 p
    // 递归复制右子树（如果存在右子树）
    if (x->right != 0) {
        // 将右子树复制给 top->right，右子树的父节点设为 top
        top->right = _copy(right(x), top);
    }
    // 继续复制左子树
    p = top;  // 将父节点 p 更新为 top，准备复制左子树
    x = left(x);  // 将 x 更新为左子节点
    // 循环复制左子树的所有节点
    while (x != 0) {
        // 克隆当前左子节点
        NodePtr y = _cloneNode(x);
        p->left = y;  // 设置父节点 p 的左子节点为 y
        y->parent = p;  // 设置 y 的父节点为 p
        // 如果当前左子节点 y 有右子树，递归复制右子树
        if (x->right) {
            y->right = _copy(right(x), y);  // 递归复制 y 的右子树，父节点为 y
        }
        // 更新父节点为 y，准备复制 y 的左子树
        p = y;
        x = left(x);  // 更新 x 为左子节点
    }
    // 返回复制后的子树根节点 top
    return top;
}

template <class Key, class Value, class KeyOfValue, class Compare>
RbTree<Key, Value, KeyOfValue, Compare>
&RbTree<Key, Value, KeyOfValue, Compare>::operator=(
        const RbTree<Key, Value, KeyOfValue, Compare> &x) {
    if (this != &x) {  // 检查自赋值，避免对自身赋值
        clear();  // 先移除当前红黑树的所有节点，恢复到初始状态
        keyCompare = x.keyCompare;  // 复制比较器对象，用于比较节点的键值

        if (x.root() != 0) {  // 如果 x 是非空树，则需要进行树的复制
            root() = _copy(x.root(), header);  // 通过调用 _copy 函数递归复制 x 的树
            leftmost() = minimum(root());      // 设定当前树的最左节点
            rightmost() = maximum(root());     // 设定当前树的最右节点
            nodeCount = x.nodeCount;           // 复制树的节点数量
        }
    }
    return *this;  // 返回当前对象的引用，支持链式赋值
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::clear() {
    if (nodeCount != 0) {      // 如果树中有节点，即非空树
        _erase(root());        // 调用 _erase 函数递归地移除整棵树
        root() = 0;            // 清空根节点，表示树为空
        leftmost() = header;   // 将最左节点重置为 header，表示没有节点
        rightmost() = header;  // 将最右节点重置为 header，表示没有节点
        nodeCount = 0;         // 清空节点计数，树中没有节点
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename RbTree<Key, Value, KeyOfValue, Compare>::iterator, bool>
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(const Value &v) {
    NodePtr y = header;  // y 指向插入点的父节点，初始化为 header
    NodePtr x = root();  // 从根节点开始查找合适的插入位置
    bool comp = true;     // comp 用于比较值的大小关系，初始化为 true

    // 往下寻找合适的插入点
    while (x != 0) {
        y = x;  // 记录父节点
        comp = keyCompare(KeyOfValue()(v), key(x));  // 比较插入值与当前节点的键值
        // 如果插入值小于当前节点的键值，则往左走；否则往右走
        x = comp ? left(x) : right(x);
    }

    iterator j = iterator(y);  // 令迭代器 j 指向插入节点的父节点
    if (comp) {  // 如果 comp 为 true，表示插入值应该在左子树
        if (j == begin()) {  // 如果插入点是最左边
            // 如果插入点的父节点是最左节点，说明键值不重复，可以插入
            return pair<iterator, bool>(_insert(x, y, v), true);
        } else {
            // 如果插入点的父节点不是最左节点，可能存在重复键值，向前检查
            --j;  // 向前移动迭代器 j，检查是否存在相同的键值
        }
    }

    // 对于两种情况的判断
    // 1. comp = false, 表示插入值应该在右子树
    // 2. comp = true, 此时 j 为祖父节点，可能存在重复键值
    if (keyCompare(key(j.node), KeyOfValue()(v))) {
        // 如果 j < v，说明插入的值比 j 小，可以插入
        return pair<iterator, bool>(_insert(x, y, v), true);  // 插入成功
    }

    // 到这里说明插入的值与树中的值重复
    return pair<iterator, bool>(j, false);  // 返回现有节点，并且插入失败
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(const Value &v) {
    NodePtr y = header;  // y 指向 x 的父节点，初始化为 header
    NodePtr x = root();  // 从根节点开始查找合适的插入位置

    // 查找插入点
    while (x != 0) {  // 循环直到找到一个空的位置 (插入点)
        y = x;  // 更新父节点 y 为当前节点 x
        // 如果 v < x，则往左走，否则往右走
        x = keyCompare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }

    // 到这里，x 已经是空的位置，y 是 x 的父节点，v 是待插入的值
    // _insert 用来插入新节点，x 是插入点，y 是插入点的父节点
    return _insert(x, y, v);  // 调用 _insert 函数插入新值并返回迭代器
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(iterator position,
                                                      const Value &v) {
    // 检查插入位置是否为 begin()
    if (position.node == header->left) {
        if (size() > 0 &&                      // 树非空
            keyCompare(KeyOfValue()(v),        // 且 v 小于树中的第一个元素
                       key(position.node))) {  // 确保键值不重复且插入位置正确
            return _insert(position.node, position.node, v);  // 插入新节点
        } else {  // 如果重复或者位置不正确
            return insertUnique(v).first;  // 递归调用 insertUnique 插入新值
        }
    }
        // 检查插入位置是否为 end()
    else if (position.node == header) {   // 插入位置在 end()
        if (keyCompare(key(rightmost()),    // end() 位置小于 v
                       KeyOfValue()(v))) {  // 确保键值不重复且插入位置正确
            return _insert(0, rightmost(), v);  // 在树的末尾插入新节点
        } else {  // 如果重复或者位置不正确
            return insertUnique(v).first;  // 递归调用 insertUnique 插入新值
        }
    } else {  // 插入位置为其他中间位置
        iterator before = position;
        --before;  // 获取插入位置之前的节点
        // 检查 v 是否在 before 和 position 之间且不重复
        if (keyCompare(key(before.node), KeyOfValue()(v)) &&
            keyCompare(KeyOfValue()(v),        // before < v < position
                       key(position.node))) {  // 确保键值不重复且插入位置正确
            if (right(before.node) == 0) {  // 如果 before 节点没有右子节点
                return _insert(0, before.node, v);  // 插入到 before 的右子节点
            } else {  // 如果 before 节点有右子节点
                return _insert(position.node, position.node, v);  // 插入到 position 位置
            }
        } else {  // 如果重复或者位置不正确
            return insertUnique(v).first;  // 递归调用 insertUnique 插入新值
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(iterator position,
                                                     const Value &v) {
    // 如果插入位置是树的最左边 (begin())
    if (position.node == header->left) {
        // 检查树是否为空且 v >= begin()，如果是，插入新节点
        if (size() > 0                          // 非空树
            && !keyCompare(key(position.node),  // begin() >= v
                           KeyOfValue()(v))) {  // 位置正确
            return _insert(position.node, position.node, v);  // 插入新节点
        } else {  // 如果位置不正确（可能 v 小于树中所有元素），递归调用 insertEqual 插入
            return insertEqual(v);
        }
    }
        // 如果插入位置是树的最右边 (end())
    else if (position.node == header) {
        // 检查 v >= end()，如果是，插入新节点
        if (!keyCompare(KeyOfValue()(v),      // v >= end()
                        key(rightmost()))) {  // 位置正确
            return _insert(0, rightmost(), v);  // 插入新节点
        } else {  // 如果位置不正确（可能 v 大于树中所有元素），递归调用 insertEqual 插入
            return insertEqual(v);
        }
    }
        // 如果插入位置在树中的中间
    else {
        iterator before = position;  // 获取插入位置的前一个节点
        --before;  // 使 before 指向插入位置的前一个节点
        // 检查 v 是否在 before 和 position 之间（即 v >= before 且 v <= position）
        if (!keyCompare(KeyOfValue()(v), key(before.node)) &&
            !keyCompare(key(position.node),  // position >= v >= before
                        KeyOfValue()(v))) {  // 位置正确
            // 如果 before 没有右子节点，插入到 before 的右子节点
            if (right(before.node) == 0) {
                return _insert(0, before.node, v);  // 插入新节点
            } else {  // 否则，插入到 position 位置
                return _insert(position.node, position.node, v);  // 插入新节点
            }
        } else {  // 如果位置不正确（即 v 不在 before 和 position 之间），递归调用 insertEqual 插入
            return insertEqual(v);
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::erase(iterator position) {
    // 通过调用 RebalanceForErase 函数移除指定位置的节点并重新平衡树
    NodePtr y = RebalanceForErase(position.node, header->parent, header->left,
                                  header->right);
    delete y;  // 删除节点 y
    --nodeCount;  // 树的节点数量减一
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::find(const Key &k) {
    NodePtr y = header;  // y 最终指向最接近的 >= k 的节点
    NodePtr x = root();  // 从根节点开始查找

    while (x != 0) {  // 遍历树直到找到匹配节点或者到达空节点
        if (!keyCompare(key(x), k)) {  // 如果 k <= x
            y = x;  // 更新 y 为当前节点
            x = left(x);  // 在左子树中继续查找
        } else {  // 如果 k > x
            x = right(x);  // 在右子树中继续查找
        }
    }

    iterator j = iterator(y);  // 创建指向 y 的迭代器
    // 如果没有找到匹配的节点，则返回 end()
    return (j == end() || keyCompare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::lowerBound(const Key &k) {
    NodePtr y = header;  // 最终指向首个 >= k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                   // 非空树
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);  // 继续在左子树查找
        } else {  // k > x
            x = right(x);  // 继续在右子树查找
        }
    }

    return iterator(y);  // 返回首个 >= k 的节点的迭代器
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::upperBound(const Key &k) {
    NodePtr y = header;  // 最终指向首个 > k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                  // 非空树
        if (keyCompare(k, key(x))) {  // k < x
            y = x, x = left(x);  // 继续在左子树查找
        } else {  // k >= x
            x = right(x);  // 继续在右子树查找
        }
    }

    return iterator(y);  // 返回首个 > k 的节点的迭代器
}

template <class Key, class Value, class KeyOfValue, class Compare>
pair<typename RbTree<Key, Value, KeyOfValue, Compare>::iterator,
        typename RbTree<Key, Value, KeyOfValue, Compare>::iterator>
RbTree<Key, Value, KeyOfValue, Compare>::equalRange(const Key &k) {
    // 返回一个包含 [lowerBound(k), upperBound(k)) 范围的迭代器对
    return pair<iterator, iterator>(lowerBound(k), upperBound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare>
bool RbTree<Key, Value, KeyOfValue, Compare>::_rb_verify() const {
    // 判断红黑树是否合法

    // 判断空树的合法性
    if (nodeCount == 0 || begin() == end()) {
        return nodeCount == 0 && begin() == end() && header->left == header &&
               header->right == header;
    }

    int len = _blackCount(leftmost(), root());  // 计算黑色节点的数量
    // 循环遍历节点，判断红黑树的结构是否符合红黑树的性质
    for (constIterator it = begin(); it != end(); ++it) {
        NodePtr x = it.node;
        NodePtr L = left(x);
        NodePtr R = right(x);

        // 判断是否有两个连续红色节点
        if (x->color == Red) {
            if ((L && L->color == Red) || (R && R->color == Red)) {
                return false;  // 两个连续的红色节点不合法
            }
        }

        // 判断左子节点和右子节点的位置是否正确
        if (L && keyCompare(key(x), key(L))) {
            return false;  // 左子节点的键值比父节点大
        }
        if (R && keyCompare(key(R), key(x))) {
            return false;  // 右子节点的键值比父节点小
        }

        // 判断叶子节点到根节点的黑色节点个数是否相同
        if (!L && !R && _blackCount(x, root()) != len) {
            return false;  // 如果叶子节点到根节点的黑色节点个数不一致，返回 false
        }
    }

    // 判断最左最右节点是否正确
    if (leftmost() != Node::minimum(root())) {
        return false;  // 最左节点应该是最小值
    }
    if (rightmost() != Node::maximum(root())) {
        return false;  // 最右节点应该是最大值
    }

    return true;  // 如果通过了所有检查，返回 true
}

template <class Key, class Value, class KeyOfValue, class Compare>
int RbTree<Key, Value, KeyOfValue, Compare>::_blackCount(NodePtr node, NodePtr root) {
    if (node == nullptr) {
        return 0;  // 如果节点为空，黑色节点数为0
    } else {
        int bc = node->color == Black ? 1 : 0;  // 判断当前节点是否为黑色，是则计数+1
        if (node == root) {
            return bc;  // 如果当前节点是根节点，则返回黑色节点数
        } else {
            return bc + _blackCount(node->parent, root);  // 向上递归父节点，计算黑色节点数量
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::RotateLeft(NodePtr x, NodePtr &root) {
    NodePtr y = x->right;      // 令 y 为旋转点的右子节点
    x->right = y->left;        // x 的右子节点更改为 y 的左子节点
    if (y->left != nullptr) {  // 如果 y 的左子节点存在
        y->left->parent = x;   // 更改其左子节点的父节点为 x
    }
    y->parent = x->parent;  // y 的父节点更改为 x 的父节点

    // 令 y 完全替代 x (接收 x 对于其父节点的关系)
    if (x == root) {  // 如果 x 是根节点 (此时 y 的父节点是 header)
        root = y;  // 将 y 设置为根节点
    } else if (x == x->parent->left) {  // 如果 x 是父节点的左子节点
        x->parent->left = y;            // 将 y 设置为父节点的左子节点
    } else {                            // 如果 x 是父节点的右子节点
        x->parent->right = y;           // 将 y 设置为父节点的右子节点
    }
    y->left = x;  // 将 x 设置为 y 的左子节点
    x->parent = y;  // 更新 x 的父节点为 y
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::RotateRight(NodePtr x, NodePtr &root) {
    NodePtr y = x->left;        // 令 y 为旋转点的左子节点
    x->left = y->right;         // x 的左子节点更改为 y 的右子节点
    if (y->right != nullptr) {  // 如果 y 的右子节点存在
        y->right->parent = x;   // 更改其右子节点的父节点为 x
    }
    y->parent = x->parent;  // y 的父节点更改为 x 的父节点

    // 令 y 完全替代 x (接收 x 对于其父节点的关系)
    if (x == root) {  // 如果 x 是根节点 (此时 y 的父节点是 header)
        root = y;     // 将 y 设置为根节点
    } else if (x == x->parent->right) {  // 如果 x 是父节点的右子节点
        x->parent->right = y;  // 将 y 设置为父节点的右子节点
    } else {                   // 如果 x 是父节点的左子节点
        x->parent->left = y;   // 将 y 设置为父节点的左子节点
    }
    y->right = x;   // 将 x 设置为 y 的右子节点
    x->parent = y;  // 更新 x 的父节点为 y
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::Rebalance(NodePtr x, NodePtr &root) {
    x->color = Red;  // 设置新节点颜色为红色, 因为如果插入的节点是黑色, 必然会导致树不平衡
    while (x != root && x->parent->color == Red) {  // 当 x 非根节点且 x 的父节点为红色时需要进行平衡操作
        // 一: 父节点是祖父节点的左子节点
        if (x->parent == x->parent->parent->left) {
            NodePtr y = x->parent->parent->right;  // 令 y 为伯父节点
            if (y && y->color == Red) {  // 1: 伯父节点存在且为红色
                x->parent->color = Black;  // 更改父节点为黑色
                y->color = Black;  // 更改伯父节点为黑色
                x->parent->parent->color = Red;  // 更改祖父节点为红色
                x = x->parent->parent;  // 祖父颜色由黑变红可能会破坏上层平衡，将当前节点更改为祖父节点继续往上检查
            } else {
                if (x == x->parent->right) {  // 2: 伯父节点不存在或为黑色
                    x = x->parent;
                    RotateLeft(x, root);  // 需要先进行左旋成为左子节点
                }
                x->parent->color = Black;  // 更改父节点为黑色
                x->parent->parent->color = Red;  // 更改祖父节点为红色
                RotateRight(x->parent->parent, root);  // 右旋
            }
        } else {  // 二: 父节点是祖父节点的右子节点
            NodePtr y = x->parent->parent->left;  // 令 y 为伯父节点
            if (y && y->color == Red) {  // 1: 伯父节点存在且为红色
                x->parent->color = Black;  // 更改父节点为黑色
                y->color = Black;  // 更改伯父节点为黑色
                x->parent->parent->color = Red;  // 更改祖父节点为红色
                x = x->parent->parent;  // 祖父颜色由黑变红可能会破坏上层平衡，将当前节点更改为祖父节点继续往上检查
            } else {
                if (x == x->parent->left) {  // 2: 伯父节点不存在或为黑色
                    x = x->parent;
                    RotateRight(x, root);  // 需要先进行右旋成为右子节点
                }
                x->parent->color = Black;  // 更改父节点为黑色
                x->parent->parent->color = Red;  // 更改祖父节点为红色
                RotateLeft(x->parent->parent, root);  // 左旋
            }
        }
    }
    root->color = Black;  // 根节点永远为黑色
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::RebalanceForErase(NodePtr z,
                                                           NodePtr &root,
                                                           NodePtr &leftmost,
                                                           NodePtr &rightmost) {
    NodePtr y = z;  // 令 y 指向被删除的节点 z
    NodePtr x = nullptr;  // x 用来指向被删除节点的子节点，可能为空
    NodePtr xParent = nullptr;  // 用来保存 x 的父节点

    // 根据被删除节点的子节点情况进行处理
    if (y->left == nullptr) {  // 如果 z 没有左子节点
        x = y->right;  // 令 x 为 y 的右子节点
    } else if (y->right == nullptr) {  // 如果 z 只有左子节点
        x = y->left;  // 令 x 为 y 的左子节点
    } else {  // 如果 z 有两个子节点
        // 找到 z 的后继节点 (即右子树中的最小节点)
        y = y->right;
        while (y->left != nullptr) {
            y = y->left;
        }
        x = y->right;  // 令 x 为后继节点的右子节点 (可能为空)
    }

    if (y != z) {  // 如果 z 有两个子节点 (后继节点被用来取代 z)
        // 用后继节点 y 取代 z 的位置
        z->left->parent = y;  // 调整 z 的左子节点的父节点指向 y
        y->left = z->left;  // 让 y 的左子节点指向 z 的左子节点
        if (y != z->right) {  // 如果 y 不是 z 的右子节点
            xParent = y->parent;
            if (x) {
                x->parent = y->parent;  // 如果 x 存在，调整 x 的父节点指向 y 的父节点
            }
            y->parent->left = x;  // 将 y 的父节点的左子节点指向 x
            y->right = z->right;  // 将 y 的右子节点指向 z 的右子节点
            z->right->parent = y;  // 将 z 的右子节点的父节点指向 y
        } else {  // 如果 y 就是 z 的右子节点，不需要调整
            xParent = y;
        }

        // 更新父节点指向 y
        if (root == z) {  // 如果删除的是根节点
            root = y;  // y 成为新的根节点
        } else if (z->parent->left == z) {  // 如果 z 是父节点的左子节点
            z->parent->left = y;  // 将 y 设置为父节点的左子节点
        } else {
            z->parent->right = y;  // 如果 z 是父节点的右子节点，将 y 设置为父节点的右子节点
        }
        y->parent = z->parent;  // 将 y 的父节点指向 z 的父节点
        std::swap(y->color, z->color);  // 交换 z 和 y 的颜色
        y = z;  // 重新指向需要删除的节点
    } else {  // 如果 z 只有一个子节点 (x)，用 x 取代 z
        xParent = y->parent;
        if (x) {
            x->parent = y->parent;  // 如果 x 存在，调整 x 的父节点指向 y 的父节点
        }

        // 更新根节点或者父节点指向 x
        if (root == z) {  // 如果删除的是根节点
            root = x;  // x 成为新的根节点
        } else if (z->parent->left == z) {  // 如果 z 是父节点的左子节点
            z->parent->left = x;  // 将 x 设置为父节点的左子节点
        } else {
            z->parent->right = x;  // 将 x 设置为父节点的右子节点
        }

        // 如果删除的是最左节点，更新 leftmost
        if (leftmost == z) {
            if (z->right == nullptr) {
                leftmost = z->parent;  // 如果右子节点为空，将 leftmost 设置为父节点
            } else {
                leftmost = Node::minimum(x);  // 否则，找到 x 的最左子节点
            }
        }

        // 如果删除的是最右节点，更新 rightmost
        if (rightmost == z) {
            if (z->left == nullptr) {
                rightmost = z->parent;  // 如果左子节点为空，将 rightmost 设置为父节点
            } else {
                rightmost = Node::maximum(x);  // 否则，找到 x 的最右子节点
            }
        }
    }

    // 重新平衡红黑树
    // 删除红色节点不会破坏平衡
    if (y->color != Red) {
        // 如果 x 不是根节点且 x 为空或颜色为黑色，意味着少了一个黑色节点，需要修复
        while (x != root && (x == nullptr || x->color == Black)) {
            if (x == xParent->left) {  // 如果 x 是父节点的左子节点
                NodePtr w = xParent->right;  // w 为 x 的兄弟节点
                if (w->color == Red) {  // 兄弟节点为红色
                    w->color = Black;
                    xParent->color = Red;
                    RotateLeft(xParent, root);  // 对父节点进行左旋
                    w = xParent->right;  // 重新设置 w
                }
                // 兄弟节点为黑色，且兄弟的左右子节点也为黑色
                if ((w->left == nullptr || w->left->color == Black) &&
                    (w->right == nullptr || w->right->color == Black)) {
                    w->color = Red;
                    x = xParent;  // 将 x 移动到父节点
                    xParent = xParent->parent;
                } else {
                    // 兄弟节点为黑色，且其左子节点为红色，右子节点为黑色
                    if (w->right == nullptr || w->right->color == Black) {
                        if (w->left) {
                            w->left->color = Black;
                        }
                        w->color = Red;
                        RotateRight(w, root);
                        w = xParent->right;
                    }
                    w->color = xParent->color;  // 兄弟节点颜色变为父节点的颜色
                    xParent->color = Black;  // 父节点变为黑色
                    if (w->right) {
                        w->right->color = Black;  // 兄弟的右子节点变为黑色
                    }
                    RotateLeft(xParent, root);  // 对父节点进行左旋
                    break;  // 结束修复
                }
            } else {  // 如果 x 是父节点的右子节点，过程与左子节点对称
                NodePtr w = xParent->left;
                if (w->color == Red) {
                    w->color = Black;
                    xParent->color = Red;
                    RotateRight(xParent, root);
                    w = xParent->left;
                }
                if ((w->right == nullptr || w->right->color == Black) &&
                    (w->left == nullptr || w->left->color == Black)) {
                    w->color = Red;
                    x = xParent;
                    xParent = xParent->parent;
                } else {
                    if (w->left == nullptr || w->left->color == Black) {
                        if (w->right) {
                            w->right->color = Black;
                        }
                        w->color = Red;
                        RotateLeft(w, root);
                        w = xParent->left;
                    }
                    w->color = xParent->color;
                    xParent->color = Black;
                    if (w->left) {
                        w->left->color = Black;
                    }
                    RotateRight(xParent, root);
                    break;
                }
            }
        }
        if (x) {
            x->color = Black;  // 如果 x 不为空，设置 x 为黑色
        }
    }
    return y;  // 返回被删除的节点
}

#endif //LIBRARYMANAGEMENT_RBTREE_H
