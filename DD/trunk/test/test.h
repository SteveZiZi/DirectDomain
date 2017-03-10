#ifndef INCLUDED_TEST
#define INCLUDED_TEST

#include "../base/String.h"
#include "../include/StdInclude.h"

typedef enum
{
	TEST_OK,
	TEST_FAIL,
	TEST_BUG,
}TEST_RESULT;

class Test
{
public:
	virtual inline ~Test()
	{}
	virtual TEST_RESULT run() = 0;
	virtual void AddTest(Test *tcase){}
protected:
	void print(TEST_RESULT ret);

};


class TestCase: public Test
{
public:
	TestCase(const char *name)
	{ CaseName.assign((unsigned char *)name);}
	virtual inline ~TestCase()
	{}
	virtual TEST_RESULT run(){ return TEST_OK;}
    const unsigned char *GetTestCaseName();

protected:
	bstring CaseName;
};


class TestSuite:public Test
{
private:
	list<Test *> testSuit;
public:
	void AddTest(Test *tcase);
	TEST_RESULT run();
};

#endif


