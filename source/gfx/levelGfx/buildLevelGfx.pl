#!usr/bin/perl
#processes rooms.pre and associate FMP files to create source files for 
#all levels
use Switch;
use CGI qw(param);

$lvlId = param("lvl");
$mode = param("mode");

chdir ("gfx/levelGfx");

open(IN, "<$lvlId.pre") or die "could not open $lvlId.pre";
$id = $lvlId;
$pal = 0;
while(<IN>)
{
	chomp;
	@parts = split(/: /);
	$kw = $parts[0];
	$body = trim($parts[1]);
	switch ($kw)
	{
		case "collide"
		{
			$collide = $body;
		}
		case "watercollide"
		{
			$watercollide = $body;
		}
		case "foreground"
		{
			$foregroundTile = $body;
		}
		case "fmp"
		{
			$fmpFile = $body;
		}
		case "img"
		{
			$imgId = $body;
		}
		case "pal"
		{
			$pal = $body;	
		}
	}
}

$file = "$id.c";
if (($mode eq "headerOnly") || ($mode eq "wingit"))
{
	$file = "$id.tmp";
}

$imgId = trim($imgId);

$imageSrc = "png/$imgId";
$imgFile = "png/$imgId.png";
$imageId = $imgId;

print "img src: $imageSrc\n";
print "img file $imgFile\n";
print "img id $imageId\n";

if (!(-f "$imageSrc.c"))
{

	$newNum = $1 * 4;
    print "running: ../../tools/map/wingitCLI.exe -img $imgFile -output $imageSrc -symbol ${imageId}_ -noMap -imgBpp 4 -noHeader\n";
	print `wine ../../tools/map/wingitCLI.exe -img $imgFile -output $imageSrc -symbol ${imageId}_ -noMap -imgBpp 4 -noHeader`;

	rename("$imageSrc.c", "$imageSrc.c.7c1f");
	open(RAW, "<$imageSrc.c.7c1f") or die "could not open $imageSrc.c.7c1f";
	open(CONVERTED, ">$imageSrc.c") or die "could not open $imageSrc.c";
	while(<RAW>)
	{
		s/0x7C1F/0x0000/g;
		print CONVERTED;
	}
}

open(CFILE, ">$id".".h") or die "could not open $id.h";
{
	print CFILE "#define ZONE_${id}_COLLIDE $collide\n";
	print CFILE "#define ZONE_${id}_WATER_COLLIDE $watercollide\n";
	print CFILE "#define ZONE_${id}_FOREGROUND_TILE $foregroundTile\n";
	print CFILE "#define GFX_${id}_NUMTILES 768\n";

	print CFILE "#define ${id}_Tiles ${imageId}_Tiles\n";
	if ($pal == 0)
	{
		print CFILE "#define ${id}_Pal ${imageId}_Pal\n";
	}
	else
	{
		$pal = $pal  * 16;
		print CFILE "#define ${id}_Pal ((u16 *)&(${imageId}_Pal[$pal]))\n";
	}

	print CFILE "extern const u16 * ${imageId}_Pal[];\n";
	print CFILE "extern const u16 * ${imageId}_Tiles[];\n";

}


sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };
