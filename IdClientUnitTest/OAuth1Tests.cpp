#include <string>
#include "UT.h"

#include "../IdClient/Platform.h"
#include "../IdClient/OAuth1/OAuth1.h"

UT_BeginTest(OAuth1GenerateNonceTest)
{
	wchar_t* nonce1 = oa1GetNonce();
	UT_AssertNotNull(nonce1);

	wchar_t* nonce2 = oa1GetNonce();
	UT_AssertNotNull(nonce2);

	UT_Assert(wcscmp(nonce1, nonce2) != 0, "Generated nonce cannot be same");

	idFreeMemory((void**)&nonce1);
	idFreeMemory((void**)&nonce2);
}
UT_EndTest


UT_BeginTest(OAuth1GenerateTimeStampTest)
{
	wchar_t* ts = oa1GetTimestampStr();
	UT_AssertNotNull(ts);
	UT_AssertNotEquals(ts[0], 0);

	idFreeMemory((void**)&ts);
}
UT_EndTest

UT_BeingTestGroup(OAuth1Tests)

UT_AddTest(OAuth1GenerateNonceTest)
UT_AddTest(OAuth1GenerateTimeStampTest)

UT_EndTestGroup

