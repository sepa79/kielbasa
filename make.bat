@ECHO OFF
set OSCAR_HOME="c:\Program Files (x86)\oscar64\bin"
cd .\translation\
python3.exe .\create_translations.py
cd ..

@REM 

set FILENAME="VilliageMapHiresMainBig - (8bpc, 64x64) Map.bin"
set GRASS_TILES_COUNT=16

@REM cd .\assets\charGfx\
@REM python3.exe .\replace02-remove-repeat.py %FILENAME% %GRASS_TILES_COUNT%
@REM cd ..

@REM call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG %*
call %OSCAR_HOME%\oscar64 -n -dNOFLOAT -dNOLONG -tf=crt %*
@REM call %OSCAR_HOME%\oscar64 -n -O3 -tf=crt %*
