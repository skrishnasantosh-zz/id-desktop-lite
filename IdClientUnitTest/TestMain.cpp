#include "UT.h"

UT_DeclareTestGroup(Base64Tests)
UT_DeclareTestGroup(UrlEncoderTests)
UT_DeclareTestGroup(HmacSha1Tests)

int main() 
{
	UT_RunTestGroup(UrlEncoderTests);
	UT_RunTestGroup(Base64Tests);
	UT_RunTestGroup(HmacSha1Tests);

	return 0;
}