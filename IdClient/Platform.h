#pragma once

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>


#ifdef __cplusplus
extern "C" {
#endif


wchar_t* UrlEncode(const wchar_t* url, unsigned long* platformErrorCode);
wchar_t* UrlDecode(const wchar_t* url, unsigned long* platformErrorCode);

wchar_t* Base64Encode(const uint8_t* data, const size_t dataLen, unsigned long* platformErrorCode);

uint8_t* HmacSha1(size_t* destLen, const uint8_t* data, const size_t dataLen, const uint8_t* key, const size_t keyLen, unsigned long* platformErrorCode);

wchar_t* HttpGet(int* statusCode, const wchar_t* fullUrl, const wchar_t* headers, unsigned long* platformErrorCode); //Returns HTTP Status Codes. 
wchar_t* HttpPost(int* statusCode, const wchar_t* fullUrl, const wchar_t* headers, unsigned long* platformErrorCode); //Returns HTTP Status Codes. 

wchar_t* GetPlatformName(wchar_t* platformName);

void* AllocateMemory(const size_t destSize);
void* ReallocateMemory(void* block, const size_t destSize);
void FreeMemory(void** block);

#ifdef __cplusplus
}
#endif
