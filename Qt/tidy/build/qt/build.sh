#!/bin/bash

PRJ_DIR=../../prj

cd ${PRJ_DIR}/vKeyBoard
/mcuzone/work/qt481/_install/bin/qmake vKeyBoard.pro
if [ "$1" == "debug" ];then
    make debug
elif [ "$1" == "clean" ];then
    make clean
else 
    make release
fi

cd -

cd ${PRJ_DIR}/SmartBuilding
/mcuzone/work/qt481/_install/bin/qmake SmartBuilding.pro
if [ "$1" == "debug" ];then
    make debug
elif [ "$1" == "clean" ];then
    make clean
else 
    make release
fi



