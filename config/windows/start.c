/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: start.c,v 1.6 2001/11/12 14:55:23 takayuki Exp $
 */


#include "jsp_kernel.h"
#include "task.h"
#include "vwindows.h"
#include "hal_msg.h"
#include "hw_timer.h"
#include "objbase.h"
#include "shellapi.h"
#include "resource.h"


struct tagDestructionProcedureQueue
{
	struct tagDestructionProcedureQueue * Next;
	void (*DestructionProcedure)(void *);
	void * Parameter;
};

extern void kernel_start();
extern void kernel_exit();

HINSTANCE ProcessInstance;
HANDLE PrimaryThreadHandle;
HWND PrimaryDialogHandle;
HANDLE CurrentRunningThreadHandle;
HANDLE ForceShutdownHandle = INVALID_HANDLE_VALUE;

struct tagDestructionProcedureQueue * DestructionProcedureQueue;

 /*
  * カーネルスタータ
  *   kernel_startは最後にexit_and_dispatchを呼ぶので、コンテキスト破棄に
  *   備えて新しいスレッドを生成する。
  */

DWORD WINAPI
KernelStarter(LPVOID param)
{
	kernel_start();
	return 0;
}

DWORD WINAPI
ForceShutdownHandler(LPVOID param)
{
	Sleep(10000);
	ExitProcess(0);
	return 0;
}

Inline LRESULT CALLBACK
PrimaryDialogCommandHandler(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case IDC_CLOCKSUPPLY:
		{
			int state;

			state = SendDlgItemMessage(PrimaryDialogHandle, IDC_CLOCKSUPPLY,BM_GETCHECK,0,0);
			if(state == BST_CHECKED)
			{
				hw_timer_terminate();
				if(CurrentRunningThreadHandle != INVALID_HANDLE_VALUE)
					SuspendThread(CurrentRunningThreadHandle);
			}else
			{
				if(state == BST_UNCHECKED)
				{
					if(CurrentRunningThreadHandle != INVALID_HANDLE_VALUE)
						ResumeThread(CurrentRunningThreadHandle);
					hw_timer_initialize();
				}
			}

			break;
		}

	default:
		return FALSE;
	}

	return TRUE;
}


