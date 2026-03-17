#include <iostream>                                                 // 输入输出流：cin, cout, getline
#include <fstream>                                                  // 文件流：ifstream, ofstream
#include <cstring>                                                  // C字符串函数：strcmp, strcpy, strstr, strchr
#include <algorithm>                                                // 算法库：std::sort
#include <cctype>                                                   // 字符函数：isdigit()
#include <iomanip>                                                  // I/O格式化：setw()
#ifdef _WIN32
#include <windows.h>                                                // Windows API：SetConsoleOutputCP/SetConsoleCP
#endif
using namespace std;

const int MAX = 1000, LEN = 50;                                     // MAX: 最大联系人数, LEN: 字符串长度

struct Person {                                                     // 联系人结构体
    char Name[LEN], Job[LEN], Title[LEN], Phone[20], Email[100], Type[20];
};                                                                  // Name:姓名 Job:职务 Title:职称 Phone:手机 Email:邮箱 Type:分类

class Book {                                                        // 通讯录类
    Person data[MAX];                                               // 联系人数组
    int count = 0;                                                  // 当前联系人数量

    void clearBuf() { while(cin.get()!='\n'); }                     // 清空输入缓冲区
    
    void input(char *s, int n, const char *p) { cout << p; cin.getline(s, n); }  // 带提示的字符串输入
    
    bool validPhone(const char *p) {                                // 验证手机号：7-15位数字，允许空格、横线、加号
        int d = 0;
        for(; *p; p++) if(isdigit(*p)) d++; else if(!strchr(" -+", *p)) return false;
        return d >= 7 && d <= 15;
    }
    
    bool validEmail(const char *e) {                                // 验证邮箱：必须含@和.且.在@后，或为空
        bool at = false, dot = false;
        for(; *e; e++) { if(*e=='@') at=true; if(at && *e=='.') dot=true; }
        return !*e || (at && dot);
    }
    
    int find(const char *name) {                                    // 按姓名查找，返回索引或-1
        for(int i = 0; i < count; i++) if(!strcmp(data[i].Name, name)) return i;
        return -1;
    }
    
    void classify(Person &p) {                                      // AI智能分类：根据关键字自动分类
        if(strstr(p.Name,"爸")||strstr(p.Name,"妈")||strstr(p.Name,"哥")||strstr(p.Name,"姐")||
           strstr(p.Name,"弟")||strstr(p.Name,"妹")||strstr(p.Name,"爷")||strstr(p.Name,"奶")||
           strstr(p.Name,"伯")||strstr(p.Name,"叔")||strstr(p.Name,"姑")||strstr(p.Name,"舅")||
           strstr(p.Name,"姨")||strstr(p.Name,"婶")||strstr(p.Name,"婆")||strstr(p.Name,"公")||
           strstr(p.Name,"儿")||strstr(p.Name,"女"))
            { strcpy(p.Type, "家人"); return; }                     // 家人：姓名含亲属称呼
        if(strstr(p.Job,"经理")||strstr(p.Job,"主管")||strstr(p.Job,"总监")||strstr(p.Job,"工程")||
           strstr(p.Job,"职员")||strstr(p.Job,"秘书")||strstr(p.Job,"助理")||strstr(p.Job,"会计")||
           strstr(p.Job,"主任")||strstr(p.Job,"科长")||strstr(p.Job,"处长")||strstr(p.Job,"局长")||
           strstr(p.Title,"架构")||strstr(p.Title,"职员")||strstr(p.Title,"秘书")||strstr(p.Title,"助理"))
            { strcpy(p.Type, "同事"); return; }                     // 同事：职务/职称含职场称谓
        if(strstr(p.Job,"校长")||strstr(p.Job,"院长")||strstr(p.Job,"老师")||strstr(p.Job,"教授")||strstr(p.Job,"教员")||strstr(p.Job,"讲师")||
           strstr(p.Job,"导师")||strstr(p.Job,"辅导员")||strstr(p.Job,"教练"))
            { strcpy(p.Type, "老师"); return; }                     // 老师：职务含教育相关称谓
        if(strstr(p.Job,"班长")||strstr(p.Job,"委员")||strstr(p.Job,"学生")||strstr(p.Job,"学长")||strstr(p.Job,"学弟")||
           strstr(p.Job,"学妹")||strstr(p.Job,"室友")||strstr(p.Job,"校友"))
            { strcpy(p.Type, "同学"); return; }                     // 同学：职务含学校相关称谓
        strcpy(p.Type, "朋友");                                     // 默认分类为朋友
    }
    
