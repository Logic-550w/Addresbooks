#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 1000
#define STR_LEN 50

typedef struct {
    char Name[STR_LEN], Job[STR_LEN], Title[STR_LEN];
    char Phone[20], Email[100], Type[20];
} Person;

typedef struct {
    Person Data[MAX_CONTACTS];
    int Count;
} Book;

void ClearBuffer() { while(getchar()!='\n'); }

int InputString(char *str, int len, const char *prompt) {
    printf("%s", prompt);
    if(!fgets(str, len, stdin)) return 0;
    str[strcspn(str, "\n")] = 0;
    return 1;
}

int ValidPhone(const char *phone) {
    int digitCount = 0;
    for(; *phone; phone++) 
        if(isdigit(*phone)) digitCount++; 
        else if(!strchr(" -+", *phone)) return 0;
    return digitCount >= 7 && digitCount <= 15;
}

int ValidEmail(const char *email) {
    int hasAt = 0, hasDot = 0;
    for(; *email; email++) { 
        if(*email == '@') hasAt = 1; 
        if(hasAt && *email == '.') hasDot = 1; 
    }
    return !*email || (hasAt && hasDot);
}

int FindByName(Book *book, const char *name) {
    for(int idx = 0; idx < book->Count; idx++)
        if(!strcmp(book->Data[idx].Name, name)) return idx;
    return -1;
}

void Classify(Person *person) {
    if(strstr(person->Name,"爸")||strstr(person->Name,"妈")||strstr(person->Name,"哥")||strstr(person->Name,"姐")||
       strstr(person->Name,"弟")||strstr(person->Name,"妹")||strstr(person->Name,"爷")||strstr(person->Name,"奶")) {
        strcpy(person->Type, "家人"); return; }
    if(strstr(person->Job,"经理")||strstr(person->Job,"主管")||strstr(person->Job,"总监")||strstr(person->Job,"工程师")||
       strstr(person->Title,"工程师")||strstr(person->Title,"架构师")) {
        strcpy(person->Type, "同事"); return; }
    if(strstr(person->Job,"老师")||strstr(person->Job,"教授")||strstr(person->Title,"老师")||strstr(person->Title,"教授")) {
        strcpy(person->Type, "老师"); return; }
    if(strstr(person->Job,"班长")||strstr(person->Job,"学生")||strstr(person->Title,"学生")) {
        strcpy(person->Type, "同学"); return; }
    strcpy(person->Type, "朋友");
}

void ShowOne(Person *person, int index) {
    printf("\n--- 联系人 #%d ---\n姓名: %s\n职务: %s\n职称: %s\n手机: %s\nEmail: %s\n分类: %s\n------------------\n",
           index+1, person->Name, *person->Job? person->Job:"无", *person->Title? person->Title:"无",
           *person->Phone? person->Phone:"无", *person->Email? person->Email:"无", person->Type);
}

