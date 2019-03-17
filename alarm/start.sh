#!/bin/sh

name=epics_alarm
script_dir=$(dirname $(readlink -f $0))
bin_dir=$script_dir/bin
param_dir=$script_dir/param

#______________________________________________________________________________
session=`tmux ls | grep $name`
if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name \
	"$bin_dir/EpicsAlarm $param_dir/channel_list.txt"
else
    echo "reattach session $name"
    tmux a -t $name
fi