Inline LRESULT CALLBACK
HALMessageHandler(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case HALMSG_DESTROY:
		if(lParam != 0)
		{
			TCB * tcb = (TCB *)lParam;
			TerminateThread(tcb->tskctxb.ThreadHandle,0);
			CloseHandle(tcb->tskctxb.ThreadHandle);
			tcb->tskctxb.ThreadHandle = INVALID_HANDLE_VALUE;
		}else
		{
			if(CurrentRunningThreadHandle != INVALID_HANDLE_VALUE)
			{
				TerminateThread(CurrentRunningThreadHandle,0);
				CloseHandle(CurrentRunningThreadHandle);
				if(runtsk != 0)
					runtsk->tskctxb.ThreadHandle = INVALID_HANDLE_VALUE;
				CurrentRunningThreadHandle = INVALID_HANDLE_VALUE;
			}
		}
		runtsk = 0l;

	case HALMSG_DISPATCH:
		if(CurrentRunningThreadHandle != INVALID_HANDLE_VALUE)
			SuspendThread(CurrentRunningThreadHandle);

		if(runtsk != schedtsk)
		{
			if((int)((long)schedtsk-(long)tcb_table)/sizeof(TCB)>10)
				runtsk = 0l;
		}

		if(lParam == 0)
			runtsk = schedtsk;
		else
			runtsk = (TCB *)lParam;

		if(runtsk != 0l)
		{
			CurrentRunningThreadHandle = runtsk->tskctxb.ThreadHandle;
			_kernel_calltex();
			ResumeThread(runtsk->tskctxb.ThreadHandle);
		}else
		{
			CurrentRunningThreadHandle = 0l;
		}

		break;

	case HALMSG_INTERRUPT:
		{
			HANDLE work;
			if(lParam != 0)
				ras_int((unsigned int)lParam);

			if((work = sch_int()) != INVALID_HANDLE_VALUE)
			{
				SuspendThread(CurrentRunningThreadHandle);
				CurrentRunningThreadHandle = work;
				ResumeThread(work);
			}

			break;
		}
	case HALMSG_EXECUTEPROCEDURE:
		{
			void ** work = (void **)lParam;
			((void (*)(void *))(*work))(*(work+1));
			break;
		}
	case HALMSG_ADDDESTRUCTIONPROCEDURE:
		{
			struct tagDestructionProcedureQueue * scope;
			void ** work = (void **)lParam;

			scope = DestructionProcedureQueue;

			if((DestructionProcedureQueue = GlobalAlloc(GMEM_FIXED, sizeof(struct tagDestructionProcedureQueue))) != NULL)
			{
				DestructionProcedureQueue->DestructionProcedure = *(work);
				DestructionProcedureQueue->Parameter = *(work+1);
				DestructionProcedureQueue->Next = scope;
			}
			break;
		}
	case HALMSG_QUITREQUEST:
		{
			struct tagDestructionProcedureQueue * destqueue;
			void * destarea;

			ForceShutdownHandle = CreateThread(NULL, 0, ForceShutdownHandler, 0, NULL, NULL);

			hw_timer_terminate();

			if(CurrentRunningThreadHandle != NULL)
				SuspendThread(CurrentRunningThreadHandle);

			destqueue = DestructionProcedureQueue;
			while(destqueue != NULL)
			{
				(*destqueue->DestructionProcedure)(destqueue->Parameter);
				destarea = destqueue;
				destqueue = destqueue->Next;
				GlobalFree((HGLOBAL)destarea);
			}

			fin_int();
			fin_exc();
			DeleteCriticalSection(&CPULock);

			DestroyWindow(PrimaryDialogHandle);
			break;
		}

	default:
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK PrimaryDialogProc(HWND hDlg,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case HALMSG_MESSAGE+1:
		if(lParam == WM_RBUTTONUP)
			kernel_exit();

		break;

	case HALMSG_MESSAGE:
		return HALMessageHandler(wParam,lParam);

	case WM_COMMAND:
		return PrimaryDialogCommandHandler(wParam,lParam);
	
	case WM_INITDIALOG:
		{
			DWORD ThreadID;
			NOTIFYICONDATA nid;

			PrimaryDialogHandle = hDlg;

			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
			nid.uID = ID_NOTIFYICON;
			lstrcpy(nid.szTip,"TOPPERS/JSP");
			nid.hWnd = hDlg;
			nid.uCallbackMessage = HALMSG_MESSAGE+1;
			nid.hIcon = LoadIcon(ProcessInstance,MAKEINTRESOURCE(IDI_ERTLICON));

			Shell_NotifyIcon(NIM_ADD,&nid);

			PrimaryThreadHandle = (HANDLE)hDlg;
			CurrentRunningThreadHandle = CreateThread(NULL,0,KernelStarter,NULL,0,&ThreadID);
			break;
		}
	case WM_CLOSE:
			kernel_exit();
			break;

	case WM_DESTROY:
		{
			NOTIFYICONDATA nid;

			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = PrimaryThreadHandle;
			nid.uID = ID_NOTIFYICON;
			Shell_NotifyIcon(NIM_DELETE,&nid);

			PrimaryThreadHandle = INVALID_HANDLE_VALUE;
			PostQuitMessage(0);
			break;
		}
	case WM_TIMER:
		if((wParam & 0xffff0000) == 0xffff0000)
		{
			return HALMessageHandler(HALMSG_INTERRUPT,(wParam&0xffff));
		}
	default:
		return FALSE;
	}
	return TRUE;
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShow)
{
	MSG msg;

	if(FAILED(CoInitialize(NULL)))
		return 0;

	ProcessInstance = hInstance;
	DestructionProcedureQueue = NULL;

	OnDebug InitializeDebugServices(hInstance,PrimaryDialogHandle);
	OnDevice InitializeDeviceSupportModule();

	CurrentRunningThreadHandle = INVALID_HANDLE_VALUE;
	CreateDialog(hInstance,"PrimaryDialog",NULL,PrimaryDialogProc);
	ShowWindow(PrimaryDialogHandle,SW_HIDE);

	OnDebug ShowWindow(PrimaryDialogHandle,SW_SHOW);

	while(GetMessage(&msg,NULL,0,0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT)
			msg.message = 0;
	}

	OnDevice FinalizeDeviceSupportModule();
	OnDebug FinalizeDebugServices();

	{
		int i;
			for(i=0;i<_kernel_tmax_tskid;i++)
			{
				if(_kernel_tcb_table[i].tskctxb.ThreadHandle != INVALID_HANDLE_VALUE)
				{
					TerminateThread(_kernel_tcb_table[i].tskctxb.ThreadHandle,0);
					CloseHandle(_kernel_tcb_table[i].tskctxb.ThreadHandle);
					_kernel_tcb_table[i].tskctxb.ThreadHandle = INVALID_HANDLE_VALUE;
				}
			}
	}

	CoUninitialize();

	if(ForceShutdownHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(ForceShutdownHandle,0);
		CloseHandle(ForceShutdownHandle);
	}

	ExitProcess(msg.wParam);
	return msg.wParam;
}