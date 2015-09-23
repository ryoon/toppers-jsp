/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: start.c,v 1.1 2000/11/14 16:31:38 takayuki Exp $
 */
#include "jsp_kernel.h"
#include "task.h"
#include "vwindows.h"
#include "hal_msg.h"
#include "hw_timer.h"
#include "objbase.h"
#include "shellapi.h"
#include "resource.h"

extern void kernel_start();
extern void kernel_exit();

HANDLE ProcessInstance;
HANDLE PrimaryDialogHandle;
HANDLE PrimaryThreadHandle;
HANDLE CurrentRunningThreadHandle;

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
		}else
		{
			if(CurrentRunningThreadHandle != INVALID_HANDLE_VALUE)
			{
				TerminateThread(CurrentRunningThreadHandle,0);
				CloseHandle(CurrentRunningThreadHandle);
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
			DestroyWindow(hDlg);
			break;

	case WM_DESTROY:
		{
			NOTIFYICONDATA nid;
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.uID = ID_NOTIFYICON;
			nid.hWnd = hDlg;
			Shell_NotifyIcon(NIM_DELETE,&nid);
			InvalidateRect(GetDesktopWindow(),NULL,FALSE);
			UpdateWindow(GetDesktopWindow());
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

	OnDebug InitializeDebugServices(hInstance,PrimaryDialogHandle);
	
	CurrentRunningThreadHandle = INVALID_HANDLE_VALUE;
	PrimaryDialogHandle = CreateDialog(hInstance,"PrimaryDialog",NULL,PrimaryDialogProc);
	ShowWindow(PrimaryDialogHandle,SW_HIDE);

	OnDebug ShowWindow(PrimaryDialogHandle,SW_SHOW);

	while(GetMessage(&msg,NULL,0,0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	OnDebug FinalizeDebugServices();

	CoUninitialize();
	ExitProcess(msg.wParam);
	return msg.wParam;
}