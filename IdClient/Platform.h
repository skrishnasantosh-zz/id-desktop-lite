#pragma once

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

typedef char* UTF8Char;

#ifdef __cplusplus
extern "C" {
#endif


wchar_t* idUrlEncode(const wchar_t* url, unsigned long* platformErrorCode);
wchar_t* idUrlDecode(const wchar_t* url, unsigned long* platformErrorCode);

wchar_t* idBase64Encode(const uint8_t* data, const size_t dataLen, unsigned long* platformErrorCode);

uint8_t* idHmacSha1(size_t* destLen, const uint8_t* data, const size_t dataLen, const uint8_t* key, const size_t keyLen, unsigned long* platformErrorCode);

wchar_t* idHttpGet(int* statusCode, const wchar_t* fullUrl, const wchar_t* headers, unsigned long* platformErrorCode); //Returns HTTP Status Codes. 
wchar_t* idHttpPost(int* statusCode, const wchar_t* fullUrl, const wchar_t* headers, unsigned long* platformErrorCode); //Returns HTTP Status Codes. 

wchar_t* idGetPlatformName(wchar_t* platformName);

void* idAllocateMemory(const size_t destSize);
void* idReallocateMemory(void* block, const size_t destSize);
int idFreeMemory(void** block);

#ifdef __cplusplus
}
#endif
