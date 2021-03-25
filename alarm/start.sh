#!/bin/sh

name=epics_alarm
script_dir=$(dirname $(readlink -f $0))
bin_dir=$script_dir/bin
param_dir=$script_dir/param
if [ $# = 0 ]; then
    param_file=$param_dir/channel_list.txt
else
    param_file=$1
fi

#______________________________________________________________________________
session=`tmux ls | grep $name`
if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name "$bin_dir/EpicsAlarm $param_file"
else
    echo "reattach session $name"
    tmux a -t $name
fi
