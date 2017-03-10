#include "Test.h"


void Test::print(TEST_RESULT ret)
{

	if (ret == TEST_OK)
	{
		printf("测试通过\n");
	}
	else if (ret == TEST_FAIL)
	{
		printf("测试存在严重问题\n");
	}
	else
	{
		printf("测试发现bug\n");

	}
}


const unsigned char *TestCase::GetTestCaseName()
{
	return CaseName.c_str();
}

void TestSuite::AddTest(Test *tcase)
{
	if (tcase != 0)
	 testSuit.push_back(tcase);
}


TEST_RESULT TestSuite::run()
{

	TEST_RESULT ret;
	int i = 0;
     
	for (list<Test *>::iterator iter = testSuit.begin(); iter != testSuit.end(); iter++)
	{
		printf("****************当前运行第%d个测试用例**********************\n",i++);
		ret = (*iter)->run();
		print(ret);
	}

	if (i != distance(testSuit.begin(),testSuit.end()))
	{	printf("测试失败\n");
	   return TEST_FAIL;
	}

	return TEST_OK;
}
