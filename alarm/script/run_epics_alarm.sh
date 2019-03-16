#!/bin/sh

script_dir=$(dirname $(readlink -f $0))
bin_dir=$script_dir/../bin
param_dir=$script_dir/../param

#______________________________________________________________________________
session=`tmux ls | grep epics_alarm`
if [ -z "$session" ]; then
    echo "create new session epics_alarm"
    tmux new-session -d -s epics_alarm \
	"$bin_dir/EpicsAlarm $param_dir/channel_list.txt"
else
    tmux a -t epics_alarm
fi
