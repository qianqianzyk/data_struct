#include "bookManager.h"
#include <iostream>
#include <fstream>
#include "rbTree.h"
using namespace std;

// 使用构造函数
BookManager::BookManager()  : currentMaxId(0) {
    LoadMaxId("../data/book_max_id.txt");
}

// 使用析构函数
BookManager::~BookManager() {
    UpdateMaxId("../data/book_max_id.txt");
}

// 初始化图书数据
void BookManager::Init(string path,string fileType) {
    // 拼接文件路径
    string file = path + fileType;

    ifstream in;
    // 尝试以读取模式打开文件
    in.open(file, ios::in);
    // 如果文件无法打开
    if (!in.is_open()) {
        // 如果文件不存在，则创建文件并打开
        ofstream out;
        out.open(file, ios::out | ios::app); // 以追加模式打开
        out.close();  // 创建文件后立即关闭
        // 再次尝试以读取模式打开文件
        in.open(file, ios::in);
    }

    Book book;
    // 读取文件内容，直到文件末尾
    while (in.peek() != EOF) {  // 使用 peek 检查文件末尾
        in >> book;  // 通过流读取每个 Book 对象
        libraryManager.insertUnique(libraryManager.end(), book);  // 插入到图书管理容器中
    }

    // 关闭文件
    in.close();
}

// 加载图书最大ID
void BookManager::LoadMaxId(string filePath) {
    ifstream in(filePath);
    if (in.is_open()) {
        in >> currentMaxId;  // 如果文件存在，读取最大ID
        in.close();
    } else {
        // 如果文件不存在，创建文件并将 currentMaxId 初始化为 0
        ofstream out(filePath);
        if (out.is_open()) {
            out << 0;  // 初始化最大ID为0
            out.close();
        }
        currentMaxId = 0;  // 初始化 currentMaxId 为 0
    }
}

// 更新图书最大ID
void BookManager::UpdateMaxId(string filePath) {
    ofstream out(filePath, ios::trunc);
    if (out.is_open()) {
        out << currentMaxId;
        out.close();
    }
}

