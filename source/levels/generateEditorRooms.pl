#!usr/bin/perl

use File::Copy;

opendir(DIR, ".");
@files = grep(/\.dor$/,readdir(DIR));
closedir(DIR);

foreach $file (@files) {
	$prefix = $file;
	$prefix =~ s/\.dor$//g;
	$preFile = $prefix.".pre";
	if (!(-f $preFile)) {
		open OUT,">$preFile";
		print OUT "";
		close OUT;
	}

	$mapFile = $prefix.".fmp";

	if (!(-f $mapFile)) {

		$template = getTemplate($file);
		copy($template, $mapFile);
	}

}

opendir(DIR, ".");
@files = grep(/\.pre$/,readdir(DIR));
closedir(DIR);
foreach $file (@files) {
	$prefix = $file;
	$prefix =~ s/\.pre$//g;
	$dorFile = $prefix.".dor";
	if (!(-f $dorFile)) {
		open OUT,">$dorFile";
		print OUT "";
		close OUT;
	}
}

sub getTemplate(){
	my $dorFile = @_[0];
	$pre = $dorFile;
	$pre =~ s/_[0-9]_[0-9]\.dor//g;
	if ($pre eq "ovr") {
		return "ovr_template_fmp";
	}

	open DOR, "<$dorFile";
	while (<DOR>) {
		$line = $_;
		if ($line =~ m/size:  ([1-9]),([1-9])/){
			if ($1 eq "2") {
				if ($2 eq "2") {
					return "d_template_2_2_fmp";
				} else {
					return "d_template_2_1_fmp";
				}

			}else {
				if ($2 eq "2") {
					return "d_template_1_2_fmp";
				} else {
					return "d_template_fmp";

				}
			}

		}

	}
	
}


