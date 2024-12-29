#ifndef LIBRARYMANAGEMENT_BOOKMANAGER_H
#define LIBRARYMANAGEMENT_BOOKMANAGER_H

#include "book.h"
#include "rbTree.h"

// 图书馆图书管理核心类
class BookManager {
private:
    // 用于从 Book 对象中提取书籍编号
    struct IdOfBook {
        // 重载函数调用运算符，返回书籍的编号
        const int& operator()(const Book& book) const { return book.GetId(); }
    };

    // 红黑树类型定义，使用书籍编号作为键值
    typedef RbTree<int, Book, IdOfBook, std::less<>> RbTree;

    // 管理图书的红黑树容器
    RbTree libraryManager;

    // 记录当前book的id最大值
    int currentMaxId;

public:
    // 构造函数
    BookManager();

    // 析构函数
    ~BookManager();

    // 初始化书籍数据，从指定路径加载数据文件
    void Init(string path,string fileType);

    // 加载图书最大ID
    void LoadMaxId(string filePath);

    //  更新图书最大ID
    void UpdateMaxId(string filePath);

    // 添加书籍到图书馆
    void Insert();

    // 检查图书馆是否为空
    bool Empty();

    // 分页查询所有书籍，currPage 为当前页码，pageSize 为每页显示数量
    void FindByPage(int currPage, int pageSize);

    // 根据书籍编号查找书籍
    void FindByID();

    // 根据 ISBN 号查找书籍
    void FindByISBN();

    // 根据书籍编号更新书籍信息
    void UpdateByID();

    // 根据 ISBN 号更新书籍信息
    void UpdateByISBN();

    // 根据书籍编号删除书籍
    void RemoveByID();

    // 根据 ISBN 号删除书籍
    void RemoveByISBN();

    // 借出书籍操作
    void Lend();

    // 归还书籍操作
    void Return();

    // 查询所有已借出的书籍
    void FindAllLend();

    // 保存书籍数据到文件
    void Save(string path, string fileType);

    // 测试红黑树功能
    void TestRbTree();
};

#endif //LIBRARYMANAGEMENT_BOOKMANAGER_H
