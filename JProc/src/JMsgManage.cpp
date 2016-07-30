#include "StdAfx.h"
#include "JMsgManage.h"

JMsgManage::JMsgManage()
{
	m_pJMessage = NULL;
	m_pFree = NULL;
	m_pMsgHead = NULL;
	m_pMsgTail = NULL;
}

JMsgManage::~JMsgManage()
{
	for (int i = 0; i < m_nMsgSize; i++)
	{
		SAFE_DELETE(m_pJMessage[i]->pBuffer);
		SAFE_DELETE(m_pJMessage[i]);
	}

	SAFE_DELETE(m_pJMessage);
	SAFE_DELETE(m_pMsgHead);
}

bool JMsgManage::InitMsgManage(int nMsgLength, int nMsgSize)
{
	m_nMsgLength = nMsgLength;
	m_nMsgSize = nMsgSize;

	//创建对象指针连续的内存空间
	m_pJMessage = new JMessage *[m_nMsgSize];
	if (!m_pJMessage)
		return false;

	//创建各个消息的缓冲区
	for (int i = 0; i < m_nMsgSize; i++)
	{
		if ((m_pJMessage[i] = new JMessage()) == NULL)
			return false;
		if ((m_pJMessage[i]->pBuffer = new byte[m_nMsgLength]) == NULL)
			return false;
	}

	//创建消息头保护结点
	m_pMsgHead = new JMessage();
	if (!m_pMsgHead)
		return false;

	//清空消息队列
	ClearMessage();
	return true;
}

void JMsgManage::ClearMessage()
{
	m_mutex.Lock();

	//设置空闲结点的后向指针
	for (int i = 0; i < m_nMsgSize - 1; i++)
		m_pJMessage[i]->m_pNext = m_pJMessage[i + 1];

	m_pJMessage[m_nMsgSize - 1]->m_pNext = NULL;

	//设置空闲队列的队头指针
	m_pFree = m_pJMessage[0];

	//设置消息头和消息尾指针
	m_pMsgHead->m_pNext = NULL;
	m_pMsgTail = m_pMsgHead;
	m_nMsgCount = 0;

	m_mutex.Unlock();
}

void JMsgManage::_PushFront(JMessage *pNode)
{
	pNode->m_pNext = m_pMsgHead->m_pNext;
	m_pMsgHead->m_pNext = pNode;

	m_nMsgCount++;

	//队列不为空时，队尾不变
	if (m_pMsgTail == m_pMsgHead)
		m_pMsgTail = pNode;
}

void JMsgManage::_PushBack(JMessage *pNode)
{
	m_nMsgCount++;

	m_pMsgTail->m_pNext = pNode;
	m_pMsgTail = pNode;
}

JMessage *JMsgManage::_TakeFront()
{
	JMessage *pNode = m_pMsgHead->m_pNext;
	m_pMsgHead->m_pNext = pNode->m_pNext;

	m_nMsgCount--;

	if (m_pMsgTail == pNode)
		m_pMsgTail = m_pMsgHead;

	return pNode;
}

void JMsgManage::_Delete(JMessage *pParent, JMessage *pNode)
{
	pParent->m_pNext = pNode->m_pNext;

	m_nMsgCount--;

	if (m_pMsgTail == pNode)
		m_pMsgTail = pParent;

	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
}

bool JMsgManage::SendMessage(dword dwEvent, byte *pBuff, int nLength)
{
	m_mutex.Lock();

	if (m_pFree)
	{
		JMessage *pJMessage = _GetNode();
		pJMessage->m_pNext = NULL;

		pJMessage->dwEvent = dwEvent;
		pJMessage->nLength = nLength;
		memcpy(pJMessage->pBuffer, pBuff, nLength);

		_PushFront(pJMessage);
	}
	else
	{
		m_mutex.Unlock();
		return false;
	}

	m_mutex.Unlock();
	return true;
}

bool JMsgManage::PostMessage(dword dwEvent, byte *pBuff, int nLength)
{
	m_mutex.Lock();

	if (m_pFree)
	{
		JMessage *pJMessage = _GetNode();
		pJMessage->m_pNext = NULL;

		pJMessage->dwEvent = dwEvent;
		pJMessage->nLength = nLength;
		memcpy(pJMessage->pBuffer, pBuff, nLength);

		_PushBack(pJMessage);
	}
	else
	{
		m_mutex.Unlock();
		return false;
	}

	m_mutex.Unlock();
	return true;
}

bool JMsgManage::GetMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax)
{
	m_mutex.Lock();

	JMessage *pParent = m_pMsgHead;
	while (pParent->m_pNext)
	{
		JMessage *pNode = pParent->m_pNext;
		if (pNode->dwEvent >= dwFilterMin && (pNode->dwEvent <= dwFilterMax || dwFilterMax == 0))
		{
			pJMessage->dwEvent = pNode->dwEvent;
			pJMessage->nLength = pNode->nLength;
			memcpy(pJMessage->pBuffer, pNode->pBuffer, pNode->nLength);

			//从消息队列删除并放入空闲队列
			_Delete(pParent, pNode);

			m_mutex.Unlock();
			return pJMessage->dwEvent != EV_QUIT;
		}
		pParent = pParent->m_pNext;
	}

	//OnWaitMessage();

	m_mutex.Unlock();
	return pJMessage->dwEvent != EV_QUIT;
}

bool JMsgManage::PeekMessage(JMessage *pJMessage, bool bRemove)
{
	m_mutex.Lock();

	if (m_nMsgCount == 0)
	{
		m_mutex.Unlock();
		return false;
	}

	JMessage *pNode = m_pMsgHead->m_pNext;
	pJMessage->dwEvent = pNode->dwEvent;
	pJMessage->nLength = pNode->nLength;
	memcpy(pJMessage->pBuffer, pNode->pBuffer, pNode->nLength);

	if (bRemove)
		_RtnNode(_TakeFront());

	m_mutex.Unlock();
	return true;
}

bool JMsgManage::PeekMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax, bool bRemove)
{
	//取消息头结点，可以快速处理
	if (dwFilterMin == 0 && dwFilterMax == 0)
		return PeekMessage(pJMessage, bRemove);

	//从这里开始加锁
	m_mutex.Lock();

	JMessage *pParent = m_pMsgHead;
	while (pParent->m_pNext)
	{
		JMessage *pNode = pParent->m_pNext;
		if (pNode->dwEvent >= dwFilterMin && (pNode->dwEvent <= dwFilterMax || dwFilterMax == 0))
		{
			pJMessage->dwEvent = pNode->dwEvent;
			pJMessage->nLength = pNode->nLength;
			memcpy(pJMessage->pBuffer, pNode->pBuffer, pNode->nLength);

			//从消息队列删除并放入空闲队列
			if (bRemove)
				_Delete(pParent, pNode);

			m_mutex.Unlock();
			return true;
		}
		else
		{
			//删除时不能移动
			pParent = pParent->m_pNext;
		}
	}
	
	m_mutex.Unlock();
	return false;
}
