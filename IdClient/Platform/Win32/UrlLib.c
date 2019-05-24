#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <string.h>
#include <windows.h>
#include <Shlwapi.h>
#include <WinInet.h>

#include "../../Platform.h"

#pragma comment(lib, "Shlwapi.lib")


wchar_t* UrlEncode(const wchar_t* url, unsigned long* platformErrorCode)
{
	HRESULT apiResult = E_FAIL;
	wchar_t sizeTest[1] = { 0 };
	wchar_t* dest = NULL;
	DWORD destSize = 1;

	if (url == NULL)
		return NULL;

	size_t urlLen = wcslen(url);	

	if (urlLen == 0 || urlLen > INTERNET_MAX_URL_LENGTH)
		return NULL;

	apiResult = UrlEscapeW(url, sizeTest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult) && apiResult != E_POINTER)  //E_POINTER expected
		return NULL;

	dest = (wchar_t*)AllocateMemory((destSize) * sizeof(wchar_t));

	if (dest == NULL)
		return NULL;

	apiResult = UrlEscapeW(url, dest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult))
	{
		FreeMemory(dest);
		*dest = NULL;
		return NULL;
	}

	return dest;
}

wchar_t* UrlDecode(const wchar_t* url, unsigned long* platformErrorCode)
{
	HRESULT apiResult = E_FAIL;
	wchar_t sizeTest[1] = { 0 };
	wchar_t* dest = NULL;
	DWORD destSize = 1;

	if (url == NULL)
		return NULL;

	size_t urlLen = wcslen(url);

	if (urlLen == 0 || urlLen > INTERNET_MAX_URL_LENGTH)
		return NULL;

	apiResult = UrlUnescapeW(url, sizeTest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult) && apiResult != E_POINTER)  //E_POINTER expected
		return NULL;

	dest = (wchar_t*)AllocateMemory((destSize) * sizeof(wchar_t));

	if (dest == NULL)
		return NULL;

	apiResult = UrlUnescapeW(url, dest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult))
	{
		FreeMemory(dest);
		*dest = NULL;
		return NULL;
	}

	return dest;
}
