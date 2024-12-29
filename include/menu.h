#ifndef LIBRARYMANAGEMENT_MENU_H
#define LIBRARYMANAGEMENT_MENU_H

// Menu 类
// 负责管理图书馆系统的菜单显示与交互逻辑
class Menu {
public:
    // 初始菜单
    static void Start();

    // 主菜单
    // 显示系统的主要功能选项，例如查找图书、更新状态、删除图书等
    static void Main();

    // 查找菜单
    // 提供查找功能的选项界面，例如按编号查找或按状态筛选
    static void Find();

    // 更新菜单
    // 提供图书信息更新选项，例如修改图书借阅状态或其他信息
    static void Update();

    // 删除菜单
    // 提供图书删除选项，允许用户删除指定编号或条件的图书
    static void Remove();

    // 借出或归还菜单
    // 提供与图书借阅或归还相关的操作选项
    static void LendOrReturn();
};


#endif //LIBRARYMANAGEMENT_MENU_H
