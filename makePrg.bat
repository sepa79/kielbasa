@ECHO OFF
set OSCAR_HOME="c:\Program Files\oscar64\bin"
call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG -pp %*
