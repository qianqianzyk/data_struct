#include "book.h"

// 默认构造函数
Book::Book() = default;

// 有参构造函数
Book::Book(int id, const string& ISBN, const string& name, const string& author,
           const string& publisher, int year, bool borrowStatus,
           const string& borrower)
        : id(id),                          // 初始化编号
          ISBN(ISBN),                      // 初始化 ISBN
          name(name),                      // 初始化书名
          author(author),                  // 初始化作者
          publisher(publisher),            // 初始化出版社
          year(year),                      // 初始化出版年份
          borrowStatus(borrowStatus),      // 初始化借阅状态
          borrower(borrower) {}            // 初始化借阅人

// 默认析构函数
Book::~Book() = default;

// 获取编号
const int& Book::GetId() const { return id; }

// 设置编号
void Book::SetId(int id) { Book::id = id; }

// 获取 ISBN
const string& Book::GetISBN() const { return ISBN; }

// 设置 ISBN
void Book::SetISBN(const string& ISBN) { Book::ISBN = ISBN; }

// 获取书名
const string& Book::GetName() const { return name; }

// 设置书名
void Book::SetName(const string& name) { Book::name = name; }

// 获取作者
const string& Book::GetAuthor() const { return author; }

// 设置作者
void Book::SetAuthor(const string& author) { Book::author = author; }

// 获取出版社
const string& Book::GetPublisher() const { return publisher; }

// 设置出版社
void Book::SetPublisher(const string& publisher) {
    Book::publisher = publisher;
}

// 获取出版年份
int Book::GetYear() const { return year; }

// 设置出版年份
void Book::SetYear(int year) { Book::year = year; }

// 获取借阅状态
bool Book::GetBorrowStatus() const { return borrowStatus; }

// 设置借阅状态
void Book::SetBorrowStatus(bool status) { Book::borrowStatus = status; }

// 获取借阅人
const string& Book::GetBorrower() const { return borrower; }

// 设置借阅人
void Book::SetBorrower(const string& borrower) { Book::borrower = borrower; }

// 重载输入流运算符，用于输入书的相关信息
istream& operator>>(istream& in, Book& book) {
    // 输入编号、ISBN、书名、作者、出版社、出版年份和借阅状态
    in >> book.id >> book.ISBN >> book.name >> book.author >> book.publisher >>
       book.year >> book.borrowStatus;

    // 如果书处于借阅状态，还需输入借阅人信息
    if (book.borrowStatus) {
        in >> book.borrower;
    } else {
        book.borrower.clear();  // 如果未借阅，清空借阅人信息
    }
    return in;
}

// 重载输出流运算符，用于输出书的相关信息
ostream& operator<<(ostream& out, const Book& book) {
    // 判断是否为标准输出
    if (&out == &cout) {
        out << "  书籍编号: " << book.id
            << "  ISBN: " << book.ISBN
            << "  书名: " << book.name
            << "  作者: " << book.author
            << "  出版社: " << book.publisher
            << "  出版年份: " << book.year
            << "  借阅状态: " << (book.borrowStatus ? "已借出" : "在库中");

        if (book.borrowStatus) {
            out << "  借阅人: " << book.borrower;
        }
    } else {
        // 格式化输出到文件，字段以空格分隔
        out << book.id << " " << book.ISBN << " " << book.name << " "
            << book.author << " " << book.publisher << " " << book.year << " "
            << book.borrowStatus << " " << (book.borrowStatus ? book.borrower : "");
    }
    return out;
}