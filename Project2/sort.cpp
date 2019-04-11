#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
using namespace std;
vector <int> Array1;
vector <int> Array2;
typedef struct thread_data{
    int start_index;
    int length;
}parameters;
void* sort_array(void* para){
    parameters* data;
    data = (parameters*)para;
    sort(Array1.begin()+data->start_index, Array1.begin()+data->start_index+data->length);
    return nullptr;
}
int main(void){
    pthread_t tid_1, tid_2;
    int element;
    parameters* data1 = (parameters*)malloc(sizeof(parameters));
    parameters* data2 = (parameters*)malloc(sizeof(parameters));
    while(scanf("%d", &element)){
        if(element == -1){
            break;
        }
        Array1.push_back(element);
        Array2.push_back(element);
    }
    int length = Array1.size();
    int first_len=0;
    if(length%2 == 0){
        first_len = length/2;
    }
    else{
        first_len = length/2 + 1;
    }
    void* a;
    void* b;
    data1->start_index = 0;
    data1->length = first_len;
    pthread_create(&tid_1, NULL, sort_array, (void*)data1);
    pthread_join(tid_1, &a);
    data2->start_index = first_len;
    data2->length = length-first_len;
    pthread_create(&tid_2, NULL, sort_array, (void*)data2);
    pthread_join(tid_2, &b);
    
    for(int i=0;i<Array1.size();i++){
        cout << Array1[i] << endl;
    }
    int start_1 = 0;
    int start_2 = first_len;
    int k = 0;
    for(int i = start_1, j=start_2;i<first_len && j<length;){
        if(Array1[i] < Array1[j]){
            Array2[k++] = Array1[i++];
        }
        else{
            Array2[k++] = Array1[j++];
        }
        if(j== length && i == first_len -1){
            Array2[k++] = Array1[i++];
        } 
        if(j== length-1 && i == first_len ){
            Array2[k++] = Array1[j++];
        } 
    }
    cout << "Validiation Array" << endl;
    for(int i=0;i<Array2.size();i++){
        cout << Array2[i] << endl;
    }
    
    
    return 0;
}