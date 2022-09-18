#include <unity.h>
#include <vector>

#include "interp.hpp"

void setUp(void) 
{
	// set stuff up here
}

void tearDown(void) 
{
	// clean stuff up here
}

int main( int argc, char **argv) 
{
	UNITY_BEGIN();
	//							  0  1  2  3   4   5
	std::vector<float> headers = {3, 4, 5, 10, 20, 21};

	TEST_ASSERT_TRUE(headers.begin()+0 == left_bound(headers.begin(), headers.end(), -1.0f));
	TEST_ASSERT_TRUE(headers.begin()+0 == left_bound(headers.begin(), headers.end(), 0.0));
	TEST_ASSERT_TRUE(headers.begin()+0 == left_bound(headers.begin(), headers.end(), 3.5));
	TEST_ASSERT_TRUE(headers.begin()+1 == left_bound(headers.begin(), headers.end(), 4.0));
	TEST_ASSERT_TRUE(headers.begin()+1 == left_bound(headers.begin(), headers.end(), 4.1));
	TEST_ASSERT_TRUE(headers.begin()+1 == left_bound(headers.begin(), headers.end(), 4.9999));
	TEST_ASSERT_TRUE(headers.begin()+2 == left_bound(headers.begin(), headers.end(), 5.0));
	TEST_ASSERT_TRUE(headers.begin()+2 == left_bound(headers.begin(), headers.end(), 6.0));
	TEST_ASSERT_TRUE(headers.begin()+2 == left_bound(headers.begin(), headers.end(), 9.9999));
	TEST_ASSERT_TRUE(headers.begin()+4 == left_bound(headers.begin(), headers.end(), 20.0));
	TEST_ASSERT_TRUE(headers.begin()+4 == left_bound(headers.begin(), headers.end(), 20.9999));
	TEST_ASSERT_TRUE(headers.begin()+5 == left_bound(headers.begin(), headers.end(), 21.0));
	TEST_ASSERT_TRUE(headers.begin()+5 == left_bound(headers.begin(), headers.end(), 22.0));

	TEST_ASSERT_TRUE(headers.begin()+0 == right_bound(headers.begin(), headers.end(), -1.0f));
	TEST_ASSERT_TRUE(headers.begin()+0 == right_bound(headers.begin(), headers.end(), 0.0));
	TEST_ASSERT_TRUE(headers.begin()+1 == right_bound(headers.begin(), headers.end(), 3.5));
	TEST_ASSERT_TRUE(headers.begin()+1 == right_bound(headers.begin(), headers.end(), 4.0));
	TEST_ASSERT_TRUE(headers.begin()+2 == right_bound(headers.begin(), headers.end(), 4.1));
	TEST_ASSERT_TRUE(headers.begin()+2 == right_bound(headers.begin(), headers.end(), 4.9999));
	TEST_ASSERT_TRUE(headers.begin()+2 == right_bound(headers.begin(), headers.end(), 5.0));
	TEST_ASSERT_TRUE(headers.begin()+3 == right_bound(headers.begin(), headers.end(), 6.0));
	TEST_ASSERT_TRUE(headers.begin()+3 == right_bound(headers.begin(), headers.end(), 9.9999));
	TEST_ASSERT_TRUE(headers.begin()+4 == right_bound(headers.begin(), headers.end(), 20.0));
	TEST_ASSERT_TRUE(headers.begin()+5 == right_bound(headers.begin(), headers.end(), 20.9999));
	TEST_ASSERT_TRUE(headers.begin()+5 == right_bound(headers.begin(), headers.end(), 21.0));
	TEST_ASSERT_TRUE(headers.begin()+5 == right_bound(headers.begin(), headers.end(), 22.0));

	UNITY_END();
}
