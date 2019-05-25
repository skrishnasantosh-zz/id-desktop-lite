#include "IdClient.h"

HAUTH CreateTokenAuthorize(AuthType authType, const wchar_t* environ, const wchar_t* clientKey, const wchar_t* clientSecret, const wchar_t* callbackUrl)
{
	return HAUTH();
}

HAUTH GetAccessToken(HAUTH* phToken)
{
	return HAUTH();
}

HAUTH RefreshToken(HAUTH hToken)
{
	return HAUTH();
}

int ValidateToken(HAUTH hToken)
{
	return 0;
}

void FreeToken(HAUTH* phToken)
{
}

const wchar_t* GetToken(HAUTH hToken)
{
	return nullptr;
}

const wchar_t* GetSecret(HAUTH hToken)
{
	return nullptr;
}

const wchar_t* GetUserName(HAUTH hToken)
{
	return nullptr;
}

const wchar_t* GetUserId(HAUTH hToken)
{
	return nullptr;
}

const time_t GetTokenExpiry(HAUTH hToken)
{
	return time_t();
}
