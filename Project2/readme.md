# Lab 02 Readme  
------------------------------
16281150 杨子耀
## I. Thread Control methods by Linux
(1) Create thread : Create a thread   
(2) Cancel thread:terminate the thread   
(3) Join thread: the main thread wait the child thread to terminate, the same as the *wait()* in the process concept.   
(4) Get id: get the thread itself id
## II. Design of the Program
### 1. The mulithread sudoku design
#### (1) Basic data structure and their meanings 
a. 2D-array, data type: int, size: 9*9, name: Sudoku. Usage: store the Sudoku data.

b. 1D Array; data type: int, (0 or 1)
size: 27, name,validation, usage: for specific error detection in sudoku,which can specify to the one row, one column and one specific block.if the element in validation array is 0, it means that this part has some problem. 
    
c. 1D Array, data type: void* ,size: 27, name, res;
Usage: for thread return result.
    
d. struct, data type: struct, name: parameters
Usage, as the parameters pass to the function which is executed in the thread.
#### (2) Function
##### a. void* check_row(void* para);
Usage: check rows, para shows that the position of the first element in the row. If the row is correct(1 to 9, no duplicates), if will return 1, else it will return 0.
##### b. void * check_col(void *para)
Usage: check cols,  para shows that the position of the first element in the col.If the col is correct(1 to 9, no duplicates), if will return 1, else it will return 0.
    
##### c. void* check_sudoku(void* para)
Usage: check sudoku blocks, para shows the left most index, which can identify the specific block to schedule. 
    
##### d. void input_sudoku()
Usage: Input the Sudoku.

#### (3) Design
Create 27 threads to examine the correctness of each row, each column and each block. 
Use **pthread_create()** and **pthread_join()** to run the functions. 
Each thread funcition's result will return to the validation array. 
The main process checks the validation array to identify which part is error. 

### 2. Mulithread Sort
#### (1) Basic data structure and their meanings 
##### a. vector <int> Array1, Array2 
Array1 is the inital array, Array2 is the result array.    
##### b. parameters
Struct, which contains the array start_index and length.    
The start_index refers to the sorting sub-array start index ,which is the index referred to the initial array. This parameter is used for the **sort()** method. The **sort()** method needs a start index and a end index. Thus, to obtain the end index, the length attribution use the calculate the end index, which is start_index + length. 
#### (2) Function
##### a.void* sort_array(void* para);
According to the para, extract the start_index and the length to create the start_index and the end_index. Based on these two parameters, sort this specific part in Array1.
#### (3) Design
We create two threads to sort two half parts in indivually. Once two threads finished the its functions, the Array1 is totally sorted. 
## III. Snapshots 
### 1. Sudoku result
#### (1) Right Sudoku validation
The correct sudoku is as follows.

![Right Sudoku](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/sudoku3.png)

The result is:
![Right Sudoku Result](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/sudoku_right.png)

#### (2) Error test
We modify the sudoku. We changed the first element at the 6th row, which change 7 to 6. 
![Error Sudoku](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/column_error.png)

The error detection result is as follows:
![Sudoku error detection](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/col_error_res.png)

### 2. Sort result
The sort array is as follows:

![Unsorted array](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/test_array.png)

The sorted array result is as follows:

![Sorted array](https://raw.githubusercontent.com/coder-bear/OS-homework/master/Project2/picture/sort_res.png)

## IV. Problems encoutered and solutions
### 1. Sudoku problem
For each thread ,we use the global data structure to identify which specific number is correct or not. But the shared buffer makes chaos.   
We solve the problem by the allocating the separate resources in each thread. 

## V. Reference materials
POSIX multiple thread: https://www.cnblogs.com/mywolrd/archive/2009/02/05/1930707.html
