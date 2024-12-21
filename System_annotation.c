#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义一个学生信息的结构体，用于存储学生的基本数据
typedef struct Student {
    char id[15];       // 学号（字符串格式，最大长度为14字符+终止符）
    char name[20];     // 姓名（字符串格式，最大长度为19字符+终止符）
    double score;      // 成绩（浮点型，用于表示分数）
} Student;

// 定义链表的节点结构体
typedef struct LNode {
    Student data;      // 每个节点包含一个学生数据
    struct LNode *next; // 指向下一个节点的指针
} LNode, *LinkList;     // LinkList是指向LNode的指针类型

// 初始化链表（创建链表的头节点）
void InitList_L(LinkList *L) {
    *L = (LinkList)malloc(sizeof(LNode)); // 为头节点分配内存
    if (*L == NULL) { // 检查内存分配是否成功
        printf("内存分配失败！\n");
        exit(1); // 分配失败则退出程序
    }
    (*L)->next = NULL; // 初始化头节点的next指针为空（链表初始为空）
}


// 从外部文件创建链表并导入学生成绩
void CreateList(LinkList L, const char *filename) {
    FILE *fp = fopen(filename, "r"); // 打开文件以读取模式
    if (fp == NULL) { // 检查文件是否成功打开
        printf("文件打开失败！\n");
        return;
    }

    char buffer[100]; // 定义临时缓冲区，用于读取文件中的每一行
    LNode *tail = L;  // tail是链表尾部指针，初始指向头节点，用于插入新节点

    // 跳过文件的第一行（通常是表头，不包含具体数据）
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        printf("文件内容为空或读取失败！\n");
        fclose(fp); // 关闭文件后返回
        return;
    }

    // 从文件中逐行读取学生数据
    while (!feof(fp)) {
        // 为新学生分配一个链表节点
        LNode *newNode = (LNode *)malloc(sizeof(LNode));
        if (fscanf(fp, "%s %s %lf", newNode->data.id, newNode->data.name, &newNode->data.score) == 3) {
            newNode->next = NULL;    // 设置新节点的next指针为空
            tail->next = newNode;    // 将当前链表尾节点的next指向新节点
            tail = newNode;          // 更新尾指针为新节点
        } else {
            free(newNode); // 如果数据读取失败，释放新节点内存
            break; // 结束循环
        }
    }

    fclose(fp); // 关闭文件
    printf("%s导入学生成绩信息完毕！\n", filename); // 提示导入完成
}
/*这个函数 CreateList 通过读取指定文件导入学生成绩数据到链表。
首先打开文件并跳过表头，接着逐行读取数据。
每次读取时，为新学生分配内存创建节点，将读取的数据存入节点并将节点追加到链表尾部。
尾指针 tail 确保新节点插入高效。
读取失败时释放内存并退出循环，最后关闭文件。
设计上充分考虑文件读取异常和内存管理，确保链表的动态扩展和数据完整导入。*/


// 查询学生成绩（通过学号或姓名进行查找）
void Search(LinkList L, const char *key) {
    LNode *p = L->next; // 从链表第一个节点开始（头节点是哨兵节点，不存数据）
    int found = 0;      // 标记变量，用于记录是否找到匹配的学生信息

    while (p != NULL) { // 遍历链表直到末尾
        // 如果输入的学号或姓名与当前节点的数据匹配
        if (strcmp(p->data.id, key) == 0 || strcmp(p->data.name, key) == 0) {
            printf("查找成功！\n");
            // 打印学生信息
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", p->data.id, p->data.name, p->data.score);
            found = 1;  // 设置标记变量为1，表示找到
            break;      // 查找到后直接退出循环
        }
        p = p->next; // 移动到链表的下一个节点
    }

    if (!found) { // 如果未找到
        printf("查找失败！\n");
    }
}
/*函数 Search 用于在链表中查找学生信息。
它从链表头节点的下一个节点开始遍历，逐个比对每个节点的 id 和 name 是否与关键字 key 匹配。
如果找到，打印学生信息并退出循环；若遍历结束仍未匹配，输出查找失败。
函数通过标志变量 found 控制查找结果，保证逻辑清晰且高效。
设计中充分利用链表结构进行顺序查找，适合小规模数据场景。*/


