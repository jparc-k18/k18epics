#!/bin/sh

while true
do
    ret=(`ps ax | grep K18Monitor | grep gl840 | grep -v SCREEN | grep -v /bin/sh`)
    pid=${ret[0]}
    nthread=`ls /proc/$pid/task 2>/dev/null | wc -l`
    if [ $nthread -gt 24 ]; then
	date
	echo "K18Monitor gl840($pid) : killed $nthread threads"
	kill $pid
    fi

    ret=(`ps ax | grep K18Monitor | grep mppc | grep -v SCREEN | grep -v /bin/sh`)
    pid=${ret[0]}
    nthread=`ls /proc/$pid/task 2>/dev/null | wc -l`
    if [ $nthread -gt 24 ]; then
	date
	echo "K18Monitor mppc($pid) : killed $nthread threads"
	kill $pid
    fi

    sft_board=`caget -t MPPC:SFT:BOARD 2>&1`
    cft_board=`caget -t MPPC:CFT:BOARD 2>&1`
    if [ "$sft_board" = "-99.99" -o "$cft_board" = "-99.99" ]
    then
	date
	echo "K18Monitor mppc($pid) : killed caget problem"
	kill $pid
    fi

    sleep 1
done
