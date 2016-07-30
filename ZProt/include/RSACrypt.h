#ifndef __RSACrypt_h__
#define __RSACrypt_h__

#define PL			 33 //如果重新生成p与q，则PL定义了p与q的最大位数。这里定义为33保证p与q达到512bit长度

typedef struct
{
	int				 length;
	unsigned int	 n[MAX];
} Lint;

class CRSACrypt {
  public:
	CRSACrypt(bool bGenKey = false);
	virtual ~CRSACrypt();

  public:
	void			 Generate();
	bool			 SaveKey();
	std::string		 Encrypt(const std::string& Ms);
	std::string		 Decrypt(const std::string& Cs);

  private:
	Lint			 m_e;
	Lint			 m_d;
	Lint			 m_n;
};

#endif
