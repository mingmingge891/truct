/*---------------------------------------
-* File:    malloc
-* AUTHOR£º kite
-* E_MAIL:  1216104245@qq.com
-* TIME£º   2017/03/06
-* VERSION£ºV1.0
-* WARNING£º
-*---------------------------------------*/

#include "malloc.h"	    

__align(8) unsigned char membase[MEM_SIZE];//ÄÚ´æ³Ø(8×Ö½Ú¶ÔÆë)
//ÄÚ´æ¿ØÖÆ¿é
unsigned short	int mmt[MMTS];//ÄÚ´æ¹ÜÀí±í

void malloc_init(void)
{
	unsigned int i=0;
	for(i=0;i<MMTS;i++)
	{
		mmt[i]=0;
	}
}

void * my_malloc(unsigned int size)
{
	unsigned int need_blocks=0;//ÐèÒªµÄÄÚ´æ¿éÊý
	unsigned int free_blocks=0;//Á¬Ðø¿ÕÄÚ´æ¿éÊý
	unsigned int blocks=0;//
	unsigned int i;
	
	need_blocks=size /MEM_BANK_MIN_SIZE;
	if(size%MEM_BANK_MIN_SIZE)
		need_blocks++;
	for(i=0;i<MMTS;i++)
	{
		if(mmt[i]==0)
			free_blocks++;
		else
			free_blocks=0;
		if(need_blocks==free_blocks)//ÕÒµ½ÁËÁ¬Ðøneed_blocks¸ö¿ÕÄÚ´æ¿é
		{
			i=(i+1)-free_blocks;
			for(blocks=i;blocks<(i+free_blocks);blocks++)
			{
				mmt[blocks]=1;//±ê¼ÇÎªÕ¼ÓÃ
			}
			mmt[i]=need_blocks;//´æ·ÅÕ¼ÓÃµÄ´óÐ¡
			return (membase+i*MEM_BANK_MIN_SIZE);//·µ»Ø»ñÈ¡µ½µÄµØÖ·
		}
	}
	return 0x00000000L;
}

unsigned char my_free(void *mem_pointer)
{
	unsigned int free;
	unsigned int num;
	unsigned int i;
	free=(unsigned int)mem_pointer-(unsigned int)&membase[0];
	free/=MEM_BANK_MIN_SIZE;
	num=mmt[free];
	for(i=free;i<free+num;i++)
	{
		mmt[i]=0;
	}
	return 1;
}
