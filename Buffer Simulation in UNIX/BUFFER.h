#ifndef Buffer_H    //So that buffer.h is included only once when compiling main
#define Buffer_H

#include <iostream>
#include<string.h>
using namespace std;
const int Free     = 0;		//Buffer is free
const int Locked   = 1;		//Buffer is in use by a process
const int Invalid  = 2;		//Data in buffer is invalid
const int DelWr    = 3;		//Buffer marked for delayed write
const int ProcWait = 4;		//A process is waiting for this buffer

//struct of a buffer in the buffer cache
struct Buffer{
    int blockNumber;			//Disk block number it corresponds to
    int status;					//Status of buffer
    string Data;				//Data in buffer
    struct Buffer* hash_next;	//Pointer to next buffer in hash queue
    struct Buffer* hash_prev;	//Pointer to prev buffer in hash queue
    struct Buffer* free_next;	//Pointer to next buffer in free list
    struct Buffer* free_prev;	//Pointer to prev buffer in free list
public:
    Buffer()
    {   
		/*  
            Objective - To initialize structure members.
			Input - None.
			Output - None.
			
		*/
		blockNumber = 0;		
        status = Free;
        Data = "";
        hash_next = hash_prev= 0;
        free_next = free_prev = 0;
    }

    Buffer(int blockNumber, int status = Free, string Data = " ", Buffer* hash_next = 0, Buffer* hash_prev = 0, Buffer* free_next = 0, Buffer* free_prev = 0)
	{
		/*
            Objective - To initialize structure members with given parameters.
			Input - Buffer Parameters.
			Output - None.
			
		*/        
		
		this->blockNumber = blockNumber;
        this->status = status;
        this->Data = Data;
        this->hash_next = hash_next;
        this->hash_prev = hash_prev;
        this->free_next = free_next;
        this->free_prev = free_prev;
    }
    
	void display()
	{
		/*
            Objective - To display buffer details.
			Input - None.
			Output - None.
			
		*/
        cout<<"Block Number : ";
        if(blockNumber != 0)
		{
            cout<<blockNumber<<endl;
        }
        else
		{
            cout<<"Buffer Not Allocated\n";
        }

        cout<<"Status Of Buffer: ";
        if(status == 0){
            cout<<"Buffer Not Allocated\n";
        }else if(status == 1){
            cout<<"Buffer Currently Locked\n";
        }else if(status == 2){
            cout<<"Buffer Data Invalid\n";
        }else if(status == 3){
            cout<<"Buffer Marked Delayed Write\n";
        }else if(status == 4){
            cout<<"Process Waiting for Buffer\n";
        }
        cout<<"Data : "<<Data<<endl;
    }
};
#endif
