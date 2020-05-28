#!/bin/sh

top_dir=$(dirname $(readlink -f $0))

name=epics_logger
channel_list=channel_list_e40_20200528.txt

session=`tmux ls | grep $name 2>/dev/null`

command="$top_dir/bin/EpicsLogger \
    $top_dir/param/$channel_list \
    $top_dir/data"

if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name "$command"
else
    # echo "session $name already exists"
    echo "attach session $name"
    tmux a -t $name
fi
