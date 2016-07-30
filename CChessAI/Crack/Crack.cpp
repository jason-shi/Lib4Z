#include "StdAfx.h"
#include "Crack.h"
#include "Traversal.h"
#include "Further.h"
#include "MoveList\MoveList.h"
#include "Generate\Generate.h"
#include "Astode\Astode.h"

using namespace std;

static HANDLE		 g_hThreadCrack = NULL;

CCrack::CCrack()
{
}

CCrack::~CCrack()
{
}

bool CCrack::Initial()
{
	::CreateAllTable();

	if (!::CreateMoveList())
		return false;
	if (!::CreateAstode())
		return false;

	//��������ɵ�������
	TPiece tTemp;
	int next = (int)&tTemp.next - (int)&tTemp;
	int prev = (int)&tTemp.prev - (int)&tTemp;
	if (!g_pZDeposit->Alloc(sizeof(TPiece), 32, next, prev))
		return false;

	return true;
}

void CCrack::Release()
{
	DeleteMoveList();
	DeleteAstode();
	DeleteAllTable();
}

static int CallCrack(LPVOID *lpParam)
{
	//�ƽ����ݳ�ʼ��
	memcpy(bakboard, board, sizeof(board));
	g_nFurtherEnd = 2;
	g_nBoundLower = 0;
	g_bFullAstode = false;
	//α�ƽ�ʱ�������ݿ�
	if (!g_tCrackParam.bPesudo)
		InitAstode();
	//��������ƽ����
	int nMaxDepth = g_tCrackParam.nMaxDepth;
	if (nMaxDepth == 0)
		nMaxDepth = MaxCrackDepth;
	//������ȶԿ������㷨
	for (nStep = 0, nDepth = 2; nDepth <= nMaxDepth; nStep = 0, nDepth += 2)
	{
		g_nActualDepth = nDepth;

		//Further��ȵ�����α�ƽ�ʱ�ܵ���
		if (g_nStoreSize < 0.7 * MaxStoreSize || g_tCrackParam.bPesudo)
			g_nFurtherEnd += 2;

		GetBoardDynData();
		int nResult = Traversal();
		//���ݿ��������㱾�غ�
		if (nResult == FULLASTODE)
		{
			memcpy(board, bakboard, sizeof(board));
			nDepth -= 2;
			continue;
		}
		if (nResult != REDLOSE)
		{
			memcpy(board, bakboard, sizeof(board));
			return nResult;
		}

		//�ڸ������δ�ƽ�
		if (nDepth < g_nFurtherEnd)
		{
			int nResult = CallFurther(g_tCrackParam.nFurther);
			memcpy(board, bakboard, sizeof(board)); //��ԭ����
			if (nResult == INTERRUPT)
				return INTERRUPT;
		}
	}
	return REDLOSE;
}

int CCrack::GetThinkDepth() const
{
	return g_nActualDepth;
}

int CCrack::GetAstodeSize() const
{
	return g_nStoreSize + g_nBufferSize;
}

int CCrack::GetSpendTime() const
{
	int nEndTick = GetTickCount();
	return nEndTick - g_nStartTick;
}

TCrackResult& CCrack::GetCrackResult() const
{
	return g_tCrackResult;
}
//-------------------------------------------------------------------------------------
static VOID WINAPI ThreadRFCrack(LPVOID *lpParam)
{
	g_nStartTick = GetTickCount();

	int nResult = ::CallCrack(lpParam);
	if (nResult == INTERRUPT)
		return;

	//������
	memset(&g_tCrackResult, 0, sizeof(TCrackResult));
	g_tCrackResult.nRound = g_pZCrack->GetThinkDepth() / 2;
	g_tCrackResult.nAstodeSize = g_pZCrack->GetAstodeSize();
	g_tCrackResult.nSpendTime = g_pZCrack->GetSpendTime();
	if (nResult == BLKLOSE)
	{
		g_tCrackResult.src = MoveList[0][trial[0] - 1]->src;
		g_tCrackResult.dest = MoveList[0][trial[0] - 1]->dest;
	}
	g_pZCrack->PostCrackResult(nResult);
}

