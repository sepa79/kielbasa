@ECHO OFF
set OSCAR_HOME="c:\Program Files (x86)\oscar64\bin"
cd .\translation\
python3.exe .\create_translations.py
cd ..
@REM call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG %*
call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG -tf=crt %*
@REM call %OSCAR_HOME%\oscar64 -n -O3 -tf=crt %*