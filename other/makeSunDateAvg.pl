#!/usr/bin/perl
use warnings;
use strict;
use Time::Piece;
use Math::Round;

my $filename = $ARGV[0];

open(FH, '<', $filename) or die $!;

my $st = 0;
my $rt = 0;
my $i = 0;
my $done = 7;
my $col = 0;
my $maxCol = 8;
my $count = 0;
while(<FH>) {
   chomp($_);
   my ($date, $raise, $set) = split(/,/, $_);

   $rt += Time::Piece->strptime($raise, '%H:%M:%S')->hour;
   $st += Time::Piece->strptime($set, '%H:%M:%S')->hour;

   $i++;
   if( $i == $done ){
      $i = 0;

      $st /= $done;
      $rt /= $done;
      # printf "%3d, %3d,  ", round($rt), round($st);
      # printf "%3d, ", round($rt);
      printf "%3d, ", round($st);
      $st = 0;
      $rt = 0;
      
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