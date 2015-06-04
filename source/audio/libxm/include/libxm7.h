
#define XM7_ERR_NOT_A_VALID_MODULE				( 0x01 )
#define XM7_ERR_UNKNOWN_MODULE_VERSION			( 0x02 )
#define XM7_ERR_UNSUPPORTED_NUMBER_OF_CHANNELS	( 0x03 )

#define XM7_ERR_UNSUPPORTED_PATTERN_HEADER		( 0x08 )
#define XM7_ERR_INCOMPLETE_PATTERN				( 0x09 )

#define XM7_ERR_UNSUPPORTED_INSTRUMENT_HEADER	( 0x10 )

#define XM7_ERR_NOT_ENOUGH_MEMORY				( 0x100 )

#define XM7_STATE_ERROR							( 0x8000 )
// BETA: it's an unused define...

typedef struct {
	u8 Note;                // 0 = no note; 1..96 = C-0...B-7; 97 = key off
	u8 Instrument;          // 1..128
	u8 Volume;              // 0..64 (0x40)
	u8 EffectType;
	u8 EffectParam;
}__attribute__ ((packed)) XM7_SingleNote_Type;

typedef struct {
    XM7_SingleNote_Type Noteblock[1];
}__attribute__ ((packed)) XM7_SingleNoteArray_Type;

typedef struct {
  s8 Data[1];
}__attribute__ ((packed)) XM7_SampleData_Type;

typedef struct {
  s16 Data[1];
}__attribute__ ((packed)) XM7_SampleData16_Type;

typedef struct {

   union {
   XM7_SampleData_Type*    SampleData;
   XM7_SampleData16_Type*  SampleData16;
   };
   
   u32 Length;            // lenght of data, always in bytes
   
   u32 LoopStart;
   u32 LoopLength;        // BETA: can be removed if it's the end of the sample... BETA
   
   char Name[22];
   
   u8 Volume;
   u8 Panning;            // (0..0xFF, left to right. 0x80 = center )
   
   s8 RelativeNote;      //  (note relative to C-4)
   s8 FineTune;          //  (finetune, in 128th of an half-tone)
   
   u8 Flags;             //  bit 0: it has a loop
                         //  bit 4: it's a 16 bit sample
	
} XM7_Sample_Type;

typedef struct {

  u16 x;
  u16 y;

} XM7_EnvelopePoints_Type;

typedef struct {

    XM7_Sample_Type* Sample[16];      // pointers to the samples
    
	XM7_EnvelopePoints_Type VolumeEnvelopePoint[12];   //  -- Envelope points: x,y...x,y.... 
	XM7_EnvelopePoints_Type PanningEnvelopePoint[12];  //  -- in couples (2 words/point => a maximum of 12 points).
	
	u16 VolumeFadeout;
	
	char Name[22];
	
	u8 SampleforNote[96];             // sample number for note 1..96
	
	u8 NumberofSamples;               // 0..16  (zero... no samples!)
	
	u8 NumberofVolumeEnvelopePoints;
	u8 NumberofPanningEnvelopePoints;
	
	u8 VolumeSustainPoint;
	u8 VolumeLoopStartPoint;
	u8 VolumeLoopEndPoint;
	
	u8 PanningSustainPoint;
	u8 PanningLoopStartPoint;
	u8 PanningLoopEndPoint;
	
	u8 VolumeType;                 //  bit 0: On; 1: Sustain; 2: Loop
	u8 PanningType;                //  bit 0: On; 1: Sustain; 2: Loop
	
	u8 VibratoType;
	u8 VibratoSweep;
	u8 VibratoDepth;
	u8 VibratoRate;

} XM7_Instrument_Type;

