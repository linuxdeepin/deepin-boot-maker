#!/usr/bin/env perl 
use strict;
use warnings;

my $DBM_PRO = "../deepin-boot-maker.pro";
my $DBM_QRC = "../deepin-boot-maker.qrc";

if ( -f $DBM_PRO && -f $DBM_PRO ){
	print "OK\n";
} else {
	print "please run this script in dir <project_root>/src/po .\n";
	exit 2;
}

my @po_files = glob "*.po";
my @po_langs = map { s/\.po$//; $_ } @po_files;
print "po langs : $_ \n" for @po_langs;
add_lang( $DBM_PRO, '^\s+po/([a-zA-Z_]+)\.ts', '^\s+po/zh_CN\.ts' );
add_lang( $DBM_QRC, '^\s+<file>po/([a-zA-Z_]+)\.qm</file>', '^\s+<file>po/zh_CN\.qm</file>' );
sub add_lang {
	my ( $target_fp, $regex, $zhcn_regex ) = @_;
	my @need_add;
	my @aleady_have;
	open my $target_fh , '<', $target_fp;
	for (<$target_fh>){
		if ( /$regex/ ){
			print "get :".$1 ."\n";
			push @aleady_have , $1;
		}
	}
	close $target_fh;
	for my $l ( @po_langs ) {
		if (! grep { $_ eq $l } @aleady_have ){
			print "do not have $l  in $target_fp\n";
			push @need_add, $l;
		}
	}

	#write
	do {
		local $^I = '.bak';
		local @ARGV = ($target_fp);
		while (my $line = <>){	
			if ( $line =~ /$zhcn_regex/ ) {
				print $line;
				for (@need_add ) {
					my $s = $line =~ s/zh_CN/$_/r;
					print $s;
				}
			} else {
				print $line;
			}
		}
	}

}

