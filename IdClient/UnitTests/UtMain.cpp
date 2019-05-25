#include "UT.h"

UT_DeclareTestGroup(UrlTests)
UT_DeclareTestGroup(Base64Tests);
UT_DeclareTestGroup(HmacSha1Tests);
UT_DeclareTestGroup(OAuth1Tests);

int main(int argc, char** argv)
{
	(argc);
	(argv);

	UT_RunTestGroup(UrlTests);
	UT_RunTestGroup(Base64Tests);
	UT_RunTestGroup(HmacSha1Tests);
	UT_RunTestGroup(OAuth1Tests);
}

