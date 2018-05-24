#!/bin/sh

while true
do
    ret=(`ps ax | grep K18Monitor | grep gl840 | grep -v SCREEN | grep -v /bin/sh`)
    pid=${ret[0]}

    nthread=`ls /proc/$pid/task | wc -l`

    if [ $nthread -gt 24 ]; then
	date
	echo "K18Monitor $pid : killed $nthread threads"
	kill $pid
    fi

    sleep 10
done