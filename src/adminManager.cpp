#include "adminManager.h"
#include <fstream>
#include <iostream>
#include "rbTree.h"
using namespace std;

// 使用默认构造函数
AdminManager::AdminManager() = default;

// 使用默认析构函数
AdminManager::~AdminManager() = default;

// 初始化管理员数据
void AdminManager::Init(string path, string fileType) {
    // 拼接文件路径
    string file = path + fileType;

    // 打开文件读取数据
    ifstream in;
    in.open(file, ios::in);  // 以输入模式打开文件
    if (!in.is_open()) {  // 如果文件无法打开，说明文件可能不存在
        // 如果文件不存在，创建文件并立即关闭
        ofstream out;
        out.open(file, ios::out | ios::app);  // 以追加模式打开文件
        out.close();  // 创建文件后关闭
        in.open(file, ios::in);  // 重新以输入模式打开文件
    }

    // 临时存储管理员信息的变量
    Admin user;
    // 读取文件内容直到文件末尾
    while (in.peek() != EOF) {  // 使用 peek 检查是否到文件末尾
        in >> user;  // 从文件中读取管理员数据到 user
        // 将读取到的管理员数据插入到 adminManager 容器中
        adminManager.insertUnique(adminManager.end(), user);
    }

    // 关闭文件
    in.close();
}

// 管理员注册功能
bool AdminManager::Register() {
    string secretKey, name, password, rePassword;

    // 提示用户输入密钥
    cout << "请输入密钥: ";
    cin >> secretKey;
    if (secretKey != "ZJUT") {  // 检查密钥是否正确
        cout << "密钥不正确!请重试" << endl;
        return false;
    }

    // 提示用户输入用户名
    cout << "请输入您的用户名: ";
    cin >> name;
    if (adminManager.find(name) != adminManager.end()) {  // 检查用户名是否已注册
        cout << "该用户名已注册" << endl;
        return false;
    }

    // 提示用户输入密码
    cout << "请输入您的密码: ";
    cin >> password;
    cout << "请确认您的密码: ";
    cin >> rePassword;
    if (password != rePassword) {  // 检查两次输入的密码是否一致
        cout << "密码不匹配!请重试" << endl;
        return false;
    }

    // 创建新的管理员对象并插入到管理员管理器中
    Admin user(name, password);
    adminManager.insertUnique(adminManager.end(), user);
    cout << "恭喜你!账号注册成功" << endl;
    return true;
}

// 管理员登录功能
bool AdminManager::Login() {
    string name, password;
    // 提示用户输入用户名
    cout << "请输入您的用户名: ";
    cin >> name;
    // 提示用户输入密码
    cout << "请输入您的密码: ";
    cin >> password;

    // 查找输入的用户名是否存在
    auto it = adminManager.find(name);
    if (it == adminManager.end()) {  // 用户名不存在
        cout << "用户名或密码不正确!请重试" << endl;
        return false;
    }

    // 检查输入的密码是否与注册的密码匹配
    if ((*it).GetAdminPassword() != password) {  // 密码不匹配
        cout << "用户名或密码不正确!请重试" << endl;
        return false;
    }

    cout << "登录成功!" << endl;
    return true;
}

// 保存管理员数据到文件
void AdminManager::Save(string filePath, string fileType) {
    // 打开临时文件进行数据写入，使用追加模式
    ofstream out;
    out.open(filePath + ".temp", ios::app);  // 打开文件以追加模式写入数据
    if (!out.is_open()) {  // 如果文件无法打开，输出错误信息
        cout << "无法打开文件!请重试!" << endl;
        return;
    }

    // 遍历 adminManager 容器，将每个管理员数据写入临时文件
    auto it = adminManager.begin();
    while (it != adminManager.end()) {
        out << *(it++) << endl;  // 写入管理员数据并移动到下一个元素
    }

    // 关闭输出流
    out.close();

    // 删除原文件并将临时文件重命名为原文件名
    remove((filePath + fileType).c_str());  // 删除原文件
    rename((filePath + ".temp").c_str(), (filePath + fileType).c_str());  // 将临时文件重命名为原文件名
}
