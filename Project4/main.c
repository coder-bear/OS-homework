#include "lab4.h"
int main(int argc, char* argv[]){
    size_t initial_start;
    printf("请输入内存开始地址：\n");
    scanf("%d", &initial_start);
    getchar();
    size_t capacity;
    size_t initial_memory = atoi(argv[1]);
    mm_block* head = NULL;
    char* str = (char*)malloc(sizeof(char)*MAX); 
    int n = MAX;
    int flag = 1; //Exit flag, 0 refers to exit, 1 refers to execute in the loop.
    while(1){
        printf("allocator>");
        fgets(str, n, stdin);
        for(int i=0;i<strlen(str);i++){
            if(str[i] == '\n'){
                str[i] = '\0';
            }
        }
        parse_input(str);
        if(strcmp(args[0], "RQ") == 0){
            //对头部进行特殊处理。
            if(NULL == head){
                printf("Head null\n");
                if(atoi(args[2]) > initial_memory){
                    printf("Out of Range!\n");
                    break;
                }
                mm_block* p = (mm_block*)malloc(sizeof(mm_block));
                p->start = 0;
                p->end = atoi(args[2]);
                p->name = args[1];
                head = create_linklist(p);
                free(p);
                p = NULL;
            }
            else{
                mm_block* block = allocate(head, args[1], atoi(args[2]), args[3][0], initial_start, initial_memory);
                head = insert_linklist(head, block);
            }
        }
        else if(strcmp(args[0], "RL") == 0){
            head = release(head, args[1]);   
        }
        else if(strcmp(args[0], "STAT") == 0){
            memory_stat(head, initial_memory);
        }
        else if(strcmp(args[0], "C") == 0){
            head = compaction(head, initial_start); 
        }
        else if(strcmp(args[0], "X") == 0){
            printf("Exit program!\n");
            break;
        }
        else{
            printf("Unknown action: %s! Please input the correct action.\n", args[0]);
            break;
        }
    }
    free(head);
    head  = NULL;
    return 0;
}