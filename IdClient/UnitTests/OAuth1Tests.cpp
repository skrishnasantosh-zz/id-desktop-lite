#include <string>
#include "UT.h"

#include "../Internal/OAuth.h"

#define TEST_CONSUMER_KEY  u"mycloud-dev.autodesk.com"
#define TEST_CONSUMER_SECRET u"DevAutode$k"
#define TEST_CALLBACK_URL u"https://accounts-dev.autodesk.com"

using namespace Autodesk::Identity::Client;

UT_BeginTest(OAuth1NonceTest)
{
	OAuth1 oauth1(TEST_CONSUMER_KEY, TEST_CONSUMER_SECRET, TEST_CALLBACK_URL);	
	pstring nonce = oauth1.GetNonce();
	
	UT_Assert(nonce.length() > 0);
}
UT_EndTest

UT_BeginTest(OAuth1GetTimeStampTest)
{
	OAuth1 oauth1(TEST_CONSUMER_KEY, TEST_CONSUMER_SECRET, TEST_CALLBACK_URL);
	pstring nonce = oauth1.GetTimestampStr();

	UT_Assert(nonce.length() > 0);
}
UT_EndTest

UT_BeginTest(OAuth1SignatureNormalTest)
{
	OAuth1 oauth1(TEST_CONSUMER_KEY, TEST_CONSUMER_SECRET, TEST_CALLBACK_URL);

	pstring nonce = u"OmOraUJCvdr";
	pstring ts = u"1558796429";

	pstring signature = oauth1.GetSignature(HTTP_GET, u"https://accounts-dev.autodesk.com/", map<pstring, pstring>(),  nonce, ts);

	UT_Assert(signature.compare(u"StMwQqMuNcH9BLB6mpZZ2EJlOUg%3D") == 0);

}
UT_EndTest

UT_BeginTest(OAuth1AuthHeaderTest)
{
	OAuth1 oauth1(TEST_CONSUMER_KEY, TEST_CONSUMER_SECRET, TEST_CALLBACK_URL);

	pstring authString = oauth1.GetAuthHeaderStr(HTTP_GET, u"https://accounts-dev.autodesk.com/", map<pstring, pstring>());

	UT_Assert(authString.find_first_of(u"OAuth ", 0) == 0);
}
UT_EndTest


UT_BeingTestGroup(OAuth1Tests)

UT_AddTest(OAuth1NonceTest)
UT_AddTest(OAuth1GetTimeStampTest)
UT_AddTest(OAuth1SignatureNormalTest)
UT_AddTest(OAuth1AuthHeaderTest)

UT_EndTestGroup
