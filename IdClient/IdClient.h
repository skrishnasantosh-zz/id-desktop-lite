#pragma once

#include <inttypes.h>
#include "Internal/HToken.h"

#ifdef __cplusplus
extern "C" {
#endif

enum OAuthType
{
	OAUTH2,
	OAUTH1
};

struct __oaCommon
{
	wchar_t* m_clientKey;
	wchar_t* m_clientSecret;
	wchar_t* m_callBackUrl;
	wchar_t* m_token;	
	wchar_t* m_secret;

	uint8_t m_isAuthorized;

	unsigned long long m_expiresInMs;

	wchar_t* m_userName;
	wchar_t* m_userId;
};

union __oaType
{
	struct __oaCommon oa1; //oa1.m_token is requesttoken if isAuthorized is false
	struct __oa2
	{
		wchar_t* m_refreshToken;
		wchar_t* m_tokenType;
		wchar_t* m_authorizationCode;
	} oa2;
};

typedef struct __oa
{
	enum OAuthType m_authType;
	union __oaType oa;
} *HAUTH;


HTOKEN CreateTokenAuthorize(enum AuthType authType, const wchar_t* environ, const wchar_t* clientKey, const wchar_t* clientSecret, const wchar_t* callbackUrl);

HTOKEN GetAccessToken(HTOKEN* phToken);

HTOKEN RefreshToken(HTOKEN hToken);
int ValidateToken(HTOKEN hToken);

void FreeToken(HTOKEN* phToken);

const wchar_t* GetToken(HTOKEN hToken);
const wchar_t* GetSecret(HTOKEN hToken);
const wchar_t* GetUserName(HTOKEN hToken);
const wchar_t* GetUserId(HTOKEN hToken);

const time_t GetTokenExpiry(HTOKEN hToken);

#ifdef __cplusplus
}
#endif