Building Anguna
----------------
Anguna is designed to be built using devkitPro's GBA and DS toolchain.
It is setup to be built on windows, although has been successfully compiled
on linux using wine to run some of the windows-only utilties.  It uses a number of
custom converters and perl scripts as well.



Prerequisites
-------------------

-DevkitPro's toolchain https://sourceforge.net/projects/devkitpro
	Unfortunately, devkitPro's versioning is rather messy, and APIs have changed somewhat,
	so the standard method of installing devkitPro (the windows updater) may not work.
	Anguna was built using the July 2008 release of the various devkitPro products.

	I used the windows auto-installer, but if that doesn't work, you will most likely need:
	-devkitARM
	-libnds
	-libgba

	You will need to set it up according to the instructions, that's beyond the scope of this
	guide.

-Perl

-If you want to edit level maps, you need Mappy http://www.tilemap.co.uk/mappy.php


Building
---------------
Generally, change to the GBA or the DS source tree, and run make.
You can change paths and settings by editing the file makerules






