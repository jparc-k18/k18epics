#!/bin/sh

name=slack_alarm
script_dir=$(dirname $(readlink -f $0))

#______________________________________________________________________________
session=`tmux ls | grep $name`
if [ -z "$session" ]; then
    echo "create new session $name"
    tmux new-session -d -s $name \
	"while true; do $script_dir/monitor.py; sleep 60; done"
else
    echo "reattach session $name"
    tmux a -t $name
fi
