#pragma once

#include <inttypes.h>

#define AUTH_URL_MAXSTR (2048) //2KB only on IE - hence Adsk Identity would support only that
#define AUTH_GEN_MAXSTR (1024)

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
	char16_t m_clientKey[AUTH_GEN_MAXSTR];
	char16_t m_clientSecret[AUTH_GEN_MAXSTR];
	char16_t m_callBackUrl[AUTH_GEN_MAXSTR];
	char16_t m_token[AUTH_GEN_MAXSTR];
	char16_t m_secret[AUTH_GEN_MAXSTR];

	uint8_t m_isAuthorized;

	unsigned long long m_expiresInMs;
};

union __oaData
{
	struct 
	{
		struct __oaCommon m_data; //oa1.m_token is requesttoken if isAuthorized is false

		char16_t m_userName[AUTH_GEN_MAXSTR];
		char16_t m_userId[AUTH_GEN_MAXSTR];
	} m_oa1;

	struct
	{
		struct __oaCommon m_data;

		char16_t m_refreshToken[AUTH_GEN_MAXSTR];
		char16_t m_tokenType[AUTH_GEN_MAXSTR];
		char16_t m_authorizationCode[AUTH_GEN_MAXSTR];

	} m_oa2;
};

typedef struct __oa
{
	char16_t m_environ[4];
	enum OAuthType m_authType;
	union __oaData m_oa;

} *HAUTH;

#ifdef _WIN32
#define CHAR_TYPE wchar_t
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
#define CHAR_TYPE char32_t
#else
#error Unsupported platform
#endif


HAUTH CreateTokenAuthorize(enum AuthType authType, const CHAR_TYPE* env, const const CHAR_TYPE* clientKey, const CHAR_TYPE* clientSecret, const CHAR_TYPE* callbackUrl);

HAUTH GetAccessToken(HAUTH* phToken);

HAUTH RefreshToken(HAUTH hToken);
int ValidateToken(HAUTH hToken);

void FreeToken(HAUTH* phToken);

const wchar_t* GetToken(HAUTH hToken);
const wchar_t* GetSecret(HAUTH hToken);
const wchar_t* GetUserName(HAUTH hToken);
const wchar_t* GetUserId(HAUTH hToken);

const time_t GetTokenExpiry(HAUTH hToken);

#ifdef __cplusplus
}
#endif