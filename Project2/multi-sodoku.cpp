#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <error.h>
#include <set>
#define ROW 0
#define COL 1
#define NINE 2
using namespace std;
int Sudoku[9][9] = {
    {6, 2, 4, 5 ,3 ,9, 1, 8, 7},
    {5, 1, 9, 7, 2 ,8, 6, 3, 4}, 
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1}, 
    {6, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};
int validation[27] = {1};
void* res[27];
//validation[0], 代表行是否正确，validation[1],代表列是否正确， 随后， 2-10代表数独的1-9的九宫格。
typedef struct thread_para{
    int row;
    int column;
} parameters;
pthread_t pth[27];

void* check_row(void* para){
    parameters* data;
    data = (parameters*)para;
    int set[9] = {0};
    int row = data->row;
    int index;
    for(int j=0;j<9;j++){
        index = Sudoku[row][j];
        if(set[index] == 0){
            set[index] = 1;
        }
        else{
            return (void *)0;
        }
    }
    return (void *)1;
}

void* check_col(void* para){
    parameters* data;
    int set[9] = {0};
    data = (parameters*)para;
    int col = data->column;
    for(int i=0;i<9;i++){
        if(set[Sudoku[i][col]] == 0){
            set[Sudoku[i][col]] = 1;
        }
        else{
            return (void *)0;
        }
    }
    return (void *)1;
}
void* check_sodoku(void* para){
    parameters* data;
    data = (parameters*)para;
    int row = data->row;
    int col = data->column;
    int index;
    int set[9] = {0};
    for(int i=row;i<row+3;i++){
        for(int j=col;j<col+3; j++){
            index = Sudoku[i][j];
            if(set[index] == 0){
                set[index] = 1;
            }
            else{
                return (void *)0;
            }
        }
    }
    return (void *)1;
}

void input_sudoku(){
    printf("Please input the Sudoku array:\n");
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            scanf("%d", &Sudoku[i][j]);
        }
    }
}

int main(void){
    parameters *data[27];
    void* res[27];
    int index;
    int flag = 1;
    //input_sudoku();
    //row detection
    for(int i=0;i<9;i++){
        data[i] = (parameters*)malloc(sizeof(parameters));
        data[i]->row = i;
        data[i]->column = 0;
        pthread_create(&pth[i], NULL, check_row, (void*)data[i]);
        pthread_join(pth[i], &res[i]);
        validation[i] = (long)res[i];
    }
    
    //col detection 
    for(int j=9; j<18;j++){
        data[j] = (parameters*)malloc(sizeof(parameters));
        data[j]->column = j-9;
        data[j]->row = 0;
        pthread_create(&pth[j], NULL, check_col, (void*)data[j]);
        pthread_join(pth[j], &res[j]);
        validation[j] = (long)res[j];
    }
    //block detection
    for(int k=18; k<27; k++){
        data[k] = (parameters*)malloc(sizeof(parameters));
        data[k]->row = (int)((k-18)/3)*3;
        data[k]->column = ((k-18)%3)*3;
        pthread_create(&pth[k], NULL, check_sodoku, (void*)data[k]);
        pthread_join(pth[k], &res[k]);
        validation[k] = (long)res[k];
    }

    for(int i=0;i<27;i++){
        if(validation[i] == 0){
            flag = 0;
            if(i>=0 && i<9){
                printf("Row %d error\n", i+1);
            }
            else if(i<18){
                printf("Column %d Error\n", i-8);
            }
            else{
               printf("the %dth block Error\n", i-17); 
            }
        }
    }
    if(flag == 1){
        printf("Sudoku right\n");
    }
    return 0;
}
 