#include <iostream>
#include  <iomanip>
#include <math.h>

//#include "map.h"
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)	// Change for different sizes
#define        CACHE_LINE_SIZE        (4)

int blocknum=CACHE_SIZE/ CACHE_LINE_SIZE; //number of blocks
int indexbits=log2(blocknum);      // number of index bits
int word_offset=(log2(CACHE_LINE_SIZE/4));   //offset word by using log2 to the line size divided by 4

enum cacheResType {MISS=0, HIT=1};

// making a struct for the block
struct block{
   bool valid;
    int tag;
    int bit;
};
//block *cache;
block *cache=new block [blocknum]; // making array of blocks


/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36119 * (m_z & 65535) + (m_z >> 16);
    m_w = 16000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGen2()
{
	static unsigned int addr=0;
	return (addr++)%(DRAM_SIZE);
}

unsigned int memGen3()
{
	static unsigned int addr=0;
	return  rand_()%(64*1024);
}

unsigned int memGen1()
{
	return rand_()%(DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr=0;
	return (addr++)%(1024*4);
}

unsigned int memGen5()
{
	static unsigned int addr=0;
	return (addr++)%(1024*64);
}

unsigned int memGen6()
{
	static unsigned int addr=0;
	return (addr+=64)%(128*1024);
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
   
//    cout<<byte_offset;
    int mask1=(pow(2,indexbits)-1);  //used for masking
          int indexDC =(addr>>(2+word_offset)) & mask1 ; //index
//     int byteoffset= addr &3;
//    int mask2=(pow(2,word_offset)-1);
//    int wordoffset =(addr>>2) & mask2;
//    int indexDC=(addr>>(word_offset+2))%blocknum;
    
      int tagDC = (addr >> (2+word_offset+indexbits));  // tag

	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit
   
    //checking for miss or hit
    if(cache[indexDC].valid ==true  && cache[indexDC].tag==tagDC ) //checkin if the valid bit and the tag are equal
        
    
    {
//        cout<<indexDC<<endl<<cache[indexDC].valid<<endl<<cache[indexDC].bit<<endl;
        return HIT;
    }
//
else{
//    cout<<indexDC<<endl<<cache[indexDC].valid<<endl<<cache[indexDC].bit<<endl;  //used for vaildation
 
        
        cache[indexDC].valid=true;            // making the valid bit =1
                 cache[indexDC].tag=tagDC; // adding the tag inside the array
//           cache[indexDC].bit=wordoffset;
   
    
       

		return MISS;
	
}
}
//
//#ifdef SIM_FA
//// Fully Associative Cache Simulator
//cacheResType cacheSimFA(unsigned int addr)
//{
//	// This function accepts the memory address for the read and
//	// returns whether it caused a cache miss or a cache hit
//	if (famap.search(addr)==true)
//		return HIT;
//
//	else{
//		famap.fadd_addr(addr);
//		return MISS;
//
//	}
//}
//#endif

char *msg[2] = {(char *)"Miss",(char *)"Hit"};

#define		NO_OF_Iterations	1000000// Change to 1,000,000
int main()
{
    for(int i=0;i<blocknum;i++){
        cache[i].bit =0;
    }
	unsigned int hit = 0;
	cacheResType r;
	
	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";

	for(int inst=0;inst<NO_OF_Iterations;inst++)
	{
		addr = memGen4();
		r = cacheSimDM(addr);
		if(r == HIT) hit++;
		cout <<"0x" << setfill('0') << setw(8) << dec << addr <<" ("<< msg[r] <<")\n";
	}
	cout << "Hit ratio = " << (100*hit/NO_OF_Iterations)<< endl;
    cout << "miss ratio = " << 100-(100*hit/NO_OF_Iterations)<< endl;
	cout<<"number of misses "<<1000000-hit<<endl;
}