// 按指定位置插入新的学生成绩
void ListInsert(LinkList L, const Student *stu, int position) {
    LNode *p = L;  // 从头节点开始，用于遍历链表
    int i = 0;     // 记录当前遍历到的节点位置

    // 遍历链表寻找插入位置的前一个节点
    while (p != NULL && i < position - 1) {
        p = p->next;
        i++;
    }

    // 如果指定位置无效（超过链表长度）
    if (p == NULL) {
        printf("插入位置无效！\n");
        return;
    }

    // 创建新节点
    LNode *newNode = (LNode *)malloc(sizeof(LNode));
    if (newNode == NULL) { // 检查内存分配是否成功
        printf("内存分配失败！\n");
        return;
    }

    newNode->data = *stu;  // 将学生信息复制到新节点的数据域
    newNode->next = p->next;  // 新节点的next指向当前位置的下一个节点
    p->next = newNode;        // 当前节点的next指向新节点

    printf("插入成功！\n");
}
/*
函数 ListInsert 实现在链表指定位置插入学生信息。
首先通过遍历链表找到目标位置的前一个节点，确保插入位置有效。
若位置无效或内存分配失败，输出错误信息并返回。
成功分配内存后，将学生数据拷贝到新节点，并更新指针：新节点指向当前位置的下一个节点，前一个节点指向新节点。
此设计利用链表的动态特性，实现高效插入操作，适用于链表中的任意位置插入场景。*/


// 删除学生成绩，根据学号和姓名匹配
void Delete(LinkList L, const char *id, const char *name) {
    LNode *p = L;    // 当前节点的前一个节点
    LNode *q;        // 用于暂存待删除节点
    int found = 0;   // 标志变量，记录是否找到匹配信息

    while (p->next != NULL) { // 遍历链表直到最后一个节点
        // 如果找到匹配的学生信息
        if (strcmp(p->next->data.id, id) == 0 && strcmp(p->next->data.name, name) == 0) {
            q = p->next;           // 待删除的节点
            p->next = q->next;     // 前节点的next跳过待删除节点，直接指向其后续节点
            printf("学生成绩信息删除成功！\n");
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", q->data.id, q->data.name, q->data.score);
            free(q);               // 释放删除节点的内存
            found = 1;
            return;
        }
        p = p->next; // 移动到下一个节点
    }

    if (!found) { // 如果未找到匹配项
        printf("学号和姓名不匹配！删除失败！\n");
    }
}
/*函数 Delete 通过遍历链表实现删除指定学生信息。
函数逐个检查节点，寻找学号 id 和姓名 name 同时匹配的节点。
找到匹配项时，将前节点的 next 指向目标节点的后续节点，并释放目标节点内存，保证链表结构完整。
若未找到匹配项，输出删除失败。
设计中通过双指针 p 和 q 控制前后节点关系，确保删除操作安全高效，同时避免内存泄漏。*/


// 修改学生成绩（通过学号和姓名匹配）
void Modify(LinkList L, const char *id, const char *name) {
    LNode *p = L->next; // 遍历链表
    int found = 0;      // 标志变量，记录是否找到匹配

    while (p != NULL) {
        // 如果找到匹配的学生信息
        if (strcmp(p->data.id, id) == 0 && strcmp(p->data.name, name) == 0) {
            printf("请输入新的成绩：");
            scanf("%lf", &p->data.score); // 输入新的成绩值
            printf("学生成绩信息修改成功！\n");
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", p->data.id, p->data.name, p->data.score);
            found = 1;
            return;
        }
        p = p->next; // 移动到下一个节点
    }

    if (!found) { // 如果未找到匹配项
        printf("学号和姓名不匹配！修改失败！\n");
    }
}
/*函数 Modify 用于修改链表中指定学生的成绩。
函数通过遍历链表逐个检查节点，匹配学号 id 和姓名 name。
找到匹配的节点后，提示用户输入新的成绩并更新节点数据。
若未找到匹配项，则输出修改失败。
设计上使用标志变量 found 判断匹配状态，确保修改操作的可靠性。
遍历过程中逐步移动指针，保证链表结构完整且操作高效。*/


