
#define _CRT_SECURE_NO_WARNINGS

#include "../Platform.h"
#include "OAuth1.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define MAX_NONCE_CHARS 12
#define ALPHABET_COUNT 26
#define NUMERAL_COUNT 10

wchar_t* oa1GetNonce()
{
	wchar_t* nonce = (wchar_t*)idAllocateMemory(MAX_NONCE_CHARS + 2);

	if (nonce == NULL)
		return NULL;
		
	for (size_t i = 0; i < MAX_NONCE_CHARS; ++i) {
		int randomChar = rand() % (ALPHABET_COUNT + ALPHABET_COUNT + NUMERAL_COUNT);
		if (randomChar < ALPHABET_COUNT)
			nonce[i] = (wchar_t)('a' + randomChar);
		else if (randomChar < ALPHABET_COUNT + ALPHABET_COUNT)
			nonce[i] = (wchar_t)('A' + randomChar - ALPHABET_COUNT);
		else
			nonce[i] = (wchar_t)('0' + randomChar - ALPHABET_COUNT - ALPHABET_COUNT);
	}

	nonce[MAX_NONCE_CHARS] = 0;

	return nonce;
}


wchar_t* oa1GetTimestampStr()
{
	wchar_t* tsStr = NULL;
	int64_t t;

	t = (int64_t)(time(NULL)); //WARN: Assumption that time_t will be 64 bit signed integer on all platforms. Code may start behaving weirdly if assumption fails
	
	std::wstring value = std::to_wstring(t);
	
	tsStr = (wchar_t*)idAllocateMemory(value.length() + 1);
	if (tsStr == NULL)
		return NULL;

	wcscpy(tsStr, value.c_str());
	
	return tsStr;
}