#pragma once

#include "../Platform.h"
#include "../IdClient.h"


#ifdef __cplusplus
extern "C" {
#endif


HAUTH oa1Init(const wchar_t* consumerKey, const wchar_t* consumerSecret, const wchar_t* callback, const wchar_t* token, const wchar_t* tokenSecret);

wchar_t* oa1GetNonce();
wchar_t* oa1GetTimestampStr();

wchar_t* oa1GetSignature(HAUTH oauthHandle, const wchar_t* fullUrl, const wchar_t* additionalHeaders, wchar_t* nonce, wchar_t* timestamp);

wchar_t* oa1RequestToken(HAUTH oauthHandle, const wchar_t* requestTokenUrl);
wchar_t* oa1AuthorizeToken(HAUTH oauthHandle, const wchar_t* authorizeUrl);
wchar_t* oa1AccessToken(HAUTH oauthHandle, const wchar_t* accessTokenUrl);

#ifdef __cplusplus
}
#endif
