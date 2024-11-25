#! /bin/sh

mkdir ROOT

for FILE in ./*
do
	if [ -d $FILE ]
	then
	  # echo ${FILE}
		echo ./ROOT/${FILE}.root
		hadd ./ROOT/${FILE}.root ${FILE}/*.root
	fi
done


rm ./ROOT/ROOT.root

hadd ./ROOT/Data.root ./ROOT/Run2018*.root
mv ./ROOT/NNLO_10to50.root ./ROOT/NNLO_10to50_v1.root
hadd ./ROOT/NNLO_10to50.root ./ROOT/NNLO_10to50_v1.root ./ROOT/NNLO_10to50_v2.root
