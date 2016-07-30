#ifndef __corepacketdefine_h__
#define __corepacketdefine_h__

#include <netstream.h>
#include <defpacketdefine.h>

NET_PACKET(CUserInfo)
{
  public:
	std::string		 nick; //�ǳ�
	char			 gender; //�Ա�
	unsigned char	 age; //����
	std::string		 craft; //�л�
	std::string		 area; //����
	std::string		 mobile; //�ֻ���
	int				 SGNumber; //SG��
	std::string		 email; //����
	std::string		 declaration; //����

  public:
	CUserInfo() {}
	CUserInfo(const CUserInfo& info)
	{
		*this = info;
	}
	CUserInfo& operator=(const CUserInfo& info)
	{
		if(this == &info)
			return *this;
		nick = info.nick;
		gender = info.gender;
		age = info.age;
		craft = info.craft;
		area = info.area;
		mobile = info.mobile;
		SGNumber = info.SGNumber;
		email = info.email;
		declaration = info.declaration;
		return *this;
	}
};
NET_PACKET(CUserData)
{
  public:
	int				 score; //����
	char			 degree; //�ȼ�
	int				 recharge; //��ֵ�ܶ�
	int				 contribution; //����ֵ
	int				 nChairID; //��������id
	int				 nStatus; //���״̬

  public:
	CUserData() {}
	CUserData(const CUserData& data)
	{
		*this = data;
	}
	CUserData& operator=(const CUserData& data)
	{
		if(this == &data)
			return *this;
		score = data.score;
		degree = data.degree;
		recharge = data.recharge;
		contribution = data.contribution;
		nChairID = data.nChairID;
		nStatus = data.nStatus;
		return *this;
	}
};
NET_PACKET(CBreviary)
{
  public:
	std::string		 nick; //�ǳ�
	int				 score; //����
	int				 SGNumber; //SG��
	int				 nChairID; //��������id
	int				 nStatus; //���״̬

  public:
	CBreviary() {}
	CBreviary(const CBreviary& breviary)
	{
		*this = breviary;
	}
	CBreviary& operator=(const CBreviary& breviary)
	{
		if(this == &breviary)
			return *this;
		nick = breviary.nick;
		score = breviary.score;
		SGNumber = breviary.SGNumber;
		nChairID = breviary.nChairID;
		nStatus = breviary.nStatus;
		return *this;
	}
};
NET_PACKET(CLoginReq)
{
	int				 nPID;
};
NET_PACKET(CLoginAct)
{
	int				 nPID;
	int				 nRoomID;
};
NET_PACKET(CLoginNot)
{
};
NET_PACKET(CLoginRej)
{
	char			 cCode;
};
NET_PACKET(CUserListReq)
{
};
NET_PACKET(CUserListNot)
{
	std::vector<CBreviary> vecBreviary; //��������б�
};
NET_PACKET(CPlayerNumberNot)
{
	int				 nPlayerNum;
};
NET_PACKET(CEchoNot)
{
	unsigned int	 nNow;
};
NET_PACKET(CEnterTableAct) //����������Ϊ
{
	int				 nTableID;
	int				 nChairID;
};
NET_PACKET(CEnterTableNot) //�������ӹ㲥
{
	int				 nTableID;
	int				 nChairID;
	char			 cOwner;
};
NET_PACKET(CEnterTableRej) //�������Ӿܾ�
{
	char			 cCode;
};
NET_PACKET(CLeaveTableAct) //�뿪������Ϊ
{
};
NET_PACKET(CLeaveTableNot) //�뿪���ӹ㲥
{
	int				 nTableID;
	int				 nChairID;
	char			 cOwner;
};
NET_PACKET(CLeaveTableRej) //�뿪���Ӿܾ�
{
	char			 cCode;
};
NET_PACKET(CReadyAct) //������Ϊ
{
};
NET_PACKET(CReadyNot) //���ֹ㲥
{
	int				 nTableID;
	int				 nChairID;
};
NET_PACKET(CDetailReq) //��ϸ��Ϣ����
{
	std::string		 nick;
};
NET_PACKET(CDetailNot) //��ϸ��Ϣ�㲥
{
	CUserInfo		 userInfo;
	CUserData		 userData;
};
NET_GET(CUserInfo)
{
	return is >> pt.opcode >> pt.nick >> pt.gender >> pt.age >> pt.craft >> pt.area >> pt.mobile >> pt.SGNumber >> pt.email >> pt.declaration;
}
NET_PUT(CUserInfo)
{
	return os << pt.opcode << pt.nick << pt.gender << pt.age << pt.craft << pt.area << pt.mobile << pt.SGNumber << pt.email << pt.declaration;
}
NET_GET(CUserData)
{
	return is >> pt.opcode >> pt.score >> pt.degree >> pt.recharge >> pt.contribution >> pt.nChairID >> pt.nStatus;
}
NET_PUT(CUserData)
{
	return os << pt.opcode << pt.score << pt.degree << pt.recharge << pt.contribution << pt.nChairID << pt.nStatus;
}
NET_GET(CBreviary)
{
	return is >> pt.opcode >> pt.nick >> pt.score >> pt.SGNumber >> pt.nChairID >> pt.nStatus;
}
NET_PUT(CBreviary)
{
	return os << pt.opcode << pt.nick << pt.score << pt.SGNumber << pt.nChairID << pt.nStatus;
}
NET_GET(CLoginReq)
{
	return is >> pt.opcode >> pt.nPID;
};
NET_PUT(CLoginReq)
{
	return os << pt.opcode << pt.nPID;
};
NET_GET(CLoginAct)
{
	return is >> pt.opcode >> pt.nPID >> pt.nRoomID;
};
NET_PUT(CLoginAct)
{
	return os << pt.opcode << pt.nPID << pt.nRoomID;
};
NET_GET(CLoginNot)
{
	return is >> pt.opcode;
};
NET_PUT(CLoginNot)
{
	return os << pt.opcode;
};
NET_GET(CLoginRej)
{
	return is >> pt.opcode >> pt.cCode;
};
NET_PUT(CLoginRej)
{
	return os << pt.opcode << pt.cCode;
};
NET_GET(CUserListReq)
{
	return is >> pt.opcode;
}
NET_PUT(CUserListReq)
{
	return os << pt.opcode;
}
NET_GET(CUserListNot)
{
	return is >> pt.opcode >> pt.vecBreviary;
}
NET_PUT(CUserListNot)
{
	return os << pt.opcode << pt.vecBreviary;
}
NET_GET(CEchoNot)
{
	return is >> pt.opcode >> pt.nNow;
}
NET_PUT(CEchoNot)
{
	return os << pt.opcode << pt.nNow;
}
NET_GET(CEnterTableAct)
{
	return is >> pt.opcode >> pt.nTableID >> pt.nChairID;
}
NET_PUT(CEnterTableAct)
{
	return os << pt.opcode << pt.nTableID << pt.nChairID;
}
NET_GET(CEnterTableNot)
{
	return is >> pt.opcode >> pt.nTableID >> pt.nChairID >> pt.cOwner;
}
NET_PUT(CEnterTableNot)
{
	return os << pt.opcode << pt.nTableID << pt.nChairID << pt.cOwner;
}
NET_GET(CEnterTableRej)
{
	return is >> pt.opcode >> pt.cCode;
}
NET_PUT(CEnterTableRej)
{
	return os << pt.opcode << pt.cCode;
}
NET_GET(CLeaveTableAct)
{
	return is >> pt.opcode;
}
NET_PUT(CLeaveTableAct)
{
	return os << pt.opcode;
}
NET_GET(CLeaveTableNot)
{
	return is >> pt.opcode >> pt.nTableID >> pt.nChairID >> pt.cOwner;
}
NET_PUT(CLeaveTableNot)
{
	return os << pt.opcode << pt.nTableID << pt.nChairID << pt.cOwner;
}
NET_GET(CLeaveTableRej)
{
	return is >> pt.opcode >> pt.cCode;
}
NET_PUT(CLeaveTableRej)
{
	return os << pt.opcode << pt.cCode;
}
NET_GET(CPlayerNumberNot)
{
	return is >> pt.opcode >> pt.nPlayerNum;
}
NET_PUT(CPlayerNumberNot)
{
	return os << pt.opcode << pt.nPlayerNum;
}
NET_GET(CReadyAct)
{
	return is >> pt.opcode;
}
NET_PUT(CReadyAct)
{
	return os << pt.opcode;
}
NET_GET(CReadyNot)
{
	return is >> pt.opcode >> pt.nTableID >> pt.nChairID;
}
NET_PUT(CReadyNot)
{
	return os << pt.opcode << pt.nTableID << pt.nChairID;
}
NET_GET(CDetailReq)
{
	return is >> pt.opcode >> pt.nick;
}
NET_PUT(CDetailReq)
{
	return os << pt.opcode << pt.nick;
}
NET_GET(CDetailNot)
{
	return is >> pt.opcode >> pt.userInfo >> pt.userData;
}
NET_PUT(CDetailNot)
{
	return os << pt.opcode << pt.userInfo << pt.userData;
}

#endif
