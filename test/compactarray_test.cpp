#include <gtest/gtest.h>
#include <sealib/compactarray.h>

TEST(CompactArrayTest,insert) {
	CompactArray *a=new CompactArray(1500,1.5);
	for(int b=0; b<50; b++) a->insert(b,3);
	ASSERT_EQ(a->get(COMPACTARRAY_DATA,0),~0);
	a->insert(1,2); //flip 4th bit from left
	ASSERT_EQ(a->get(COMPACTARRAY_DATA,0),0xEFFFFFFF);
	//ASSERT_EQ(a->get(COMPACTARRAY_GROUP,0),0xEF);
	a->insert(4,0); //flip upper half of 3rd nibble
	ASSERT_EQ(a->get(COMPACTARRAY_DATA,0),0xEF3FFFFF);
	delete a;
}