#ifndef	__CmdManage_h__
#define	__CmdManage_h__

#include <SingleTon.h>
#include <boost\function.hpp>

class ICommand;
typedef boost::function<void(ICommand *pCommand)>		 FN_CommandProc;

class ICmdZler;
class ICommand {
  public:
	ICommand();
	virtual ~ICommand();

  public:
	void			 SetCommandProc(FN_CommandProc CommandProc);
	void			 Excute();

  protected:
	FN_CommandProc	 OnCommandProc;
};

class ICmdZler {
  public:
	ICmdZler();
	virtual ~ICmdZler();

  public:
	virtual bool	 ProcessCmd(int nType);
	virtual bool	 ProcessCmd(ICommand *pCommand);

  public:
	ICmdZler		*m_pNext;
};

class CCmdManage {
  public:
	SINGLETON(CCmdManage);
	~CCmdManage();
  private:
	CCmdManage();

  public:
	void			 AddCmdZler(ICmdZler *pCmdZler);
	void			 DelCmdZler(ICmdZler *pCmdZler);
	bool			 ProcessCmd(int nType);
	bool			 ProcessCmd(ICommand *pCommand);

  public:
	void			 SetActived(ICmdZler *pCmdZler);
	ICmdZler		*GetActived() const;

  protected:
	ICmdZler		*m_pHead;
	ICmdZler		*m_pActived;
};

inline void CCmdManage::SetActived(ICmdZler *pCommand)
{
	m_pActived = pCommand;
}

inline ICmdZler *CCmdManage::GetActived() const
{
	return m_pActived;
}

#define g_pCmdManage			 CCmdManage::GetInstancePtr()

#endif
