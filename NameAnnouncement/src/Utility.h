#pragma once
#include <string>

//�@string��wstring�ɕϊ�����֐�
//�@@param {string} [str] - �ϊ��������}���`�o�C�g������
//�@@param {wstring} [dest] - �ϊ�����wstring
void widen(const std::string& str, std::wstring& dest);