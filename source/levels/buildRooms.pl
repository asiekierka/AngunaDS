#!usr/bin/perl
#processes rooms.pre and associate FMP files to create source files for 
#all levels
use Switch;
use CGI qw(param);

$lvlId = param("lvl");
$mode = param("mode");

($zoneData,$junk) = split(/_/,$lvlId,2);
$zoneData = "zone_".$zoneData;

chdir ("source/levels");
mkdir ("tmp");
my $minimap = "0,0";

#if no gfx specified, use overworld...
$gfx = "overworld";
$backdrop = "defaultBd";
$isOverworld = 0;
#if overworld, do defaults....
if (substr($lvlId,0,4) eq "ovr_")
{
	($over,$x,$y) = split(/_/, $lvlId);
	$isOverworld = 1;
	$gfx = "overworld";
	$backdrop = "defaultBd";
	$minimap = "$x,$y";
	$door = "";

	$door .= addDoor($x-1,$y,"LEFTEDGE");
	$door .= addDoor($x+1,$y,"RIGHTEDGE");
	$door .= addDoor($x,$y-1,"TOPEDGE");
	$door .= addDoor($x,$y+1,"BOTTOMEDGE");

	$doorData = $door;		

}
else
{
	($d,$x,$y) = split(/_/, $lvlId);
	#print "d: $d, x: $x, y: $y\n";

	($y,$junk) = split(/\./, $y);
	#print "y: $y\n";
	if (!$y){
		$y = 0;
	}
	$minimap = "$x,$y";
}

sub addDoor
{
	($lx,$ly,$edge) = @_;

	$loc = "ovr_${lx}_${ly}";
	if (-f ($loc.".pre"))
	{
		return "$loc, $edge;";
	}


	return "";
}

`dos2unix $lvlId.dor`;
`dos2unix $lvlId.pre`;

open(DORIN, "<$lvlId.dor") or print "could not open $lvlId.dor (not necessarily a problem)\n";
open(IN, "<$lvlId.pre") or die "could not open $lvlId.pre";
$id = $lvlId;
#clearRoom();

$darkEnemy = "0,0,lanternMask,0,0;";

$respawn = 0;
$dark = 0;
while(<DORIN>)
{
	chomp;
	@parts = split(/: /);
	$kw = $parts[0];
	$body = $parts[1];
	switch ($kw)
	{
		case "gfx"
		{
			$gfx = $body;
		}
		case "backdrop"
		{
			$backdrop = $body;
		}
		case "minimap"
		{
			if (!($body =~ m/^0,0/))
			{
				print "updating minimap: $body\n";
				$minimap = $body;
			}
		}
		case "door"
		{
			$doorData .= $body;
		}
		case "enemy"
		{
			$enemyData .= $body;
		}
		case "respawn"
		{
			$respawn = $body;
		}
		case "dark"
		{
			$dark = $body;
			$enemyData .= $darkEnemy;
		}
		case "stairs"
		{
			$_ = $body;
			($body,$junk) = split(/;/);
			@stairParts = split(/,/, $body);
			$enemyData .= "TT(".$stairParts[0].",".$stairParts[1]."),map_object,0,OBJ_STAIRS_".$stairParts[5].";";
			$doorData .= $stairParts[2].", STAIRS_TO(".$stairParts[0].",".$stairParts[1].",".$stairParts[3].",".$stairParts[4].");";
		}
	}
}
$noEnemies = 0;
while(<IN>)
{
	chomp;
	@parts = split(/: /);
	$kw = trim($parts[0]);
	$body = trim($parts[1]);
	if ($lvlId eq "ovr_0_0")
	{
		print "keyword: $kw\n";
	}
	switch ($kw)
	{
		case "gfx"
		{
			$gfx = $body;
		}
		case "noEnemies"
		{
			$noEnemies = 1;
		}
		case "noEnemies;"
		{
			$noEnemies = 1;
		}
		case "backdrop"
		{
			$backdrop = $body;
		}
		case "minimap"
		{
			if (!($body =~ m/^0,0/))
			{
				print "updating minimap: $body\n";
				$minimap = $body;
			}
		}
		case "door"
		{
			$doorData .= $body;
		}
		case "enemy"
		{
			$enemyData .= $body;
		}
		case "respawn"
		{
			$respawn = $body;
		}
		case "dark"
		{
			$dark = $body;
			$enemyData .= $darkEnemy;
		}
		case "stairs"
		{
			$_ = $body;
			($body,$junk) = split(/;/);
			@stairParts = split(/,/, $body);
			$enemyData .= "TT(".$stairParts[0].",".$stairParts[1]."),map_object,0,OBJ_STAIRS_".$stairParts[5].";";
			$doorData .= $stairParts[2].", STAIRS_TO(".$stairParts[0].",".$stairParts[1].",".$stairParts[3].",".$stairParts[4].");";
		}

	}
}
if ($lvlId eq "ovr_0_0")
{
	print "ovr: $isOverworld, enemies: $noEnemies\n";
}

