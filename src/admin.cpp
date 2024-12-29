#include "admin.h"

// Admin 默认构造函数
// 默认实现
Admin::Admin() = default;


// Admin 参数化构造函数
// 使用初始化列表将传入的用户名和密码赋值给成员变量
Admin::Admin(const string &adminName, const string &adminPassword)
        : admin_name(adminName), admin_password(adminPassword) {}

// Admin 析构函数
// 默认实现
Admin::~Admin() = default;


// 获取管理员用户名
// 返回成员变量 admin_name
const string &Admin::GetAdminName() const {
    return admin_name;
}

// 设置管理员用户名
// 将传入的参数 adminName 赋值给成员变量 admin_name
void Admin::SetAdminName(const string &adminName) {
    admin_name = adminName;
}

// 获取管理员密码
// 返回成员变量 admin_password
const string &Admin::GetAdminPassword() const {
    return admin_password;
}

// 设置管理员密码
// 将传入的参数 adminPassword 赋值给成员变量 admin_password
void Admin::SetAdminPassword(const string &adminPassword) {
    admin_password = adminPassword;
}

// 重载输入流操作符
// 从输入流中读取管理员用户名和密码，并将它们赋值给 Admin 对象
istream &operator>>(istream &in, Admin &admin) {
    in >> admin.admin_name >> admin.admin_password; // 从流中依次读取用户名和密码
    return in; // 返回输入流引用以支持链式操作
}

// 重载输出流操作符
// 将 Admin 对象的用户名和密码输出到流中
ostream &operator<<(ostream &out, const Admin &admin) {
    out << admin.admin_name << " " << admin.admin_password; // 格式化输出用户名和密码
    return out; // 返回输出流引用以支持链式操作
}

