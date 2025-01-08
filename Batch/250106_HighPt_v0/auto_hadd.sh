#! /bin/sh

mkdir ROOT

ERAS=(
"2016_preVFP"
"2016_postVFP"
"2017"
"2018"
)

for era in ${ERAS[@]}
do
  mkdir -p ROOT/$era
  cd $era

  for FILE in ./*
  do
  	if [ -d $FILE ]
  	then
      echo $FILE
  		hadd ../ROOT/$era/${FILE}.root ${FILE}/*.root
  	fi
  done

  cd ..
  hadd ./ROOT/$era/Data.root ./ROOT/$era/Run*.root
  mv ./ROOT/$era/NNLO_10to50.root ./ROOT/$era/NNLO_10to50_v1.root
  hadd ./ROOT/$era/NNLO_10to50.root ./ROOT/$era/NNLO_10to50_v1.root ./ROOT/$era/NNLO_10to50_v2.root
done