if (($isOverworld == 1) && ($noEnemies == 0))
{
	addEnemies($x, $y);
}


open(CFILE, ">$id".".c") or die "could not open $id.c";
open(STDH, "<standardHeader.h") or die "could not open standard header file";
while (<STDH>)
{
	print CFILE $_;
}

print CFILE "\n\n\n";
close STDH;
`dos2unix $id.c.tmp`;
open (MAP, "<$id.c.tmp") or die "could not open temporary version of $id.c ($id.c.tmp)";
$hasFG = 0;
while (<MAP>)
{
	$line = $_;
	$oline = $_;
	if (m/{/)
	{
		@chips = ();;
	}
	if ((m/	([ 0-9],)*/) && ($mode eq "16px"))
	{
		$tmpLine = "	";
		$nextLine = "	";
		@lineParts = split(",", $line, 16);
		foreach (@lineParts)
		{
			push(@chips, $_);
		}
		$line = "";
		if ($oline =~ m/}/)
		{
			#$line .= "};\n\n";
			$xCtr = 0;
			$firstLine = "	";
			$secondLine = "	";
			foreach (@chips)
			{
				$start = $_;
				$start = $start * 1;
				$firstLine .= $start++.",";
				#$firstLine .= $start++.",  ";
				#$secondLine .= $start++.",";
				#$secondLine .= $start++.",  ";
	
				if(++$xCtr == $width)
				{
					$line = $firstLine."\n".$secondLine."\n\n"; 
					$xCtr = 0;
					$firstLine = "	";
					$secondLine = "	";
					print CFILE $line;
				}
			}
			$line = "};\n";
		}
	
	}
	if ((m/const int level_.*_(.*) = ([0-9]*);/) && ($mode eq "16px"))
	{
		if ($1 eq "width")
		{
			$width = $2;
		}
		
		$line =~ s/;/*2;/g;
	}
	if ((m/.define LEVEL_level_.*_.* [0-9]*\s*$/) && ($mode eq "16px"))
	{
		chomp $line;
		$line .= "*2\n";
	}
	$line =~ s/const unsigned short (level_.*_map)/const unsigned char \1/g;
	print CFILE $line;
}
close MAP;
@doors = split(/;/, $doorData);
my $doorctr = 0;
$doorStruct = "";
foreach(@doors)
{
	$door = $_;
	($toLvl, $junk) = split(/,/, $door);
	print CFILE "extern const LevelData level_$toLvl;\n";
	if (!($door =~ m/STAIRS/)){
		$door .= ",false";
	}

	$doorStruct .= "	{(void *)&level_$door },\n";
	$doorctr++;
	
}
print CFILE "const DoorData ${id}_door_0[] = \n";
print CFILE "{\n$doorStruct\n};\n";

@enemies = split(/;/, $enemyData);
my $enemyCtr = 0;
$enemyStruct = "";
foreach (@enemies)
{
	$enemy = $_;
	($x, $y, $enemyId, $itemDrop, $special) = split(/,/, $enemy);
	chomp($enemyId);
	if (length($enemyId) > 0){
		print CFILE "extern const EnemyType enemy_${enemyId}_def;\n";
		$enemyStruct .= "	{$x, $y, (EnemyType*)&(enemy_${enemyId}_def), $itemDrop, $special},\n";
		$enemyCtr++;
	}
}

print CFILE "const EnemyStartData ${id}_EnemyStarts[] = \n";
print CFILE "{\n$enemyStruct\n};\n";

$fgData = "(u16*)level_${id}_fg1_map";

$gfx = trim($gfx);

print CFILE <<EOF;

#include "../gfx/levelGfx/$gfx.h"

//extern const unsigned short ${gfx}_Pal[];
//extern const unsigned char ${gfx}_Tiles[];
//extern const BackDropData $backdrop;
extern const ZoneData $zoneData;

const LevelData level_$id = {
	LEVEL_level_${id}_WIDTH,LEVEL_level_${id}_HEIGHT,	//map x,y size
	(u16*)level_${id}_map,  			//pointer to map definition array
	$fgData, 		 			//pointer to map definition array
	PUN(u16 *)${gfx}_Pal, 	//pointer to palette
	GFX_${gfx}_NUMTILES,		//number of graphics tiles to load
	(u8 *)${gfx}_Tiles,		//pointer to grapic defn's
	ZONE_${gfx}_COLLIDE,			//minimum collision tile #
	ZONE_${gfx}_WATER_COLLIDE,			//minimum collision tile #
	null, //backdrop
	$enemyCtr,						//num enemies
	(EnemyStartData*)&${id}_EnemyStarts,	//enemy pointer
	$minimap,					//minimap location
	$doorctr,						//number of doors
	(DoorData*)&${id}_door_0,	//door pointer
	$respawn,
	$dark,
	(ZoneData *)&$zoneData


};	

EOF



################RANDOM ENEMY LOADING JUNK#########################

sub addEnemies
{
	(my $x, my $y) = @_;

	open OVR_EN, "<overworldEnemies.txt";

	while (<OVR_EN>)
	{
		$line = $_;
		if (matchesRoom($line,$x,$y))
		{
			$xof = <OVR_EN>;
			$xof =~ s/^\s+//;
			$xof =~ s/\s+$//;

			my @choices = ();
			
			#read all the possibilities
			$line = <OVR_EN>;
			$break = 0;
			while ($break == 0)
			{
				$line =~ s/^\s+//;
				$line =~ s/\s+$//;
				push(@choices, $line);

				$line = <OVR_EN>;
				if ($line =~ m/xroom/)
				{
					$break = 1;
				}
			}

			#randomly pick $xof number of enemy types
			my @toUse = ();
			for (my $i = 0; $i < $xof; $i++) 
			{
				$count = @choices;
				my $random = int(rand($count));
				push(@toUse, $choices[$random]);
				print $choices[$random]."\n";
				splice(@choices, $random, 1);
			}

			#randomly pick number of enemies for each enemy type
			for ($i = 0; $i < $xof; $i++)
			{
				$line = $toUse[$i];
				($numberLine,$enemyId) = split(/:/, $line);
				($min,$max) = split(/-/,$numberLine);
				$choiceRange = $max - $min;
				my $numEnemies = int(rand($choiceRange)) + $min;

				$enemyLine = "EERAND(1,1),$enemyId,ITEM_RANDOM_1,0;";

				for (my $j = 0; $j < $numEnemies; $j++) {
					print $enemyLine."\n";
					$enemyData.=$enemyLine;
				}

			}
			
		}
	}
}

sub matchesRoom
{
	(my $line, my $x, my $y) = @_;
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;
	if ($line =~ m/rooms/)
	{
		($junk,$roomInfo) = split(/ /, $line);
		($min, $max) = split(/-/, $roomInfo);
		($xmin,$ymin) = split(/,/,$min);
		($xmax,$ymax) = split(/,/,$max);
		if (($x >= $xmin) && ($x <= $xmax) && ($y >= $ymin) && ($y <= $ymax))
		{
			return 1;
		}
	}
	return 0;
}


sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };
