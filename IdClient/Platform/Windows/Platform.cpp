#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Shlwapi.h>
#include <wincrypt.h>
#include <WinInet.h>

#include <codecvt>
#include <string>
#include <vector>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Wininet.lib")

#include "../../Platform.h"

using namespace Autodesk::Identity::Client::Internal;

struct BLOB_DMY {
	BLOBHEADER header;
	DWORD len;
	BYTE key[0];
};

thread_local uint64_t m_platformErrorCode = 0;

pstring Platform::UrlEncode(const pstring& url)
{
	HRESULT apiResult = E_FAIL;
	wchar_t sizeTest[1] = { 0 };
	DWORD destSize = 1;

	size_t urlLen = url.length();

	m_platformErrorCode = 0;

	if (urlLen == 0 || urlLen > INTERNET_MAX_URL_LENGTH)
		return pstring();

	wstring urlW = ToDefaultString<wstring>(url);

	apiResult = UrlEscape(urlW.c_str(), sizeTest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult) && apiResult != E_POINTER)  //E_POINTER expected
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	if (destSize <= 0)
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	wstring destW;
	destW.resize(destSize);

	apiResult = UrlEscape(urlW.c_str(), &(destW.data())[0], &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult))
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	destW.resize(destSize);
	pstring dest = FromDefaultString<wstring>(destW);
	return dest;
}

pstring Platform::UrlDecode(const pstring& url)
{
	HRESULT apiResult = E_FAIL;
	wchar_t sizeTest[1] = { 0 };
	DWORD destSize = 1;

	size_t urlLen = url.length();

	m_platformErrorCode = 0;

	if (urlLen == 0 || urlLen > INTERNET_MAX_URL_LENGTH)
		return pstring();

	wstring urlW = ToDefaultString<wstring>(url);

	apiResult = UrlUnescape(&(urlW.data())[0], sizeTest, &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult) && apiResult != E_POINTER)  //E_POINTER expected
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	if (destSize <= 0)
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	wstring destW;
	destW.resize(destSize);

	apiResult = UrlUnescape(&(urlW.data())[0], &(destW.data())[0], &destSize, URL_ESCAPE_PERCENT | URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_ASCII_URI_COMPONENT);

	if (!SUCCEEDED(apiResult))
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	destW.resize(destSize);
	pstring dest = FromDefaultString<wstring>(destW);
	
	return dest;
}

pstring Platform::Base64Encode(const vector<uint8_t>& data)
{
	DWORD destSizeRecv = 0;
	BOOL result = FALSE;	

	size_t dataLength = data.size();

	if (dataLength > MAXDWORD || dataLength == 0)
		return pstring();
	
	result = CryptBinaryToStringW((const BYTE*)&data[0], (DWORD)dataLength, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &destSizeRecv);

	if (!result || destSizeRecv == 0)
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}
		
	wstring destW;
	destW.resize(destSizeRecv - 1);

	result = CryptBinaryToStringW((const BYTE*)&data[0], (DWORD)dataLength, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, &(destW.data())[0], &destSizeRecv);

	if (!result || destSizeRecv == 0)
	{
		m_platformErrorCode = (uint64_t)::GetLastError();
		return pstring();
	}

	pstring dest = FromDefaultString<wstring>(destW);

	return dest;
}

vector<uint8_t> Platform::HmacSha1(const vector<uint8_t>& data, const vector<uint8_t>& key)
{
	HCRYPTPROV  hProv = NULL;
	HCRYPTHASH  hHash = NULL;
	HCRYPTKEY   hKey = NULL;
	HCRYPTHASH  hHmacHash = NULL;
	DWORD       dwDataLen = 0;
	HMAC_INFO   hMacInfo = { 0 };
	BOOL		result = FALSE;
	vector<uint8_t> hashValue;
	const int hashSize = 20;
	
	m_platformErrorCode = 0;

	hMacInfo.HashAlgid = CALG_SHA1;

	result = CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_NEWKEYSET);
	if (!result)
		m_platformErrorCode = GetLastError();	

	struct BLOB_DMY* blob = NULL;
	size_t keyLen = key.size();
	size_t dataLen = data.size();

	if (keyLen > MAXDWORD || dataLen > MAXDWORD)
		return vector<uint8_t>();

	DWORD blobSize = sizeof(struct BLOB_DMY) + key.size() + 1;

	blob = (struct BLOB_DMY*)malloc(blobSize);

	if (blob == NULL)
		return vector<uint8_t>();

	memset(blob, 0, blobSize);

	blob->header.bType = PLAINTEXTKEYBLOB;
	blob->header.aiKeyAlg = CALG_RC2;
	blob->header.reserved = 0;
	blob->header.bVersion = CUR_BLOB_VERSION;
	blob->len = (DWORD)keyLen;

	memcpy(&(blob->key), &(key[0]), keyLen + 1); //Copy zero at end	

	result = CryptImportKey(hProv, (BYTE*)blob, blobSize, 0, CRYPT_IPSEC_HMAC_KEY, &hKey);
	if (!result)
		m_platformErrorCode = GetLastError();

	result = CryptCreateHash(hProv, CALG_HMAC, hKey, 0, &hHmacHash);
	if (!result)
		m_platformErrorCode = GetLastError();

	result = CryptSetHashParam(hHmacHash, HP_HMAC_INFO, (BYTE*)&hMacInfo, 0);
	if (!result)
		m_platformErrorCode = GetLastError();

	result = CryptHashData(hHmacHash, (BYTE*)&data[0], dataLen, 0);
	if (!result)
		m_platformErrorCode = GetLastError();

	hashValue.resize(hashSize, 0);
	dwDataLen = hashSize;

	result = CryptGetHashParam(hHmacHash, HP_HASHVAL, (BYTE*)&(hashValue[0]), &dwDataLen, 0);
	if (!result)
		m_platformErrorCode = GetLastError();

	if (hHmacHash)
		CryptDestroyHash(hHmacHash);
	if (hKey)
		CryptDestroyKey(hKey);
	if (hHash)
		CryptDestroyHash(hHash);
	if (hProv)
		CryptReleaseContext(hProv, 0);
	
	return hashValue; //PlatformErrorCode set to 0 and return NULL if out of memory
}

