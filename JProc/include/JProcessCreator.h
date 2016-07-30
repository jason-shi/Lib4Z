#ifndef __JProcessCreator_h__
#define __JProcessCreator_h__

class JProcess;
class JProcessCreator {
  public:
	virtual JProcess	*CreateJProcess(dword dwProcID, byte byPriority) = 0;
};

#endif
