# Microsoft Developer Studio Project File - Name="Toppers" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Toppers - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Toppers.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Toppers.mak" CFG="Toppers - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Toppers - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Toppers - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Toppers - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Ot /Ow /Oi /Ob1 /I "../" /I "../../../kernel" /I "../../../include" /I "../../../windows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Og
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Toppers - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Zi /Od /I "../" /I "../../../kernel" /I "../../../include" /I "../../../windows" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAs /FR /YX /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Toppers - Win32 Release"
# Name "Toppers - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\WINDOWS\kernel_cfg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\WINDOWS\sample1.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\WINDOWS\kernel_id.h
# End Source File
# Begin Source File

SOURCE=..\..\..\windows\offset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\WINDOWS\sample1.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\hal_resource.rc
# End Source File
# Begin Source File

SOURCE=..\icon1.ico
# End Source File
# End Group
# Begin Group "kernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\kernel\banner.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\check.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\cyclic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\cyclic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\dataqueue.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\dataqueue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\eventflag.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\eventflag.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\exception.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\interrupt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\interrupt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\jsp_kernel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\mailbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\mailbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\mempfix.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\mempfix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\queue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\semaphore.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\startup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\sys_manage.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\task.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\task.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\task_except.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\task_manage.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\task_sync.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\time_event.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\time_event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\time_manage.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\wait.c
# End Source File
# Begin Source File

SOURCE=..\..\..\kernel\wait.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\include\itron.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\jsp_services.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\jsp_stddef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\kernel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logtask.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\serial.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\serial_old.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\syslog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\timer.h
# End Source File
# End Group
# Begin Group "systask"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\systask\logtask.c
# End Source File
# Begin Source File

SOURCE=..\..\..\systask\syslog.c
# End Source File
# Begin Source File

SOURCE=..\..\..\systask\timer.c
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cpu_config.c
# End Source File
# Begin Source File

SOURCE=..\cpu_config.h
# End Source File
# Begin Source File

SOURCE=..\cpu_context.h
# End Source File
# Begin Source File

SOURCE=..\cpu_defs.c
# End Source File
# Begin Source File

SOURCE=..\cpu_defs.h
# End Source File
# Begin Source File

SOURCE=..\debugout.c
# End Source File
# Begin Source File

SOURCE=..\debugout.h
# End Source File
# Begin Source File

SOURCE=..\hal_msg.h
# End Source File
# Begin Source File

SOURCE=..\hw_timer.h
# End Source File
# Begin Source File

SOURCE=..\serial_console.c
# End Source File
# Begin Source File

SOURCE=..\start.c
# End Source File
# Begin Source File

SOURCE=..\sys_config.c
# End Source File
# Begin Source File

SOURCE=..\sys_config.h
# End Source File
# Begin Source File

SOURCE=..\sys_defs.h
# End Source File
# Begin Source File

SOURCE=..\vwindows.h
# End Source File
# End Group
# Begin Group "configuration"

# PROP Default_Filter "cfg"
# Begin Source File

SOURCE=..\..\..\WINDOWS\sample1.cfg

!IF  "$(CFG)" == "Toppers - Win32 Release"

!ELSEIF  "$(CFG)" == "Toppers - Win32 Debug"

# Begin Custom Build
InputDir=\Sources\Program sources\CVS home\JSP\WINDOWS
InputPath=..\..\..\WINDOWS\sample1.cfg
InputName=sample1

BuildCmds= \
	cd $(InputDir) \
	cl /E /EP /I"../config/windows" $(InputName).cfg | cfg \
	

"kernel_id.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"kernel_cfg.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "libjsp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\libjsp\jsp_perror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\libjsp\strerror.c
# End Source File
# End Group
# End Target
# End Project