// 对学生成绩按分数从高到低排序
void Sort(LinkList L) {
    if (L->next == NULL || L->next->next == NULL) return; // 如果链表为空或只有一个节点则无需排序

    LNode *p, *q; // 指针p、q用于嵌套遍历
    Student temp; // 临时变量，用于交换数据

    // 使用简单的冒泡排序对链表中的数据进行排序
    for (p = L->next; p->next != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (p->data.score < q->data.score) { // 如果当前节点的分数小于后节点
                temp = p->data; // 交换两个节点的数据
                p->data = q->data;
                q->data = temp;
            }
        }
    }
}
/*函数 Sort 使用冒泡排序对链表中的学生成绩进行降序排列。
函数先判断链表是否为空或仅含一个节点，若是则无需排序。
通过双重循环嵌套，利用两个指针 p 和 q 遍历链表中的所有节点，逐一比较成绩。若前节点分数小于后节点分数，则交换节点中的数据。
设计中避免直接调整指针结构，通过数据域交换实现排序，确保链表结构稳定且逻辑清晰，适用于小规模数据的排序操作。*/


// 显示所有学生成绩信息
void Show(LinkList L) {
    LNode *p = L->next; // 从链表第一个节点开始
    int index = 1;      // 序号，用于标记当前是第几个学生

    printf("\n当前学生成绩管理系统信息：\n\n");

    // 打印表头
    printf("%-10s  %-15s  %-15s  %-10s\n", "序号", "学号", "姓名", "成绩");
    printf("------------------------------------------------------\n");

    // 遍历链表并输出每个学生的信息
    while (p != NULL) {
        printf("%-10d  %-15s  %-15s  %-10.2f\n", index, p->data.id, p->data.name, p->data.score);
        p = p->next; // 移动到下一个节点
        index++;
    }
}
/*函数 Show 用于输出链表中所有学生的成绩信息。
通过遍历链表，从头节点的下一个节点开始逐一访问。
打印时，先输出表头，确保信息展示清晰，再逐行输出每个节点的学号、姓名和成绩，同时显示序号，方便用户查看数据。
函数利用格式化输出对齐内容，提升可读性。
设计中遍历链表直至末尾，保证链表所有数据完整展示，适合用于学生成绩管理系统的数据查看功能。*/


// 将学生成绩导出到外部文件
void Save(LinkList L, const char *filename) {
    printf("当前学生成绩导出到外部文件...\n"); // 提示开始导出

    FILE *fp = fopen(filename, "w"); // 以写入模式打开文件
    if (fp == NULL) { // 检查文件是否打开成功
        printf("导出失败！\n");
        return;
    }

    LNode *p = L->next; // 从链表第一个节点开始遍历
    int index = 1;      // 序号，用于记录是第几个学生

    // 写入文件表头信息
    fprintf(fp, "%-20s  %-23s  %-21s  %-10s\n", "序号", "学号", "姓名", "成绩");

    // 遍历链表，将每个学生信息写入文件
    while (p != NULL) {
        // 格式化输出每个学生的信息到文件
        fprintf(fp, "%-10d\t  %-20s\t  %-15s\t  %-10.2f\n", index, p->data.id, p->data.name, p->data.score);
        p = p->next; // 移动到下一个节点
        index++;
    }

    fclose(fp); // 关闭文件
    printf("成绩信息导出到%s完毕！\n", filename); // 提示导出完成
}
/*函数 Save 将链表中的学生成绩数据导出到指定文件。
首先以写入模式打开文件，若文件打开失败则提示并返回。
接着遍历链表，从头节点的下一个节点开始，逐个将学生的序号、学号、姓名和成绩格式化写入文件。
通过表头和清晰的格式输出，确保数据有序且易读。
遍历完成后关闭文件，并提示导出成功。
该设计实现了数据持久化，便于信息备份和外部存储。*/


