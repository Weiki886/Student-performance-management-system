#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 定义学生成绩信息的结构体
typedef struct Student {
    char id[15];       // 学号
    char name[20];     // 姓名
    double score;      // 成绩
} Student;

// 定义单链表节点结构体
typedef struct LNode {
    Student data;      // 节点数据域
    struct LNode *next; // 节点指针域
} LNode, *LinkList;

// 初始化链表
void InitList_L(LinkList *L) {
    *L = (LinkList)malloc(sizeof(LNode));
    if (*L == NULL) {
        printf("内存分配失败！\n");
        exit(1);
    }
    (*L)->next = NULL;
}

//导入学生成绩 
// 从外部文件创建链表
void CreateList(LinkList L, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return;
    }

    char buffer[100]; // 临时存储文件行内容
    LNode *tail = L; // 尾插法

    // 跳过文件的第一行
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        printf("文件内容为空或读取失败！\n");
        fclose(fp);
        return;
    }

    while (!feof(fp)) {
        LNode *newNode = (LNode *)malloc(sizeof(LNode));
        if (fscanf(fp, "%s %s %lf", newNode->data.id, newNode->data.name, &newNode->data.score) == 3) {
            newNode->next = NULL;
            tail->next = newNode;
            tail = newNode;
        } else {
            free(newNode);
            break;
        }
    }

    fclose(fp);

    // 显示导入完成信息
    printf("%s导入学生成绩信息完毕！\n", filename);
}



// 查询学生成绩 
void Search(LinkList L, const char *key) {
    LNode *p = L->next; // 从链表第一个节点开始查找
    int found = 0;      // 用于记录是否找到学生信息

    while (p != NULL) {
        if (strcmp(p->data.id, key) == 0 || strcmp(p->data.name, key) == 0) {
            printf("查找成功！\n");
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", p->data.id, p->data.name, p->data.score);
            found = 1;
            break;
        }
        p = p->next;
    }

    if (!found) {
        printf("查找失败！\n");
    }
}


