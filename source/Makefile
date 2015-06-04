ifeq ($(strip $(DEVKITARM)),)
DEVKITPRO = /home/tolbert/tools/devkitPro
DEVKITARM = $(DEVKITPRO)/devkitARM
$(warning "DEVKITARM not set in environment, setting to $(DEVKITARM))
endif

include $(DEVKITARM)/ds_rules

OUTPUT=anguna

$(OUTPUT).ds.gba	: 	$(OUTPUT).nds
$(OUTPUT).nds	: 	$(OUTPUT).arm9 audio/arm7/sound.arm7 gfx/angunaIcon.bmp
		ndstool -c $(OUTPUT).nds -7 audio/arm7/sound.arm7 -9 $(OUTPUT).arm9 -b gfx/angunaIcon.bmp "Anguna"
$(OUTPUT).arm9	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)


include makerules 

audio/arm7/sound.arm7: audio/arm7/source/main7.c
	cd audio/arm7 && make

PLATFORM = platform.o dsMisc.o input.o memcpy.o time.o
SYSTEM =  sound.o
GUI = text.o  splash.o  textData.o subscreen.o hud.o shop.o healer.o
CHARACTERS =  spriteMgr.o characters.o mainChar.o bullet.o enemies.o enemyDatabase.o
MISCCODE =  main.o misc.o  level.o bg.o  minimap.o   gameState.o  items.o  darkness.o overworld.o
ASSEMBLY = ndscpy.o ndsset.o
CORE = $(SYSTEM) $(GUI) $(CHARACTERS) $(MISCCODE) $(PLATFORM) $(ASSEMBLY) 

CUTSCENES = gfx/cutscene_gameStart.o gfx/cutscene_dungeonWindow.o
HUDGFX =  gfx/alphabet.o gfx/hud.o
SPLASH = gfx/splashMenu.o gfx/chiliBite.o gfx/spriteAttack.o gfx/bg_gameover.o gfx/menuBottomDay.o gfx/menuTopNoTitle.o gfx/dayMenuNewGame.o gfx/mainMenuTitleTop.o
MISCGFX = gfx/backdrop.o gfx/mainCharGfx.o gfx/spritePalette.o gfx/angel.o gfx/overworld_minimap.o
GRAPHICS =  $(CUTSCENES) $(HUDGFX) $(SPLASH) $(MISCGFX)


