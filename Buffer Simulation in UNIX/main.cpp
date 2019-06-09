#include "Buffer.h" 	//Buffer.h is a user defined header file that initialises the structure of a Buffer.
#include "BufferCache.h"	//BufferCache.h is a user defined header file that maintains the structure of HashQueue and Freelist.
#include<string.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include <signal.h>
#include<fstream>

#define SIGAL 1 	//For signal call to allocate Buffer

using namespace std;
ifstream fd2;				//A read file descriptor to open block.csv.
BufferCache *freeList;		//An object of the BufferCache Class to maintain the Free list
BufferCache **hashArray;	//An Object of the BufferCache Class to maintain the Hash Queues

int hashValue(int blockNumber)
{ 	/*
		Objective - To find the Hash Value of a given Block number.
		Input - A Block number.
		Output - A Hash Value.

	*/
    	int hashValue = 0;
    	hashValue = blockNumber % 4;	
    	return hashValue;
}
	
void display()
{	/*
		Objective - To display the Hash Queue and Freelist.
		Input - None.
		Output - None.

	*/
		cout<<"\n------------------------------------------------"<<endl;
    	cout<<"Hash Queue Details : \n";
    	for(int i = 0;i < 4;i++)
    	{
        	hashArray[i]->displayQueue('H');
    	}
    	cout<<"\n------------------------------------------------"<<endl;
     	cout<<"Free List Details :\n";
		freeList->displayQueue('F');
}
		
Buffer* getblk(int blockNumber)
{	/*
		Objective - To return a Buffer that can be allocated to the given Block number
		Input - A Block number.
		Output - A Buffer.

	*/
    	Buffer* allocatedBuffer = NULL;
    	int hashKey = hashValue(blockNumber);
    	Buffer* blockBuffer = hashArray[hashKey]->findBuffer(blockNumber,'H');
    
    	if(blockBuffer != NULL) 
    	{      //scenario 1 (Buffer is on Hashqueue and not locked)
    			if(blockBuffer->status == Free)
				{
						cout<<"\n\nScenario 1 -- BUFFER "<<blockNumber<<" FOUND :)"<<endl;
            			allocatedBuffer = blockBuffer;
            			if(freeList->removeBuffer(blockNumber,'F'))
            				allocatedBuffer->status = Locked;
        		}
	        	//scenario 5 (Process waits for THIS buffer)
        		else if(blockBuffer->status==Locked) 
        		{
        				cout<<"\n\nScenario 5 --Process going to sleep :/ for block number "<<blockNumber<<endl;
        				//make a process sleep on event THIS buf becomes free
        				blockBuffer->status=ProcWait;
        				sleep(2);
						//return NULL;
				}
    	}
    	else
    	{      //scenario 2(Buffer is not in BufCache, get from freelist)
       			Buffer* freeBuffer = freeList->getBuffer_From_Head_Of_Freelist();
       			if(freeBuffer != NULL)
				{
          				if(freeBuffer->status != DelWr)
						{
							cout<<"\n\nScenario 2 -- buffer "<<blockNumber<<" from free list"<<endl;
            				allocatedBuffer = freeBuffer;
            				freeBuffer->status=Locked;
            				allocatedBuffer->status=Locked;
							hashArray[hashKey]->Insertion_at_Tail(freeBuffer,'H');
            			}
            			else //scenario 3 (marked for delayed write)
            			{
            				cout<<"\n\nscenario 3 - delayed write"<<endl;
            				cout<<"\nWriting the data to disk...";
            				cout<<"\n"<<freeBuffer->Data<<endl;
            				//make it sleep (writing to disk..)
            				//sleep(2);
            				//add buf st head of freelist
            				freeBuffer->status=Free;
            				freeList->Insertion_at_Head_Of_FreeList(freeBuffer);
						}
        		}
        		else //scenario 4 (Nobuffer in free list)
        		{
        				cout<<"\n\nScenario 4--NO BUFFER IN FREE LIST :( PROCESS WILL SLEEP."<<endl;
        				//make the process sleep 
        				sleep(3);
						//return NULL;
				}
    	}
    return allocatedBuffer;
}

bool brelse(int blockNumber)
{		/*
		Objective - To release a Buffer with the given Block number.
		Input - A Block number.
		Output - True/False.

		*/
    	bool blockFreed = false;       //To return if a block is freed or not
  		int hashKey = hashValue(blockNumber);
    	Buffer* block = hashArray[hashKey]->findBuffer(blockNumber,'H');
    	if(block != NULL)
		{
        	blockFreed = true;
        	block->status = Free;  //Change the status of the Freed Block
        	freeList->Insertion_at_Tail(block,'F');
    	}
    	return blockFreed;
}

