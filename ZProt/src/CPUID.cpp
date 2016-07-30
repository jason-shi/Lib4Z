#include "StdAfx.h"
#include "CPUID.h"

using namespace std;

//�����洢��Ϣ
DWORD				 deax;
DWORD				 debx;
DWORD				 decx;
DWORD				 dedx;

void ExeCPUID(DWORD veax) //��ʼ��CPU
{
	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}
}

long CCPUID::GetCPUFreq() //��ȡCPUƵ��,��λ: MHZ
{
	int start1, start2;
	_asm rdtsc
	_asm mov start1, eax
	Sleep(50);

	_asm rdtsc
	_asm mov start2, eax
	return (start2 - start1) / 50000;
}

string CCPUID::GetManID() //��ȡ��������Ϣ
{
	char ID[25]; //�洢��������Ϣ
	memset(ID, 0, sizeof(ID)); //��������� ID

	ExeCPUID(0); //��ʼ��
	memcpy(ID + 0, &debx, 4); //��������Ϣǰ�ĸ��ַ����Ƶ�����
	memcpy(ID + 4, &dedx, 4); //�м��ĸ�
	memcpy(ID + 8, &decx, 4); //����ĸ�

	//������� char * ,���������;�ʷ��� string ֵ
	return string(ID);
}

string CCPUID::GetCPUType()
{
	const DWORD id = 0x80000002; //��0x80000002��ʼ,��0x80000004����
	char CPUType[49]; //�����洢CPU�ͺ���Ϣ
	memset(CPUType, 0, sizeof(CPUType)); //��ʼ������

	for (DWORD t = 0 ; t < 3 ; t++ )
	{
		ExeCPUID(id + t);

		//ÿ��ѭ������,������Ϣ������
		memcpy(CPUType + 16 * t + 0, &deax, 4);
		memcpy(CPUType + 16 * t + 4, &debx, 4);
		memcpy(CPUType + 16 * t + 8, &decx, 4);
		memcpy(CPUType + 16 * t + 12, &dedx, 4);
	}

	return string(CPUType);
}

bool CCPUID::IsMMXSupport()
{
	int iEDXValue;

	_asm {
		mov eax, 1
		cpuid
		mov iEDXValue, edx
	}
	return (iEDXValue & 0x800000) != 0;	
}
