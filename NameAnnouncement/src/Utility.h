#pragma once
#include <string>

//　stringをwstringに変換する関数
//　@param {string} [str] - 変換したいマルチバイト文字列
//　@param {wstring} [dest] - 変換するwstring
void widen(const std::string& str, std::wstring& dest);