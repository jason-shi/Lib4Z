#include "StdAfx.h"
#include "CorePlayerCreator.h"
#include "CorePlayer.h"
#include "PlayerManage.h"

CCorePlayerCreator::CCorePlayerCreator()
{
}

CCorePlayerCreator::~CCorePlayerCreator()
{
}

IPassive* CCorePlayerCreator::CreateCorePlayer()
{
	return new CCorePlayer();
}
