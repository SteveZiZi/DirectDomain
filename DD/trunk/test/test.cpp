#include "Test.h"


void Test::print(TEST_RESULT ret)
{

	if (ret == TEST_OK)
	{
		printf("����ͨ��\n");
	}
	else if (ret == TEST_FAIL)
	{
		printf("���Դ�����������\n");
	}
	else
	{
		printf("���Է���bug\n");

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
		printf("****************��ǰ���е�%d����������**********************\n",i++);
		ret = (*iter)->run();
		print(ret);
	}

	if (i != distance(testSuit.begin(),testSuit.end()))
	{	printf("����ʧ��\n");
	   return TEST_FAIL;
	}

	return TEST_OK;
}
