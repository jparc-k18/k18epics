#!/bin/sh

list=param/channel_list_e07_2017.txt
log=data/epics_`date +%Y%m%d_%H%M%S`.log

bin/epics_logger $list data > $log &
