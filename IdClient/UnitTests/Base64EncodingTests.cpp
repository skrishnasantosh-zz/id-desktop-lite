#include <string>
#include "UT.h"

#include "../Platform.h"

using namespace Autodesk::Identity::Client::Internal;

UT_BeginTest(Base64EncodingTest)
{
	const string data = "Santosh is a good boy";
	const pstring dataBase64 = u"U2FudG9zaCBpcyBhIGdvb2QgYm95";
	
	Platform platform;

	std::vector<uint8_t> bytes(data.begin(), data.end());

	auto base64 = platform.Base64Encode(bytes);
	
	UT_AssertEquals(dataBase64.length(), base64.length());
	UT_Assert(dataBase64.compare(base64) == 0);
}
UT_EndTest


UT_BeingTestGroup(Base64Tests)

UT_AddTest(Base64EncodingTest)

UT_EndTestGroup
