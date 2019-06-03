#include <stdio.h>
#include <stdlib.h>
#define MASK 0x000000FF 
#define PAGE_TABLE_SIZE 256
#define FRAME_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define MEMORY_CAPACITY 128

int page_table[PAGE_TABLE_SIZE];
int TLB[TLB_SIZE][2];
char* memory[MEMORY_CAPACITY];

void init_page_table(){
    for(int i=0;i<PAGE_TABLE_SIZE;i++){
        page_table[i] = -1;
    }
}

void init_tlb(){
    for(int i=0; i<16; i++){
        TLB[i][0] = -1;
        TLB[i][1] = -1;
    }
}

//Search TLB
int search_tlb(int page_number){
    int frame_number = -1;
    for(int i=0;i<16;i++){
        if(TLB[i][0] == page_number){
            frame_number = TLB[i][1];
            break;
        }
    }
    return frame_number;
}


//Search page table
int search_page_table(int page_number){
    int frame_number = -1;
    if(page_table[page_number] != -1){
        frame_number = page_table[page_number];
    }
    return frame_number;
}

//没有页面置换的版本, version 1.0
int insert_memory(char *frame_address){
    static int frame_index = -1;
    memory[(++frame_index)%MEMORY_CAPACITY] = frame_address;
    return frame_index%MEMORY_CAPACITY;
}

int insert_tlb(int page_number, int frame_number){
    static int tlb_index = 0;
    TLB[tlb_index%TLB_SIZE][0] = page_number;
    TLB[tlb_index%TLB_SIZE][1] = frame_number;
    tlb_index++;
    return 0;
}

int insert_page_table(int page_number, int frame_number){
    page_table[page_number] = frame_number;
    return 0;
}

int handle_page_fault(int page_number, int offset){
    FILE *fp = fopen("./BACKING_STORE.bin", "rb");
    int frame_number = -1;
    char *frame = (char*)malloc(sizeof(char)*256);
    fseek(fp, 0, SEEK_SET);
    if(fseek(fp, PAGE_SIZE*page_number, SEEK_SET) == 0){
        int value;
        if((value = fread(frame, sizeof(unsigned char), FRAME_SIZE, fp)) > 0){
            frame_number = insert_memory(frame);
            insert_page_table(page_number, frame_number);
            insert_tlb(page_number, frame_number);
        }
        fclose(fp);
    }
    return frame_number;
}


int main(int argc, char* argv[]){
    double tlb_hit_times = 0;
    double page_table_hit_times = 0;
    FILE *fp;
    char str[1];
    fp = fopen(argv[1], "r");
    size_t physical_address = -1;

    if(fp == NULL) {
      perror("Error opening file");
      return -1;
    }
    init_tlb();
    init_page_table();
    char *buf = (char*)malloc(sizeof(char)*20);
    printf("---------------------------------------------------------------\n");
    while(fgets(buf, 20, fp)!=NULL){
        size_t logical_address = atoi(buf);
        size_t offset = logical_address&MASK;
        size_t frame_number = -1;
        printf("offset: %x\n", offset);
        size_t page_number = (logical_address>>8)&MASK;
        printf("page number: %d\n", page_number);   

        //查找TLB
        if((frame_number = search_tlb(page_number)) != -1){
            char* ptr = memory[frame_number];
            physical_address = (frame_number << 8) + offset;
            printf("TLB hit, physical address is: %d, value is: %d\n",physical_address, ptr[offset]);
            tlb_hit_times++;
        }

        //TLB miss, 查找页表
        else if((frame_number = search_page_table(page_number))!=-1){
            char* ptr = memory[frame_number];
            physical_address = (frame_number << 8) + offset;
            printf("Page table hit, physical address is: %d, value is %d\n", physical_address, ptr[offset]);
            page_table_hit_times ++;
        }

        //TLB miss, page table miss, page fault. 
        else{
            printf("Page fault!\n");
            int frame_number = handle_page_fault(page_number, offset);
            physical_address = (frame_number << 8) + offset;
            printf("Handle page fault, physical address is : %d\n", physical_address, memory[frame_number][offset]);
        }    
    }
    fclose(fp);
    printf("---------------------------------------------------------------\n");
    printf("TLB rate: %.2f%c \n", tlb_hit_times/10, '%');
    printf("Page rate: %.2f%c\n", page_table_hit_times/10, '%');
    return 0;
}