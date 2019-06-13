Contiguous Memory Allocation Simulation
===================
##Installation
Installation is only supported on Linux/OS X platform.
### 1. OS X & Linux
   Get into the directory of the source file and input the following command to make:
   <pre>make</pre>

   To clean the generated file, please input the following commands:
   <pre>make clean</pre>

## 2. Usage example
We test the correctness of the usage through <b>STAT</b>.
### (1) Request memory block
We implement different allocation policy to stimulate the requirement of the policy
#### a. First fit
<pre> allocator> RQ [process_name] [request_size] F </pre>
picture first fit
#### b. Best fit
<pre> allocator> RQ [process_name] [request_size] B </pre>
picture Best fit
#### c. Worst fit
<pre> allocator> RQ [process_name] [request_size] W </pre>
picture Worst fit
### (2) Compaction
<pre> allocator> C</pre>
picture Compaction.
### (3) Show state
<pre> allocator> STAT</pre>
picture showstate
### (4) Exit
<pre> allocator> X</pre>
picture Exit

##3. Design of the program
###(1)Data structure
For each memory block, it can be illustrated as the following structrue:
<pre> typedef struct block{
    char *name ;
    size_t start;
    size_t end;
    struct block *next;
}mm_block;</pre> 
This structure shows that each block's start, end, and its owner process. The memory structure is organized in linkedlist. 
### (2)Specific functions
#### a. Allocation
First, we use <b>First-fit</b>policy to find whether the unused block in the memory can satisify our allocation demand. If we can find the demanding block, we record the pre-allocated block information——the block's size and the block start. This information is prepared for the policy-based allocation. If we can not find the demanded block,we can just exit the function and return a allocation-failed message. 
Second, if we can identify that there is an unused block that can satisfy our demand. Next we will consider about the policy and select the block based on the specific policy. Different policy will be illustrated in the following statements.
##### i. First policy
In order of the address, we search the needed block in the address order, if we get the fitted block, we end our search.
##### ii. Best policy
We sort the unused block with the capacity. Then we select the fitted block.
##### iii. Worst policy
We sort the unused block with the inversed capacity. Then we select the largest-capacity fitted block for future usage.
#### b. Release
In a linkedlist, when we release an used block, functioning to the data structure, it is the linklist node deletion. What we should consider is the head node, if we delete the head node, we should switch the head node with the next node of the head node.
#### c. Compaction
Also, Compaction is that if the next node is available, let the next node block's start equals to the previous node end + 1 and keep the original block size.
####d. Exit
Just exit.

## 4. Problems encountered and solution
### (1) Linkedlist
Not so familiar with the linkedlist, thus pay much time.
### (2) Several bound situations
We forgot to consider lots of bound conditions into the program. The head node is the most difficult for me. Because almost every operation to node will change the head pointer.

## 5. Memory management methods
### (1) Static allocation
### (2) Dynamic allocation
First fit, best fit, next fit, worst fit.
### (3) Buddy system

## 6. Reference
[1]Makefile:https://blog.csdn.net/haoel/article/details/2886/
[2]Three different kinds of fit: https://www.jianshu.com/p/7bbddfd42b9c
