#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//wdnmd C的二级指针可以就是计算机中内存存储其他内容的地址。
/**************************
 * Debug记录：2019.5.08 create_node, insert_node测试成功， allocate插入连续节点测试成功。
 * Debug记录：2019.05.10 测试完毕
 * Debug记录：2019.05.11 综合测试，makefile编写，文档撰写
 * **************************/
#define MAX 100
char *args[4];
typedef struct block{
    //int flag = 0; // flag为1表示被使用， flag为0表示没有被使用。
    char *name ;
    size_t start;
    size_t end;
    struct block *next;
}mm_block;
void parse_input(char *string);

mm_block* create_linklist(mm_block *block);

mm_block* insert_linklist(mm_block* head, mm_block* block);

void memory_stat(mm_block* head, int end);

mm_block* allocate(mm_block* head, const char* process_name, int size, \
char policy, int initial_start, int max_capacity);

mm_block* release(mm_block* head, const char* process_name);

mm_block* compaction(mm_block* head, int initial_start);