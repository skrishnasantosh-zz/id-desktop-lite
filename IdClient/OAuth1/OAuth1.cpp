
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "../Platform.h"
#include "../Internal/OAuth.h"

#define MAX_NONCE_CHARS 12
#define ALPHABET_COUNT 26
#define NUMERAL_COUNT 10

using namespace Autodesk::Identity::Client;


OAuth1::OAuth1(const pstring& consumerKey, const pstring& consumerSecret, const pstring& callback)
{
	m_clientKey = consumerKey;
	m_clientSecret = consumerSecret;
	m_callback = callback;
}

pstring OAuth1::GetNonce()
{
	pstring dest;
	dest.resize(MAX_NONCE_CHARS, 0U);

	for (size_t i = 0; i < MAX_NONCE_CHARS; ++i) 
	{
		int randomChar = rand() % (ALPHABET_COUNT + ALPHABET_COUNT + NUMERAL_COUNT);
			
		if (randomChar < ALPHABET_COUNT)
			dest[i] = (char16_t)('a' + randomChar);
		else if (randomChar < ALPHABET_COUNT + ALPHABET_COUNT)
			dest[i] = (char16_t)('A' + randomChar - ALPHABET_COUNT);
		else
			dest[i] = (char16_t)('0' + randomChar - ALPHABET_COUNT - ALPHABET_COUNT);
	}

	return dest;
}

pstring OAuth1::GetTimestampStr()
{
	int64_t t;
	Internal::Platform platform;

	t = (int64_t)(time(NULL)); //WARN: Assumption that time_t will be 64 bit signed integer on all platforms. Code may start behaving weirdly if assumption fails
	
	auto value = std::to_string(t);
	auto dest = platform.Strings.FromMbs(value);
	
	return dest;
}

pstring OAuth1::Authorize(const pstring& url)
{
	return pstring();
}

pstring OAuth1::GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier)
{
	return pstring();
}

pstring OAuth1::GetAuthHeaderStr(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries)
{
	//OAuth oauth_consumer_key = "mycloud-dev.autodesk.com", oauth_signature_method = "HMAC-SHA1", oauth_timestamp = "1558796429", oauth_nonce = "OmOraUJCvdr", oauth_version = "1.0", oauth_signature = "StMwQqMuNcH9BLB6mpZZ2EJlOUg%3D"
	pstring authHeader = u"OAuth ";
	pstring nonce, timestamp;

	pstring signature = GetSignature(method, url, queries, nonce, timestamp);

	authHeader.append(u"oauth_consumer_key = \"" + m_clientKey + u"\", " +
		u"oauth_signature_method = \"HMAC_SHA1\", oauth_timestamp = \"" + timestamp + u"\", " +
		u"oauth_nonce = \"" + nonce + u"\", oauth_version = \"1.0\", " +
		u"oauth_signature = \"" + signature + u"\"");
	
	return authHeader;
}


pstring OAuth1::GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, pstring& nonce, pstring& timestamp)
{
	map<pstring, pstring> queryList(queries);

	Internal::Platform platform;
	Internal::Url urlParts;

	if (!platform.TryParseUrl(url, urlParts))
		return pstring();

	if (nonce.length() == 0)
		nonce = GetNonce();

	if (timestamp.length() == 0)
		timestamp = GetTimestampStr();

	pstring methodStr = GetHttpMethodStr(method);
	pstring signBase(methodStr);

	signBase.append(u"&");

	pstring justUrl = GetFullyFormedUrl(urlParts);
	pstring justQuery = GetQueryParamsForSigning(queryList, nonce, timestamp);

	pstring encodedQuery = platform.UrlEncode(justQuery);
	pstring encodedUrl = platform.UrlEncode(justUrl);

	signBase.append(encodedUrl);
	signBase.append(u"&");
	signBase.append(encodedQuery);

	string mbsSignBase = platform.Strings.ToUtf8(signBase);
	string mbsKey = platform.Strings.ToUtf8(platform.UrlEncode(m_clientSecret) + u"&");

	vector<uint8_t> signBaseBytes(mbsSignBase.begin(), mbsSignBase.end());

	vector<uint8_t> keyBytes(mbsKey.begin(), mbsKey.end());

	vector<uint8_t> hmacDigest = platform.HmacSha1(signBaseBytes, keyBytes);
	pstring base64Digest = platform.Base64Encode(hmacDigest);

	pstring signature = platform.UrlEncode(base64Digest);

	return signature;
}

//helpers

pstring OAuth1::GetHttpMethodStr(HttpMethod method)
{
	switch (method)
	{
	case HTTP_GET:
		return u"GET";
	case HTTP_POST:
		return u"POST";
	case HTTP_PATCH:
		return u"PATCH";
	case HTTP_PUT:
		return u"PUT";
	case HTTP_DELETE:
		return u"DELETE";
	default:
		return pstring();
	}
}

pstring OAuth1::GetFullyFormedUrl(Internal::Url& urlParts)
{
	Internal::Platform platform;
	pstring justUrl = urlParts.m_protocol;

	if (urlParts.m_protocol.length() > 0)
	{
		if (justUrl.front() == u'h' || justUrl.front() == u'H')
		{
			if ((justUrl.back() == u's' || justUrl.back() == u'S'))
			{
				if (urlParts.m_port == 443)
					urlParts.m_port = 0; //TODO: Verify this - strip the default ports
			}
			else if ((justUrl.back() == u'h' || justUrl.back() == u'H'))
			{
				if (urlParts.m_port == 80)
					urlParts.m_port = 0; //TODO: Verify this - strip the default ports
			}
		}
	}
	else
		justUrl = u"https"; //default to https

	justUrl.append(u"://");
	justUrl.append(urlParts.m_host);

	if (urlParts.m_port != 0)
	{
		justUrl.append(u":");
		justUrl.append(platform.Strings.FromUI64(urlParts.m_port));
	}

	justUrl.append(u"/");

	if (urlParts.m_path.length() > 0)
		justUrl.append(urlParts.m_path);

	return justUrl;
}

pstring OAuth1::GetQueryParamsForSigning(map<pstring, pstring>& queries, const pstring& nonce, const pstring& timeStamp)
{
	Internal::Platform platform;

	queries.insert(pair<pstring, pstring>(pstring(u"oauth_version"), u"1.0"));
	queries.insert(pair<pstring, pstring>(pstring(u"oauth_consumer_key"), m_clientKey));
	queries.insert(pair<pstring, pstring>(pstring(u"oauth_nonce"), nonce));
	queries.insert(pair<pstring, pstring>(pstring(u"oauth_signature_method"), pstring(u"HMAC-SHA1")));
	queries.insert(pair<pstring, pstring>(pstring(u"oauth_timestamp"), timeStamp));
	queries.insert(pair<pstring, pstring>(pstring(u"oauth_token"), pstring(u""))); //Add Empty token if token not there

	size_t pos = 0;
	size_t qlen = queries.size();

	pstring justQuery;

	for (map<pstring, pstring>::iterator it = queries.begin(); it != queries.end(); ++it)
	{
		pstring qpair = it->first + u"=" + platform.UrlDecode(it->second);
		justQuery.append(qpair);

		if (++pos < qlen)
			justQuery.append(u"&");
	}

	return justQuery;
}
