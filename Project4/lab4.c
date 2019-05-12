#include "lab4.h"
void parse_input(char *string){
    char **str_array;
    const char *delimeter = " ";
    int i = 0;
    //initial the function. 
    for(int j=0;j<4;j++){
        args[j] = NULL;
    }
    char *p = strtok(string, delimeter);
    while(p!=NULL){
        args[i] = (char*)malloc(sizeof(char)*1000);
        strcpy(args[i], p);
        p = strtok(NULL,  delimeter);
        i++;
    }
}

mm_block* create_linklist(mm_block *block){
    mm_block* head = (mm_block*)malloc(sizeof(mm_block));
    if(head == NULL){
        printf("Allocate Error!\n");
    }
    else{
        head->next = NULL;
        head->start = block->start;
        head->end = block->end;
        head->name = block->name;
    }
    return head;
}

//插入的节点，空间一定是可分配的。
mm_block* insert_linklist(mm_block* head, mm_block* block){
    if(NULL == head){
        printf("Empty linklist!\n");
        return NULL;
    }
    mm_block* ptr = head;
    int flag = 0;
    int j = 0 ;
    if(block->end < head->start){
        printf("头部插入\n");
        block->next = head;
        head = block;
    }
    else{
        while(ptr->next!= NULL){
            //如果找到对应的未用区域，则将内存块插入链表结构中。
            if(block->start > ptr->end && block->end < ptr->next->start){
                block->next = ptr->next;
                ptr->next = block;
                flag = 1;
            }
            ptr = ptr->next;
        }
        if(ptr->next == NULL && flag == 0){
            ptr->next = block;
        }
    }
    return head;
}

void memory_stat(mm_block* head, int end){
    //遍历链表，比较下一个block的start和当前end是否有差值，如果有则被识别为unused.
    //链表为空，报错。
    if(NULL == head){
        printf("STAT Error, get an empty linklist!\n");
    }
    mm_block *ptr = head;
    //头节点不是内存的开始，需要特殊处理头部空间。
    if(head->start > 0){
        if(head->start > 1){
            printf("Addresses ");
            printf("[%d", 0);
            printf(":%d] ", ptr->start - 1);
            printf("Unused\n");
        }
        else{
            printf("Addresses ");
            printf("[%d]", 0);
            printf("Unused\n");
        }
    }
    while(ptr != NULL){
        printf("Addresses ");
        printf("[%d", ptr->start);
        printf(":%d] ", ptr->end);
        printf("Process %s\n", ptr->name);
        if(ptr->next != NULL){
            if(ptr->next->start - ptr->end > 2){
                printf("Addresses ");
                printf("[%d", ptr->end+1);
                printf(":%d] ", ptr->next->start-1);
                printf("Unused\n");
            }
            else if(ptr->next->start - ptr->end == 2){
                printf("Addresses ");
                printf("[%d]", ptr->end+1);
                printf("Unused\n");
            }
        }
        else{
            if(end - ptr->end == 1){
                printf("Addresses ");
                printf("[%d]", end);
                printf("Unused\n");
            }
            if (end - ptr->end > 1){
                printf("Addresses ");
                printf("[%d", ptr->end+1);
                printf(":%d] ", end);
                printf("Unused\n");
            }
        }     
        ptr = ptr->next;
    }
}

mm_block* allocate(mm_block* head, const char* process_name, int size, char policy, int initial_start, int max_capacity){
    //根据容量和策略查找内存。
    int flag = 0;
    int mm_block_start = -1;//要分配的内存块的首地址。
    mm_block* p = head; //遍历链表
    mm_block* ptr = (mm_block*)malloc(sizeof(char)); //作为返回值的内存块指针
    size_t end = initial_start + max_capacity - 1;
    int cap = 0;
    //先使用First-fit的策略测试是否能找到合适的块
    int value = head->start - initial_start;
    if( value > size){
        mm_block_start = initial_start;
        cap = head->start - initial_start;
        flag = 2;
    }
   
    while(p->next!=NULL && flag!=2){
        if(p->next->start - p->end -1>= size){
            flag = 1;
            mm_block_start = p->end + 1;
            break;
        }
        p = p->next;
    }
    
    //最后一块才满足条件
    if((end - p->end - 1>= size) && p->next == NULL) {    
        mm_block_start = p->end + 1;
        flag = 2;
    }    

    if(flag != 0){
        switch (policy)
        {
            //将最先满足要求的内存块分配给作业，可以减少查找时间
            case 'F':
                break;

            //最差适应算法，找出能满足作业大小要求且大小最大的区域，使得链表的
            case 'W':
                if(flag == 1){
                    p = head;
                    while(p->next!=NULL){
                        if(p->next->start - p->end -1 >= size){
                            if(p->next->start - p->end - 1 > cap){
                                cap = p->next->start - p->end - 1;
                                mm_block_start = p->end + 1;
                            }
                        }
                        p = p->next;
                    }
                    if(end - p->end > cap){
                        mm_block_start = p->end + 1;
                    }
                }
                break;

            case 'B':
                if(flag == 1){
                    p = head;
                    cap = MAX;
                    while(p->next!=NULL){
                        if(p->next->start - p->end -1>= size){
                            if(p->next->start - p->end -1< cap){
                                cap = p->next->start - p->end - 1;
                                mm_block_start = p->end + 1;
                            }
                        }
                        p = p->next;
                    }
                    if(end - p->end < cap ){
                        mm_block_start = p->end + 1;
                    }
                }
                break;

            default:
                mm_block_start = -1;
                break;
        }

        if(mm_block_start != -1){
            //封装内存块结构体
            ptr = (mm_block*)malloc(sizeof(mm_block));
            ptr->start = mm_block_start;
            ptr->end = mm_block_start + size -1;
            ptr->name = process_name;
            ptr->next = NULL;
        }
        else{
            printf("Undefined allocation policy %c", policy);
            ptr = NULL;
        }
    }
    else{
        printf("Allocation error! No free space!\n");
        ptr = NULL;
    }

    return ptr;
}

mm_block* release(mm_block* head, const char* process_name){
    int detect_flag = 0;
    if(NULL == head){
        printf("Empty linklist!");
        return NULL;
    }
    else{
        mm_block* ptr = head;
        if(strcmp(head->name, process_name) == 0){
            detect_flag = 1;
            mm_block *p;
            if(head->next == NULL){
                free(head);
                head = NULL;
            }
            else{
                p = head;
                head = head->next;
                free(p);
                p = NULL;
            }
        }
        while(ptr->next != NULL){
            if(strcmp(ptr->next->name, process_name) == 0){
                detect_flag = 1;
                if(NULL == ptr->next->next){
                    free(ptr->next);
                    ptr->next = NULL;
                }
                else{
                    mm_block* temp = ptr->next;
                    ptr->next = ptr->next->next;
                    free(temp);
                }
                break;
            }
            ptr = ptr->next;
        }
        if(detect_flag == 0){
            printf("Block not found, release error!\n");
        }
    }
    return head;
}

//压缩
mm_block* compaction(mm_block* head, int initial_start){
    int block_size = 0;
    if(NULL == head){
        printf("Empty linklist!");
        return NULL;
    }
    mm_block* ptr = head;
    if(head->start - initial_start > 0){
        block_size = head->end - head->start;
        head->start = initial_start;
        head->end = head->start + block_size;
    }
    while(ptr->next!=NULL){
        block_size = ptr->next->end - ptr->next->start;//实际的内存块大小是end-start+1
        ptr->next->start = ptr->end + 1;
        ptr->next->end = ptr->next->start + block_size;
        ptr = ptr->next;
    }
    return head;
}