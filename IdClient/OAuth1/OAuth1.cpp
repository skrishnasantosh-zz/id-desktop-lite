
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

pstring GetHttpMethodStr(HttpMethod method);

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
	auto dest = platform.FromMbs(value);	
	
	return dest;
}

pstring OAuth1::GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers)
{
	return GetSignature(method, url, queries, headers, u"", u"");
}

pstring OAuth1::GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers, const pstring& nonce, const pstring& timestamp)
{
	map<pstring, pstring> q(queries);
	map<pstring, pstring> h(headers);

	Internal::Platform platform;
	Internal::Url urlParts;

	pstring n, ts;

	if (!platform.TryParseUrl(url, urlParts))
		return pstring();

	if (nonce.length() == 0)
		n = GetNonce();

	if (timestamp.length() == 0)
		ts = GetTimestampStr();

	pstring methodStr = GetHttpMethodStr(method);
	pstring signBase(methodStr);

	signBase.append(u"&");

	pstring justUrl = urlParts.m_protocol;

	if (urlParts.m_protocol.length() > 0)
	{
		if (justUrl.front() == 'h' || justUrl.front() == 'H')
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
		justUrl.append(platform.FromUI64(urlParts.m_port));
	}

	justUrl.append(u"/");

	if (urlParts.m_path.length() > 0)
		justUrl.append(urlParts.m_path);
	
	q.insert(pair<pstring, pstring>(pstring(u"oauth_consumer_key"), this->m_clientKey));
	q.insert(pair<pstring, pstring>(pstring(u"oauth_nonce"), nonce));
	q.insert(pair<pstring, pstring>(pstring(u"oauth_signature_method"), pstring(u"HMAC-SHA1")));
	q.insert(pair<pstring, pstring>(pstring(u"oauth_timestamp"), timestamp));
	q.insert(pair<pstring, pstring>(pstring(u"oauth_token"), pstring(u""))); //Add Empty token if token not there



	return pstring();
}

pstring OAuth1::Authorize(const pstring& url)
{
	return pstring();
}

pstring OAuth1::GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier)
{
	return pstring();
}

//helpers

static pstring GetHttpMethodStr(HttpMethod method)
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