// 添加书籍到图书馆
void BookManager::Insert() {
    string ISBN, name, author, publisher;
    int year, count;

    cout << "请输入ISBN编号: ";
    cin >> ISBN;

    cout << "请输入图书名称: ";
    cin >> name;

    cout << "请输入作者: ";
    cin >> author;

    cout << "请输入出版社: ";
    cin >> publisher;

    cout << "请输入出版年份: ";
    cin >> year;

    cout << "请输入图书数量: ";
    cin >> count;

    cout << "请确认图书信息（输入y/yes确认）: " << endl
         << "ISBN: " << ISBN << "  图书名称: " << name
         << "  作者: " << author << "  出版社: " << publisher
         << "  出版年份: " << year << "  数量: " << count << "\n> ";

    string confirm;
    cin.get();  // 清除上一次输入的换行符
    getline(cin, confirm);  // 获取用户的确认输入

    // 如果用户确认，开始添加图书
    if (confirm == "y" || confirm == "yes") {
        // 根据数量逐个添加图书
        while (count--) {
            libraryManager.insertUnique(libraryManager.end(),
                                        Book(currentMaxId + 1, ISBN, name,
                                             author, publisher, year, false, ""));
            currentMaxId++;
        }
        cout << "添加成功" << endl;
    } else {
        cout << "添加已取消" << endl;
    }

    cout << "是否继续添加？（输入y/yes继续;输入n/no取消）\n> ";
    getline(cin, confirm);

    if (confirm == "y" || confirm == "yes") {
        Insert();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 检查图书馆是否为空
bool BookManager::Empty() {
    return libraryManager.empty();
}

// 分页查询所有书籍，currPage 为当前页码，pageSize 为每页显示数量
void BookManager::FindByPage(int currPage, int pageSize) {
    size_t size = libraryManager.size(); // 获取图书总数
    size_t totalPages = (size - 1) / pageSize + 1; // 计算总页数

    // 检查输入的页码是否合法
    if (currPage < 0 || currPage > totalPages) {
        cout << "输入的页码不正确，请重新输入（输入0退出）\n> ";
        if (cin >> currPage && currPage) { // 如果输入合法且不为0，递归调用
            FindByPage(currPage, pageSize);
        }
        return; // 退出当前调用
    }

    // 计算当前页的起始索引
    int firstIndex = (currPage - 1) * pageSize;
    auto it = libraryManager.begin();
    while (firstIndex--) {
        ++it; // 将迭代器移动到当前页的起始位置
    }

    // 输出当前页的图书信息
    cout << "-------------------------------" << endl;
    int n = pageSize;
    while (n-- && it != libraryManager.end()) {
        cout << *(it++) << endl;
    }
    cout << "-------------------------------" << endl;

    // 提示用户当前所在页，并允许跳转到其他页
    cout << "当前为第 " << currPage << " 页，共 " << totalPages
         << " 页，请输入跳转页码（输入0退出）\n> ";
    if (cin >> currPage && currPage) {
        FindByPage(currPage, pageSize); // 递归调用以跳转到指定页码
    }
}

// 根据书籍编号查找书籍
void BookManager::FindByID() {
    int id;
    cout << "请输入要查找的图书ID: "; // 提示用户输入图书ID
    cin >> id;

    auto it = libraryManager.find(id); // 根据ID查找图书
    if (it != libraryManager.end()) {
        cout << *it << endl; // 如果找到，输出图书信息
    } else {
        cout << "该图书ID不存在" << endl; // 如果未找到，提示用户
    }

    // 提示用户是否继续查找
    cout << "是否继续查找？（输入y/yes继续）\n> ";
    string confirm;
    cin.get(); // 清除上一次输入的换行符
    getline(cin, confirm); // 获取用户确认输入
    if (confirm == "y" || confirm == "yes") {
        FindByID(); // 递归调用以继续查找
    }
}

// 根据 ISBN 号查找书籍
void BookManager::FindByISBN() {
    string ISBN;
    cout << "请输入要查询的ISBN号：";
    cin >> ISBN;

    bool find = false; // 表示是否找到书籍
    int inCount = 0, outCount = 0; // 记录馆内和借出的书籍数量
    auto it = libraryManager.begin(); // 初始化迭代器
    while (it != libraryManager.end()) {
        if (it->GetISBN() == ISBN) { // 如果找到ISBN匹配的书籍
            if (!find) { // 如果是第一次找到，输出书籍信息
                cout << "ISBN: " << it->GetISBN()
                     << "  书名: " << it->GetName()
                     << "  作者: " << it->GetAuthor()
                     << "  出版社: " << it->GetPublisher()
                     << "  出版年份: " << it->GetYear() << endl;
                find = true; // 标记为已经找到
            }
            if (it->GetBorrowStatus()) { // 如果书籍已经被借出
                cout << "书籍ID: " << it->GetId()
                     << "  借阅者: " << it->GetBorrower() << endl;
                ++outCount; // 借出的书籍数量加1
            } else {
                ++inCount; // 馆内书籍数量加1
            }
        }
        ++it; // 移动到下一本书
    }
    if (find) {
        cout << "共 " << inCount + outCount << " 本书  ";
        if (outCount) {
            cout << "其中借出: " << outCount << " 本书  ";
        }
        cout << "馆内: " << inCount << " 本书" << endl;
    } else {
        cout << "没有找到该ISBN号的书籍" << endl;
    }

    // 是否继续查询
    cout << "是否继续查询？（输入y/yes继续;输入n/no取消）\n> ";
    string confirm;
    cin.get(); // 读取多余的换行符
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        FindByISBN();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 根据书籍编号更新书籍信息
void BookManager::UpdateByID() {
    int id;
    cout << "请输入要更新的书籍ID：";
    cin >> id;

    auto it = libraryManager.find(id); // 查找指定ID的书籍
    if (it != libraryManager.end()) { // 如果找到该书籍
        cout << "原书籍信息：" << endl
             << "ISBN: " << it->GetISBN() << "  书名: " << it->GetName()
             << "  作者: " << it->GetAuthor()
             << "  出版社: " << it->GetPublisher()
             << "  出版年份: " << it->GetYear() << endl
             << "是否更新？（输入y/yes确认更新）\n> ";
        string confirm;
        cin.get(); // 读取多余的换行符
        getline(cin, confirm); // 获取用户输入
        if (confirm == "y" || confirm == "yes") { // 用户确认更新
            string updateISBN, updateName, updateAuthor, updatePublisher;
            int updateYear;

            // 输入更新的内容，如果输入"0"则不更新
            cout << "请输入更新后的ISBN号（输入0表示不更新）：";
            cin >> updateISBN;
            if (updateISBN == "0") {
                updateISBN = it->GetISBN(); // 保持原ISBN
            }
            cout << "请输入更新后的书名（输入0表示不更新）：";
            cin >> updateName;
            if (updateName == "0") {
                updateName = it->GetName(); // 保持原书名
            }
            cout << "请输入更新后的作者（输入0表示不更新）：";
            cin >> updateAuthor;
            if (updateAuthor == "0") {
                updateAuthor = it->GetAuthor(); // 保持原作者
            }
            cout << "请输入更新后的出版社（输入0表示不更新）：";
            cin >> updatePublisher;
            if (updatePublisher == "0") {
                updatePublisher = it->GetPublisher(); // 保持原出版社
            }
            cout << "请输入更新后的出版年份（输入0表示不更新）：";
            cin >> updateYear;
            if (updateYear == 0) {
                updateYear = it->GetYear(); // 保持原出版年份
            }

            // 输出更新后的书籍信息，确认是否更新
            cout << "请确认更新后的书籍信息（输入y/yes确认）： " << endl
                 << "ISBN: " << updateISBN << "  书名: " << updateName
                 << "  作者: " << updateAuthor
                 << "  出版社: " << updatePublisher
                 << "  出版年份: " << updateYear << "\n> ";
            cin.get(); // 读取多余的换行符
            getline(cin, confirm); // 获取用户输入
            if (confirm == "y" || confirm == "yes") { // 确认更新
                // 设置更新后的书籍信息
                it->SetISBN(updateISBN);
                it->SetName(updateName);
                it->SetAuthor(updateAuthor);
                it->SetPublisher(updatePublisher);
                it->SetYear(updateYear);
                cout << "成功更新" << endl;
            } else {
                cout << "更新已取消" << endl;
            }
        } else {
            cout << "更新已取消" << endl;
        }
    } else {
        cout << "没有找到该书籍ID" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续更新
    cout << "是否继续更新？（输入y/yes继续;输入n/no取消）\n> ";
    string confirm;
    getline(cin, confirm); // 获取用户输入
    if (confirm == "y" || confirm == "yes") {
        UpdateByID();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 根据 ISBN 号更新书籍信息
void BookManager::UpdateByISBN() {
    string ISBN;
    cout << "请输入要更新的书籍ISBN号：";
    cin >> ISBN;

    bool find = false; // 用于标记是否找到匹配的书籍
    auto it = libraryManager.begin(); // 初始化迭代器，指向库中的第一个元素
    while (it != libraryManager.end()) { // 遍历库中的书籍
        if (it->GetISBN() == ISBN) { // 如果找到匹配的ISBN
            find = true; // 标记为找到
            break; // 退出循环
        }
        ++it; // 继续查找下一本书
    }
    if (find) { // 如果找到书籍
        cout << "原书籍信息：" << endl
             << "ISBN: " << it->GetISBN() << "  书名: " << it->GetName()
             << "  作者: " << it->GetAuthor()
             << "  出版社: " << it->GetPublisher()
             << "  出版年份: " << it->GetYear() << endl
             << "是否更新？（输入y/yes确认更新）\n> ";
        string confirm;
        cin.get(); // 读取多余的换行符
        getline(cin, confirm); // 获取用户输入
        if (confirm == "y" || confirm == "yes") { // 用户确认更新
            string updateISBN, updateName, updateAuthor, updatePublisher;
            int updateYear;

            // 提示用户输入更新的内容，如果输入"0"则不更新该项
            cout << "请输入更新后的ISBN号（输入0表示不更新）：";
            cin >> updateISBN;
            if (updateISBN == "0") {
                updateISBN = it->GetISBN(); // 保持原ISBN
            }
            cout << "请输入更新后的书名（输入0表示不更新）：";
            cin >> updateName;
            if (updateName == "0") {
                updateName = it->GetName(); // 保持原书名
            }
            cout << "请输入更新后的作者（输入0表示不更新）：";
            cin >> updateAuthor;
            if (updateAuthor == "0") {
                updateAuthor = it->GetAuthor(); // 保持原作者
            }
            cout << "请输入更新后的出版社（输入0表示不更新）：";
            cin >> updatePublisher;
            if (updatePublisher == "0") {
                updatePublisher = it->GetPublisher(); // 保持原出版社
            }
            cout << "请输入更新后的出版年份（输入0表示不更新）：";
            cin >> updateYear;
            if (updateYear == 0) {
                updateYear = it->GetYear(); // 保持原出版年份
            }

            // 显示更新后的书籍信息，确认是否更新
            cout << "请确认更新后的书籍信息（输入y/yes确认）：" << endl
                 << "ISBN: " << updateISBN << "  书名: " << updateName
                 << "  作者: " << updateAuthor
                 << "  出版社: " << updatePublisher
                 << "  出版年份: " << updateYear << "\n> ";
            cin.get(); // 读取多余的换行符
            getline(cin, confirm); // 获取用户输入

            if (confirm == "y" || confirm == "yes") { // 如果用户确认更新
                // 更新书籍信息
                while (it != libraryManager.end()) {
                    if (it->GetISBN() == ISBN) { // 确认找到该书籍
                        it->SetISBN(updateISBN);
                        it->SetName(updateName);
                        it->SetAuthor(updateAuthor);
                        it->SetPublisher(updatePublisher);
                        it->SetYear(updateYear);
                    }
                    ++it; // 移动到下一本书
                }
                cout << "成功更新" << endl;
            } else {
                cout << "更新已取消" << endl;
            }
        } else {
            cout << "更新已取消" << endl;
        }
    } else {
        cout << "没有找到该ISBN号的书籍" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续更新
    cout << "是否继续更新？（输入y/yes继续;输入n/no取消）\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        UpdateByISBN();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 根据书籍编号删除书籍
void BookManager::RemoveByID() {
    int id;
    cout << "请输入要删除的书籍ID：";
    cin >> id;

    auto it = libraryManager.find(id); // 查找指定ID的书籍
    if (it != libraryManager.end()) { // 如果找到该书籍
        cout << "书籍信息：" << endl
             << *it << endl
             << "是否删除？（输入y/yes确认删除）\n> ";
        string confirm;
        cin.get(); // 读取多余的换行符
        getline(cin, confirm); // 获取用户输入
        if (confirm == "y" || confirm == "yes") { // 用户确认删除
            if (it->GetBorrowStatus()) { // 如果书籍已借出
                cout << "该书籍当前已被借出，借阅者: " << it->GetBorrower()
                     << "，确定要删除吗？（输入y/yes确认）\n> ";
                getline(cin, confirm); // 获取用户确认输入
                if (confirm == "y" || confirm == "yes") { // 用户确认删除
                    libraryManager.erase(it); // 删除书籍
                    cout << "成功删除" << endl;
                } else {
                    cout << "删除已取消" << endl;
                }
            } else {
                libraryManager.erase(it); // 如果书籍未借出，直接删除
                cout << "成功删除" << endl;
            }
        } else {
            cout << "删除已取消" << endl;
        }
    } else {
        cout << "没有找到该书籍ID" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续删除
    cout << "是否继续删除？（输入y/yes继续;输入n/no取消）\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        RemoveByID();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 根据 ISBN 号删除书籍
void BookManager::RemoveByISBN() {
    string ISBN;
    cout << "请输入要删除的书籍ISBN号：";
    cin >> ISBN;

    bool find = false; // 用于标记是否找到匹配的书籍
    auto it = libraryManager.begin(); // 初始化迭代器，指向库中的第一个元素
    while (it != libraryManager.end()) { // 遍历库中的书籍
        if (it->GetISBN() == ISBN) { // 如果找到匹配的ISBN
            find = true; // 标记为找到
            break; // 退出循环
        }
        ++it; // 继续查找下一本书
    }
    if (find) { // 如果找到书籍
        cout << "书籍信息：" << endl
             << "ISBN: " << it->GetISBN() << "  书名: " << it->GetName()
             << "  作者: " << it->GetAuthor()
             << "  出版社: " << it->GetPublisher()
             << "  出版年份: " << it->GetYear() << endl
             << "是否删除？（输入y/yes确认删除）\n> ";
        string confirm;
        cin.get(); // 读取多余的换行符
        getline(cin, confirm); // 获取用户输入
        if (confirm == "y" || confirm == "yes") { // 用户确认删除
            // 遍历所有书籍并删除与指定ISBN匹配的书籍
            while (it != libraryManager.end()) {
                auto itAfter = ++it; // 备份当前迭代器位置，用于后续操作
                --it; // 恢复迭代器到当前位置
                if (it->GetISBN() == ISBN) { // 如果找到匹配的书籍
                    if (it->GetBorrowStatus()) { // 如果书籍已借出
                        cout << "书籍ID: " << it->GetId()
                             << " 已被借出，借阅者: " << it->GetBorrower()
                             << "，是否删除？（输入y/yes确认删除）\n> ";
                        getline(cin, confirm); // 获取用户输入
                        if (confirm == "y" || confirm == "yes") { // 用户确认删除
                            libraryManager.erase(it); // 删除书籍
                        }
                    } else {
                        libraryManager.erase(it); // 如果书籍未借出，直接删除
                    }
                }
                it = itAfter; // 移动到下一本书
            }
            cout << "删除完成" << endl;
        } else {
            cout << "删除已取消" << endl;
        }
    } else {
        cout << "没有找到该ISBN号的书籍" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续删除
    cout << "是否继续删除？（输入y/yes继续;n/no取消）\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        RemoveByISBN();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 借出书籍操作
void BookManager::Lend() {
    int id;
    string borrower;

    cout << "请输入要借出的书籍ID：";
    cin >> id;
    auto it = libraryManager.find(id); // 查找指定ID的书籍
    if (it != libraryManager.end()) { // 如果找到该书籍
        if (it->GetBorrowStatus()) { // 如果书籍已经借出
            cout << "该书籍已经被借出" << endl;
            cin.get(); // 读取多余的换行符
        } else {
            // 如果书籍未借出，提示用户输入借阅者信息
            cout << "请输入借阅者姓名：";
            cin >> borrower;
            // 显示书籍的借出信息，确认借出
            cout << "请确认借出信息（输入y/yes确认）： " << endl
                 << "书籍ID: " << id << "  ISBN: " << it->GetISBN()
                 << "  书名: " << it->GetName()
                 << "  作者: " << it->GetAuthor()
                 << "  出版社: " << it->GetPublisher()
                 << "  出版年份: " << it->GetYear() << "  借阅者: " << borrower
                 << "\n> ";
            string confirm;
            cin.get(); // 读取多余的换行符
            getline(cin, confirm); // 获取用户输入
            if (confirm == "y" || confirm == "yes") { // 用户确认借出
                it->SetBorrowStatus(true); // 设置书籍为已借出
                it->SetBorrower(borrower); // 设置借阅者
                cout << "成功借出" << endl;
            } else {
                cout << "借出已取消" << endl;
            }
        }
    } else {
        cout << "没有找到该书籍ID" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续借出书籍
    cout << "是否继续借出？（输入y/yes继续;n/no取消）\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        Lend();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 归还书籍操作
void BookManager::Return() {
    int id;
    cout << "请输入要归还的书籍ID：";
    cin >> id;
    auto it = libraryManager.find(id); // 查找指定ID的书籍
    if (it != libraryManager.end()) { // 如果找到该书籍
        if (!it->GetBorrowStatus()) { // 如果书籍未被借出
            cout << "该书籍未借出" << endl;
            cin.get(); // 读取多余的换行符
        } else {
            // 如果书籍已借出，提示用户确认归还信息
            cout << "请确认归还信息（输入y/yes确认）： " << endl
                 << "书籍ID: " << id << "  ISBN: " << it->GetISBN()
                 << "  书名: " << it->GetName()
                 << "  作者: " << it->GetAuthor()
                 << "  出版社: " << it->GetPublisher()
                 << "  出版年份: " << it->GetYear()
                 << "  借阅者: " << it->GetBorrower() << "\n> ";
            string confirm;
            cin.get(); // 读取多余的换行符
            getline(cin, confirm); // 获取用户输入
            if (confirm == "y" || confirm == "yes") { // 用户确认归还
                it->SetBorrowStatus(false); // 设置书籍为未借出
                it->SetBorrower(""); // 清空借阅者信息
                cout << "成功归还" << endl;
            } else {
                cout << "归还已取消" << endl;
            }
        }
    } else {
        cout << "没有找到该书籍ID" << endl;
        cin.get(); // 读取多余的换行符
    }

    // 是否继续归还书籍
    cout << "是否继续归还？（输入y/yes继续;n/no取消）\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        Return();
    } else if (confirm == "n" || confirm == "no") {
        cout << "取消操作成功" << endl;
    } else {
        cout << "无效输入，请重新输入!" << endl;
    }
}

// 查询所有已借出的书籍
void BookManager::FindAllLend() {
    BookManager LendBook; // 创建一个新的 LibraryManager 对象来保存借出的书籍
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) { // 遍历所有书籍
        if (it->GetBorrowStatus()) { // 如果书籍已借出
            LendBook.libraryManager.insertUnique(LendBook.libraryManager.end(), *it); // 将借出的书籍加入到新的 LendBook 中
        }
        ++it; // 继续查找下一本书
    }
    if (LendBook.Empty()) { // 如果没有借出的书籍
        cout << "当前没有借出的书籍" << endl;
        return;
    }
    LendBook.FindByPage(1, 20); // 显示借出的书籍（分页显示，显示前20本）
}

// 保存图书数据到文件
void BookManager::Save(string filePath, string fileType) {
    // 打开一个临时文件用于追加写入数据
    ofstream out;
    out.open(filePath + ".temp", ios::app);  // 使用 ".temp" 作为临时文件名
    if (!out.is_open()) { // 如果文件打开失败，输出错误信息并返回
        cout << "无法打开文件!请重试!" << endl;
        return;
    }

    // 遍历所有图书并将数据写入文件
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        out << *(it++) << endl;  // 将每本图书的信息写入文件
    }

    out.close(); // 完成写入后关闭文件

    // 删除原文件，并将临时文件重命名为原文件
    remove((filePath + fileType).c_str());  // 删除原文件
    rename((filePath + ".temp").c_str(), (filePath + fileType).c_str());  // 将临时文件重命名为原文件名
}

void BookManager::TestRbTree() {

    // 插入一些节点
    // 创建四个 Book 对象，分别设置 id 和其他信息
    Book book1(2, "10", "Book Title 10", "Author 1", "Publisher 1", 2020, false, "");
    Book book2(3, "20", "Book Title 20", "Author 2", "Publisher 2", 2021, false, "");
    Book book3(6, "15", "Book Title 15", "Author 3", "Publisher 3", 2022, false, "");
    Book book4(7, "5", "Book Title 5", "Author 4", "Publisher 4", 2023, false, "");

    // 插入到图书馆的红黑树中
    libraryManager.insertUnique(book1);
    libraryManager.insertUnique(book2);
    libraryManager.insertUnique(book3);
    libraryManager.insertUnique(book4);

    // 输出初始树的中序遍历结果
    cout << "In-order traversal of the tree: ";
    libraryManager.inOrderTraversal(libraryManager.rootNode());
    cout << endl;

    // 删除最右节点
    libraryManager.removeRightmost();

    // 再次中序遍历树
    cout << "In-order traversal after removing the rightmost node: ";
    libraryManager.inOrderTraversal(libraryManager.rootNode());
    cout << endl;
}