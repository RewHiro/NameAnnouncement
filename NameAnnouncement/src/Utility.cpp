#include "Utility.h"

//�@string��wstring�ɕϊ�����֐�
//�@@param {string} [str] - �ϊ��������}���`�o�C�g������
//�@@param {wstring} [dest] - �ϊ�����wstring
void widen(const std::string& str, std::wstring& dest)
{
	wchar_t* wcs = new wchar_t[str.length() + 1];
	size_t ret;
	mbstowcs_s(&ret, wcs, str.length() + 1, str.c_str(), _TRUNCATE);
	dest = wcs;
	delete[]wcs;
}