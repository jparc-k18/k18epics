#!/bin/sh

script_dir=$(dirname $(readlink -f $0))

cmd_list=(
    tr700.cmd
    separator.cmd
    sksd4.cmd
    k18line.cmd
    aline.cmd
    accel.cmd
    hadron.cmd
    zrrx40.cmd
    zrrx45.cmd
    gl840_bgo.cmd
    caenhv1.cmd
    caenhv2.cmd
    caenhv3.cmd
    caenhv4.cmd
    hddaq.cmd
)

for cmd in ${cmd_list[@]}
do
    screen -ls | grep $cmd >/dev/null && continue
    echo "start K18Monitor $cmd"
    screen -S $cmd -d -m sh -c "$script_dir/K18Monitor.sh $cmd"
done
