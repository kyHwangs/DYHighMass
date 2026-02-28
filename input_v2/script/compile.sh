#! /bin/sh

ext=${1##*.}
fname=`basename ${1} .${ext}`

g++ \
-I$INSTALL_DIR_PATH/include \
-L$INSTALL_DIR_PATH/lib $INSTALL_DIR_PATH/lib/libDYHighMassLIB.so $YAMLPATH/libyaml-cpp.so.0.6.3 `root-config --cflags --libs` ${fname}.cc -o ${fname}
echo "Done!"