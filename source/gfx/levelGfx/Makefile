include ../../makerules

#export PATH := /c/perl/bin

ZONES = $(patsubst %.pre,%, $(wildcard *.pre))
ZONEH = cave.h dungeon_d3.h dungeonGreen.h overworld2.h overworld.h overworldGraveyard.h overworldRocky.h zone1.h
ALLPNG = $(wildcard png/*.png)
ALLPNGC = $(patsubst %.png,%.c, $(wildcard png/*.png))

all: $(ZONEH) 


%.h : 
	$(PERL) buildLevelGfx.pl lvl=$(patsubst %.h,%,$@) mode=wingit

clean:
	rm *.h
	rm png/*.c
	rm png/*.c.7c1f
