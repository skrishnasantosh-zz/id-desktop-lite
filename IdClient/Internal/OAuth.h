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

	pstring GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers, const pstring& nonce = nullptr, const pstring& timestamp = nullptr);
	pstring GetSignature(HttpMethod method, const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers);	

public:
	pstring RequestToken(const pstring& url);
	pstring Authorize(const pstring& url);

	pstring GetAccessToken(const pstring& url, const pstring& authCodeOrVerifier);
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