// 新增学生成绩 
void ListInsert(LinkList L, const Student *stu, int position) {
    LNode *p = L;  // 用于遍历链表
    int i = 0;

    // 寻找插入位置的前一个节点
    while (p != NULL && i < position - 1) {
        p = p->next;
        i++;
    }

    // 如果位置无效
    if (p == NULL) {
        printf("插入位置无效！\n");
        return;
    }

    // 创建新节点
    LNode *newNode = (LNode *)malloc(sizeof(LNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    newNode->data = *stu;  // 复制学生信息
    newNode->next = p->next;  // 新节点的 next 指向原来的下一个节点
    p->next = newNode;        // 当前节点指向新节点

    printf("插入成功！\n");
}



// 删除学生成绩 
void Delete(LinkList L, const char *id, const char *name) {
    LNode *p = L;    // 指向当前节点的前一个节点
    LNode *q;        // 用于暂存待删除的节点
    int found = 0;   // 标记是否找到匹配的学生信息

    while (p->next != NULL) {
        if (strcmp(p->next->data.id, id) == 0 && strcmp(p->next->data.name, name) == 0) {
            q = p->next;
            p->next = q->next; // 将前一个节点指向待删除节点的下一个节点
            printf("学生成绩信息删除成功！\n");
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", q->data.id, q->data.name, q->data.score);
            free(q); // 释放内存
            found = 1;
            return;
        }
        p = p->next;
    }

    if (!found) {
        printf("学号和姓名不匹配！删除失败！\n");
    }
}


// 修改学生成绩 
void Modify(LinkList L, const char *id, const char *name) {
    LNode *p = L->next; // 遍历链表
    int found = 0;      // 标记是否找到匹配的学生信息

    while (p != NULL) {
        if (strcmp(p->data.id, id) == 0 && strcmp(p->data.name, name) == 0) {
            printf("请输入新的成绩：");
            scanf("%lf", &p->data.score);
            printf("学生成绩信息修改成功！\n");
            printf("学号: %s, 姓名: %s, 成绩: %.2f\n", p->data.id, p->data.name, p->data.score);
            found = 1;
            return;
        }
        p = p->next;
    }

    if (!found) {
        printf("学号和姓名不匹配！修改失败！\n");
    }
}


// 学生成绩排序 
void Sort(LinkList L) {
    if (L->next == NULL || L->next->next == NULL) return; // 空链表或只有一个节点
    LNode *p, *q;
    Student temp;
    for (p = L->next; p->next != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (p->data.score < q->data.score) {
                temp = p->data;
                p->data = q->data;
                q->data = temp;
            }
        }
    }
}


// 显示学生成绩 
void Show(LinkList L) {
    LNode *p = L->next; // 遍历链表
    int index = 1;      // 序号

    printf("\n当前学生成绩管理系统信息（链表）读出：\n\n");

    // 打印表头
    printf("%-10s%-15s%-15s%-10s\n", "序号", "学号", "姓名", "成绩");
    printf("------------------------------------------------------\n");

    // 遍历链表并输出每个学生的信息
    while (p != NULL) {
        printf("%-10d%-15s%-15s%-10.2f\n", index, p->data.id, p->data.name, p->data.score);
        p = p->next;
        index++;
    }
}


// 导出学生成绩 
void Save(LinkList L, const char *filename) {
    printf("当前学生成绩导出到外部文件...\n"); // 导出前提示

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("导出失败！\n");
        return;
    }

    LNode *p = L->next; // 遍历链表
    int index = 1;      // 序号

    // 输出表头
    fprintf(fp, "%-20s %-23s %-21s %-10s\n", "序号", "学号", "姓名", "成绩");

    // 遍历链表并将内容写入文件
    while (p != NULL) {
        fprintf(fp, "%-10d\t %-20s\t %-15s\t %-10.2f\n", index, p->data.id, p->data.name, p->data.score);
        p = p->next;
        index++;
    }

    fclose(fp);
    printf("成绩信息导出到%s完毕！\n", filename); // 导出成功提示
}






// 主函数
int main() {
    LinkList L;
    InitList_L(&L);
    int choice;
    char filename[50], id[15], name[20];
    Student stu;

    do {
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
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("输入文件名：");
                scanf("%s", filename);
                CreateList(L, filename);
                break;
                
            case 2:
    			printf("请输入所要查找的学生学号或姓名：");
    			scanf("%s", id); // 这里使用 id 变量存储用户输入的关键字
    			Search(L, id);
    			break;
    			
    		case 3: {
    			int position;
    			printf("请输入新增学生的信息，包括：学号 姓名 成绩（用空格隔开）：");
    			scanf("%s %s %lf", stu.id, stu.name, &stu.score);
				printf("请输入待插入的位置：");
    			scanf("%d", &position);
    			ListInsert(L, &stu, position);
    			break;
			}
			
			case 4: {
    			printf("请输入所要删除的学生学号：");
    			scanf("%s", id); // 输入学号
    			printf("请输入所要删除的学生姓名：");
    			scanf("%s", name); // 输入姓名
    			Delete(L, id, name);
    			break;
			}
			
            case 5: {
    			printf("请输入所要修改的学生学号：");
    			scanf("%s", id); // 输入学号
    			printf("请输入所要修改的学生姓名：");
    			scanf("%s", name); // 输入姓名
    			Modify(L, id, name);
    			break;
			}


            case 6:
                Sort(L);
                printf("排序完毕！\n");
                break;
                
            case 7:
                Show(L);
                break;
                
            case 8:
    			printf("请输入导出文件名：");
    			scanf("%s", filename);
    			Save(L, filename);
    			break;

            case 0:
                printf("已退出系统！\n");
                break;
                
            default:
                printf("无效选择，请重新输入！\n");
        }
    } while (choice != 0);

    return 0;
}