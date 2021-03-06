# Microsoft Developer Studio Project File - Name="crusher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=crusher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "crusher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "crusher.mak" CFG="crusher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "crusher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "crusher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "crusher - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./src/Release"
# PROP Intermediate_Dir "./src/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "crusher - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./src/Debug"
# PROP Intermediate_Dir "./src/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "crusher - Win32 Release"
# Name "crusher - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\3dmath.cpp
# End Source File
# Begin Source File

SOURCE=.\src\crusher.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\errors.cpp
# End Source File
# Begin Source File

SOURCE=.\src\md2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\package.cpp
# End Source File
# Begin Source File

SOURCE=.\src\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pcx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\registry.cpp
# End Source File
# Begin Source File

SOURCE=.\src\render.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tri.cpp
# End Source File
# Begin Source File

SOURCE=.\src\unrealmesh.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\3dmath.h
# End Source File
# Begin Source File

SOURCE=.\src\crusher.h
# End Source File
# Begin Source File

SOURCE=.\src\defs.h
# End Source File
# Begin Source File

SOURCE=.\src\engine.h
# End Source File
# Begin Source File

SOURCE=.\src\errors.h
# End Source File
# Begin Source File

SOURCE=.\src\fixed.h
# End Source File
# Begin Source File

SOURCE=.\src\md2.h
# End Source File
# Begin Source File

SOURCE=.\src\package.h
# End Source File
# Begin Source File

SOURCE=.\src\palette.h
# End Source File
# Begin Source File

SOURCE=.\src\pcx.h
# End Source File
# Begin Source File

SOURCE=.\src\registry.h
# End Source File
# Begin Source File

SOURCE=.\src\render.h
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\src\tri.h
# End Source File
# Begin Source File

SOURCE=.\src\unrealmesh.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resources\bwman.ico
# End Source File
# Begin Source File

SOURCE=.\resources\Crusher.rc
# End Source File
# Begin Source File

SOURCE=.\resources\gazpacho.bmp
# End Source File
# Begin Source File

SOURCE=.\resources\gazpacho.ico
# End Source File
# Begin Source File

SOURCE=.\resources\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\resources\resource.h
# End Source File
# End Group
# Begin Group "Text Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\doc\readme.txt
# End Source File
# End Group
# End Target
# End Project
