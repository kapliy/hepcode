#!/bin/bash

# This script is designed to build files for an ntuple reader class.
# User provides the name of the class followed by a path to a sample root file

# WARNING: This isn't completely finished but it will still get most of the majority of the work done for you

# set vars from user-input
class_name=""
root_file_path=""
if [ $# == 2 ]; then
    class_name="${1}"
    root_file_path="${2}"
else
    echo "ERROR :: wrong number of arguments, please supply class name followed by the path to a root file"
    return
fi

# run tree->MakeClass()
if [ -e tree.h ]; then rm tree.h; fi
if [ -e tree.C ]; then rm tree.C; fi
echo -e "tmp_root_macro() {\nTFile *file = new TFile( \"${root_file_path}\" );\nTTree *tree = (TTree*) file->Get( \"tree\" );\ntree->MakeClass();\n}\n" > tmp_root_macro.C
root -l -q tmp_root_macro.C
rm tmp_root_macro.C

# check if tree->makeClass() succeeded
if [ ! -e tree.h ]; then
    echo "ERROR :: tree->MakeClass() failed"
    return
fi

# build header file first
echo "building header file"
cat tree.h | egrep -v 'const Int_t|fChain->SetBranch|Cut\(|Loop\(|Notify\(|Show\(' > ${class_name}.h
perl -p -i -e "s#tree_h#${class_name}_h#g" ${class_name}.h
perl -p -i -e "s#tree\(#${class_name}(#g" ${class_name}.h
cat ${class_name}.h | while read line; do
    echo $line >> ${class_name}.h.bak
    if [ "$line" == "#endif" ]; then break; fi;
done; mv ${class_name}.h.bak ${class_name}.h
cat ${class_name}.h | while read line; do
    echo $line >> ${class_name}.h.bak
    if [ "${line}" == "#include <TFile.h>" ]; then
	echo -e "#include <vector>\n#include <string>\n#include \"TrigFTKAna/Trees/${class_name}.h\"" >> ${class_name}.h.bak
    fi
done; mv ${class_name}.h.bak ${class_name}.h

# build src file
echo "building source file"
cat tree.h | grep 'fChain->SetBranch' > ${class_name}.cpp
cat ${class_name}.cpp | while read line; do
    branch_name="`echo ${line} | awk '{print $2}' | sed -e 's#&##g' | sed -e 's#,##g'`"
    echo "if( fChain->GetBranchStatus(\"${branch_name}\") ) { ${line} }" >> ${class_name}.cpp.bak
done; mv ${class_name}.cpp.bak ${class_name}.cpp


# cleanup
rm tree.h
rm tree.C

echo "DONE"
