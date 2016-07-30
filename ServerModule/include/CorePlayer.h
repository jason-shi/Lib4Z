#ifndef __CorePlayer_h__
#define __CorePlayer_h__

#include <IServerModule.h>

enum CUserStatus {
	US_ZERO,
	US_UNCONNECTED,		//δ����
	US_CONNECTED,		//���ӳɹ�
	US_LOBBY,			//��¼�ɹ�
	US_TABLE,			//��ҽ�������
	US_READY,			//����
	US_RACING,			//������
	US_BROKEN,			//������
	OTHERUSERSTATUS,
};

enum CGameMode {
	GM_ZERO,
	GM_SINGLEPLAY,
	GM_LOCALNET,
	GM_BATTLENET,
	GM_LOADREPLAY,
	GM_ENTERFORUM,
	OTHERGAMEMODE,
};

class CCoreTable;
class CPlayerManage;
class CTableManage;
class CCorePlayer : public ICorePlayer, public IPassive {
  public:
	CCorePlayer();
	~CCorePlayer();

  public:
	void			 Release();
	IConnection		*GetConnection() { return m_pConnection; }

  public:
	//IPassive
	virtual void	 OnConnection(IConnection* pConnection);
	virtual void	 OnRecv(const char* pData, int nLen);
	virtual void	 OnDisconnect();
	virtual void	 SendLoginReq();

	//ICorePlayer
	ITable			*GetTable();
	template<class T>
		void		 SendPacket(const T& packet)
	{
		COutputStream os;
		os << packet;
		if(m_pConnection)
			m_pConnection->Send(os.GetData(), os.GetLength());
	}
	virtual void	 Send(const char* pData, int nLength);

	//�������Բ���
	const int		 GetPID() { return m_nPID; }
	IPlayer			*GetIPlayer() { return m_pPlayer; }
	void			 SetIPlayer(IPlayer *pPlayer) { m_pPlayer = pPlayer; }

	void			 SetChairID(int nChairID) { m_UserData.nChairID = nChairID; m_Breviary.nChairID = nChairID; }
	int				 GetChairID() { return m_UserData.nChairID; }
	void			 SetStatus(int nStatus) { m_UserData.nStatus = nStatus; m_Breviary.nStatus = nStatus; }
	int				 GetStatus() { return m_UserData.nStatus; }
	void			 SetNick(std::string strNick) { m_UserInfo.nick = strNick; m_Breviary.nick = strNick; }
	std::string		 GetNick() { return m_UserInfo.nick; }
	void			 SetForceLeave() { m_bForceLeave = true; }
	bool			 GetForceLeave() { return m_bForceLeave; }
	void			 SetScore(int nScore) { m_UserData.score = nScore; m_Breviary.score = nScore; }
	int				 GetScore() { return m_UserData.score; }
	const			 CUserInfo& GetUserInfo() { return m_UserInfo; }
	const			 CUserData& GetUserData() { return m_UserData; }
	const CBreviary& GetBreviary() { return m_Breviary; }
	void			 SetCoreTable(CCoreTable *pCoreTable) { m_pCoreTable = pCoreTable; }
	CCoreTable		*GetCoreTable() { return m_pCoreTable; }

  public:
	void			 HeartBit(uint nNow, const char* pData, int nLength);
	int				 GetNetDelay() { return m_nNetDelay; }
	const std::string&	 GetPlayerIP();
	const unsigned short GetPlayerPort();
	void			 SetBrokenStatus(bool bBroken) { m_bBroken = bBroken; }
	bool			 IsBroken() { return m_bBroken; }
	void			 UpdateHeartBit(uint nNow){ m_nLastPacketTime = nNow; }
	void			 SendPlayerNumber();

	void			 OnPacket(const CLoginAct& act);
	void			 OnPacket(const CUserListReq& req);
	void			 OnPacket(const CEnterTableAct& act);
	void			 OnPacket(const CLeaveTableAct& act);
	void			 OnPacket(const CReadyAct& act);
	void			 OnPacket(const CEchoNot& noti);
	void			 OnPacket(const CDetailReq& req);

  private:
	CCoreTable		*m_pCoreTable;
	CPlayerManage	*m_pPlayerManage;
	CTableManage	*m_pTableManage;
	IConnection		*m_pConnection;
	IPlayer			*m_pPlayer;
	int				 m_nNetDelay; //�����ӳ�
	bool			 m_bForceLeave; //�Ƿ�Ϊ�������
	std::string		 m_strAddr; //���ip
	unsigned short	 m_nPort; //���port
	bool			 m_bBroken; //�Ƿ����״̬
	CUserInfo		 m_UserInfo; //�����Ϣ
	CUserData		 m_UserData; //�������
	CBreviary		 m_Breviary; //�������
	unsigned int	 m_nLastPacketTime; //���һ���հ�ʱ��
	int				 m_nPID; //Ψһ��ʶ
};

#endif
