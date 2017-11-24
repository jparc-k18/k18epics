#!/bin/sh

thisroot_sh=$HOME/software/ROOT/pro/bin/thisroot.sh

param=param/channel_list_e40_2017.txt

screen -AmdS EpicsJsRoot \
    sh -c ". $thisroot_sh && bin/EpicsJsRoot $param"
