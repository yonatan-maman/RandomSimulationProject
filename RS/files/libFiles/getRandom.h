#ifndef _getRandom
#define _getRandom
#include <stdlib.h>
#include <limits.h>
#include <typeinfo>
#define IS_SAME(type1,type2)    typeid(type1).hash_code() == typeid(type2).hash_code()


template <typename T>
T getRandomOfSpecificType()
{
	if (IS_SAME(T,bool))
			return (rand() %2) == 0;
	else if (IS_SAME(T,char))
		return rand()%(1+CHAR_MAX-CHAR_MIN) + CHAR_MIN;
	else if(IS_SAME(T,unsigned char))
		return rand()%(1+CHAR_MAX-CHAR_MIN) + 0;
	else if(IS_SAME(T,short))
		return (short)rand()% (SCHAR_MAX-SCHAR_MIN) + SCHAR_MIN;
	else if(IS_SAME(T,unsigned short))
		return (unsigned short)rand()% (SCHAR_MAX-SCHAR_MIN) + 0;
	else if(IS_SAME(T,int))
		return rand();
	else if(IS_SAME(T,unsigned int))
		return (unsigned int)(rand() + INT_MAX);
	return 0;
	//else if(IS_SAME(T,long))
		//case(unsigned long_):
		//case(float):
	//else if(IS_SAME(T,double))
	//	return ((double)rand()/INT_MAX)*(DBL_MAX-DBL_MIN) + DBL_MIN;
	//	return 0;
	//}

}

#endif