HttpResponse HttpGet(const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers)
{
	pstring content = u"";

	return { 0, content };
}

HttpResponse HttpPost(const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers)
{
	pstring content = u"";

	return { 0, content };
}

bool Platform::TryParseUrl(const pstring& fullUrl, Url& url)
{
	bool ret = false;
	URL_COMPONENTS urlComponents = { 0 };

	wstring wstr = ToDefaultString<wstring>(fullUrl);

	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
	urlComponents.dwSchemeLength = DWORD(-1);
	urlComponents.dwHostNameLength = DWORD(-1);
	urlComponents.dwUserNameLength = DWORD(-1);
	urlComponents.dwPasswordLength = DWORD(-1);
	urlComponents.dwUrlPathLength = DWORD(-1);
	urlComponents.dwExtraInfoLength = DWORD(-1);
	urlComponents.nPort = 0;

	ret = ::InternetCrackUrl(wstr.c_str(), wstr.length(), 0, &urlComponents);

	if (ret)
	{
		if (urlComponents.dwSchemeLength > 0 && urlComponents.lpszScheme != NULL)
		{
			wstring wstr(urlComponents.lpszScheme, urlComponents.dwSchemeLength);
			url.m_protocol = FromDefaultString(wstr);
		}

		if (urlComponents.dwHostNameLength > 0 && urlComponents.lpszHostName != NULL)
		{
			wstring wstr(urlComponents.lpszHostName, urlComponents.dwHostNameLength);
			url.m_host = FromDefaultString(wstr);
		}

		if (urlComponents.dwUrlPathLength > 1 && urlComponents.lpszUrlPath != NULL)
		{
			wstring wstr(urlComponents.lpszUrlPath, 1, urlComponents.dwUrlPathLength - 1);
			url.m_path = FromDefaultString(wstr);
		}

		if (urlComponents.dwExtraInfoLength > 0 && urlComponents.lpszExtraInfo != NULL)
		{
			wstring query(urlComponents.lpszExtraInfo, urlComponents.dwExtraInfoLength + 1); //Skip the '?'
			unsigned int port = urlComponents.nPort;

			size_t fragmentPos = query.find_last_of(L'#');

			if (fragmentPos != wstring::npos)
			{
				url.m_queryString = FromDefaultString(query.substr(1, fragmentPos - 1));

				if (fragmentPos + 1 < query.length())
					url.m_fragment = FromDefaultString(query.substr(fragmentPos + 1, query.length() - 1 - (fragmentPos + 1)));
			}
			else
				url.m_queryString = FromDefaultString(query);
		}

		url.m_port = urlComponents.nPort;
	}
	
	return ret;
}

template<class TString>
TString Platform::ToDefaultString(const pstring& str)
{	
	wstring strRet(str.begin(), str.end());
	return strRet;
}

template<class TString>
pstring Platform::FromDefaultString(const TString& str)
{
	return pstring(str.begin(), str.end());
}

pstring Platform::FromMbs(const string& str)
{
	wstring strRet;
	strRet.resize(str.length());

	mbstowcs(&(strRet.data())[0], str.c_str(), str.length());

	return FromDefaultString<wstring>(strRet);
}

pstring Platform::FromUI64(uint64_t num)
{
	wstring str = std::to_wstring(num);
	return FromDefaultString<wstring>(str);
}