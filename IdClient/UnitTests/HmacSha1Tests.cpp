#include <string>
#include "UT.h"

#include "../Platform.h"

using namespace Autodesk::Identity::Client::Internal;

UT_BeginTest(HmacSha1Base64Test)
{
	Platform platform;
	const std::string szData = "Test";
	const std::string szKey = "Key";

	std::vector<uint8_t> data(szData.begin(), szData.end());
	std::vector<uint8_t> key(szKey.begin(), szKey.end());

	auto result = platform.HmacSha1(data, key);

	auto base64 = platform.Base64Encode(result);

	UT_Assert(base64.compare(u"xIcCRlnXa+IqFtO+9AF3OqeRdAU=") == 0);

}
UT_EndTest


UT_BeingTestGroup(HmacSha1Tests)

UT_AddTest(HmacSha1Base64Test)

UT_EndTestGroup

