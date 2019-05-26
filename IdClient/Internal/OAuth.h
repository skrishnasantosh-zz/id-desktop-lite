#pragma once

#include <string>
#include <map>

#include "../Platform.h"

BEGIN_NAMESPACE4(Autodesk, Identity, Client, Internal)

class OAuthBase
{
protected:
	pstring m_clientKey, m_clientSecret, m_callback;

public:
	virtual pstring Authorize(const pstring& url) = 0;
	virtual pstring GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier) = 0;
};

END_NAMESPACE4()

using namespace std;

BEGIN_NAMESPACE3(Autodesk, Identity, Client)

class OAuth1 : protected Autodesk::Identity::Client::Internal::OAuthBase
{
public:
	OAuth1(const pstring& consumerKey, const pstring& consumerSecret, const pstring& callback);

public: //public for UT's sake
	pstring GetNonce();
	pstring GetTimestampStr();
		
	pstring GetAuthHeader();

public:
	pstring RequestToken(const pstring& url);
	pstring Authorize(const pstring& url);

	pstring GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier);

	pstring GetAuthHeaderStr(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries); 
	pstring GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, pstring& nonce, pstring& timestamp);

private:
	pstring GetHttpMethodStr(HttpMethod method);
	pstring GetFullyFormedUrl(Internal::Url& urlParts);
	pstring GetQueryParamsForSigning(map<pstring, pstring>& queries, const pstring& nonce, const pstring& timeStamp);
};


class OAuth2 : protected Autodesk::Identity::Client::Internal::OAuthBase
{
public:
	OAuth2(const pstring& consumerKey, const pstring& consumerSecret, const pstring& callback);

public:
	pstring Authorize(const pstring& url);
	pstring GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier);
};

END_NAMESPACE3()