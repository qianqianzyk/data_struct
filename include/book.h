#ifndef LIBRARYMANAGEMENT_BOOK_H
#define LIBRARYMANAGEMENT_BOOK_H

#include <iostream>
using namespace std;

// Book 类表示一本书的相关信息
class Book {
private:
    int id;             // 编号 (每本书的唯一标识)
    string ISBN;        // 国际标准书号 (唯一标识书籍)
    string name;        // 书名
    string author;      // 作者
    string publisher;   // 出版社
    int year;           // 出版年份
    bool borrowStatus;  // 借阅状态 0: 在库中, 1: 借阅中
    string borrower;    // 借阅人

public:
    // 默认构造函数
    Book();

    // 带参数的构造函数，初始化书的各个属性
    Book(int id, const string& ISBN, const string& name, const string& author,
         const string& publisher, int year, bool borrowStatus,
         const string& borrower);

    // 析构函数
    ~Book();

    // 获取书的编号
    const int& GetId() const;
    // 设置书的编号
    void SetId(int id);

    // 获取书的 ISBN
    const string& GetISBN() const;
    // 设置书的 ISBN
    void SetISBN(const string& isbn);

    // 获取书名
    const string& GetName() const;
    // 设置书名
    void SetName(const string& name);

    // 获取作者
    const string& GetAuthor() const;
    // 设置作者
    void SetAuthor(const string& author);

    // 获取出版社
    const string& GetPublisher() const;
    // 设置出版社
    void SetPublisher(const string& publisher);

    // 获取出版年份
    int GetYear() const;
    // 设置出版年份
    void SetYear(int year);

    // 获取借阅状态
    bool GetBorrowStatus() const;
    // 设置借阅状态
    void SetBorrowStatus(bool borrow_status);

    // 获取借阅人
    const string& GetBorrower() const;
    // 设置借阅人
    void SetBorrower(const string& borrower);

    // 友元函数，重载输入流运算符，用于输入书的相关信息
    friend istream& operator>>(istream& in, Book& book);

    // 友元函数，重载输出流运算符，用于输出书的相关信息
    friend ostream& operator<<(ostream& out, const Book& book);
};


#endif //LIBRARYMANAGEMENT_BOOK_H