LEVELS = $(patsubst %.fmp,%.o, $(wildcard levels/*.fmp))
BACKDROPS = $(patsubst %.c,%.o, $(wildcard gfx/levelGfx/backdrops/*.c))
ZONES = $(patsubst %.c,%.o, $(wildcard levels/zones/*.c))
LEVELGFXH = $(patsubst %.pre,%.h, $(wildcard gfx/levelGfx/*.pre))
LEVELGFX = $(patsubst %.png,%.o, $(wildcard gfx/levelGfx/png/*.png))
ENEMIES = $(patsubst %.c,%.o, $(wildcard enemies/*.c)) 
PORTRAITS = $(patsubst %.png,%.o, $(wildcard enemies/portraits/*.png))
PORTRAITSH = $(patsubst %.h,%.o, $(wildcard enemies/portraits/*.png))
MODS =  $(patsubst %.bin,%.o, $(wildcard audio/mods/*.bin))
MODS_SRC = $(wildcard audio/mods/*.bin)
WAVS =  $(patsubst %.wav,%.o, $(wildcard audio/wavs/*.wav))
WAVS_SRC = $(wildcard audio/wavs/*.wav)
#SAMPLE_DEFS = audio/sampleStarts.o
#SAMPLES = audio/samples.o 

#AUDIO_OBJS = $(MODS) $(SAMPLES) $(SAMPLE_DEFS)
AUDIO_OBJS = $(MODS) $(WAVS)
SAMPLES = 

AUDIO = $(AUDIO_OBJS) audio/audio.o

CUSTOMLDFLAGS = -Laudio/libxm/lib
CUSTOMLIBS =  -lxm79
CUSTOMINCLUDE = -Iaudio/mikmod/include



OBJECTS = $(CORE) $(GRAPHICS) $(LEVELS) $(BACKDROPS) $(LEVELGFX) $(ENEMIES) $(PORTRAITS) $(ZONES) $(AUDIO)
PROJECT = anguna.elf

all : $(PROJECT) depend 

anguna.elf : Makefile $(OBJECTS)  config.h
	@$(CC) $(LDFLAGS) $(CUSTOMLDFLAGS) $(OBJECTS) $(CUSTOMLIBS) $(LIBENTRY) -o $@ 


gfx/levelGfx/%.h gfx/levelGfx/%.c: gfx/levelGfx/%.pre gfx/levelGfx/makefile gfx/levelGfx/buildLevelGfx.pl
	cd gfx/levelGfx && make


gfx/levelGfx/png/%.o : gfx/levelGfx/png/%.c 
	$(CC) $(CFLAGS) $< -o $@

gfx/levelGfx/png/%.c : gfx/levelGfx/png/%.png
	cd gfx/levelGfx && make

audio/mods/%.o : audio/mods/%.bin
	@$(bin2o)

$(WAVS) : $(patsubst %.o,%.wav,$@)
	$(PERL) audio/convertWav.pl $(patsubst %.o,%.wav,$@) $(WINE)
	
sound.o : $(WAVS)

$(ASSEMBLY) : $(patsubst %.o,%.s,$@)
	$(AS) $(patsubst %.o,%.s,$@) -o $@

$(LEVELGFXH) : $(patsubst %.h,%pre,$@)
	cd gfx/levelGfx && make


%.c : %.fmp %.pre %.dor $(patsubst %.pre,%.h, $(wildcard gfx/levelGfx/*.pre)) levels/buildRooms.pl levels/overworldEnemies.txt levels/standardHeader.h $(LEVELGFXH)
	levels/fmp2gba.exe $< $@.tmp $(patsubst levels/%.c,level_%, $@) NOGFX
	$(PERL) levels/buildRooms.pl lvl=$(patsubst levels/%.c,%, $@) mode=16px

$(ENEMIES) : $(PORTRAITS) $(PORTRAITSH) $(SAMPLES)

enemies/portraits/%.o : enemies/portraits/%.png
	$(WINE) tools/map/wingitCLI.exe -img $< -output $(patsubst %.o,%_portrait,$@) -symbol $(patsubst enemies/portraits/%.png,%_portrait_,$<) -imgBpp 4  -metaWidth 1 -metaHeight 1 -palStart 0 -palEnd 63
	$(CC) $(CFLAGS) $(patsubst %.o,%_portrait.c,$@) -o $@


gfx/overworld_minimap.o : gfx/png/maps/angunaWorldMapSmall.png
	$(WINE) tools/map/wingitCLI.exe -img $< -output gfx/overworld_minimap -symbol overworld_minimap -imgBpp 4 -metaWidth 1 -metaHeight 1 -palStart 0 -palEnd 15 
	$(CC) $(CFLAGS) gfx/overworld_minimap.c -o gfx/overworld_minimap.o

main.o : main.c config.h

mainChar.o : mainChar.c mainChar.h config.h bullet.h

misc.o : misc.c config.h

text.o : text.h config.h

shop.o : $(PORTRAITS)

minimap.o : gfx/overworld_minimap.o


%.o : %.c 
	$(CC) $(CFLAGS) $(CUSTOMINCLUDE) $< -o $@

%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

config.h :
	echo " " > config.h

.PHONY : clean depend all release

all:
	$(OUTPUT).nds
	
	
depend :
	$(DEVKITARM)/bin/$(CC) $(CFLAGS) -MM *.c > .depend

clean :
	-$(RM) *.o *.elf *.bin *.tmp.c
	cd levels && $(RM) *.o *.c.tmp
	cd levels/zones && $(RM) *.o
	cd enemies/portraits && $(RM) *.o
	cd enemies/portraits && $(RM) *.c
	cd enemies/portraits && $(RM) *.h
	cd enemies && $(RM) *.o
	cd gfx && $(RM) *.o
	cd gfx/levelGfx && $(RM) *.h
	cd gfx/levelGfx/png && $(RM) *.o *.c.7c1f
	cd gfx/levelGfx/png && $(RM) *.o *.c.7c1f *.c
	cd audio/arm7 && make clean
	cd audio/wavs && $(RM) *.o *.s *.h
	cd audio/mods && $(RM) *.o *.s *.h

release :
	$(ZIP) angunaDS.zip anguna.nds readme.txt MikMod.txt

	


-include .depend
