#include "Utility.h"

//　stringをwstringに変換する関数
//　@param {string} [str] - 変換したいマルチバイト文字列
//　@param {wstring} [dest] - 変換するwstring
void widen(const std::string& str, std::wstring& dest)
{
	wchar_t* wcs = new wchar_t[str.length() + 1];
	mbstowcs(wcs, str.c_str(), str.length() + 1);
	dest = wcs;
	delete[]wcs;
}