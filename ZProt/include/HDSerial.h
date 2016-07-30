#ifndef __HDSerial_h__
#define __HDSerial_h__

#include <windows.h>
#include <stdio.h>

#define SENDIDLENGTH			sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE
#define IDENTIFY_BUFFER_SIZE	512
#define FILE_DEVICE_SCSI		0x0000001b
#define IOCTL_SCSI_MINIPORT_IDENTIFY	((FILE_DEVICE_SCSI << 16) + 0x0501)
#define IOCTL_SCSI_MINIPORT		0x0004D008 //see NTDDSCSI.H for definition
#define IDE_ATAPI_IDENTIFY		0xA1 //Returns ID sector for ATAPI.
#define IDE_ATA_IDENTIFY		0xEC //Returns ID sector for ATA.
#define IOCTL_GET_DRIVE_INFO	0x0007c088
#define IOCTL_GET_VERSION		0x00074080

typedef struct _IDSECTOR
{
	USHORT			 wGenConfig;
	USHORT			 wNumCyls;
	USHORT			 wReserved;
	USHORT			 wNumHeads;
	USHORT			 wBytesPerTrack;
	USHORT			 wBytesPerSector;
	USHORT			 wSectorsPerTrack;
	USHORT			 wVendorUnique[3];
	CHAR			 sSerialNumber[20];
	USHORT			 wBufferType;
	USHORT			 wBufferSize;
	USHORT			 wECCSize;
	CHAR			 sFirmwareRev[8];
	CHAR			 sModelNumber[40];
	USHORT			 wMoreVendorUnique;
	USHORT			 wDoubleWordIO;
	USHORT			 wCapabilities;
	USHORT			 wReserved1;
	USHORT			 wPIOTiming;
	USHORT			 wDMATiming;
	USHORT			 wBS;
	USHORT			 wNumCurrentCyls;
	USHORT			 wNumCurrentHeads;
	USHORT			 wNumCurrentSectorsPerTrack;
	ULONG			 ulCurrentSectorCapacity;
	USHORT			 wMultSectorStuff;
	ULONG			 ulTotalAddressableSectors;
	USHORT			 wSingleWordDMA;
	USHORT			 wMultiWordDMA;
	BYTE			 bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL
{
	ULONG			 HeaderLength;
	UCHAR			 Signature[8];
	ULONG			 Timeout;
	ULONG			 ControlCode;
	ULONG			 ReturnCode;
	ULONG			 Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE			 bVersion; // Binary driver version.
	BYTE			 bRevision; // Binary driver revision.
	BYTE			 bReserved; // Not used.
	BYTE			 bIDEDeviceMap; // Bit map of IDE devices.
	DWORD			 fCapabilities; // Bit mask of driver capabilities.
	DWORD			 dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//获取硬盘序列号的类
class CHDSerial {
  public:
	CHDSerial();
	virtual ~CHDSerial();

  public:
	char			*GetHDSerial();

  protected:
	void _stdcall	 Win9xReadHDSerial(WORD *buffer);
	char			*WORDToChar(WORD diskdata [256], int firstIndex, int lastIndex);
	char			*DWORDToChar(DWORD diskdata [256], int firstIndex, int lastIndex);
	BOOL			 WinNTReadSCSIHDSerial(DWORD *buffer);
	BOOL			 WinNTReadIDEHDSerial(DWORD *buffer);
	BOOL			 WinNTGetIDEHDInfo(HANDLE hPhysicalDriveIOCTL,
									   PSENDCMDINPARAMS pSCIP,
									   PSENDCMDOUTPARAMS pSCOP,
									   BYTE bIDCmd,
									   BYTE bDriveNum,
									   PDWORD lpcbBytesReturned);
};

#endif