bool CCrack::InitThreadRFCrack(TCrackParam *pCrackParam)
{
	if (!g_hThreadCrack)
	{
		memcpy(&g_tCrackParam, pCrackParam, sizeof(TCrackParam));
		memcpy(board, pCrackParam->board, sizeof(board));
		g_bInterrupt = false;
		//���������ƽ��߳�
		g_hThreadCrack = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadRFCrack, NULL, 0, 0);
		if (g_hThreadCrack)
			return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------
static int BFCrackEachOne(LPVOID *lpParam)
{
	static int bfcboard[11 * 16] = { 0 };
	memcpy(bfcboard, board, sizeof(board));

	vector<TMove>::iterator iter;
	for (iter = vecTestMove.begin(); iter != vecTestMove.end(); iter++)
	{
		//�޳��ظ����߷�
		if (iter->src == g_tCrackParam.src && iter->dest == g_tCrackParam.dest)
			continue;

		board[iter->dest] = board[iter->src];
		board[iter->src] = 0;

		::GetBoardDynData();
		if (InCheckBlk())
		{
			memcpy(board, bfcboard, sizeof(board));
			continue;
		}
		int nResult = ::CallCrack(lpParam);
		if (nResult != BLKLOSE)
		{
			memcpy(board, bfcboard, sizeof(board));
			return nResult;
		}
		//��ԭ����
		memcpy(board, bfcboard, sizeof(board));
	}
	return BLKLOSE;
}

static VOID WINAPI ThreadBFCrack(LPVOID *lpParam)
{
	int nCheckPos;
	GetBoardDynData();
	if (CalcCheckPos(nCheckPos) == false)
	{
		//�췽���ڱ�����״̬
		g_pZCrack->PostCrackResult(REDLOSE);
		return;
	}
	//�õ��ڷ���һ���ŷ��ݴ�
	nStep = 1;
	::GenerateBlk(nCheckPos);

	vecTestMove.clear();
	for (int i = 0; i < Trial[1]; i++)
	{
		vecTestMove.push_back(*MoveList[1][i]);
	}
	//����ƽⲢ�������ֿ��غ���
	int nResult = ::BFCrackEachOne(lpParam);
	if (nResult == INTERRUPT)
		return;

	g_pZCrack->PostCrackResult(nResult);
}

bool CCrack::InitThreadBFCrack(TCrackParam *pCrackParam)
{
	if (!g_hThreadCrack)
	{
		memcpy(&g_tCrackParam, pCrackParam, sizeof(TCrackParam));
		memcpy(board, pCrackParam->board, sizeof(board));

		g_bInterrupt = false;
		//���������ƽ��߳�
		g_hThreadCrack = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadBFCrack, NULL, 0, 0);
		if (g_hThreadCrack)
			return true;
	}
	return false;
}

void CCrack::WaitForThreadCrack()
{
	DWORD dwResult = WaitForSingleObject(g_hThreadCrack, 3000);
	switch (dwResult)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		window->MessageBox(L"   �ǰ�ȫֹͣ-��ʱ   ", L"�澯", MB_ICONWARNING | MB_OK);
		break;
	case WAIT_FAILED:
		window->MessageBox(L"   �ǰ�ȫֹͣ-�쳣   ", L"�澯", MB_ICONWARNING | MB_OK);
		break;
	}
	CloseHandle(g_hThreadCrack);
	g_hThreadCrack = NULL;
}

void CCrack::ExitThreadCrack()
{
	if (g_hThreadCrack)
	{
		g_bInterrupt = true;
		WaitForThreadCrack();
	}
	PeekMessage(NULL, HWND(*window), m_uMsgId, m_uMsgId, PM_REMOVE);
}
