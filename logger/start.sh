#!/bin/sh

thisroot_sh=`root-config --prefix`/bin/thisroot.sh

param=param/channel_list.txt

screen -AmdS EpicsLogger \
    sh -c ". $thisroot_sh && bin/EpicsLogger $param data"
