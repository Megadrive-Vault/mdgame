del out\rom.bin
%GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen
cp out\rom.bin regen\rom.bin
regen\Regen.exe rom.bin
del regen\rom.bin