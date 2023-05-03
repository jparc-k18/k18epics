#!/bin/sh

top_dir=$(dirname $(readlink -f $0))

name=epics_jsroot

session=`tmux ls | grep $name 2>/dev/null`
command="$top_dir/epics_jsroot.py"

if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name "$command"
else
    echo "reattach session $name"
    tmux a -t $name
fi
