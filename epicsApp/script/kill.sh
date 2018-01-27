#!/bin/sh

pkill -f "/bin/.*K18Monitor.sh.*"
if [ $? = 0 ]; then
    echo "# K18Monitor.sh : killed"
else
    echo "# K18Monitor.sh : no process"
fi

killall -q K18Monitor
if [ $? = 0 ]; then
    echo "# K18Monitor    : killed"
else
    echo "# K18Monitor    : no process"
fi
