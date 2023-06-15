#!/bin/sh

pgrep -af epics_jsroot.py
pkill -2 -f epics_jsroot.py
