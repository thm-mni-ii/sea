#include "sealib/compactarray.h"
#include <stdio.h>

void CompactArray::insert(unsigned int i,unsigned int p) {
	//values per group: 3/e, groups per data: dataWidth/groupWidth
	if(i>=valueCount) return;
	int groupIndex=(int)floor(i/(double)(3/e));
	int dataIndex=(int)floor(groupIndex/(dataWidth/(double)groupWidth));
	int groupOffset=(int)fmod(groupIndex,dataWidth/(double)groupWidth);
	int valueOffset=(int)fmod(i,3/e);
	//insert p into slot 'data X + group Y + value Z'
	//data[dataIndex]=~0;
	unsigned a=data[dataIndex];
	#ifdef COMPACTARRAY_DEBUG
	printf("before: %08x, ",a);
	#endif
	unsigned b1=(unsigned)pow(2,valueWidth)-1,
		b2=(dataWidth-valueWidth)-groupOffset*groupWidth-valueOffset*valueWidth;
	unsigned b=b1<<b2;
	#ifdef COMPACTARRAY_DEBUG
	printf(" shifting %08x %d to the left: %08x... ",b1,b2,b);
	#endif
	unsigned c=a&~b;
	#ifdef COMPACTARRAY_DEBUG
	printf(" masking %08x with %08x: %08x... ",a,~b,c);
	#endif
	unsigned d=c|(p<<b2);
	#ifdef COMPACTARRAY_DEBUG
	printf(" melding %08x with %08x: %08x... ",c,p<<b2,d);
	#endif
	data[dataIndex]=d;
	#ifdef COMPACTARRAY_DEBUG
	printf("after: %08x\n",data[dataIndex]);
	printf("offs(%u)=%d.%d.%d\t%u\n",i,dataIndex,groupOffset,valueOffset, p);
	#endif
}

unsigned int CompactArray::get(unsigned int selector,unsigned int i) {
	switch(selector) {
		case COMPACTARRAY_DATA:
			if(i>dataCount) return COMPACTARRAY_FAULT;
			else return data[i];
		case COMPACTARRAY_GROUP:
			//...

		default: return COMPACTARRAY_FAULT;
	}
}
unsigned int CompactArray::get(unsigned int i) {
	return get(COMPACTARRAY_VALUE,i);
}

CompactArray::CompactArray(unsigned int count,double epsilon) {
	e=epsilon;
	//the following is valid if the inserted values can have 3 states:
	valueCount=count;
	valueWidth=(int)ceil(log(3)/log(2));
	groupWidth=(int)ceil(3*(log(3)/log(2))/e); //bits for a group of 3/e (e.g. 2) consec. colors
	groupCount=(int)ceil(count/(groupWidth/(double)valueWidth));
	dataWidth=8*sizeof(unsigned int);
	dataCount=(int)ceil((groupCount*groupWidth)/(double)dataWidth);
	#ifdef COMPACTARRAY_DEBUG
	printf("vw=%d, gw=%d, gc=%d, dc=%d\n",valueWidth,groupWidth,groupCount,dataCount);
	#endif
	data=new unsigned int[dataCount];
}

CompactArray::~CompactArray() {
	delete[] data;
}
