# Buffer-Simulation-in-UNIX

The program intends to simulate the Buffer management system in Unix Operating System.

Operating System used - Ubuntu
Programming Language  - C++


### HEADER FILE - BUFFER.h

It defines the basic structure of a buffer in the buffer cache.
The defined integer constants are the possible values that the 'status' of the buffer may take-
* Free	 - the buffer is free	
* Locked- - the buffer is being used by a process
* Invalid- the buffer has invalid data
* DelWr	 - the buffer is marked for delayed write
* ProcWait - a process is waiting for this buffer to become free.

The structure of a buffer comprises of:
* blockNumber	- the disk block number of which it has content
* status- the status of the buffer	
* hash_next- pointer to next buffer on the hash queue
* hash_prev- pointer to the previous buffer on hash queue
* free_next- pointer to the next free buffer on free list	
* free_prev- pointer to the previous free buffer on free list

The possible functions of a buffer-
* a default constructor
* a parameterized constructor
* a display function (to display details about the buffer



### HEADER FILE - BufferCache.h

This file includes a class - BufferCache that implements the functionality of a buffer cache.

**Its data members are-**
head and tail - both of Buffer type, to mark the head and tail of the linked list

**The member functions are-**
* constructor- to initialize head and tail of the linked list to NULL
* isEmpty- to check if the list is empty or not
* displayQueue- to display the list (hash queue or free list)
* findBuffer- finds the said buffer in hash queue/free list and return it
* searchBuffer- checks if the said buffer is in the hash queue/free list
* Insertion_at_Head_Of_FreeList	- to insert the buffer at the head of free list (in case of delayed write)
* Insertion_at_Tail- to insert the buffer at the tail of freelist (when a buffer becomes free) or hash queue 
                                   (to place it in the corresponding hash queue)
* getBuffer_From_Head_Of_Freelist- removes and returns a buffer from the head of free list if possible
* removeBuffer- to remove a buffer from hash queue/free list


### MAIN FILE - main.cpp

**This program simulates the buffer management system.**

**Functions-**
* hashValue- takes as input the block number and returns its hash value, i.e, on which has queue should it be placed
* display- to display the hash queue and the free list
* getblk- takes as input a block number, returns the block if available, implements other scenarios as applicable
* brelse- takes as input a block number, returns true if that block was successfully freed; false otherwise
* updateBuffer- updates data on a buffer block
* allocate- the signal handler function for allocation request signal, SIGAL
	

**Flow of control-**
The main() opens a file, block.csv, with 2 different file descriptors - one read descriptor and 1 write descriptor. 
It then does a signal() system call to catch an allocate request from the child and will call the allocate() signal handler function.
The fork() system call is done twice, in order to produce 3 child processes and a parent process.
The parent process initializes the free list and the hash queues and runs infinitely.
Each of the 3 child process will generate a random block number, write its request in block.csv and signal the parent to allocate
 it that block.The parent then reads the csv and tries to allocate a block by invoking the getblk() function. 


## PROBLEMS FACED
* To somehow achieve the master slave relationship - i.e., the child must not be able to access the buffer data structures.
* To decide and execute where to release the buffer allotted, i.e, calling brelse().
* Achieving delayed write and race condition scenarios.
