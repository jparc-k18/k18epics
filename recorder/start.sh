#!/bin/sh

top_dir=$(dirname $(readlink -f $0))

name=epics_recorder
channel_list=channel_list.txt

session=`tmux ls | grep $name 2>/dev/null`

command="$top_dir/epics_recorder.py"

if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name "$command"
else
    # echo "session $name already exists"
    echo "attach session $name"
    tmux a -t $name
fi
