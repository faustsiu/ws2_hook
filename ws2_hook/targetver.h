#pragma once

// �U�C�����|�w�q�̧C���x�ݨD�C�̧C���x�ݨD�O���U�ت�����
// Windows�BInternet Explorer ���A��ư������ε{���һݥ\�઺
// �̦������C�������@�ΡA�O�b���w�Χ�s���������x�W�ҥ�
// �Ҧ��i�Ϊ��\��C

// �p�G�z�������u����������x�A�Эק�U�C�w�q�C
// �Ѧ� MSDN ���o���P���x�����Ȫ��̷s��T�C
#ifndef WINVER                          // ���w�̧C���x�ݨD�� Windows Vista�C
#define WINVER 0x0600           // �N���ܧ󬰰w�� Windows ��L�������A��ȡC
#endif

#ifndef _WIN32_WINNT            // ���w�̧C���x�ݨD�� Windows Vista�C
#define _WIN32_WINNT 0x0600     // �N���ܧ󬰰w�� Windows ��L�������A��ȡC
#endif

#ifndef _WIN32_WINDOWS          // ���w�̧C���x�ݨD�� Windows 98�C
#define _WIN32_WINDOWS 0x0410 // �N���ܧ󬰰w�� Windows Me (�t) �H�᪩�����A��ȡC
#endif

#ifndef _WIN32_IE                       // ���w�̧C���x�ݨD�� Internet Explorer 7.0�C
#define _WIN32_IE 0x0700        // �N���ܧ󬰰w�� IE ��L�������A��ȡC
#endif
