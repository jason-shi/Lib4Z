#ifndef __RSACrypt_h__
#define __RSACrypt_h__

#define PL			 33 //�����������p��q����PL������p��q�����λ�������ﶨ��Ϊ33��֤p��q�ﵽ512bit����

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
