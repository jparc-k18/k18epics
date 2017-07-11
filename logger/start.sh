#!/bin/sh

thisroot_sh=$HOME/software/ROOT/pro/bin/thisroot.sh

param=param/channel_list_e07_2017.txt

screen -AmdS EpicsLogger \
    sh -c ". $thisroot_sh && bin/epics_logger $param data"
