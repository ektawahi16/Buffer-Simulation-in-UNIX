#include "BUFFER.h"  //Buffer.h is a user defined header file that initialises the structure of a Buffer.
#include <iostream>
using namespace std;

class BufferCache
{
    Buffer* head;		//Head of linked list
    Buffer* tail;		//Tail of linked list
	public:
    BufferCache()
    {
		/*
            Objective - To initialize head and tail to NULL.
			Input - None.
			Output - None.
			
		*/
    	head=tail=NULL;
	}
    
	bool isEmpty()
	{
		/*
            Objective - To check if list is empty or not.
			Input - None.
			Output - True/False.
			
		*/
        bool empty = false;
        if(head == NULL)
		{
            empty = true;
        }
        return empty;
    }
    
    void displayQueue(char Type)
	{
        /*
            Objective - To display the Hash Queue of Free list.
			Input - Type of linked list (Hash Queue or Free list).
			Output - None.
			
		*/
		Buffer* temp = head;
        if(!isEmpty()){
            do{
                cout<<"("<<temp->blockNumber<<" "<<temp->status<<")\n";
                if(Type=='F')
                	temp = temp->free_next;
                else if(Type=='H')
                	temp = temp->hash_next;
            }while(temp != head);
        }
    }

    Buffer* findBuffer(int blockNumber,char Type)
	{
		/*
            Objective - To find the Buffer with the given Block number and return it.
			Input - Block number and type of list(Hash Queue or Free list).
			Output - A Buffer.
			
		*/
        Buffer* foundBuffer = NULL;  
        if(!isEmpty())
		{
            Buffer* temp = head;
            do
			{
                if(temp->blockNumber == blockNumber)
				{
                    foundBuffer = temp;
                    break;
                }
                if(Type == 'H')
				{
                    temp = temp->hash_next;
                }
                else if(Type == 'F')
				{
                    temp = temp->free_next;
                }
            }while(temp != head);
        }
        return foundBuffer;
    }
    
    bool searchBuffer(int blockNumber,char Type)
	{
		/*
            Objective - To check if a Buffer with the given blocknumber is in the list.
			Input - Block number and type of list(Hash queue or Free list).
			Output - True/False.
			
		*/
        bool searchedBuffer = true;
        if(findBuffer(blockNumber,Type) == NULL)
            searchedBuffer = false;
        return searchedBuffer;
    }

    void Insertion_at_Head_Of_FreeList(Buffer* newBuffer)
	{
		/*
            Objective - To add the Buffer at the head of free list.
			Input - A Buffer.
			Output - None.
			
		*/
        if(head == NULL)
		{
            newBuffer->free_next = newBuffer->free_prev = newBuffer;
            head = tail = newBuffer;
        }
		else
		{
            newBuffer->free_next = head;
            newBuffer->free_prev = tail;
            tail->free_next = head->free_prev = newBuffer;
            head = newBuffer;
        }
    }

     void Insertion_at_Tail(Buffer* newBuffer,char Type)
	 {
	 	/*
            Objective - To add the given buffer at the tail of list.
			Input - A Buffer and type of list(Hash Queue or Free list).
			Output - None.
			
		*/
        if(head == NULL)
		{
            if(Type == 'H')
			{
                newBuffer->hash_next = newBuffer->hash_prev = newBuffer;
            }
			else if(Type == 'F')
			{
                newBuffer->free_next = newBuffer->free_prev = newBuffer;
            }
            head = tail = newBuffer;
        }
		else
		{
            if(Type == 'H'){
                newBuffer->hash_next = tail;
                newBuffer->hash_next = head;
                head->hash_prev = tail->hash_next = newBuffer;
                tail = tail->hash_next;
            }else if(Type == 'F'){
                newBuffer->free_prev= tail;
                newBuffer->free_next= head;
                head->free_prev = tail->free_next = newBuffer;
                tail = tail->free_next;
            }
        }
    }

     Buffer* getBuffer_From_Head_Of_Freelist()
	 {
	 	/*  
            Objective - To remove a buffer from head of Free list and return it.
			Input - None.
			Output - A Buffer.
			
		*/
        Buffer* removed = NULL;
        if(!isEmpty()){
            if(head == head->free_next)
			{ //Only Buffer in the Linked List
                removed = head;
                head = tail = NULL;
            }
			else
			{
                tail->free_next = head->free_next;
                head->free_next->free_prev = head->free_prev;
                removed = head;
                head = head->free_next;
            }
        }
        return removed;
    }

     bool removeBuffer(int blockNumber,char Type)
	 {
	 	/*  
            Objective- To remove the Buffer with given block number from list.
			Input - Block number and type of list(Hash Queue or Free list).
			Output - True/False.
			
		*/
        bool removed = false;
        Buffer* temp = findBuffer(blockNumber,Type);

        if(Type == 'H')
		{
        //Hash Queue Removal
            if(temp->hash_next == temp)
			{ //Only Buffer in Linked List
                head = tail = NULL;
            }
			else
			{
                if(temp == head)
				{ //If Head Buffer is the Buffer toBeRemoved from Linked List
                    head = head->hash_next;
                }
				else if(temp == tail)
				{ //If Tail Buffer is the Buffer toBeRemoved from Linked List
                    tail = tail->hash_prev;
                }
                temp->hash_prev->hash_next= temp->hash_next;
                temp->hash_next->hash_prev = temp->hash_prev;
            }
        }else if(Type == 'F')
		{
        	//Free List Removal
            if(temp->free_next == temp)
			{ //Only Buffer in Linked List
                head = tail = NULL;
            }
			else
			{
                if(temp == head)
				{ //If Head Buffer is the Buffer toBeRemoved from Linked List
                    head = head->free_next;
                }
				else if(temp == tail)
				{ //If Tail Buffer is the Buffer toBeRemoved from Linked List
                    tail = tail->free_prev;
                }
                temp->free_prev->free_next = temp->free_next;
                temp->free_next->free_prev = temp->free_prev;
            }
        }
        return removed;
    }
};
