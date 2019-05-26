#pragma once

#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4702)

typedef const char* (*fnTest)();

#define UT_AddTest(test) do { const char* message = test();  \
								printf("\n   Running test %s():", #test);\
                                if (message) { printf("\n      %s\n", message); exit(-1); return message;} else { printf("\n      PASSED.\n"); } } while (0);

#define UT_BeginTest(testName) const char* testName() { \
								const char *__message = NULL; 

#define UT_EndTest return __message; }


#define UT_BeingTestGroup(testGroupName) const char* testGroupName() { \
							printf("\nRUNNING UNIT TESTS FOR - %s :", #testGroupName);

#define UT_EndTestGroup return NULL; }

#define UT_AssertMessage(test, message) do { if (!(test)) return message; } while (0)
#define UT_Assert(test) do { if (!(test)) return "Assert FAILED : "#test ; } while (0)

#define UT_AssertEquals(a, b) do { if ((a != b)) return "AssertEquals FAILED : "#a "==" #b; } while (0)
#define UT_AssertNotEquals(a, b) do { if ((a == b)) return "AssertNotEquals FAILED : "#a "==" #b ; } while (0)

#define UT_AssertNotNull(a) if (a == NULL) return "AssertNotNull FAILED : " #a;
#define UT_AssertNotNullCleanup(a, b) if (a == NULL) { b; return "AssertNotNull FAILED : " #a; }

#define UT_AssertGreaterThan(a, b) do { if ((a < b)) return "AssertGreaterThan FAILED : "#a "==" #b ; } while (0)


#define UT_RunTestGroup(testGroup) testGroup();

#define UT_DeclareTestGroup(testGroupName) extern const char* testGroupName();

//extern unsigned long long g_totalTests;

//#define UT_BeginTest(testName) static const char* testName() { \
								//const char *__message = NULL; g_totalTests++;