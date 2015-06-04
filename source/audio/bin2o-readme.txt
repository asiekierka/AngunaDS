bin20 v1.0
=============

By:		anli, one of the #gbadev devotees
		Member of Parallax Sisters
		anli@parallaxsisters.nu

Disclaimer:	This utility is still testware! Anyway, it seems to work ok.
		If you find an error or thinks it can be improved, send me a mail!
		Anyway, if any damage is caused by the use of it, for instance if your
		computer melts down, you cannot hold us responsible for it!

Format:		bin2o.exe [options] <infile.dat> <outfile.o> <variable name>

		options:
		/align=nn   nn is the address alignment of the variable)
		/const      making a const array (useful for ROM images)
		/arm
		/thumb
		/interwork


Description:	This is a utility to create an elf object file from a binary file.
		I am aware of the fact that such utilities are already available, but they
		seem to create strange symbol names that I dont want to have. That symbol
		names are based on the filename from which the object file was created.

		Instead, you can decide what symbol name you want, and also tell which alignment
		your array shall have. The alignment can be important for some BIOS functions
		that wants source buffers to start on a boundary divisable with 4 (for instance).
		Dont forget to use the /const switch if you want the buffer to be placed in Read Only Memory.
		
		How I use this utility:

		Lets assume that I have a file called "hello.mod". I have a mod file containing the
		song jinglebells. It is 36232 bytes long. Now I write this on the command line:

		bin2o /const /arm hello.mod hello.o jinglebells

		This creates the file hello.o in the directory.
		Now I add this to my source file (or a header file that is included from it):

		#define JINGLEBELLS_LENGTH 36232
		extern const char jinglebells[];

		And now I can use the array from my program.
		Dont forget to add the .o-file when you are linking!
		(Put it as an additional argument to the command ld).

Category:	Freeware

Version history:
		v1.0
		v1.1 Fixed a bug when alignment is set (with the alignment switch)
		     This was causing an incorrect ELF-file. (doh!)