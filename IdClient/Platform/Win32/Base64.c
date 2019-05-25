#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <string.h>
#include <Windows.h>
#include <wincrypt.h>

#pragma comment(lib, "Crypt32.lib")

#include "../../Platform.h"

wchar_t* idBase64Encode(const uint8_t* data, const size_t dataLen, unsigned long* platformErrorCode)
{
	DWORD destSizeRecv = 0;
	BOOL result = FALSE;	
	wchar_t* dest = NULL;

	if (data == NULL || dataLen == 0)
		return NULL;

	result = CryptBinaryToStringW((const BYTE*)data, dataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &destSizeRecv);

	if (!result)
		return NULL;	

	dest = (wchar_t*)idAllocateMemory(destSizeRecv);
	if (dest == NULL)
		return NULL;

	memset(dest, 0, destSizeRecv);

	result = CryptBinaryToStringW((const BYTE*)data, dataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, dest, &destSizeRecv);

	if (!result)
	{
		idFreeMemory(&dest);
		return NULL;
	}

	return dest;
}
