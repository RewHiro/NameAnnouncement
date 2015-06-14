#include "Utility.h"

//�@string��wstring�ɕϊ�����֐�
//�@@param {string} [str] - �ϊ��������}���`�o�C�g������
//�@@param {wstring} [dest] - �ϊ�����wstring
void widen(const std::string& str, std::wstring& dest)
{
	wchar_t* wcs = new wchar_t[str.length() + 1];
	mbstowcs(wcs, str.c_str(), str.length() + 1);
	dest = wcs;
	delete[]wcs;
}