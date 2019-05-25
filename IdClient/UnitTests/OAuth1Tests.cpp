#include <string>
#include "UT.h"

#include "../Internal/OAuth.h"

#define TEST_CONSUMER_KEY  u"mycloud-dev.autodesk.com"
#define TEST_CONSUMER_SECRET u"DevAut0d3sk!"
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
	//OAuth oauth_consumer_key="mycloud-dev.autodesk.com",oauth_signature_method="HMAC-SHA1",oauth_timestamp="1558796429",oauth_nonce="OmOraUJCvdr",oauth_version="1.0",oauth_signature="StMwQqMuNcH9BLB6mpZZ2EJlOUg%3D"
	//https://accounts-dev.autodesk.com/
	OAuth1 oauth1(TEST_CONSUMER_KEY, TEST_CONSUMER_SECRET, TEST_CALLBACK_URL);
	pstring signature = oauth1.GetSignature(HTTP_GET, u"https://accounts-dev.autodesk.com/", map<pstring, pstring>(), map<pstring, pstring>(), u"OmOraUJCvdr", u"1558796429");

}
UT_EndTest


UT_BeingTestGroup(OAuth1Tests)

UT_AddTest(OAuth1NonceTest)
UT_AddTest(OAuth1GetTimeStampTest)
UT_AddTest(OAuth1SignatureNormalTest)

UT_EndTestGroup
