#!/bin/sh

thisroot_sh=`root-config --prefix`/bin/thisroot.sh

param=param/channel_list_e40_2017.txt
#param=param/channel_list_e07_2017.txt

screen -AmdS EpicsLogger \
    sh -c ". $thisroot_sh && bin/EpicsLogger $param data"
