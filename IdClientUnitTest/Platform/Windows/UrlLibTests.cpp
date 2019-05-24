#include <string>
#include "../../UT.h"

#include "../../../IdClient/Platform.h"

using namespace std;

UT_BeginTest(UrlEncodeUrlTest)
{
	const wchar_t* googleQuery = L"https://www.google.com/search?rlz=1C1CHBD_enSG824SG824&ei=BNHlXNO3L73Uz7sP2tCmwAM&q=Query+String+Test&oq=Query+String+Test&gs_l=psy-ab.3..0j0i22i30l5j0i22i10i30.24146.25946..26050...0.0..0.74.790.17......0....1..gws-wiz.......0i71j35i39j0i67j0i131j0i131i67j0i10.extLlcL1q4g";
	wchar_t* encodeResult = NULL;
	wchar_t* decodeResult = NULL;
	unsigned long platformErrorCode = 0;

	encodeResult = UrlEncode(googleQuery, &platformErrorCode);

	UT_AssertNotNull(encodeResult);

	decodeResult = UrlDecode(encodeResult, &platformErrorCode);

	UT_AssertNotNullCleanup(decodeResult, FreeMemory((void**)&encodeResult));

	UT_AssertEquals(wcscmp(decodeResult, googleQuery), 0);

	FreeMemory((void**)&encodeResult);
	FreeMemory((void**)&decodeResult);
}
UT_EndTest


UT_BeingTestGroup(UrlEncoderTests)

	UT_AddTest(UrlEncodeUrlTest)

UT_EndTestGroup
