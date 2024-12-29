#ifndef LIBRARYMANAGEMENT_ADMINMANAGER_H
#define LIBRARYMANAGEMENT_ADMINMANAGER_H

#include "admin.h"
#include "rbTree.h"

// 定义管理员管理类 AdminManager
class AdminManager {
private:
    // 提取 Admin 对象的用户名
    struct NameOfUser {
        // 重载函数调用运算符，返回 Admin 对象的用户名
        const string& operator()(const Admin& admin) const { return admin.GetAdminName(); }
    };

    // 定义红黑树类型，用于存储管理员信息
    // - 键类型：string（管理员用户名）
    // - 值类型：Admin（管理员对象）
    // - 提取键的方式：NameOfUser
    // - 比较方式：std::less<>（默认按字典顺序比较字符串）
    typedef RbTree<string, Admin, NameOfUser, std::less<>> RbTree;

    RbTree adminManager; // 管理员信息的红黑树实例

public:
    AdminManager(); // 默认构造函数

    ~AdminManager(); // 析构函数

    // 初始化管理员数据
    // 参数:
    // - path: 文件路径
    // - fileType: 文件类型
    void Init(string path, string fileType);

    // 管理员注册功能
    // 返回值:
    // - true: 注册成功
    // - false: 注册失败
    bool Register();

    // 管理员登录功能
    // 返回值:
    // - true: 登录成功
    // - false: 登录失败
    bool Login();

    // 保存管理员数据到文件
    // 参数:
    // - path: 保存路径
    // - fileType: 文件类型
    void Save(string path, string fileType);
};

#endif //LIBRARYMANAGEMENT_ADMINMANAGER_H
