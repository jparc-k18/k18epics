#!/bin/sh

name=epics_jsroot
script_dir=$(dirname $(readlink -f $0))
bin_dir=$script_dir/bin
param_dir=$script_dir/param
channel_list=$param_dir/channel_list_e42_20210701.txt

#______________________________________________________________________________
session=`tmux ls | grep $name`
if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name \
	"$bin_dir/EpicsJsRoot $channel_list"
else
    echo "reattach session $name"
    tmux a -t $name
fi
