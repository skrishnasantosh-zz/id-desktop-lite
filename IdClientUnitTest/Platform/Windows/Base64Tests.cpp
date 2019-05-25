#include <string.h>
#include "../../UT.h"

#include "../../../IdClient/Platform.h"

using namespace std;

UT_BeginTest(Base64EncodeTextTest)
{
	const char* data = "Santosh is a good boy";
	const wchar_t* dataBase64 = L"U2FudG9zaCBpcyBhIGdvb2QgYm95";
	unsigned long platformErrorCode = 0;

	wchar_t* base64 = idBase64Encode((const uint8_t*)data, strlen(data), &platformErrorCode);
	
	UT_AssertEquals(wcscmp(base64, dataBase64), 0);

	idFreeMemory((void**)&base64);

}
UT_EndTest


UT_BeingTestGroup(Base64Tests)

	UT_AddTest(Base64EncodeTextTest)

UT_EndTestGroup

