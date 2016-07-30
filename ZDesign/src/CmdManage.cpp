#include "StdAfx.h"
#include <boost\bind.hpp>

ICommand::ICommand()
{
	OnCommandProc = NULL;
}

ICommand::~ICommand()
{
}

void ICommand::SetCommandProc(FN_CommandProc CommandProc)
{
	OnCommandProc = CommandProc;
}

void ICommand::Excute()
{
	if (OnCommandProc)
		OnCommandProc(this);
}

ICmdZler::ICmdZler()
{
	m_pNext = NULL;
}

ICmdZler::~ICmdZler()
{
}

bool ICmdZler::ProcessCmd(int nType)
{
	return false;
}

bool ICmdZler::ProcessCmd(ICommand *pCommand)
{
	return false;
}

CCmdManage::CCmdManage()
{
	m_pHead = new ICmdZler();
}

CCmdManage::~CCmdManage()
{
	delete m_pHead;
}

void CCmdManage::AddCmdZler(ICmdZler *pCmdZler)
{
	if (pCmdZler)
	{
		//插入链尾
		ICmdZler *pNode = m_pHead;
		while (pNode->m_pNext)
			pNode = pNode->m_pNext;
		pNode->m_pNext = pCmdZler;
	}
}

void CCmdManage::DelCmdZler(ICmdZler *pCmdZler)
{
	ICmdZler *pNode = m_pHead;
	//查找目标并删除
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext == pCmdZler)
		{
			pNode->m_pNext = pCmdZler->m_pNext;
			pCmdZler->m_pNext = NULL;
			break;
		}
		pNode = pNode->m_pNext;
	}
}

bool CCmdManage::ProcessCmd(int nType)
{
	ICmdZler *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->ProcessCmd(nType))
			return true;
	}
	return false;
}

bool CCmdManage::ProcessCmd(ICommand *pCommand)
{
	ICmdZler *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->ProcessCmd(pCommand))
			return true;
	}
	return false;
}
