FMP2GBA - Convert Robin Burrows' MAPPY format FMP file to .c file for GBA
-------------------------------------------------------------------------

This version  modified by Nathan Tolbert

fmp2gba is created by Dave Etherton and does not fall 
under the same license as Anguna.

This code was released to the public by Dave in 2006,
but is owned by Dave.

--------------------------------------------

Version 1.00 - first public release
Version 1.01 - add source to the demo

Get mappy from http://www.geocities.com/SiliconValley/Vista/7336/

Note -- don't confuse mappy the tile editor with mappy the emulator!
	They are completely unrelated.

Example usage: fmp2gba test.fmp output.c Test

Creates output.c file containing three arrays:

Test_colormap - a 256 element array of unsigned shorts containing the 
	palette in GBA format.

Test_graphics - the tile data in 8x8 chunks.  If your tile size in mappy
	is 16x16, this will produce 64 bytes of information per tile.

Test_map - the map data, as bytes.  The indices are terms of the original
	"macro tiles".  So, for example, if your tile size in mappy is
	16x16, index 3 refers to tiles 12 through 15.

Make sure your FMP file is in 1.0 format (change the type in the MapTools
Map Properties dialog box) and re-save if necessary.  Your tiles should be
in 8 bit format in order for it to work correctly.

Mappy can use different tile sizes, but this program will break them down
into contiguous 8x8 sub tiles (scanning left-right, top-bottom).  Mappy's
animation format is not currently supported.

Ideally you'd use 8x8 tiles in Mappy to get the best re-use of tile memory,
although if you're planning on using text-mode graphics you'll need to
widen the datatype for the ..._map output array.

The included demo ROM runs on real hardware.  The source code to the demo
is now included as well, and requires the arm thumb GCC compiler installed
to c:/arm (get it from www.devrs.com/gba)

Please report any bugs and forward any improvements back to me.

-Dave Etherton
 etherton@pacbell.net
