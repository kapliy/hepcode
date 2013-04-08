#!/bin/bash

# This a script to be used whenever you change the directory location of your xml output. It updates the metadata.xml.bz2
# file to reflect the current directory location

# When sourced it will run over all of the ntuple_*.aen folders in your current directory and update the metadata
# corresponding to each folder. It also backs up your original metadata.xml.bz2 file in metadata.xml.bz2.bak

# loop over all ntuple directories in current directory
for adir in ntuple_*.aen; do
  cd $adir
  # make backup
  cp metadata.xml.bz2 metadata.xml.bz2.bak
  # unzip the metadata so it can be edited
  bunzip2 metadata.xml.bz2
  # determine the old and new directory locations, and use perl to replace the old with the new
  newdir=`pwd`
  olddir=""; cat metadata.xml | while read line; do 
    if [ "${line:0:12}" == "<_directory>" ]; then 
      olddir=`echo $line | sed -e 's#<_directory>##g' | sed -e 's#</_directory>##g'`; 
      echo -e "old directory: $olddir\nnew directory: $newdir\n; 
      perl -p -i -e "s#${olddir}#${newdir}#g" metadata.xml; break; 
    fi; 
  done;
  # rezip
  bzip2 metadata.xml
  cd ..
done