    void showOne(Person &p, int i) {                                // 显示单个联系人详情
        cout << "\n--- 联系人 #" << i+1 << " ---\n"
             << "姓名: " << (*p.Name?p.Name:"无") << "\n职务: " << (*p.Job?p.Job:"无")
             << "\n职称: " << (*p.Title?p.Title:"无") << "\n手机: " << (*p.Phone?p.Phone:"无")
             << "\nEmail: " << (*p.Email?p.Email:"无") << "\n分类: " << p.Type << "\n------------------\n";
    }
    
    void showTable() {                                              // 表格形式显示所有联系人
        cout << "\n+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n"
             << "| 序号 |      姓名      |      职务      |      职称      |       手机       |             Email            |    分类    |\n"
             << "+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n";
        for(int i = 0; i < count; i++) {
            Person &p = data[i];
            cout << "| " << setw(4) << i+1 << " | " << setw(14) << (*p.Name?p.Name:"无") << " | "
                 << setw(14) << (*p.Job?p.Job:"无") << " | " << setw(14) << (*p.Title?p.Title:"无") << " | "
                 << setw(16) << (*p.Phone?p.Phone:"无") << " | " << setw(28) << (*p.Email?p.Email:"无")
                 << " | " << setw(10) << p.Type << " |\n";          // setw(): 设置输出宽度
        }
        cout << "+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n";
    }

public:
    int getCount() { return count; }                                // 获取联系人数量
    
    void load() {                                                   // 从文件加载联系人数据
        ifstream f("contacts.dat", ios::binary);                    // 以二进制模式打开
        if(!f) return;
        f.read((char*)&count, sizeof(int));                         // 先读取数量
        if(count > MAX) count = MAX;
        f.read((char*)data, sizeof(Person) * count);                // 再读取数据
    }
    
    void save() {                                                   // 保存联系人数据到文件
        ofstream f("contacts.dat", ios::binary);
        f.write((char*)&count, sizeof(int));                        // 先写入数量
        f.write((char*)data, sizeof(Person) * count);               // 再写入数据
    }
    
    void add() {                                                    // 添加联系人
        char c;
        do {
            cout << "\n========== 添加联系人 ==========\n";
            if(count >= MAX) { cout << "已满！\n"; clearBuf(); return; }
            Person p = {};                                          // 零初始化结构体
            do { input(p.Name, LEN, "姓名(0返回): ");
                if(!strcmp(p.Name, "0")) return;
            } while(!*p.Name || (find(p.Name) >= 0 && cout << "无效或已存在！\n"));
            input(p.Job, LEN, "职务: "); input(p.Title, LEN, "职称: ");
            do input(p.Phone, 20, "手机: "); while(!validPhone(p.Phone) && cout << "格式错误！\n");
            do input(p.Email, 100, "Email: "); while(!validEmail(p.Email) && cout << "格式错误！\n");
            classify(p);                                            // 自动分类
            data[count++] = p;
            cout << "成功！分类: " << p.Type << "\n继续？(Y/N): ";
            cin >> c; clearBuf();
        } while(c == 'Y' || c == 'y');
    }
    
    void show() {                                                   // 显示所有联系人
        cout << "\n========== 所有联系人(" << count << "人) ==========\n";
        if(count) showTable(); else cout << "空！\n";
        cout << "按回车返回..."; clearBuf();
    }
    
    void del() {                                                    // 删除联系人
        int opt;
        while(1) {
            cout << "\n========== 删除联系人 ==========\n1.删除单个 2.清空 0.返回\n选择: ";
            cin >> opt; clearBuf();
            if(opt == 0) return;
            if(opt == 1) {
                if(!count) { cout << "空！\n"; clearBuf(); continue; }
                char name[LEN]; input(name, LEN, "姓名(0返回): ");
                if(!strcmp(name, "0")) continue;
                int i = find(name);
                if(i < 0) { cout << "未找到！\n"; clearBuf(); continue; }
                showOne(data[i], i);
                cout << "确认？(Y/N): "; char c; cin >> c; clearBuf();
                if(c == 'Y' || c == 'y') {
                    for(int j = i; j < count-1; j++) data[j] = data[j+1];  // 删除：后面元素前移
                    count--; cout << "已删除！\n";
                }
                clearBuf();
            } else if(opt == 2) {
                cout << "确认清空？(Y/N): "; char c; cin >> c; clearBuf();
                if(c == 'Y' || c == 'y') { count = 0; cout << "已清空！\n"; }
                clearBuf();
            }
        }
    }
    
