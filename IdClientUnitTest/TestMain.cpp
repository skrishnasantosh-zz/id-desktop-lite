#include "UT.h"

UT_DeclareTestGroup(Base64Tests)
UT_DeclareTestGroup(UrlEncoderTests)
UT_DeclareTestGroup(HmacSha1Tests)
UT_DeclareTestGroup(OAuth1Tests)

int main() 
{
	UT_RunTestGroup(UrlEncoderTests);
	UT_RunTestGroup(Base64Tests);
	UT_RunTestGroup(HmacSha1Tests);
	UT_RunTestGroup(OAuth1Tests);

	return 0;
}