typedef struct {

    // status of the engine/module/playback                   ( BETA: STILL UNUSED! )
	u16 State;               // bit 13: playbacking if set
	                         // bit 14: loaded/ready if set
	                         // bit 15: error if set
    
	// these won't change during the playback!
	char ModuleName  [20];
	char TrackerName [20];
	
	u16 ModuleLength;
	u16 RestartPoint;
	
	u8 NumberofChannels;      // 1..16 ok, > 16 NO!
	u8 NumberofInstruments;   // 1..128
	
	u16 NumberofPatterns;     // up to 256
	
	u8 ModuleFlags;         // bit 0: Freq. table  ( 0= Amiga, 1= linear ) (BETA: ignored)
	u8 DefaultTempo;        // 1..31  (ticks per 'line', default 6)
	u8 DefaultBPM;          // 32..255 (BeatPerMinute)
	
	u8 PatternOrder[256];   // play the patterns in this order (PatternNumber is 0..255)
	
	u16 PatternLength[256];  // the lenght (in lines) of each pattern (min 1, max 256)  (default=64!)
	
	XM7_SingleNoteArray_Type* Pattern [256];   // pointer to the beginning of every single pattern
	                                           // length = (PatternLength)*(NumberofChannels)*5  (bytes)

    XM7_Instrument_Type* Instrument[128]; // pointer to the instruments

	// these _could_ change during the playback!
	u8 CurrentTempo;        // copy of DefaultTempo (at startup)
	u8 CurrentBPM;          // copy of DefaultBPM   (at startup)
	u8 CurrentGlobalVolume; // 0x40                 (at startup)

	// these _will_ change during the playback
	u8 CurrentSongPosition;      //  the pattern in playback now (position in the PatternOrder array)
	u8 CurrentPatternNumber;     //  the pattern in playback now (its number!)
	
	u8 CurrentLine;              //  the line in playback now (1..length of the pattern)
	u8 CurrentTick;              //  the tick in playback now (1..31)
	u16 CurrentDelayTick;        //  the ticks to "delay" after this line (effect EEx)
	
	u8 CurrentSampleVolume[16];     // the volume of the sample on this channel ( 0..0x40 )
	u8 CurrentSamplePanning[16];    // the panning of sample in this channel    ( 0..0xFF left to right, 0x80 = center )
	
	u8 CurrentSampleVolumeEnvelopeState[16];  // the envelope state of the sample on this channel
	u8 CurrentSampleVolumeEnvelopePoint[16];  // the envelope point (x) of the sample on this channel
	u8 CurrentSampleVolumeEnvelope[16];       // the envelope volume of the sample on this channel ( 0..0x40 )
	u16 CurrentSampleVolumeFadeOut[16];       // the fadeout volume of the sample on this channel  ( 0..0x8000 )
	
	s32 CurrentSamplePortamento[16];     // the period pitch
	
	u8 CurrentChannelLastNote[16];       // the number of the last note triggered in this channel        [1 - 96]
	u8 CurrentChannelLastInstrument[16]; // the number of the last instrument triggered in this channel  [1 - 128]
	
	u8 CurrentChannelIsPitchedNote[16];  // flag: the note has been pitched
	
	u8 EffectEAxMemory[16];         // the memory for the EAx (and EBx) effect  [1..0xF]
    u8 EffectAxyMemory[16];         // the memory for the Axy effect            [0x01..0xF0]
	u8 EffectPxyMemory[16];         // the memory for the Pxy effect            [0x01..0xF0]
	u8 EffectRxyMemory[16];         // the memory for the Rxy effect            [0x00..0xFF]
	
	// engine flags
	u8 AmigaPanningEmulation;       // should the panning be 'fixed' in the Amiga style?

} XM7_ModuleManager_Type;

typedef struct {
	char FixedText[17];    //  ID text: must be 'Extended module: ' ; will be checked
	char XMModuleName[20];
	char FixedChar;        //  MUST be 0x1a                         ; will be checked
	char TrackerName[20];
	u16 Version;           //  MUST be 0x103 or 0x104               ; will be checked
	u32 HeaderSize;
	u16 SongLenght;
	u16 RestartPosition;
	u16 NumberofChannels;  //  1..32, > 16 won't be accepted anyway (we've got 16 chn on the DS)
    u16 NumberofPatterns;
	u16 NumberofInstruments;
	u16 XMModuleFlags;
	u16 DefaultTempo;
	u16 DefaultBPM;
	u8 PatternOrder [256];
	u8 NextDataPart [1];   // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMModuleHeader_Type;

typedef struct {
	u32 HeaderLength;                // _always_ 0x9
	u8 PackingType;                  // MUST be ZERO!
	u16 NumberofLinesinThisPattern;  // 1..256
	u16 PackedPatterndataLength;     // CAN be ZERO!
	u8 PatternData[1];               // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMPatternHeader_Type;

typedef struct {
	u32 HeaderLength;
	char Name[22];
	u8 Type;                         // MUST be ZERO! (...but I've seen everything here :| ...)
	u16 NumberofSamples;             // 0..16 ... oh yes, ...it CAN be ZERO...
	u8 NextHeaderPart [1];           // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMInstrument1stHeader_Type;

typedef struct {
	u32 HeaderLength;
	u8  SampleforNotes[96];          // the sample to be used for each note
	u16 VolumeEnvelopePoints[24];
	u16 PanningEnvelopePoints[24];   // Envelope points: x,y...x,y.... in couples 
	                                 // (2 words/point => a maximum of 12 points).
	u8 NumberofVolumePoints;
	u8 NumberofPanningPoints;

	u8 VolumeSustainPoint;
	u8 VolumeLoopStartPoint;
	u8 VolumeLoopEndPoint;

	u8 PanningSustainPoint;
	u8 PanningLoopStartPoint;
	u8 PanningLoopEndPoint;
	
	u8 VolumeType;                   // bit 0: On; 1: Sustain; 2: Loop
	u8 PanningType;                  // bit 0: On; 1: Sustain; 2: Loop
	
	u8 VibratoType;
	u8 VibratoSweep;
	u8 VibratoDepth;
	u8 VibratoRate;
	
	u16 VolumeFadeOut;
	u16 Reserved[11];               // no use

    u8 NextDataPart [1];            // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMInstrument2ndHeader_Type;

typedef struct {
	u32 Length;
	u32 LoopStart;
	u32 LoopLength;
	
	u8 Volume;
	s8 FineTune;
	u8 Type;                        // bit 0,1 - 0: no loop; 1: forward; 2:ping-pong
	                                // bit   4 - 0: 8 bit; 1: 16 bit
	u8 Panning;						// 0x00..0xff
	s8 RelativeNote;				// -128..+127
	u8 Reserved;					// no use

	char Name[22];
	
	u8 NextHeader[1];              // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMSampleHeader_Type;

// XM file format was found at http://aluigi.altervista.org/mymusic/xm.txt


// ARM7 functions
void XM7_Initialize  ();
void XM7_PlayModule  (XM7_ModuleManager_Type* TheModule);
void XM7_StopModule  ();

// ARM9 functions (... well, you can use them even on ARM7 if you want...)
u16  XM7_LoadXM   (XM7_ModuleManager_Type*, XM7_XMModuleHeader_Type*);
void XM7_UnloadXM (XM7_ModuleManager_Type*);


