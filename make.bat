@ECHO OFF
set OSCAR_HOME="c:\Program Files\oscar64\bin"
cd .\scripts\
python3.exe .\create_translations_yaml_jinja2.py
@REM cd ..\other\
@REM python3 replace02-remove-repeat.py VilliageMapZelda.bin 16
cd ..

@REM 

@REM set FILENAME="VilliageMapHiresMainBig - (8bpc, 64x64) Map.bin"
@REM set GRASS_TILES_COUNT=16

@REM cd .\assets\charGfx\
@REM python3.exe .\replace02-remove-repeat.py %FILENAME% %GRASS_TILES_COUNT%
@REM cd ..

@REM call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG %*
call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG -tf=crt -pp %*
@REM call %OSCAR_HOME%\oscar64 -n -O3 -tf=crt %*
