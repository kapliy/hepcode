#!/usr/bin/perl -w

use strict;
use FileHandle;


my $dirforntuples = "/pnfs/uchicago.edu/atlaslocalgroupdisk/group10/phys-sm/data10_7TeV/*/";

my @formattags = ( "100506.01" , "100506.02" , "100506.03" , "100519.01" , "100519.02" , "100519.03" );
my $nformats = scalar(@formattags);
print "$nformats\n";

# retrieve filenames to generate ntuple MakeClass description. generate makeclass.
my %ntuplefiles = ();
my $n = 1;
for my $formattag (@formattags) {
    chomp $formattag;
    ++$n;
    my $largestntuple = `echo ${dirforntuples}/*WZphys.${formattag}*.root* | xargs ls -lSo | head -n1 | awk '{print \$8}'`;
    chomp $largestntuple;
    #my $largestntuple = "/pnfs/uchicago.edu/atlaslocalgroupdisk/group10/phys-sm/data10_7TeV/group10.phys-sm.data10_7TeV.00152166.physics_L1Calo.recon.ESD.r1239.WZphys.100506.01.D3PD/group10.phys-sm.data10_7TeV.00152166.physics_L1Calo.recon.ESD.r1239.WZphys.100506.01.D3PD.D3PD._00083.root";
    if( !($largestntuple =~ m/D3PD/) ) {
        print "could not find ntuple for tag ${formattag}. skipping.\n";
        next;
    }
    $largestntuple =~ s/\/pnfs/dcache:\/pnfs/;
    my $classname = "D3PDWZTree";
    my $fh = FileHandle::new;
    my $Cscriptname = "./MakeD3PDWZTreeVariant.C";
    open $fh, ">${Cscriptname}";
    print $fh <<ENDCSCRIPT;
void
MakeD3PDWZTreeVariant() 
{
    TFile* f = TFile::Open( "$largestntuple" );
    if( !f ) { continue; }
    TTree* t = f->Get("physics");
    if( !t ) { continue; }
    t->MakeClass( "${classname}" );
}
ENDCSCRIPT
    system( "root -b -q ${Cscriptname}" );
    if( -e "${classname}.h" && -e "${classname}.C" ) {
        system( "mv -f ${classname}.h ${classname}_v${n}.h" );
        system( "mv -f ${classname}.C ${classname}_v${n}.cpp" );
    }
}
