#include <string.h>
#include "../../UT.h"

#include "../../../IdClient/Platform.h"

using namespace std;

UT_BeginTest(HmacSha1Base64Test)
{
	size_t destLen = 0;
	const char* szData = "Test";
	const char* szKey = "Key";
	unsigned long platformErrorCode = 0;

	uint8_t* bytes = idHmacSha1(&destLen, (const uint8_t*)szData, strlen(szData), (const uint8_t*)szKey, strlen(szKey), &platformErrorCode);

	wchar_t* base64 = idBase64Encode((const uint8_t*)bytes, destLen, &platformErrorCode);

	UT_AssertEquals(wcscmp(base64, L"xIcCRlnXa+IqFtO+9AF3OqeRdAU="), 0);

	idFreeMemory((void**)&bytes);
}
UT_EndTest


UT_BeingTestGroup(HmacSha1Tests)

UT_AddTest(HmacSha1Base64Test)

UT_EndTestGroup

