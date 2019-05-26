#pragma once

#include <string>
#include <vector>
#include <map>

#include "./Internal/IdApp.h"

using namespace std;

typedef u16string pstring;
typedef char16_t pchar;

typedef enum 
{
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE,
	HTTP_PATCH
}HttpMethod;

BEGIN_NAMESPACE4(Autodesk, Identity, Client, Internal)

typedef struct __responseHttp
{
	int m_httpStatusCode;
	pstring m_content;

}HttpResponse;

typedef struct __url
{
	pstring m_protocol;
	pstring m_host;
	unsigned int m_port;

	pstring m_path;
	pstring m_fragment;

	pstring m_queryString;

	map<pstring, pstring> m_queries;

}Url;

class Platform
{
private:

	template<class TString>
	TString GetHttpAgentName();

public:
	pstring UrlEncode(const pstring& url);
	pstring UrlDecode(const pstring& url);	

	pstring Base64Encode(const vector<uint8_t>& data);

	vector<uint8_t> HmacSha1(const vector<uint8_t>& data, const vector<uint8_t>& key);

	HttpResponse HttpGet(const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers); //Returns HTTP Status Codes. 
	HttpResponse HttpPost(const pstring& url, const map<pstring, pstring>& queries, const map<pstring, pstring>& headers); //Returns HTTP Status Codes. 

	bool TryParseUrl(const pstring& fullUrl, Url& url);

public:

	class _strings
	{
	public:
		template<class TString>
		TString ToDefaultString(const pstring& str);

		template<class TString>
		pstring FromDefaultString(const TString& str);

		pstring FromMbs(const string& str);

		pstring FromUI64(uint64_t num);

		string ToUtf8(const pstring& str);

		string ToMbs(const pstring& str);

	} Strings;
};


END_NAMESPACE4()