// 主函数，提供用户交互菜单和功能选择
int main() {
    LinkList L;         // 定义一个链表指针，用于管理学生成绩信息
    InitList_L(&L);     // 初始化链表（创建头节点）
    int choice;         // 存储用户输入的菜单选项
    char filename[50];  // 用于存储文件名
    char id[15], name[20]; // 存储学生学号和姓名
    Student stu;        // 存储新增的学生信息

    // 使用循环实现交互式菜单
    do {
        // 打印菜单界面
        printf("		        欢迎使用学生成绩管理系统         \n\n");
        printf("		******************************************\n\n");
        printf("		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("		~~                                      ~~\n");
        printf("		~~        请按1： 导入学生成绩！        ~~\n");
        printf("		~~        请按2： 查询学生成绩！        ~~\n");
        printf("		~~        请按3： 新增学生成绩！        ~~\n");
        printf("		~~        请按4： 删除学生成绩！        ~~\n");
        printf("		~~        请按5： 修改学生成绩！        ~~\n");
        printf("		~~        请按6： 学生成绩排序！        ~~\n");
        printf("		~~        请按7： 显示学生成绩！        ~~\n");
        printf("		~~        请按8： 导出学生成绩!         ~~\n");
        printf("		~~        请按0： 退出系统！            ~~\n");
        printf("		~~                                      ~~\n");
        printf("		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("选择: ");
        scanf("%d", &choice); // 读取用户的选择

        // 根据用户输入的选项执行相应功能
        switch (choice) {
            case 1: // 导入学生成绩
                printf("输入文件名：");
                scanf("%s", filename); // 读取文件名
                CreateList(L, filename); // 从文件导入数据到链表
                break;

            case 2: // 查询学生成绩
                printf("请输入所要查找的学生学号或姓名：");
                scanf("%s", id); // 使用id存储输入的关键字（可能是学号或姓名）
                Search(L, id); // 调用查找函数
                break;

            case 3: { // 新增学生成绩
                int position;
                printf("请输入新增学生的信息，包括：学号 姓名 成绩（用空格隔开）：");
                scanf("%s %s %lf", stu.id, stu.name, &stu.score); // 输入学生信息
                printf("请输入待插入的位置：");
                scanf("%d", &position); // 输入插入位置
                ListInsert(L, &stu, position); // 调用插入函数
                break;
            }

            case 4: { // 删除学生成绩
                printf("请输入所要删除的学生学号：");
                scanf("%s", id); // 输入学号
                printf("请输入所要删除的学生姓名：");
                scanf("%s", name); // 输入姓名
                Delete(L, id, name); // 调用删除函数
                break;
            }

            case 5: { // 修改学生成绩
                printf("请输入所要修改的学生学号：");
                scanf("%s", id); // 输入学号
                printf("请输入所要修改的学生姓名：");
                scanf("%s", name); // 输入姓名
                Modify(L, id, name); // 调用修改函数
                break;
            }

            case 6: // 学生成绩排序
                Sort(L); // 调用排序函数
                printf("排序完毕！\n");
                break;

            case 7: // 显示学生成绩
                Show(L); // 调用显示函数
                break;

            case 8: // 导出学生成绩
                printf("请输入导出文件名：");
                scanf("%s", filename); // 输入导出文件名
                Save(L, filename); // 调用保存函数
                break;

            case 0: // 退出系统
                printf("已退出系统！\n");
                break;

            default: // 无效输入
                printf("无效选择，请重新输入！\n");
        }
    } while (choice != 0); // 当用户输入0时退出系统

    return 0; // 程序正常退出
}
/*函数 main 设计了一个交互式菜单驱动的学生成绩管理系统。
通过循环和 switch 结构，程序根据用户输入的选项执行相应功能：导入、查询、新增、删除、修改、排序、显示和导出学生成绩。
各功能模块通过调用独立函数实现，简化逻辑并提高可维护性。
菜单界面清晰友好，用户输入灵活，包括文件名、学号、姓名等参数。
循环执行，直至用户选择退出，确保系统交互性强且功能完整。*/