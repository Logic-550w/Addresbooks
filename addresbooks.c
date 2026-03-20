#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 1000           // 最大联系人数量
#define STR_LEN 50                  // 字符串最大长度

typedef struct {
    char Name[STR_LEN], Job[STR_LEN], Title[STR_LEN];
    char Phone[20], Email[100], Type[20];
} Person;                           // 单个联系人结构体

typedef struct {
    Person Data[MAX_CONTACTS];
    int Count;
} Book;                              // 通讯录结构体

void ClearBuffer() { while(getchar()!='\n'); }      // 清空输入缓冲区
//安全输入字符串，避免缓冲区溢出
int InputString(char *str, int len, const char *prompt) {
    printf("%s", prompt);        // 输出提示信息
    if(!fgets(str, len, stdin)) return 0;      // 读取输入字符串    
    str[strcspn(str, "\n")] = 0;      // 移除换行符
    return 1;
}
// 验证手机号是否有效  (只允许数字、空格、连字符 -、加号 + ; 数字总数必须在 7 到 15 之间)
int ValidPhone(const char *phone) {
    int digitCount = 0;
    for(; *phone; phone++) 
        if(isdigit(*phone)) digitCount++; 
        else if(!strchr(" -+", *phone)) return 0;
    return digitCount >= 7 && digitCount <= 15;
}
// 验证邮箱是否有效  (必须包含 @ 字符和 . 字符)
int ValidEmail(const char *email) {
    int hasAt = 0, hasDot = 0;
    for(; *email; email++) { 
        if(*email == '@') hasAt = 1; 
        if(hasAt && *email == '.') hasDot = 1; 
    }
    return !*email || (hasAt && hasDot);        // 返回邮箱是否有效结果
}
// 根据姓名查找联系人索引  (如果找到则返回索引，否则返回 -1)
int FindByName(Book *book, const char *name) {
    for(int idx = 0; idx < book->Count; idx++)
        if(!strcmp(book->Data[idx].Name, name)) return idx;
    return -1;
}
// 对联系人进行分类  (根据姓名、职务、职称、手机、Email 进行分类  strstr 函数用于查找子字符串)
void Classify(Person *person) {
    if(strstr(person->Name,"爸")||strstr(person->Name,"妈")||strstr(person->Name,"哥")||strstr(person->Name,"姐")||
       strstr(person->Name,"弟")||strstr(person->Name,"妹")||strstr(person->Name,"爷")||strstr(person->Name,"奶")||
       strstr(person->Name,"父")||strstr(person->Name,"母")||strstr(person->Name,"舅")||strstr(person->Name,"姑姑")) {
        strcpy(person->Type, "家人"); return; }
    if(strstr(person->Job,"经理")||strstr(person->Job,"主管")||strstr(person->Job,"总监")||strstr(person->Job,"工程")||
       strstr(person->Title,"工程")||strstr(person->Title,"架构")||strstr(person->Title,"设计")) {
        strcpy(person->Type, "同事"); return; }
    if(strstr(person->Job,"老师")||strstr(person->Job,"教授")||strstr(person->Title,"老师")||strstr(person->Title,"教授")) {
        strcpy(person->Type, "老师"); return; }
    if(strstr(person->Job,"班长")||strstr(person->Job,"委员")||strstr(person->Job,"学生")||strstr(person->Title,"学生")) {
        strcpy(person->Type, "同学"); return; }
    strcpy(person->Type, "朋友");
}
// 显示单个联系人信息  (包含姓名、职务、职称、手机、Email、分类)
void ShowOne(Person *person, int index) {
    printf("\n--- 联系人 #%d ---\n姓名: %s\n职务: %s\n职称: %s\n手机: %s\nEmail: %s\n分类: %s\n------------------\n",
           index+1, person->Name, *person->Job? person->Job:"无", *person->Title? person->Title:"无",
           *person->Phone? person->Phone:"无", *person->Email? person->Email:"无", person->Type);
}
// 显示联系人表格  (包含序号、姓名、职务、职称、手机、Email、分类)
void ShowTable(Book *book) {
    printf("\n+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
    printf("| 序号 |      姓名      |      职务      |      职称      |       手机       |             Email            |    分类    |\n");
    printf("+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
    for(int idx = 0; idx < book->Count; idx++) {
        Person *person = &book->Data[idx];              // 获取当前联系人指针
        printf("| %-4d | %-14s | %-14s | %-14s | %-16s | %-28s | %-10s |\n",
               idx+1, person->Name, *person->Job? person->Job:"无", *person->Title? person->Title:"无",
               *person->Phone? person->Phone:"无", *person->Email? person->Email:"无", person->Type);
    }
    printf("+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
}
// 添加联系人  (包含姓名、职务、职称、手机、Email、分类)
void Add(Book *book) {
    char choice;
    do {
        printf("\n========== 添加联系人 ==========\n");
        if(book->Count >= MAX_CONTACTS) { printf("已满！\n"); ClearBuffer(); getchar(); return; }       // 检查通讯录是否已满
        Person person = {0};        // 初始化联系人结构体
        do { InputString(person.Name, STR_LEN, "姓名(0返回): ");        // 输入姓名
            if(!strcmp(person.Name, "0")) return;        // 如果输入 0 则返回   
        } while(!*person.Name || (FindByName(book, person.Name) >= 0 && printf("无效或已存在！\n")));
        InputString(person.Job, STR_LEN, "职务: ");        // 输入职务
        InputString(person.Title, STR_LEN, "职称: ");        // 输入职称
        do InputString(person.Phone, 20, "手机: "); while(!ValidPhone(person.Phone) && printf("格式错误！\n"));         // 验证手机格式
        do InputString(person.Email, 100, "Email: "); while(!ValidEmail(person.Email) && printf("格式错误！\n"));         // 验证邮箱格式
        Classify(&person);        // 对联系人进行分类
        book->Data[book->Count++] = person;        // 将新联系人添加到通讯录数组中
        printf("成功！分类: %s\n继续？(Y/N): ", person.Type);
        scanf(" %c", &choice); ClearBuffer();       // 输入继续标志
    } while(choice == 'Y' || choice == 'y');
}
// 显示所有联系人信息  (包含序号、姓名、职务、职称、手机、Email、分类)
void Show(Book *book) {
    printf("\n========== 所有联系人(%d人) ==========\n", book->Count);
    if(book->Count) ShowTable(book); else printf("空！\n");
    printf("按回车返回..."); getchar();
}
// 删除联系人  (根据姓名删除单个联系人或清空所有联系人)
void Delete(Book *book) {
    int choice;
    while(1) {
        printf("\n========== 删除联系人 ==========\n1.删除单个 2.清空 0.返回\n选择: ");
        if(scanf("%d", &choice) != 1) { ClearBuffer(); continue; } ClearBuffer();       // 输入选择标志
        if(choice == 0) return;
        if(choice == 1) {
            if(!book->Count) { printf("空！\n"); ClearBuffer(); getchar(); continue; }       // 检查通讯录是否为空
            char name[STR_LEN];        // 初始化姓名数组
            InputString(name, STR_LEN, "姓名(0返回): ");    // 输入姓名
            if(!strcmp(name, "0")) continue;        // 如果输入 0 则返回
            int idx = FindByName(book, name);       // 查找联系人索引
            if(idx < 0) { printf("未找到！\n"); getchar(); continue; }       // 如果未找到联系人    
            ShowOne(&book->Data[idx], idx);       // 显示联系人信息
            printf("确认？(Y/N): ");        // 输入确认标志
            char confirm; scanf(" %c", &confirm); ClearBuffer();       // 输入确认标志
            if(confirm == 'Y' || confirm == 'y') {
                for(int j = idx; j < book->Count-1; j++) book->Data[j] = book->Data[j+1];
                book->Count--; printf("已删除！\n");
            }       // 删除联系人后，更新联系人数量（数组元素前移实现删除）
            getchar();
        } else if(choice == 2) {
            printf("确认清空？(Y/N): "); 
            char confirm; scanf(" %c", &confirm); ClearBuffer();
            if(confirm == 'Y' || confirm == 'y') { book->Count = 0; printf("已清空！\n"); }
            getchar();
        }
    }
}
// 修改联系人信息  (根据姓名修改单个联系人信息)
void Modify(Book *book) {
    if(!book->Count) { printf("\n空！\n"); ClearBuffer(); getchar(); return; }       // 检查通讯录是否为空
    printf("\n========== 修改联系人 ==========\n");
    char name[STR_LEN];        // 初始化姓名数组
    InputString(name, STR_LEN, "姓名(0返回): ");    // 输入姓名
    if(!strcmp(name, "0")) return;        // 如果输入 0 则返回
    int idx = FindByName(book, name);       // 查找联系人索引
    if(idx < 0) { printf("未找到！\n"); ClearBuffer(); getchar(); return; }       // 如果未找到联系人    
    Person *person = &book->Data[idx];        // 初始化联系人指针
    ShowOne(person, idx);       // 显示联系人信息
    printf("\n新信息(回车默认不修改):\n");
    char buffer[100];        // 初始化缓冲区数组
    InputString(buffer, STR_LEN, "新姓名: "); if(*buffer) strcpy(person->Name, buffer);        // 输入新姓名
    InputString(buffer, STR_LEN, "新职务: "); if(*buffer) strcpy(person->Job, buffer);        // 输入新职务
    InputString(buffer, STR_LEN, "新职称: "); if(*buffer) strcpy(person->Title, buffer);        // 输入新职称
    InputString(buffer, 20, "新手机: "); if(*buffer && ValidPhone(buffer)) strcpy(person->Phone, buffer);        // 输入新手机
    InputString(buffer, 100, "新Email: "); if(*buffer && ValidEmail(buffer)) strcpy(person->Email, buffer);        // 输入新Email
    Classify(person);        // 对联系人进行分类
    printf("成功！分类: %s\n按回车返回...", person->Type); getchar();        // 显示分类结果
}
// 智能查找联系人  (根据姓名、职务、职称、手机、Email、分类、关键词进行查找)
void Search(Book *book) {
    if(!book->Count) { printf("\n空！\n"); ClearBuffer(); getchar(); return; }  
    printf("\n========== 智能查找 ==========\n");
    char keyword[100];        // 初始化关键词数组
    InputString(keyword, 100, "关键词(0返回): ");        // 输入关键词
    if(!strcmp(keyword, "0")) return;        // 如果输入 0 则返回
    int found = 0;        // 初始化找到联系人数量
    for(int idx = 0; idx < book->Count; idx++) {
        Person *person = &book->Data[idx];        // 初始化联系人指针
        if(strstr(person->Name,keyword)||strstr(person->Job,keyword)||strstr(person->Title,keyword)||
           strstr(person->Phone,keyword)||strstr(person->Email,keyword)||strstr(person->Type,keyword)) {
            ShowOne(person, idx); found++;        // 显示联系人信息并增加找到联系人数量
        }
    }
    printf(found ? "共%d个。\n" : "未找到！\n", found);
    printf("按回车返回..."); getchar();
}
// 按姓名排序联系人  
int CmpName(const void *a, const void *b) {
    return strcmp(((Person*)a)->Name, ((Person*)b)->Name);        // 按姓名排序
}
// 按分类排序联系人  
int CmpType(const void *a, const void *b) {
    return strcmp(((Person*)a)->Type, ((Person*)b)->Type);        // 按分类排序
}
// 按姓名排序联系人 
void Sort(Book *book) {
    printf("\n========== 按姓名排序 ==========\n");
    if(book->Count > 1) {
        qsort(book->Data, book->Count, sizeof(Person), CmpName);        // 按姓名排序
        ShowTable(book);        // 显示联系人表格
    } else printf("数量不足！\n");       
    printf("按回车返回..."); getchar();
}
// “伪”智能分类联系人  (根据姓名、职务、职称、手机、Email、分类进行智能分类)
void AI(Book *book) {
    printf("\n========== 智能分类 ==========\n");
    if(!book->Count) { printf("空！\n"); ClearBuffer(); getchar(); return; }
    for(int idx = 0; idx < book->Count; idx++) Classify(&book->Data[idx]);        // 对每个联系人进行分类
    qsort(book->Data, book->Count, sizeof(Person), CmpType);        // 按分类排序
    ShowTable(book);        // 显示联系人表格
    printf("按回车返回..."); getchar();
}
// 以二进制形式保存联系人信息到文件
int Save(Book *book) {
    FILE *file = fopen("contacts.dat", "wb");        // 打开文件
    printf("正在保存联系人信息...\n");
    if(!file) return 0;
    fwrite(&book->Count, sizeof(int), 1, file);        // 写入联系人数量
    fwrite(book->Data, sizeof(Person), book->Count, file);        // 写入联系人信息
    fclose(file);        // 关闭文件
    return 1;
}
// 从文件加载联系人信息
int Load(Book *book) {
    FILE *file = fopen("contacts.dat", "rb");        // 打开文件
    printf("正在加载联系人信息...\n");
    if(!file) return 0;
    fread(&book->Count, sizeof(int), 1, file);        // 读取联系人数量
    if(book->Count > MAX_CONTACTS) book->Count = MAX_CONTACTS;        // 如果联系人数量超过最大数量，则设置为最大数量
    fread(book->Data, sizeof(Person), book->Count, file);        // 读取联系人信息
    fclose(file);        // 关闭文件
    return 1;
}
// 显示菜单
void Menu() {
    printf("\n========================================\n");
    printf("=                                      =\n");
    printf("=          通 讯 录 管 理 系 统        =\n");
    printf("=                                      =\n");
    printf("========================================\n");
    printf("=                                      =\n");
    printf("=         1. 添加联系人                =\n");
    printf("=         2. 删除联系人                =\n");
    printf("=         3. 修改联系人                =\n");
    printf("=         4. 显示联系人                =\n");
    printf("=         5. 按姓名排序                =\n");
    printf("=         6. 智能查找                  =\n");
    printf("=         7. 智能分类                  =\n");
    printf("=         8. 保存文件                  =\n");
    printf("=         0. 退出系统                  =\n");
    printf("=                                      =\n");
    printf("========================================\n");
    printf("请选择(0-8): ");
}
// 主函数
void Menu();
int main() {
    Book book = {0};
    printf("\n欢迎使用通讯录管理系统！\n");
    if(Load(&book)) printf("已加载%d个联系人。\n", book.Count);
    else printf("创建新通讯录。\n");
    // 主循环
    int choice;
    while(1) {
        Menu();
        if(scanf("%d", &choice) != 1) { printf("无效！\n"); ClearBuffer(); continue; }
        ClearBuffer();
        switch(choice) {
            case 1: Add(&book); break;
            case 2: Delete(&book); break;
            case 3: Modify(&book); break;
            case 4: Show(&book); break;
            case 5: Sort(&book); break;
            case 6: Search(&book); break;
            case 7: AI(&book); break;
            case 8: printf("\n保存中...\n"); Save(&book); printf("完成！\n按回车返回..."); getchar(); break;
            case 0:
                printf("保存退出？(Y/N): "); 
                char confirm; scanf(" %c", &confirm);
                if(confirm == 'Y' || confirm == 'y') Save(&book);
                printf("再见！\n"); return 0;
            default: printf("无效！\n");
        }
    }
}