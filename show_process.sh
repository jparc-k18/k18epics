#!/bin/sh

process=(
    K18Monitor
    Epics
)

echo "Following processes are running ..."

for p in ${process[@]}
do
    pgrep -lf -u sks $p | awk '$2!="SCREEN" && $2!="/bin/sh" && $2!="sh" { print " " $0 }'
done

echo