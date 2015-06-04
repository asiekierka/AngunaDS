
//{{BLOCK(items)

//======================================================================
//
//	items, 256x32@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2007-12-20, 20:35:37
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

const unsigned short itemsPal[16]=
{
	0x7C1F,0x03FF,0x2529,0x4E94,0x5B3A,0x7BDE,0x084A,0x08D1,
	0x157A,0x2E9C,0x3B3D,0x0016,0x005E,0x39DF,0x56BF,0x0000,
};

const unsigned short itemsTiles[2048]=
{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,0x0000,0x8100,
	0x0000,0x7100,0x0000,0x6100,0x0000,0x6100,0x0000,0x6810,
	0x0000,0x0000,0x1111,0x0011,0x7884,0x0167,0x1117,0x1671,
	0x1111,0x1711,0x0011,0x1710,0x1117,0x1671,0x8876,0x1567,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,
	0x0000,0x1000,0x0000,0x0000,0x0000,0x1000,0x1100,0x1111,
	0x0000,0x0000,0x1110,0x0001,0x9B81,0x0111,0xA119,0x19AA,
	0xAB89,0x119A,0xAA11,0x0119,0x9AB1,0x1011,0x19AB,0x0101,

	0x0000,0x0000,0x1100,0x1111,0x4F10,0xDEFF,0x1DF1,0xE111,
	0x11E1,0x1111,0x11E1,0x1000,0x1ED1,0xE111,0xEDC1,0xDEFF,
	0x0000,0x0000,0x0001,0x0000,0x0011,0x0000,0x011D,0x0000,
	0x101E,0x0000,0x011E,0x0000,0x101D,0x0000,0x01CD,0x0001,
	0x0000,0x0000,0x0000,0x7777,0x7020,0x7CEE,0xD252,0x8EDE,
	0xB554,0xCE7D,0x4454,0xCEBB,0x5B42,0xDEB5,0xBF22,0xDE74,
	0x0000,0x0000,0x0007,0x0000,0x0778,0x0000,0x7889,0x0000,
	0x8997,0x0007,0x8887,0x0068,0x8887,0x0068,0x7777,0x0F78,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x7777,
	0x7000,0x55A9,0x7000,0xF677,0xF000,0x45F9,0xF000,0x352F,
	0xF770,0x0000,0x7A57,0x000F,0x78A7,0x000F,0xFF77,0x000F,
	0x889A,0x00FF,0x6FFF,0x0F0F,0x77F3,0x00FF,0xFFF4,0x0F0F,
	0x0000,0x0000,0x0000,0x0000,0xBB00,0x006B,0xCCB0,0x06BC,
	0x5DCB,0x6BC5,0xD5DB,0xBCDD,0xDED6,0xCDCC,0xCDC6,0xDCCC,
	0x0000,0x0000,0x0000,0x0000,0xBB00,0x0006,0xCCB0,0x006C,
	0xDDCB,0x06BC,0xCDDC,0x66BC,0xCCDD,0x66BC,0xCCCC,0x66BD,

	0x7700,0x0000,0x5A70,0x7777,0x9970,0x5555,0x9870,0x99AA,
	0x9870,0x595A,0x9870,0xA95A,0x9860,0xA999,0x9860,0xAA55,
	0x0000,0x0077,0x7777,0x0FA5,0xA555,0xFF99,0x9989,0x0F89,
	0x8975,0xFF89,0x8979,0x0F89,0x8889,0xFF87,0xAAAA,0x0F87,
	0x3000,0x0000,0x7400,0x0003,0x4F00,0x0032,0xF240,0xA924,
	0x24F0,0x2A9F,0x3F40,0x22A9,0x7374,0x7F9A,0x924F,0xF0F9,
	0x0000,0x7799,0x9900,0xF2F7,0xF799,0x0F4F,0x0F27,0x00F4,
	0x40FF,0x000F,0xF400,0x0000,0x0F40,0x0000,0x00F4,0x0000,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x9000,
	0x0000,0x0000,0x0000,0x0000,0x4000,0x0000,0x7400,0x00F4,
	0x4740,0x000F,0xF370,0x0000,0x0FF7,0x0000,0x000F,0x0000,
	0x0000,0xF700,0x0000,0x9870,0x0000,0x8A97,0x7700,0x8989,
	0x87FF,0x78A9,0xFF00,0x8987,0x2000,0x87FF,0x2000,0x7F23,
	0x0077,0x0000,0x0795,0x0000,0x79F9,0x0000,0x9875,0x0007,
	0xA7A7,0x0FF7,0xFA77,0x000F,0x2FF7,0x0000,0x232F,0x0000,

	0x0000,0x3300,0x0000,0x5430,0x0000,0xF53F,0x0000,0x442F,
	0x0000,0x34F0,0x0000,0xFFF0,0x0000,0x453F,0xF000,0x4453,
	0x0022,0x0000,0x0F35,0x0000,0xF32F,0x0000,0xF254,0x0000,
	0x0F23,0x0000,0x0F2F,0x0000,0xF234,0x0000,0x2433,0x000F,
	0x0000,0x0000,0x0005,0x0000,0x7053,0x00F7,0x8635,0xF688,
	0x6923,0xF966,0xA8F5,0xF89A,0x98FF,0xF889,0x87F0,0xF677,
	0x0000,0x0500,0x6770,0x0350,0x8886,0x053F,0xFFF9,0x0329,
	0x9AA9,0x05F9,0x8998,0x03F8,0x7876,0x00F7,0x8986,0x00F7,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x00F0,0xF000,0xFF5F,
	0x5F00,0x3F3F,0x3F00,0xFF32,0x34F0,0xF0F2,0x25F0,0x0F0F,
	0x0000,0x0000,0x0000,0x0000,0x0F00,0x0000,0xF4FF,0x000F,
	0xF3F3,0x00F4,0x22FF,0x00F3,0x2FF0,0x0F52,0xF000,0x0F42,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x9A80,0x0000,0x8775,0x8000,0x9987,0xA000,0x9997,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0068,0x0000,0x0688,0x0000,0x6A98,0x0000,0x7889,0x0000,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,0x0000,0x0100,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,0x0000,0x0101,
	0x1000,0x1010,0x0101,0x1101,0x1010,0x1111,0x1101,0x1111,
	0x0000,0x1010,0x0100,0x0101,0x1010,0x1010,0x0101,0x1101,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,

	0x0000,0x5641,0x1000,0x1568,0x7100,0x1166,0x6710,0x5776,
	0x6651,0x5116,0x1110,0x1775,0x0100,0x1111,0x0000,0x0000,
	0x5555,0x1155,0x1111,0x0111,0x0000,0x0000,0x0001,0x0000,
	0x0011,0x0000,0x0001,0x0000,0x0010,0x0000,0x0000,0x0000,
	0x8810,0xAAB8,0x1B81,0xB111,0x1181,0x1111,0x11A1,0x1000,
	0x1A91,0xB111,0xA911,0xABBB,0x9110,0x9999,0x1100,0x1111,
	0x119A,0x0010,0x011A,0x0001,0x101B,0x0000,0x011B,0x0000,
	0x101A,0x0000,0x0119,0x0000,0x1011,0x0000,0x0101,0x0000,

	0xCC11,0xCCCC,0x1110,0x1111,0x0000,0x0000,0x0000,0x1000,
	0x0000,0x1000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x1CDD,0x0010,0xCDD1,0x0101,0xDD11,0x101C,0xDEE4,0x01CD,
	0xD11C,0x1CDD,0xCEE1,0x0111,0x1110,0x1010,0x0000,0x0000,
	0xCF00,0xDEBB,0xDF00,0x7EDE,0xDF00,0x7DEE,0xCF00,0x77DE,
	0x7F00,0x77CD,0xF000,0x6677,0x0000,0xFFFF,0x0000,0x0000,
	0x6787,0xFF77,0x7887,0xF666,0x7878,0xF667,0x7787,0xF666,
	0x6777,0xF666,0x6666,0xFFF6,0xFFFF,0x0F0F,0x0000,0x0000,

	0x0000,0x3452,0x0000,0x4352,0x2000,0x5345,0x2000,0x2435,
	0x5200,0xF434,0x5200,0xF534,0xF000,0xF233,0x0000,0x0FFF,
	0xF0F5,0x00F0,0x0F0F,0x000F,0x00FF,0x0000,0x0F0F,0x0000,
	0x00F0,0x0000,0x000F,0x0000,0x00F0,0x0000,0x000F,0x0000,
	0xCC60,0xCCCC,0xCB60,0xCCCC,0xB600,0xCCCC,0x6000,0xDCBB,
	0x0000,0xBB66,0x0000,0x6660,0x0000,0x0600,0x0000,0x0000,
	0xDCCC,0x666B,0xCDCC,0x066B,0xBCDC,0x0666,0x6BBD,0x0066,
	0x666B,0x0006,0x6066,0x0000,0x0606,0x0000,0x0000,0x0000,

	0x9860,0xA99A,0x8860,0xA888,0x9860,0xA85A,0x9600,0xA85A,
	0x8600,0x89A9,0x6000,0xAA98,0x0000,0x998F,0x0000,0xFFF0,
	0x8999,0xFF87,0x7779,0x0F88,0x8879,0xFF79,0x9878,0x0FF8,
	0x9877,0xF0F8,0x899A,0x0F0F,0xF899,0x00F0,0x0FFF,0x000F,
	0x92FF,0x3002,0x2900,0xF327,0xF900,0x074F,0x0270,0x7FF3,
	0x3F70,0xF00F,0xF270,0x0000,0x0F20,0x0000,0x00F0,0x0000,
	0x0027,0x0000,0x007F,0x0000,0x07F0,0x0000,0x0307,0x0000,
	0x4F3F,0x0000,0xF4F0,0x0000,0x0F00,0x0000,0x0000,0x0000,

	0x0000,0xF900,0x9340,0x0F97,0xFFF0,0x77F9,0x7000,0xFF0F,
	0xF300,0x0000,0x0F40,0x0000,0x00F4,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0099,0xF444,0x99FF,0x0F77,
	0xFF00,0xF344,0x0000,0x0FFF,0x0000,0x0000,0x0000,0x0000,
	0x2000,0xF454,0x2000,0x3455,0x2000,0x3455,0x27F0,0x3454,
	0x887F,0x3422,0x77F0,0x2288,0xFF00,0x8877,0xF000,0xF0F0,
	0x2333,0x0000,0x2433,0x0000,0x2454,0x0000,0x2344,0x00F7,
	0x8223,0x0F77,0x7882,0x00F7,0xF778,0x000F,0xF0F0,0x0000,

	0x2F00,0xBBEE,0xBF00,0xCBB5,0xCF00,0xCDE5,0xCF00,0xDDDE,
	0xCF00,0xCCDD,0xBF00,0x66DC,0xF000,0xBBB6,0x0000,0xFFFF,
	0xCDCB,0x00FB,0xDDBB,0x00FB,0xDECC,0x00FB,0xCDDD,0x00FB,
	0xCBBB,0x00FB,0xDCC6,0x00FD,0xBDD8,0x000F,0xFFFF,0x0000,
	0xA7F0,0xF689,0x97F0,0xF688,0x87F0,0xF688,0x77FF,0xFF78,
	0xA59F,0xF789,0x889F,0xFF78,0x886F,0xFFF7,0xFFF0,0x0FFF,
	0x9A98,0x00F7,0x8986,0x00F6,0x7877,0x0FF7,0xAA96,0xFF69,
	0x987F,0xFF88,0x87FF,0xFF78,0xFFFF,0x0FFF,0x0000,0x0000,

	0x24F0,0x00F4,0x43F0,0xFFF5,0x42F0,0x5F25,0x2F00,0x5F44,
	0xF000,0x3F22,0x0000,0x3FFF,0x0000,0xFF00,0x0000,0x0000,
	0x4F00,0x0F42,0x5FFF,0x0F24,0x52F5,0x0F24,0x44F3,0x00F2,
	0x22F2,0x0F0F,0xFFF2,0x00F0,0x0FFF,0x000F,0x0000,0x0000,
	0x9000,0x9998,0x8000,0xA988,0x6000,0x9898,0x0000,0x88A6,
	0x0000,0x6600,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x789A,0x0000,0x7AA9,0x0000,0x6A9A,0x0000,0x06A9,0x0000,
	0x0006,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,0x0000,0x0100,
	0x0000,0x1010,0x0000,0x0101,0x1000,0x1010,0x0100,0x1101,
	0x1010,0x1110,0x0101,0x1111,0x1010,0x1111,0x1101,0x1111,
	0x1110,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x9744,0x9999,
	0xFF40,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x3434,0x4779,0x0777,
	0xFFFF,0x3434,0x0000,0x0FFF,0x0000,0x0000,0x0000,0x0000,
	0x4000,0x0000,0x4000,0x0004,0x7000,0x000F,0x9000,0x000F,
	0x9000,0x000F,0x9000,0x000F,0x9000,0x000F,0x9000,0x000F,
	0x0000,0x0000,0x0000,0x0000,0x0000,0xFFF0,0x0000,0x7DBF,
	0x0000,0xD5DF,0x0000,0x7D7F,0x0000,0xB86F,0x0000,0x676F,

	0x0000,0x0000,0x0000,0x0000,0x000F,0x0000,0x00F6,0x0000,
	0x0F68,0x0000,0xF76B,0x0000,0xFAFF,0x0000,0xFA7F,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x6000,0x0000,0xB600,0x0000,0xDB60,
	0xF550,0x0000,0x5500,0x000F,0x5000,0x000F,0x4660,0x000F,
	0x4776,0x000F,0x276B,0x0066,0x766E,0x00F7,0x66BC,0x000B,
	0x0000,0x5500,0x0500,0xAA55,0x5550,0x99A5,0x5AA5,0x9555,
	0x55AA,0x9555,0x59A5,0x9A95,0x99A0,0x99AA,0xAAA5,0x5555,

	0x0555,0x0000,0x55AA,0x0005,0xA5A9,0x0055,0xAA5A,0x055A,
	0x5555,0x0555,0x555A,0x0AA5,0x55A9,0x5A55,0xAA89,0x0555,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0344,
	0x4000,0x3455,0x5400,0x4555,0x5400,0x4454,0x4300,0x3444,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x4430,0x0000,0x5543,0x0004,0x5443,0x0004,0x4433,0x0003,
	0x4400,0x0034,0x5540,0x0345,0x5554,0x3455,0x5454,0x3454,
	0x5444,0x2454,0x4443,0x2344,0x4430,0x0234,0x3300,0x0023,

	0x0000,0x0000,0x4300,0x0044,0x5430,0x0455,0x4430,0x0445,
	0x4430,0x0445,0x4320,0x0344,0x3200,0x0033,0x0000,0x0000,
	0x3440,0x0000,0x4554,0x0003,0x4454,0x0003,0x3443,0x0002,
	0x2330,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0043,0x3000,0x0454,0x3000,0x0343,
	0x0000,0x0033,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x7077,0x0000,0x8797,0x0000,0x9970,0x0000,0x2220,
	0x0000,0x4432,0x7000,0xFFF7,0x8700,0x88AA,0xA700,0x6889,

	0x7777,0x0000,0xF787,0x0000,0x8F78,0x000F,0xF222,0x000F,
	0xF334,0x00F3,0x3FFF,0x000F,0x2998,0x00F3,0x2886,0x00F4,
	0x0000,0x00FF,0xF000,0x0F78,0x8F00,0x0F89,0x98F0,0xF79E,
	0x97F0,0xF9E9,0x76F0,0x8E98,0x6F00,0x9877,0xF000,0x876F,
	0x0000,0x0000,0x0000,0x0000,0x00F0,0x0000,0xFF7F,0x0000,
	0xCFF7,0x000F,0xF8DB,0x0000,0xFF78,0x0000,0x7887,0x00FF,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xF000,
	0x0000,0xF000,0x0000,0xF000,0x0000,0xF000,0x0000,0x6F00,

	0x0000,0x0000,0x0FF0,0x0000,0xFCBF,0x000F,0xBCCB,0x00FC,
	0xCBCB,0x00FB,0xBCBC,0x0FCC,0xCBCC,0x0FBC,0xBCC8,0x00FB,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0x0000,0x1000,0x0000,0x0100,0x0000,0x1010,0x0000,0x0101,
	0x0000,0x1010,0x0000,0x1101,0x1000,0x1110,0x0100,0x1111,
	0x1110,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x9000,0x000F,0x7000,0x000F,0x7000,0x000F,0x4000,0x000F,
	0x7400,0x00F4,0x7300,0x00F3,0x7400,0x00F4,0x0300,0x0003,
	0x0000,0x9A7F,0x0000,0xA9F0,0x0000,0xFF00,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0xF797,0x0000,0x0F79,0x0000,0x00FF,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0xCDB6,0x6000,0xBCCB,0xB600,0x6BCD,0xDB60,0x66BC,
	0xCEB6,0x666B,0xBDD6,0xFC66,0xCC60,0xFFCC,0x6600,0xFFF6,
	0xB66B,0x000F,0xFB66,0x0000,0x0FB6,0x000F,0xF0F6,0x0000,
	0x0FFF,0x0000,0x00FF,0x0000,0x000F,0x0000,0x0000,0x0000,
	0x599A,0x7555,0x5989,0x7A55,0x9999,0x8AA9,0xAA55,0x5A98,
	0x9A50,0x5559,0x5A50,0x5505,0x55A5,0x5A00,0x0050,0xA000,

	0xAA98,0x5AAA,0xAA97,0x0555,0xA989,0x505A,0x9855,0x005A,
	0x9A55,0x05A9,0x5A55,0x055A,0x50A5,0x0555,0x000A,0x0055,
	0x3000,0x3344,0x0000,0x2332,0x0000,0x3443,0x0000,0x4454,
	0x0000,0x4554,0x0000,0x3440,0x0000,0x0000,0x0000,0x0000,
	0x3332,0x0002,0x4333,0x0034,0x4432,0x0045,0x5433,0x0045,
	0x4303,0x0004,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3300,0x0033,0x4430,0x0334,0x4440,0x0344,
	0x4540,0x0344,0x5540,0x0345,0x4400,0x0034,0x0000,0x0000,

	0x2000,0x0332,0x3200,0x3444,0x4320,0x4544,0x4300,0x4545,
	0x4300,0x4555,0x3000,0x0444,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x3300,0x0000,
	0x3430,0x0003,0x4440,0x0003,0x3400,0x0000,0x0000,0x0000,
	0x0032,0x0000,0x0043,0x0000,0x0000,0x0033,0x3000,0x0343,
	0x3000,0x0444,0x0000,0x0043,0x0000,0x0000,0x0000,0x0000,
	0xA870,0x1688,0xA870,0x9568,0x9870,0x9168,0x9870,0x1168,
	0x8770,0x9788,0x7F00,0xF788,0xFFF0,0x7777,0xFF00,0xFFFF,

	0x28F1,0x0F73,0x2F19,0x0F7F,0x2F11,0x0F73,0x2F91,0x0F7F,
	0x88F9,0x0F77,0x787F,0x0FF7,0x7777,0x0FFF,0xFFFF,0x00FF,
	0x0000,0xB6F0,0x0000,0x6F00,0x0000,0xF000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x8997,0x0F99,0x9887,0x0F67,0x7976,0xF698,0x696F,0xF869,
	0x87F0,0xF776,0x7F00,0x0F78,0xF000,0x00FF,0x0000,0x0000,
	0x0000,0x86F0,0x0000,0x686F,0x0000,0xF65F,0xF000,0x0FF3,
	0x3F00,0x000F,0xF5F0,0x0000,0x0FF0,0x0000,0x0000,0x0000,

	0xFFF6,0x000F,0x000F,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,

	0x1000,0x1110,0x0100,0x1111,0x1010,0x1111,0x0100,0x1111,
	0x1010,0x1111,0x0100,0x1111,0x1010,0x1111,0x1101,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
	0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,0x1111,
};

//}}BLOCK(itemGfx)