#include <iostream>
#include  <iomanip>

using namespace std;

#define	DBG	1
#define	DRAM_SIZE	(64*1024*1024)
#define	CACHE_SIZE	(64*1024)


enum cacheResType { MISS = 0, HIT = 1 };

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05786902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE / 4);
}

unsigned int memGenB()
{
	static unsigned int addr = 0;
	return  rand_() % (64 * 1024);
}

unsigned int memGenC()
{
	static unsigned int a1 = 0, a0 = 0;
	a0++;
	if (a0 == 512) { a1++; a0 = 0; }
	if (a1 == 128) a1 = 0;
	return(a1 + a0 * 128);
}

unsigned int memGenD()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 4);
}

unsigned int memGenE()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 16);
}

unsigned int memGenF()
{
	static unsigned int addr = 0;
	return (addr += 64) % (128 * 1024);
}

unsigned int memGenTest1()
{
	static unsigned int addr = 0;
	return (addr += 2);
}

unsigned int memGenTest2()
{
	static unsigned int addr = 0;
	return (addr += 2) % (16 * 1024);
}

unsigned int memGenTest3()
{
	static unsigned int counter = 0;
	counter++;
	switch (counter)
	{
	case 1:return 0;
	case 2:return 4;
	case 3:return 16;
	case 4:return 132;
	case 5:return 232;
	case 6:return 160;
	case 7:return 1024;
	case 8:return 30;
	case 9:return 140;
	case 10:return 3100;
	case 11:return 180;
	case 12:return 2180;
	}
}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr, int CacheLineSize, int NumberOfLines, int cache[][2], int& index)
{
	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit
	unsigned int shift, tag, temp;
	index = (addr >> (unsigned int)log2(CacheLineSize)) % NumberOfLines;
	temp = NumberOfLines + CacheLineSize;
	shift = log2(temp);
	tag = addr >> shift;
	if (cache[index][0] == tag)
		return HIT;
	else
	{
		cache[index][0] = tag;
		if (cache[index][1] == 0)
			cache[index][1] = 1;
		return MISS;
	}
}

const char* msg[2] = { "Miss","Hit" };

#define	NO_OF_Iterations	1000000
void main()
{
	int idx;
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);
	float hit = 0;
	cacheResType r;
	int CacheLineSize = 32;
	int NumberOfLines = 32; //CACHE_SIZE / CacheLineSize;
	unsigned int addr;
	int cache[CACHE_SIZE][2];// the 100000 represents the maximum number of blocks
	cout << "Direct Mapped Cache Simulator\n";
	for (int i = 0; i < CACHE_SIZE; i++)
	{
		cache[i][0] = -1;
		cache[i][1] = 0;
	}
	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGenF();
		r = cacheSimDM(addr, CacheLineSize, NumberOfLines, cache, idx);
		if (r == HIT)
			hit++;
	}
	cout << "Hit ratio = " << dec << (100 * hit / NO_OF_Iterations) << " %" << endl;
	system("pause");
}