#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <string.h>

enum AuthType
{
	OAuth1,
	OAuth2
};

struct __idtoken
{
	wchar_t* m_environ;

	wchar_t* m_token;
	wchar_t* m_tokenSecret;
	time_t m_expiresIn;
	enum AuthType m_outhType;

	wchar_t* m_callbackUrl;

	union
	{
		struct
		{
			wchar_t* m_username;
			wchar_t* m_userId;

			wchar_t* m_consumerKey;
			wchar_t* m_consumerSecret;

		} oAuth1;

		struct
		{
			wchar_t* m_refreshToken;
			wchar_t* m_clientId;
			wchar_t* m_clientSecret;
		} oAuth2;
	} auth;

};

typedef struct __idtoken* HTOKEN;

#ifdef __cplusplus
}
#endif
