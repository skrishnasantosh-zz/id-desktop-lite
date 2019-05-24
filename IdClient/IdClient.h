#pragma once

#include "Internal/HToken.h"

#ifdef __cplusplus
extern "C" {
#endif


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