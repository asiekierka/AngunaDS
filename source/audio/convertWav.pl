#!/usr/bin/perl


$wav = $ARGV[0];
$wine = $ARGV[1];

$oFile = $wav;
$oFile =~ s/.wav$/.o/g;

$hFile = $wav;
$hFile =~ s/.wav$/.h/g;

$symbol = $wav;
$symbol =~ s/.wav$/_wav/g;
$symbol =~ s/^audio\/wavs\///g;

$symbol_length = $symbol."_length";

$fileSize = -s $wav;

#create object file.....
`$wine audio/bin2o.exe $wav $oFile $symbol`;

#create h file
print "echo \"#define $symbol_length $fileSize\" > $hFile\n";
open HEADER, ">$hFile";
print HEADER "#define $symbol_length $fileSize\n";
close HEADER;
#`echo "#define $symbol_length $fileSize" > $hFile`;

