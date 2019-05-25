#include <string>
#include "UT.h"

#include "../Platform.h"

using namespace Autodesk::Identity::Client::Internal;

UT_BeginTest(UrlEncodeUrlTest)
{
	const pstring googleQuery = u"https://www.google.com/search?rlz=1C1CHBD_enSG824SG824&ei=BNHlXNO3L73Uz7sP2tCmwAM&q=Query+String+Test&oq=Query+String+Test&gs_l=psy-ab.3..0j0i22i30l5j0i22i10i30.24146.25946..26050...0.0..0.74.790.17......0....1..gws-wiz.......0i71j35i39j0i67j0i131j0i131i67j0i10.extLlcL1q4g#foo";
	Platform platform;
	
	auto encodeResult = platform.UrlEncode(googleQuery);

	UT_Assert(encodeResult.length() > 0);

	auto decodeResult = platform.UrlDecode(encodeResult);

	UT_AssertEquals(googleQuery.length(), decodeResult.length());
	UT_Assert(googleQuery.compare(decodeResult) == 0);
}
UT_EndTest

UT_BeginTest(UrlParseTest)
{
	const pstring googleQuery = u"https://www.google.com/search?rlz=1C1CHBD_enSG824SG824&ei=BNHlXNO3L73Uz7sP2tCmwAM&q=Query+String+Test&oq=Query+String+Test&gs_l=psy-ab.3..0j0i22i30l5j0i22i10i30.24146.25946..26050...0.0..0.74.790.17......0....1..gws-wiz.......0i71j35i39j0i67j0i131j0i131i67j0i10.extLlcL1q4g#foo";
	Platform platform;

	Url url;

	platform.TryParseUrl(googleQuery, url);

	UT_Assert(url.m_protocol.compare(u"https") == 0);
	UT_Assert(url.m_host.compare(u"www.google.com") == 0);
	UT_Assert(url.m_path.compare(u"search") == 0);
	UT_Assert(url.m_queryString.compare(u"rlz=1C1CHBD_enSG824SG824&ei=BNHlXNO3L73Uz7sP2tCmwAM&q=Query+String+Test&oq=Query+String+Test&gs_l=psy-ab.3..0j0i22i30l5j0i22i10i30.24146.25946..26050...0.0..0.74.790.17......0....1..gws-wiz.......0i71j35i39j0i67j0i131j0i131i67j0i10.extLlcL1q4g") == 0);
	UT_Assert(url.m_fragment.compare(u"foo") == 0);
}
UT_EndTest

UT_BeingTestGroup(UrlTests)

UT_AddTest(UrlEncodeUrlTest)
UT_AddTest(UrlParseTest)

UT_EndTestGroup