//Update Data in Buffer
void updateBuffer(Buffer* buffer,int blockNumber,string data, int st=Locked)
{		/*
		Objective - To update the Buffer values with the values passed as parameters.
		Input - A Buffer,Block number,Data,Status of the Buffer(Locked is taken as the Default Status).
		Output - None.

		*/
    	buffer->blockNumber = blockNumber;
    	buffer->Data = data;
    	buffer->status = st;
}


void allocate(int signal_num)
{		/*
		Objective - To allocate a buffer to a Child Process when It generates a request.
		Input - A signal Number.
		Output - None.

		*/   
		int pid;			//To store the pid of the Child Process that requested the Block.
    	int blocknum;		//To store the Block number requested.
   		static int count=1;		//A static Variable used for random function calls.
    	count++;
		//cout<<"\ncount="<<count;

		
		//File read CSV file to get blocknum.
		string s;			//A string to store the line read from csv file.
		getline(fd2, s, '\n');
		pid=stoi(s.substr(0,4));
		blocknum=stoi(s.substr(5,s.length()));
	
		Buffer *b=getblk(blocknum);
	
		if(b!=NULL)		//If getblk return a  Buffer.
		{
				cout<<"\nBuffer "<<blocknum<<" allocated to process "<<pid<<endl;
				updateBuffer(b,blocknum,"Some Data");		//Update the Buffer details in Buffer.
				if(count%3==0)
				{
					//cout<<"\ncount = "<<count<<endl;
					sleep(2);
					if(brelse(blocknum))		
						cout<<"\nBuffer "<<blocknum<<" released"<<endl;
				}
				/*else if(count%3==0)
						updateBuffer(b,blocknum,"Some Data", DelWr);
				else
						updateBuffer(b,blocknum,"Some Data");*/
		}	
		else
		{       //For the unfulfilled requests write the request back to the CSV file.
				//cout<<"BUFFER UNAVAILABLE"<<endl;
		
				/*ofstream f("block.csv", ios::app);
				cout<<"\npid ="<<pid<<"  block ="<<blocknum;
				f<<pid<<","<<blocknum<<"\n";
				cout<<"\nRequest re-written to file."<<endl;*/
		}
		display();
		signal(SIGAL, allocate);		//To catch the signal from the Child Process.	
}

int main()
{
 		ofstream fd1;			//A write File Descriptor for block.csv.
	
	
  		fd1.open("block.csv");		//open CSV file in Write mode.
  		fd2.open("block.csv");		//open CSV file in Read mode.

		signal(SIGAL, allocate);
	
		int pid1=fork();		//To create a child process.
		int pid2=fork();		//to create a child process.

		int i=0;
		srand(time(NULL));
		while(i<=30)
		{
				
				if(pid1==0 && pid2>0)
				{
					//first child
					//cout<<"\nFirst child";
					int blocknum=rand()%50 +1;
					int p=getpid();
					fd1<<p<<","<<blocknum<<endl;
					//cout<<"\nProcess "<<p<<" requested : "<<blocknum<<endl;
					kill(getppid(), SIGAL);		//Signal to Call the Allocate Function.
					sleep(4);
		
				}
				else if(pid1>0 && pid2==0)
				{
					//cout<<"\nSecond child";			
					int blocknum=rand()%50 +1;
					int p=getpid();
					fd1<<p<<","<<blocknum<<endl;
					//cout<<"\nProcess "<<p<<" requested : "<<blocknum<<endl;
					kill(getppid(), SIGAL);		//Signal to Call the Allocate Function.
					sleep(4);
		
				}
				else if(pid1==0 && pid2==0)
				{
					//third child
					//cout<<"\nThird child.";
					int blocknum=rand()%50 +1;
					int p=getpid();
					fd1<<p<<","<<blocknum<<endl;
					//cout<<"\nProcess "<<p<<" requested : "<<blocknum<<endl;
					kill(getppid(), SIGAL);		//Signal to Call the Allocate Function.
					sleep(4);		
				}
				else
				{
					//Parent process
					//sleep(2);
			
					//cout<<"\nParent process..\n";
					hashArray = new BufferCache*[4];
    				for(int i=0; i < 4; i++)		//Initialise the Hash Array with Default Values
    				{
						hashArray[i] = new BufferCache();
    				}
  					freeList = new BufferCache();
   					for(int i = 0;i<4; i++)			//Initialise the Free List with Default Values.
					{
						freeList->Insertion_at_Head_Of_FreeList(new Buffer());	
    				}
    		
				while(1)		//An infinite loop so that the Parent Process keeps on running.
				{  
				
				};
				}
			i++;
	};
	
	return 0;
}
