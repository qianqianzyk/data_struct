#ifndef LIBRARYMANAGEMENT_ADMIN_H
#define LIBRARYMANAGEMENT_ADMIN_H

#include <iostream>
using namespace std;

// 定义图书管理员类 Admin
class Admin {
private:
    string admin_name;      // 图书管理员的用户名
    string admin_password;  // 图书管理员的密码

public:
    // 默认构造函数
    Admin();

    // 有参构造函数
    Admin(const string &adminName, const string &adminPassword);

    // 析构函数
    ~Admin();

    // 获取管理员用户名
    const string &GetAdminName() const;

    // 设置管理员用户名
    void SetAdminName(const string &adminName);

    // 获取管理员密码
    const string &GetAdminPassword() const;

    // 设置管理员密码
    void SetAdminPassword(const string &adminPassword);

    // 重载输入流运算符，支持从输入流中读取管理员信息
    friend istream &operator>>(istream &in, Admin &user);

    // 重载输出流运算符，支持将管理员信息输出到流中
    friend ostream &operator<<(ostream &os, const Admin &user);
};

#endif //LIBRARYMANAGEMENT_ADMIN_H
