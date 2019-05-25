#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#ifndef CALG_HMAC
#define CALG_HMAC (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_HMAC)
#endif

#ifndef CRYPT_IPSEC_HMAC_KEY
#define CRYPT_IPSEC_HMAC_KEY 0x00000100
#endif

#include <string.h>
#include <Windows.h>
#include <wincrypt.h>

#include "../../Platform.h"


struct BLOB_DMY {
	BLOBHEADER header;
	DWORD len;
	BYTE key[0];
};

uint8_t* idHmacSha1(size_t* destLen, const uint8_t* data, const size_t dataLen, const uint8_t* key, const size_t keyLen, unsigned long* platformErrorCode)
{
	HCRYPTPROV  hProv = NULL;	
	HCRYPTHASH  hHash = NULL;
	HCRYPTKEY   hKey = NULL;
	HCRYPTHASH  hHmacHash = NULL;
	DWORD       dwDataLen = 0;
	HMAC_INFO   hMacInfo = { 0 };
	BOOL		result = FALSE;
	uint8_t* pbHash = NULL;
	const int hashSize = 20; //SHA1 is 20 bytes always
	

	if (destLen == NULL || data == NULL || dataLen == NULL || key == NULL || keyLen == NULL)
		return NULL;

	if (platformErrorCode != NULL)
		* platformErrorCode = 0;

	hMacInfo.HashAlgid = CALG_SHA1;
	

	result = CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_NEWKEYSET);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	struct BLOB_DMY* blob = NULL;

	DWORD blobSize = sizeof(struct BLOB_DMY) + keyLen + 1;

	blob = (struct BLOB_DMY*)malloc(blobSize);
	memset(blob, 0, blobSize);

	blob->header.bType = PLAINTEXTKEYBLOB;
	blob->header.aiKeyAlg = CALG_RC2;
	blob->header.reserved = 0;
	blob->header.bVersion = CUR_BLOB_VERSION;
	blob->len = keyLen;

	memcpy(&(blob->key), key, keyLen + 1); //Copy zero at end	

	result = CryptImportKey(hProv, (BYTE*)blob, blobSize, 0, CRYPT_IPSEC_HMAC_KEY, &hKey);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	result = CryptCreateHash(hProv, CALG_HMAC, hKey, 0, &hHmacHash);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	result = CryptSetHashParam(hHmacHash, HP_HMAC_INFO, (BYTE*)&hMacInfo, 0);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	result = CryptHashData(hHmacHash, (BYTE*)data, dataLen, 0);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	pbHash = (uint8_t*)idAllocateMemory(hashSize + 1);
	if (pbHash == NULL)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = 0;

		result = FALSE;
	}
	memset(pbHash, 0, hashSize + 1);
	dwDataLen = hashSize;

	result = CryptGetHashParam(hHmacHash, HP_HASHVAL, (BYTE*)pbHash, &dwDataLen, 0);
	if (!result)
	{
		if (platformErrorCode != NULL)
			* platformErrorCode = GetLastError();
	}

	*destLen = (size_t)dwDataLen;

	free((void*)blob);

	if (hHmacHash)
		CryptDestroyHash(hHmacHash);
	if (hKey)
		CryptDestroyKey(hKey);
	if (hHash)
		CryptDestroyHash(hHash);
	if (hProv)
		CryptReleaseContext(hProv, 0);
	if (pbHash && !result)
		idFreeMemory(pbHash);

	return pbHash; //PlatformErrorCode set to 0 and return NULL if out of memory
}
