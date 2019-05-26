#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <codecvt>
#include <string>
#include <vector>

#include "../../Platform.h"

using namespace Autodesk::Identity::Client::Internal;

template<class TString>
TString Platform::_strings::ToDefaultString(const pstring& str)
{
	wstring strRet(str.begin(), str.end());
	return strRet;
}

template<class TString>
pstring Platform::_strings::FromDefaultString(const TString& str)
{
	return pstring(str.begin(), str.end());
}

pstring Platform::_strings::FromMbs(const string& str)
{
	wstring strRet;
	strRet.resize(str.length());

	mbstowcs(&(strRet.data())[0], str.c_str(), str.length());

	return FromDefaultString<wstring>(strRet);
}

pstring Platform::_strings::FromUI64(uint64_t num)
{
	wstring str = to_wstring(num);
	return FromDefaultString<wstring>(str);
}

string Platform::_strings::ToUtf8(const pstring& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t*>(str.data());

	return convert.to_bytes(p, p + str.size());
}

string Platform::_strings::ToMbs(const pstring & str)
{
	wstring wstr = ToDefaultString<wstring>(str);
	string dest;

	dest.resize(wstr.length(), 0);

	wcstombs(&(dest.data())[0], wstr.c_str(), wstr.length());

	return dest;
}

#endif //_WIN32
