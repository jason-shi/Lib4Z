#include "StdAfx.h"
#include "JReqManage.h"

JReqManage::JReqManage()
{
	m_pJRequest = NULL;
	m_pFree = NULL;
	m_pReqHead = NULL;
	m_pReqTail = NULL;
}

JReqManage::~JReqManage()
{
	for (int i = 0; i < m_nReqSize; i++)
	{
		SAFE_DELETE(m_pJRequest[i]->pBuffer);
		SAFE_DELETE(m_pJRequest[i]);
	}

	SAFE_DELETE(m_pJRequest);
	SAFE_DELETE(m_pReqHead);
}

bool JReqManage::InitReqManage(int nReqLength, int nReqSize)
{
	m_nReqLength = nReqLength;
	m_nReqSize = nReqSize;

	//��������ָ���������ڴ�ռ�
	m_pJRequest = new JRequest *[m_nReqSize];
	if (!m_pJRequest)
		return false;

	//����������Ϣ�Ļ�����
	for (int i = 0; i < m_nReqSize; i++)
	{
		if ((m_pJRequest[i] = new JRequest()) == NULL)
			return false;
		if ((m_pJRequest[i]->pBuffer = new byte[m_nReqLength]) == NULL)
			return false;
	}

	//������Ϣͷ�������
	m_pReqHead = new JRequest();
	if (!m_pReqHead)
		return false;

	//�����Ϣ����
	ClearRequest();
	return true;
}

void JReqManage::ClearRequest()
{
	m_mutex.Lock();

	//���ÿ��н��ĺ���ָ��
	for (int i = 0; i < m_nReqSize - 1; i++)
		m_pJRequest[i]->m_pNext = m_pJRequest[i + 1];

	m_pJRequest[m_nReqSize - 1]->m_pNext = NULL;

	//���ÿ��ж��еĶ�ͷָ��
	m_pFree = m_pJRequest[0];

	//������Ϣͷ����Ϣβָ��
	m_pReqHead->m_pNext = NULL;
	m_pReqTail = m_pReqHead;
	m_nReqCount = 0;

	m_mutex.Unlock();
}

void JReqManage::_PushFront(JRequest *pNode)
{
	pNode->m_pNext = m_pReqHead->m_pNext;
	m_pReqHead->m_pNext = pNode;

	m_nReqCount++;

	//���в�Ϊ��ʱ����β����
	if (m_pReqTail == m_pReqHead)
		m_pReqTail = pNode;
}

void JReqManage::_PushBack(JRequest *pNode)
{
	m_nReqCount++;

	m_pReqTail->m_pNext = pNode;
	m_pReqTail = pNode;
}

JRequest *JReqManage::_TakeFront()
{
	JRequest *pNode = m_pReqHead->m_pNext;
	m_pReqHead->m_pNext = pNode->m_pNext;

	m_nReqCount--;

	if (m_pReqTail == pNode)
		m_pReqTail = m_pReqHead;

	return pNode;
}

void JReqManage::_Delete(JRequest *pParent, JRequest *pNode)
{
	pParent->m_pNext = pNode->m_pNext;

	m_nReqCount--;

	if (m_pReqTail == pNode)
		m_pReqTail = pParent;

	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
}

bool JReqManage::SendRequest(dword dwEvent, byte *pBuff, int nLength)
{
	m_mutex.Lock();

	if (m_pFree)
	{
		JRequest *pJRequest = _GetNode();
		pJRequest->m_pNext = NULL;

		pJRequest->dwEvent = dwEvent;
		pJRequest->nLength = nLength;
		memcpy(pJRequest->pBuffer, pBuff, nLength);

		_PushFront(pJRequest);
	}
	else
	{
		m_mutex.Unlock();
		return false;
	}

	m_mutex.Unlock();
	return true;
}

bool JReqManage::PostRequest(dword dwEvent, byte *pBuff, int nLength)
{
	m_mutex.Lock();

	if (m_pFree)
	{
		JRequest *pJRequest = _GetNode();
		pJRequest->m_pNext = NULL;

		pJRequest->dwEvent = dwEvent;
		pJRequest->nLength = nLength;
		memcpy(pJRequest->pBuffer, pBuff, nLength);

		_PushBack(pJRequest);
	}
	else
	{
		m_mutex.Unlock();
		return false;
	}

	m_mutex.Unlock();
	return true;
}

bool JReqManage::GetRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax)
{
	m_mutex.Lock();

	JRequest *pParent = m_pReqHead;
	while (pParent->m_pNext)
	{
		JRequest *pNode = pParent->m_pNext;
		if (pNode->dwEvent >= dwFilterMin && (pNode->dwEvent <= dwFilterMax || dwFilterMax == 0))
		{
			pJRequest->dwEvent = pNode->dwEvent;
			pJRequest->nLength = pNode->nLength;
			memcpy(pJRequest->pBuffer, pNode->pBuffer, pNode->nLength);

			//����Ϣ����ɾ����������ж���
			_Delete(pParent, pNode);

			m_mutex.Unlock();
			return pJRequest->dwEvent != EV_QUIT;
		}
		pParent = pParent->m_pNext;
	}

	//OnWaitRequest();

	m_mutex.Unlock();
	return pJRequest->dwEvent != EV_QUIT;
}

bool JReqManage::PeekRequest(JRequest *pJRequest, bool bRemove)
{
	m_mutex.Lock();

	if (m_nReqCount == 0)
	{
		m_mutex.Unlock();
		return false;
	}

	JRequest *pNode = m_pReqHead->m_pNext;
	pJRequest->dwEvent = pNode->dwEvent;
	pJRequest->nLength = pNode->nLength;
	memcpy(pJRequest->pBuffer, pNode->pBuffer, pNode->nLength);

	if (bRemove)
		_RtnNode(_TakeFront());

	m_mutex.Unlock();
	return true;
}

bool JReqManage::PeekRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax, bool bRemove)
{
	//ȡ��Ϣͷ��㣬���Կ��ٴ���
	if (dwFilterMin == 0 && dwFilterMax == 0)
		return PeekRequest(pJRequest, bRemove);

	//�����￪ʼ����
	m_mutex.Lock();

	JRequest *pParent = m_pReqHead;
	while (pParent->m_pNext)
	{
		JRequest *pNode = pParent->m_pNext;
		if (pNode->dwEvent >= dwFilterMin && (pNode->dwEvent <= dwFilterMax || dwFilterMax == 0))
		{
			pJRequest->dwEvent = pNode->dwEvent;
			pJRequest->nLength = pNode->nLength;
			memcpy(pJRequest->pBuffer, pNode->pBuffer, pNode->nLength);

			//����Ϣ����ɾ����������ж���
			if (bRemove)
				_Delete(pParent, pNode);

			m_mutex.Unlock();
			return true;
		}
		else
		{
			//ɾ��ʱ�����ƶ�
			pParent = pParent->m_pNext;
		}
	}
	
	m_mutex.Unlock();
	return false;
}
