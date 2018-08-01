::**********************************************************************************
::
:: Copyright (c) 2017
:: Lumi, JSC.
:: All Rights Reserved
::
::
:: Description:
::
:: Author: ThangDH
::
:: Last Changed By:  $Author:  ThangDH $
:: Revision:         $Revision:  $
:: Last Changed:     $Date:  $
::
::**********************************************************************************
:: @ DIM_BUIL: Define list contact device need build (DIMT DIMM)
:: @ FREQ_BUILD: Define list frequency build(EU US IL IN ANZ)
:: @ VER : Version
:: @ LIBRARY : slave_routing  slave_enhanced_232
:: @ SECURITY : NON_SECURE SECURE
:: @ BOOT : BOOTLOADER_ENABLED nonBOOT
::**********************************************************************************
@ECHO OFF
SET VER=V12
SET DEVICE_BUIL=DOOR
SET FREQ_BUILD=EU US IL IN ANZ
SET LIBRARY=slave_routing
SET SECURITY=SECURE
SET BOOT=nonBOOT

ECHO ...................................START...................................
ECHO ................................ PRESS ENTER ..............................
FOR %%c IN (%DEVICE_BUIL%) DO (
    IF EXIST Makefile.StickyTarget DEL Makefile.StickyTarget
    IF EXIST Makefile.variants DEL Makefile.variants
    IF EXIST Makefile.tmp DEL Makefile.tmp
    IF NOT EXIST Makefile (ECHO Makefile not found 
        EXIT)
    RENAME Makefile Makefile.tmp
    (FOR /F "delims=" %%s IN (Makefile.tmp) DO (
        SET line=%%s
        SET /A "FLAGSAVE = 0"
        SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO !line!>tmpfile
        FINDSTR /M "APP_NAME:=DOOR" tmpfile>NUL
        IF ERRORLEVEL 1 (SET /A "FLAGSAVE = 1") ELSE (SET line=APP_NAME:=%%c_%VER%)
        ECHO(!line!
        ENDLOCAL
    ))>Makefile
    DEL tmpfile
    DEL Makefile.tmp

    FOR %%x IN (%FREQ_BUILD%) DO (
        ECHO BUILD %%c VER %VER% FREQUENCY=%%x LIBRARY=%LIBRARY% BOOT=%BOOT% SECURITY=%SECURITY%
        CALL :_DelLog
        START cmd.exe /C mk "FREQUENCY=%%x" "LIBRARY=%LIBRARY%" "BOOT=%BOOT%" "SECURITY=%SECURITY%" >build_log.txt
        TIMEOUT 1 > NUL
        CALL :_WaitBuildFinish
))
:: Remove folder if exits
IF EXIST %cd%\program\  RMDIR /S /Q %cd%\program\
:: Make folder %cd%\program
MKDIR %cd%\program
:: copy all file *.h to folder %cd%\program
FOR /R %%x IN (*.hex) DO (XCOPY %%x %cd%\program\)
:: Remove build folder
::RMDIR /S /Q %cd%\build
DEL build_log.txt
EXIT

:: @func  : _WaitBuildFinish
:: @brief : Wait buil complete, showlog
::
:_WaitBuildFinish
    SET /A "xTime = 0"
    SET /A "MAX_TIME = 600"
    :WHILE
    IF %xTime% LSS %MAX_TIME% (
        SET /A "xTime = xTime + 1"
        TIMEOUT 1 > NUL
        IF NOT EXIST build_log.txt (
            ECHO ERROR
        ) ELSE (
            2>NUL (>>build_log.txt ECHO OFF) && (GOTO :BFINISH) || (ECHO|SET /P=.)
        ) 
        GOTO :WHILE
    )

    :BFINISH
    ECHO .
    ECHO Build completed %xTime%s
EXIT /B 0

:: @func  : _DelLog
:: @brief : Delete log file
::
:_DelLog
    IF EXIST build_log.txt (
        ECHO Delete build_log
        DEL build_log.txt
    ) ELSE (
        ECHO Build start
    )
EXIT /B 0

:: @func  : _ShowLog
:: @brief : show log file
::
:_ShowLog
    FOR /F "tokens=* delims=" %%s IN (build_log.txt) DO (
        ECHO %%s
    )
EXIT /B 0
