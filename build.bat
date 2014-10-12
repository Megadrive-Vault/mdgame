util\btpose 4 4 res\gfxlin\player.bin res\gfx\player_sprites.bin
cd util\xm2esf-master\
xm2esf ..\..\res\xm\conv.xif ..\..\res\music\bgm.esf
cd ..\..
%GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen
