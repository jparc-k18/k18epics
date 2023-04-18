#!/bin/sh

pgrep -af epics_recorder.py
pkill -2 -f epics_recorder.py
