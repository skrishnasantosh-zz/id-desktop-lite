#include "IdClient.h"

HTOKEN CreateTokenAuthorize(AuthType authType, const wchar_t* environ, const wchar_t* clientKey, const wchar_t* clientSecret, const wchar_t* callbackUrl)
{
	return HTOKEN();
}

HTOKEN GetAccessToken(HTOKEN* phToken)
{
	return HTOKEN();
}

HTOKEN RefreshToken(HTOKEN hToken)
{
	return HTOKEN();
}

int ValidateToken(HTOKEN hToken)
{
	return 0;
}

void FreeToken(HTOKEN* phToken)
{
}

const wchar_t* GetToken(HTOKEN hToken)
{
	return nullptr;
}

const wchar_t* GetSecret(HTOKEN hToken)
{
	return nullptr;
}

const wchar_t* GetUserName(HTOKEN hToken)
{
	return nullptr;
}

const wchar_t* GetUserId(HTOKEN hToken)
{
	return nullptr;
}

const time_t GetTokenExpiry(HTOKEN hToken)
{
	return time_t();
}
