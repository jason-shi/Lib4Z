#include "StdAfx.h"
#include "JProcess.h"
#include "JProcManage.h"

JProcManage::JProcManage()
{
}

JProcManage::~JProcManage()
{
	for (int i = 0; i < m_vecJProc.size(); i++)
		SAFE_DELETE(m_vecJProc[i]);

	for (int i = 0; i < 8 + MAX_PRIORITY; i++)
		SAFE_DELETE(m_pProcHead[i]);
}

bool JProcManage::Initial()
{
	//������ͷ�������
	for (int i = 0; i < 8 + MAX_PRIORITY; i++)
	{
		m_pProcHead[i] = m_pProcTail[i] = new JProcess(0, 0);
		if (!m_pProcHead[i])
			return false;
		m_nProcCount[i] = 0;
	}
	return true;
}

void JProcManage::_PushFront(int nIndex, JProcess *pNode)
{
	pNode->m_pNext = m_pProcHead[nIndex]->m_pNext;
	m_pProcHead[nIndex]->m_pNext = pNode;

	m_nProcCount[nIndex]++;

	//���в�Ϊ��ʱ����β����
	if (m_pProcTail[nIndex] == m_pProcHead[nIndex])
		m_pProcTail[nIndex] = pNode;
}

void JProcManage::_PushBack(int nIndex, JProcess *pNode)
{
	m_nProcCount[nIndex]++;

	m_pProcTail[nIndex]->m_pNext = pNode;
	m_pProcTail[nIndex] = pNode;
}

JProcess *JProcManage::_TakeFront(int nIndex)
{
	JProcess *pNode = m_pProcHead[nIndex]->m_pNext;
	m_pProcHead[nIndex]->m_pNext = pNode->m_pNext;

	//����ָ��Ҫ�ÿ�
	pNode->m_pNext = NULL;

	m_nProcCount[nIndex]--;

	if (m_pProcTail[nIndex] == pNode)
		m_pProcTail[nIndex] = m_pProcHead[nIndex];

	return pNode;
}

void JProcManage::_Delete(int nIndex, JProcess *pParent, JProcess *pNode)
{
	pParent->m_pNext = pNode->m_pNext;

	m_nProcCount[nIndex]--;

	if (m_pProcTail[nIndex] == pNode)
		m_pProcTail[nIndex] = pParent;
}

void JProcManage::_TurnBack(int nIndex)
{
	if (m_nProcCount[nIndex] >= 2)
	{
		JProcess *pNode = m_pProcHead[nIndex]->m_pNext;
		m_pProcHead[nIndex]->m_pNext = pNode->m_pNext;

		pNode->m_pNext = NULL;
		m_pProcTail[nIndex]->m_pNext = pNode;
		m_pProcTail[nIndex] = pNode;
	}
}

void JProcManage::AddProcess(JProcess *pJProcess)
{
	m_mutex.Lock();

	if (pJProcess == NULL)
		return;
	m_vecJProc.push_back(pJProcess);

	//�½����̶���
	_PushBack(JP_NEWPROC, pJProcess);

	m_mutex.Unlock();
}

void JProcManage::DelProcess(JProcess *pJProcess)
{
	m_mutex.Lock();

	int nIndex = pJProcess->GetQIndex();

	//�ӽ������ڶ��в��Ҹ����
	JProcess *pParent = m_pProcHead[nIndex];
	while (pParent->m_pNext)
	{
		JProcess *pNode = pParent->m_pNext;
		if (pNode == pJProcess)
		{
			_Delete(nIndex, pParent, pNode);
			break;
		}
		pParent = pParent->m_pNext;
	}

	VECJPROC::iterator iter;
	for (iter = m_vecJProc.begin(); iter != m_vecJProc.end(); iter++)
	{
		if (*iter == pJProcess)
		{
			m_vecJProc.erase(iter);
			break;
		}
	}

	m_mutex.Unlock();
}

JProcess *JProcManage::FindProcess(dword dwProcID)
{
	m_mutex.Lock();

	VECJPROC::iterator iter;
	for (iter = m_vecJProc.begin(); iter != m_vecJProc.end(); iter++)
	{
		if ((*iter)->GetProcID() == dwProcID)
		{
			m_mutex.Unlock();
			return *iter;
		}
	}
	m_mutex.Unlock();
	return NULL;
}

bool JProcManage::Suspend(JProcess *pJProcess)
{
	int nIndex1 = pJProcess->GetQIndex();

	if (pJProcess->m_byStatus >= JP_READY)
		pJProcess->m_byStatus = JP_SUS_READY;
	else if (pJProcess->m_byStatus == JP_BLOCKED)
		pJProcess->m_byStatus = JP_SUS_BLKED;
	else
		return false;

	int nIndex2 = pJProcess->m_byStatus;

	_MoveBack(nIndex1, nIndex2);
	return true;
}

bool JProcManage::Resume(JProcess *pJProcess)
{
	return true;
}

int JProcManage::WaitMe(JProcess *pJProcess)
{
	return 0;
}

void JProcManage::JProcSchedule(byte byCPUID)
{
	//�����Ѽ������������

	byte byPriority = m_pProcHead[JP_RUNNING]->m_pNext->GetPriority();

	//�����ж����Ƶ���������
	_MoveBack(JP_RUNNING, JP_READY + byPriority);

	//TODO:��ζ���������Դ����ǣ�

	//�ӹ���-����״̬תΪ����״̬���

	//�ӹ���-����״̬תΪ����״̬���


	//�Ӿ������а����ȼ���ת
	if (m_pProcHead[JP_READY + byPriority] == m_pProcTail[JP_READY + byPriority])
	{
		byPriority++;
		if (byPriority == MAX_PRIORITY)
			byPriority = 0;
	}

	//�ҵ���Ҫ����Ľ���
	_MoveBack(JP_READY + byPriority, JP_RUNNING);

	//���ش��������������

	//ͨ�������жϼ������
}
