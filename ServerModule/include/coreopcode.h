#ifndef __coreopcode_h__
#define __coreopcode_h__

enum coreopcode {
	OP_LoginReq = 1,
	OP_LoginAct,
	OP_LoginNot,
	OP_LoginRej,
	OP_EchoNot,
	OP_PlayerNumberNot,
	OP_UserListReq,
	OP_UserListNot,
	OP_EnterTableAct,
	OP_EnterTableNot,
	OP_EnterTableRej,
	OP_LeaveTableAct,
	OP_LeaveTableNot,
	OP_LeaveTableRej,
	OP_ReadyAct,
	OP_ReadyNot,
	OP_DetailReq,
	OP_DetailNot,
};

#endif
