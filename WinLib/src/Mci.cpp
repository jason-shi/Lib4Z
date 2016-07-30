#include "stdafx.h"
#include "Mci.h"
#include "Window.h"
#include "Misc.h"
#include "mmsystem.h"

bool CMci::Open(CWindow *window)
{
	Window = window;

	return true;
}

bool CMci::Close()
{
	return true;
}

bool CMci::Play(int no)
{
	return false;
}

bool CMci::Play(LPCTSTR name)
{
	return false;
}

bool CMci::Replay()
{
	return false;
}

bool CMci::Stop()
{
	return false;
}

bool CMci::MciOpen(LPCTSTR command)
{
	DWORD err;

	TCHAR result[128];
	if ((err = mciSendString(command, result, sizeof(result), 0)) != 0) {
		return false;
	}

	LPTSTR p;
	Id = _tcstol(result, &p, 0);

	return true;
}

bool CMci::MciOpen()
{
	TCHAR command[128];
	_stprintf(command, _T("open %s wait"), device);
	return MciOpen(command);
}

bool CMci::MciOpen(LPCTSTR dev, LPCTSTR element)
{
	TCHAR command[_MAX_PATH + 128];
	_stprintf(command, _T("open \"%s!%s\" alias %s wait"), dev, element, device);
	return MciOpen(command);
}

bool CMci::MciClose()
{
	Id = 0;

	TCHAR command[128];
	_stprintf(command, _T("close %s wait"), device);
	if (mciSendString(command, NULL, 0, 0) != 0)
		return false;
	return true;
}

bool CMci::MciPlay(LPCTSTR request)
{
	TCHAR command[256];
	_stprintf(command, _T("play %s %s notify"), device, request);

	DWORD err;
	if ((err = mciSendString(command, NULL, 0, *Window)) != 0)
		return false;
	return true;
}

bool CMci::MciStop()
{
	TCHAR command[128];
	_stprintf(command, _T("stop %s wait"), device);
	if (mciSendString(command, NULL, 0, 0) != 0)
		return false;
	return true;
}

bool CMci::MciSet(LPCTSTR request)
{
	TCHAR command[128];
	_stprintf(command, _T("set %s %s wait"), device, request);

	DWORD err;
	if ((err = mciSendString(command, NULL, 0, 0)) != 0) {
		return false;
	}
	return true;
}

bool CMci::MciStatus(LPCTSTR request, LPTSTR result, int resultlen)
{
	TCHAR command[128];
	_stprintf(command, _T("status %s %s wait"), device, request);

	DWORD err;
	if ((err = mciSendString(command, result, resultlen, 0)) != 0) {
		return false;
	}
	return true;
}

int CDAudio::MediaChk()
{
	TCHAR	result[128];
	if (!MciStatus(_T("media present"), result, sizeof(result)))
		return -1;
	if (_tcsicmp(result, _T("true")) == 0)
		return 0;
	return 1;
}

bool CDAudio::Open(CWindow *win)
{
	CMci::Open(win);

	if (!MciOpen())
		return false;

	if (!MciSet(_T("time format tmsf"))) {
		MciClose();
		return false;
	}

	bool chkOk = false;

	do {
		switch (MediaChk()) {
		  case -1:
			MciClose();
			return false;

		  case 0:
			chkOk = true;
			break;

		  default:
#ifdef	notdef		//若光盘机中没有CD而出现错误时 #ifdef -> #ifndef
			if (Window->MessageBox(ApplicationTitle "光盘机中没有CD",
				ApplicationTitle, MB_RETRYCANCEL) == IDCANCEL) {
				MciClose();
				return false;
			}
			break;
#else
			MciClose();
			return false;
#endif
		}
	} while (!chkOk) ;

	IsOpen = true;

	return true;
}

bool CDAudio::Close()
{
	if (IsOpen)
	{
		Stop();
		MciClose();
	}
	IsOpen = false;
	return true;
}

bool CDAudio::Play(int no)
{
	if (!IsOpen)
		return false;

	Stop();
	PlayTrack = no;

	no++;		// 这里是以MixedCD为基准，所以播放1首乐曲

	// 检查乐曲长度
	// 设定只播放此范围

	TCHAR	request[64];
	_stprintf(request, _T("length track %d"), no);

	TCHAR	result[128];
	if (!MciStatus(request, result, sizeof(result)))
		return false;

	_stprintf(request, _T("from %d:00:00:00 to %d:%s"), no, no, result);
	if (!MciPlay(request))
		return false;
	return true;
}

bool CDAudio::Play(LPCTSTR)
{
	return false;
}

bool CDAudio::Replay()
{
	return Play(PlayTrack);
}

bool CDAudio::Stop()
{
	if (!IsOpen)
		return false;

	if (PlayTrack > 0)
	{
		MciStop();
		PlayTrack = 0;
	}
	return true;
}

bool CWaveOut::Play(LPCTSTR name)
{
	Stop();

	if (!MciOpen(_T("waveaudio"), name))
		return false;

	if (!MciPlay(_T("")))
		return false;

	playing = true;

	return true;
}

bool CWaveOut::Play(int)
{
	return false;
}

bool CWaveOut::Stop()
{
	if (playing)
	{
		playing = false;
		MciStop();
		MciClose();
	}
	return true;
}
