#!/usr/bin/perl
use warnings;
use strict;
use POSIX;

my $filename = $ARGV[0];

open(FH, '<', $filename) or die $!;

my $avg = 0;
my $i = 0;
my $done = 7;
my $col = 0;
my $maxCol = 8;
my $count = 0;
while(<FH>) {
   $avg += $_;
   $i++;
   if( $i == $done ){
      $i = 0;
      $avg /= $done;
      printf "%3d, ", ceil($avg);
      $avg = 0;
      
      $col++;
      $count++;
   }
   if($col == $maxCol){
      $col = 0;
      print "\n";
   }
}

print "\nCount: $count\n";
close(FH);