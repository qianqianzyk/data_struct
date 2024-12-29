#include <iostream>
#include "../include/menu.h"
#include "../include/adminManager.h"
#include "../include/bookManager.h"
using namespace std;

int main()
{
    AdminManager admin;
    BookManager book;
    admin.Init("../data/admin",".txt");
    book.Init("../data/book",".txt");

    Menu::Start();
    int choice;
    while(cin >> choice && choice){
        switch (choice) {
            // 登录
            case 1:{
                if(!admin.Login())
                {
                    break;
                }

                Menu::Main();
                while(cin >> choice && choice){
                    switch(choice){
                        case 1:{
                            // 添加图书
                            book.Insert();
                            break;
                        }
                        case 2:{
                            // 查找图书
                            if(book.Empty()){
                                cout << "当前暂无图书!" << endl;
                                break;
                            }

                            Menu::Find();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        // 按书号查找
                                        book.FindByID();
                                        break;
                                    }
                                    case 2: {
                                        // 按ISBN查找
                                        book.FindByISBN();
                                        break;
                                    }
                                    case 3: {
                                        // 查看所有图书
                                        book.FindByPage(1, 20);
                                        break;
                                    }
                                    default: {
                                        cout << "非法输入，请重试!" << endl;
                                        break;
                                    }
                                }
                                Menu::Find();
                            }
                            break;
                        }
                        case 3:{
                            // 修改图书信息
                            if(book.Empty()){
                                cout << "当前暂无图书!" << endl;
                                break;
                            }

                            Menu::Update();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        // 按书号修改
                                        book.UpdateByID();
                                        break;
                                    }
                                    case 2: {
                                        // 按ISBN修改
                                        book.UpdateByISBN();
                                        break;
                                    }
                                    default: {
                                        cout << "非法输入，请重试!" << endl;
                                        break;
                                    }
                                }
                                Menu::Update();
                            }
                            break;
                        }
                        case 4: {
                            // 删除图书
                            if (book.Empty()) {
                                cout << "当前暂无图书!" << endl;
                                break;
                            }

                            Menu::Remove();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        // 按书号删除
                                        book.RemoveByID();
                                        break;
                                    }
                                    case 2: {
                                        // 按ISBN删除
                                        book.RemoveByISBN();
                                        break;
                                    }
                                    default: {
                                        cout << "非法输入，请重试!" << endl;
                                        break;
                                    }
                                }
                                Menu::Remove();
                            }
                            break;
                        }
                        case 5: {
                            // 借阅管理
                            Menu::LendOrReturn();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        // 借出图书
                                        book.Lend();
                                        break;
                                    }
                                    case 2: {
                                        // 归还图书
                                        book.Return();
                                        break;
                                    }
                                    case 3: {
                                        // 查看所有已借出图书
                                        book.FindAllLend();
                                        break;
                                    }
                                    default: {
                                        cout << "非法输入，请重试!" << endl;
                                        break;
                                    }
                                }
                                Menu::LendOrReturn();
                            }
                            break;
                        }
                        default: {
                            cout << "非法输入，请重试!" << endl;
                            break;
                        }
                    }
                    Menu::Main();
                }
                break;
            }
            // 注册
            case 2: {
                admin.Register();
                break;
            }
            // 测试代码
            case 3: {
                book.TestRbTree();
                break;
            }
            // 退出
            default:{
                cout << "非法输入，请重试!"<< endl;
                break;
            }
        }
        Menu::Start();
    }

    admin.Save("../data/admin",".txt");
    book.Save("../data/book",".txt");
    cout << "欢迎下次再来!" << endl;

    return 0;
}