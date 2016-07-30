#ifndef __CorePlayerCreator_h__
#define __CorePlayerCreator_h__

class CCorePlayerCreator : public ICorePlayerCreator {
  public:
	SINGLETON(CCorePlayerCreator);
	~CCorePlayerCreator();
  private:
	CCorePlayerCreator();

  public:
	virtual IPassive	*CreateCorePlayer();
};

#endif
