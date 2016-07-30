//
//	MCI/CD-DA/Wave类别
//
//		Copyright (c) 2000-2001 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "TCHAR.h"
class CWindow;

//
//	MCI类别
//
class CMci	{
  public:
	CMci(LPCTSTR _device, LPCTSTR _errTitle);
	~CMci();

	virtual bool Open(CWindow *win);
	virtual bool Close();

	virtual bool Play(int no);
	virtual bool Play(LPCTSTR name);
	virtual bool Replay();
	virtual bool Stop();

	DWORD GetId() const { return Id; }

  protected:
	bool MciOpen(LPCTSTR command);
	bool MciOpen();
	bool MciOpen(LPCTSTR dev, LPCTSTR element);
	bool MciClose();
	bool MciPlay(LPCTSTR request);
	bool MciStop();
	bool MciSet(LPCTSTR request);
	bool MciStatus(LPCTSTR request, LPTSTR result, int resultlen);

  protected:
	CWindow *Window;
	LPCTSTR device;
	LPCTSTR errTitle;
	DWORD	Id;
} ;

// inline 成员函数

inline CMci::CMci(LPCTSTR _device, LPCTSTR _errTitle)
	: Id(0), device(_device), errTitle(_errTitle)
{
}

inline CMci::~CMci()
{
	Close();
}

//
//	CD音讯类别
//
class CDAudio: public CMci {
  public:
	CDAudio(): CMci(_T("cdaudio"), _T("CD音讯类别")), IsOpen(false), PlayTrack(0) {}

	virtual bool Open(CWindow *win);
	virtual bool Close();

	virtual bool Play(LPCTSTR name);
	virtual bool Play(int no);
	virtual bool Replay();
	virtual bool Stop();

  protected:
	int MediaChk();

  protected:
	int		PlayTrack;
	bool	IsOpen;
} ;

//
//	Wave类别
//
class CWaveOut: public CMci {
  public:
	CWaveOut(): CMci(_T("sound"), _T("Wave音讯error")), playing(false) {}

	virtual bool Play(LPCTSTR name);
	virtual bool Play(int no);
	virtual bool Stop();

  protected:
	bool	playing;
} ;

#endif
