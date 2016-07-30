#ifndef __JQuitManage_h__
#define __JQuitManage_h__

class JQuitManage {
	typedef std::vector<dword>		 VECJQUIT;
  public:
	SINGLETON(JQuitManage);
	~JQuitManage();

  private:
	JQuitManage();

  public:
	void			 AddJQuit(dword dwProcID);
	void			 DelJQuit(dword dwProcID);
	bool			 CheckJQuit(dword dwProcID);

  protected:
	CIntLock		 m_mutex;
	VECJQUIT		 m_vecJQuit;
};

#define g_pJQuitManage				JQuitManage::GetInstancePtr()

#endif