void ShowTable(Book *book) {
    printf("\n+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
    printf("| 序号 |      姓名      |      职务      |      职称      |       手机       |             Email            |    分类    |\n");
    printf("+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
    for(int idx = 0; idx < book->Count; idx++) {
        Person *person = &book->Data[idx];
        printf("| %-4d | %-14s | %-14s | %-14s | %-16s | %-28s | %-10s |\n",
               idx+1, person->Name, *person->Job? person->Job:"无", *person->Title? person->Title:"无",
               *person->Phone? person->Phone:"无", *person->Email? person->Email:"无", person->Type);
    }
    printf("+------+----------------+----------------+----------------+------------------+------------------------------+------------+\n");
}

void Add(Book *book) {
    char choice;
    do {
        printf("\n========== 添加联系人 ==========\n");
        if(book->Count >= MAX_CONTACTS) { printf("已满！\n"); ClearBuffer(); getchar(); return; }
        Person person = {0};
        do { InputString(person.Name, STR_LEN, "姓名(0返回): ");
            if(!strcmp(person.Name, "0")) return;
        } while(!*person.Name || (FindByName(book, person.Name) >= 0 && printf("无效或已存在！\n")));
        InputString(person.Job, STR_LEN, "职务: "); 
        InputString(person.Title, STR_LEN, "职称: ");
        do InputString(person.Phone, 20, "手机: "); while(!ValidPhone(person.Phone) && printf("格式错误！\n"));
        do InputString(person.Email, 100, "Email: "); while(!ValidEmail(person.Email) && printf("格式错误！\n"));
        Classify(&person);
        book->Data[book->Count++] = person;
        printf("成功！分类: %s\n继续？(Y/N): ", person.Type);
        scanf(" %c", &choice); ClearBuffer();
    } while(choice == 'Y' || choice == 'y');
}

void Show(Book *book) {
    printf("\n========== 所有联系人(%d人) ==========\n", book->Count);
    if(book->Count) ShowTable(book); else printf("空！\n");
    printf("按回车返回..."); getchar();
}

void Delete(Book *book) {
    int choice;
    while(1) {
        printf("\n========== 删除联系人 ==========\n1.删除单个 2.清空 0.返回\n选择: ");
        if(scanf("%d", &choice) != 1) { ClearBuffer(); continue; } ClearBuffer();
        if(choice == 0) return;
        if(choice == 1) {
            if(!book->Count) { printf("空！\n"); ClearBuffer(); getchar(); continue; }
            char name[STR_LEN]; 
            InputString(name, STR_LEN, "姓名(0返回): ");
            if(!strcmp(name, "0")) continue;
            int idx = FindByName(book, name);
            if(idx < 0) { printf("未找到！\n"); getchar(); continue; }
            ShowOne(&book->Data[idx], idx);
            printf("确认？(Y/N): "); 
            char confirm; scanf(" %c", &confirm); ClearBuffer();
            if(confirm == 'Y' || confirm == 'y') {
                for(int j = idx; j < book->Count-1; j++) book->Data[j] = book->Data[j+1];
                book->Count--; printf("已删除！\n");
            }
            getchar();
        } else if(choice == 2) {
            printf("确认清空？(Y/N): "); 
            char confirm; scanf(" %c", &confirm); ClearBuffer();
            if(confirm == 'Y' || confirm == 'y') { book->Count = 0; printf("已清空！\n"); }
            getchar();
        }
    }
}

void Modify(Book *book) {
    if(!book->Count) { printf("\n空！\n"); ClearBuffer(); getchar(); return; }
    printf("\n========== 修改联系人 ==========\n");
    char name[STR_LEN]; 
    InputString(name, STR_LEN, "姓名(0返回): ");
    if(!strcmp(name, "0")) return;
    int idx = FindByName(book, name);
    if(idx < 0) { printf("未找到！\n"); ClearBuffer(); getchar(); return; }
    Person *person = &book->Data[idx];
    ShowOne(person, idx);
    printf("\n新信息(回车默认不修改):\n");
    char buffer[100];
    InputString(buffer, STR_LEN, "新姓名: "); if(*buffer) strcpy(person->Name, buffer);
    InputString(buffer, STR_LEN, "新职务: "); if(*buffer) strcpy(person->Job, buffer);
    InputString(buffer, STR_LEN, "新职称: "); if(*buffer) strcpy(person->Title, buffer);
    InputString(buffer, 20, "新手机: "); if(*buffer && ValidPhone(buffer)) strcpy(person->Phone, buffer);
    InputString(buffer, 100, "新Email: "); if(*buffer && ValidEmail(buffer)) strcpy(person->Email, buffer);
    Classify(person);
    printf("成功！分类: %s\n按回车返回...", person->Type); getchar();
}

void Search(Book *book) {
    if(!book->Count) { printf("\n空！\n"); ClearBuffer(); getchar(); return; }
    printf("\n========== 智能查找 ==========\n");
    char keyword[100]; 
    InputString(keyword, 100, "关键词(0返回): ");
    if(!strcmp(keyword, "0")) return;
    int found = 0;
    for(int idx = 0; idx < book->Count; idx++) {
        Person *person = &book->Data[idx];
        if(strstr(person->Name,keyword)||strstr(person->Job,keyword)||strstr(person->Title,keyword)||
           strstr(person->Phone,keyword)||strstr(person->Email,keyword)||strstr(person->Type,keyword)) {
            ShowOne(person, idx); found++;
        }
    }
    printf(found ? "共%d个。\n" : "未找到！\n", found);
    printf("按回车返回..."); getchar();
}

int CmpName(const void *a, const void *b) {
    return strcmp(((Person*)a)->Name, ((Person*)b)->Name);
}

int CmpType(const void *a, const void *b) {
    return strcmp(((Person*)a)->Type, ((Person*)b)->Type);
}

void Sort(Book *book) {
    printf("\n========== 按姓名排序 ==========\n");
    if(book->Count > 1) {
        qsort(book->Data, book->Count, sizeof(Person), CmpName);
        ShowTable(book);
    } else printf("数量不足！\n");
    printf("按回车返回..."); getchar();
}

void AI(Book *book) {
    printf("\n========== 智能分类 ==========\n");
    if(!book->Count) { printf("空！\n"); ClearBuffer(); getchar(); return; }
    for(int idx = 0; idx < book->Count; idx++) Classify(&book->Data[idx]);
    qsort(book->Data, book->Count, sizeof(Person), CmpType);
    ShowTable(book);
    printf("按回车返回..."); getchar();
}

int Save(Book *book) {
    FILE *file = fopen("contacts.dat", "wb");
    if(!file) return 0;
    fwrite(&book->Count, sizeof(int), 1, file);
    fwrite(book->Data, sizeof(Person), book->Count, file);
    fclose(file);
    return 1;
}

int Load(Book *book) {
    FILE *file = fopen("contacts.dat", "rb");
    if(!file) return 0;
    fread(&book->Count, sizeof(int), 1, file);
    if(book->Count > MAX_CONTACTS) book->Count = MAX_CONTACTS;
    fread(book->Data, sizeof(Person), book->Count, file);
    fclose(file);
    return 1;
}

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

int main() {
    Book book = {0};
    printf("\n欢迎使用通讯录管理系统！\n");
    if(Load(&book)) printf("已加载%d个联系人。\n", book.Count);
    else printf("创建新通讯录。\n");
    
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