    void modify() {                                                 // 修改联系人
        if(!count) { cout << "\n空！\n"; clearBuf(); return; }
        cout << "\n========== 修改联系人 ==========\n";
        char name[LEN]; input(name, LEN, "姓名(0返回): ");
        if(!strcmp(name, "0")) return;
        int i = find(name);
        if(i < 0) { cout << "未找到！\n"; clearBuf(); return; }
        Person &p = data[i]; showOne(p, i);                         // 引用，直接修改原数据
        cout << "\n新信息(回车默认不修改):\n";
        char buf[100];
        input(buf, LEN, "新姓名: "); if(*buf) strcpy(p.Name, buf);
        input(buf, LEN, "新职务: "); if(*buf) strcpy(p.Job, buf);
        input(buf, LEN, "新职称: "); if(*buf) strcpy(p.Title, buf);
        input(buf, 20, "新手机: "); if(*buf && validPhone(buf)) strcpy(p.Phone, buf);
        input(buf, 100, "新Email: "); if(*buf && validEmail(buf)) strcpy(p.Email, buf);
        classify(p);                                                // 重新分类
        cout << "成功！分类: " << p.Type << "\n按回车返回..."; clearBuf();
    }
    
    void search() {                                                 // 智能查找：多字段模糊搜索
        if(!count) { cout << "\n空！\n"; clearBuf(); return; }
        cout << "\n========== 智能查找 ==========\n";
        char key[100]; input(key, 100, "关键词(0返回): ");
        if(!strcmp(key, "0")) return;
        int n = 0;
        for(int i = 0; i < count; i++) {
            Person &p = data[i];
            if(strstr(p.Name,key)||strstr(p.Job,key)||strstr(p.Title,key)||  // strstr(): 查找子串
               strstr(p.Phone,key)||strstr(p.Email,key)||strstr(p.Type,key))
                { showOne(p, i); n++; }
        }
        cout << (n ? "共" + to_string(n) + "个。\n" : "未找到！\n");  // to_string(): 数字转字符串
        cout << "按回车返回..."; clearBuf();
    }
    
    void sort() {                                                   // 按姓名排序
        cout << "\n========== 按姓名排序 ==========\n";
        if(count > 1) {
            std::sort(data, data+count, [](Person &a, Person &b){ return strcmp(a.Name, b.Name) < 0; });  // Lambda表达式
            showTable();
        } else cout << "数量不足！\n";
        cout << "按回车返回..."; clearBuf();
    }
    
    void ai() {                                                     // 智能分类：重新分类并按分类排序
        cout << "\n========== 智能分类 ==========\n";
        if(!count) { cout << "空！\n"; clearBuf(); return; }
        for(int i = 0; i < count; i++) classify(data[i]);
        std::sort(data, data+count, [](Person &a, Person &b){ return strcmp(a.Type, b.Type) < 0; });
        showTable();
        cout << "按回车返回..."; clearBuf();
    }
};

void menu() {                                                       // 显示主菜单
    cout << "\n========================================\n=                                      =\n"
         << "=          通 讯 录 管 理 系 统        =\n=                                      =\n"
         << "========================================\n=                                      =\n"
         << "=         1. 添加联系人                =\n=         2. 删除联系人                =\n"
         << "=         3. 修改联系人                =\n=         4. 显示联系人                =\n"
         << "=         5. 按姓名排序                =\n=         6. 智能查找                  =\n"
         << "=         7. 智能分类                  =\n=         8. 保存文件                  =\n"
         << "=         0. 退出系统                  =\n=                                      =\n"
         << "========================================\n请选择(0-8): ";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(936); SetConsoleCP(936);                     // Windows下设置GBK编码解决中文乱码
#endif
    Book book;
    cout << "\n欢迎使用通讯录管理系统！\n";
    book.load();                                                    // 启动时加载文件
    if(book.getCount()) cout << "已加载" << book.getCount() << "个联系人。\n";
    else cout << "创建新通讯录。\n";
    
    int c;
    while(1) {
        menu();
        if(!(cin >> c)) { cout << "无效！\n"; while(cin.get()!='\n'); continue; }
        while(cin.get()!='\n');                                     // 清除选择后的换行符
        switch(c) {
            case 1: book.add(); break;
            case 2: book.del(); break;
            case 3: book.modify(); break;
            case 4: book.show(); break;
            case 5: book.sort(); break;
            case 6: book.search(); break;
            case 7: book.ai(); break;
            case 8: cout << "\n保存中...\n"; book.save(); cout << "完成！\n按回车返回..."; while(cin.get()!='\n'); break;
            case 0:
                cout << "保存退出？(Y/N): "; char y; cin >> y;
                if(y == 'Y' || y == 'y') book.save();
                cout << "再见！\n"; return 0;
            default: cout << "无效！\n";
        }
    }
}