#!/bin/sh

set -e

script_dir=$(dirname $(readlink -f $0))
pyioc_dir=$script_dir/../pyioc

cmd_list=(
  tr700
  tr72nw_socket
  sksd4
  zrrx40 # slit
  gl840_aft
  caenhv1
  caenhv2
  caenhv3
  caenhv4
  # caenhv5
  # caenv895
  hddaq
  hultrigger
  mppcbias

  pmx18
  pressdc

  separator
  k18line
  aline
  bline
  accel
  hadron
  zrrx45
  gl840_bgo

  # gas
  tr72nw
  mqv9500
  gl840_tpc
  gl840_kurama
  gl840_dcth
  shs
  misc
  hul_scaler
)

pyioc_list=(
#    tpc-spark
  mppc_kikusui_hv
  mppc_hv
)

for cmd in ${cmd_list[@]}
do
  tmux ls | grep -x $cmd >/dev/null 2>&1 && continue
  tmux new-session -d -s ioc_$cmd \
       "$script_dir/K18Monitor.sh $cmd.cmd" && \
    echo "create session ioc_$cmd"
done

for pyioc in ${pyioc_list[@]}
do
  tmux ls | grep $pyioc >/dev/null 2>&1 && continue
#  tmux new-session -d -s pyioc-$pyioc \
  tmux new-session -d -s pyioc_$pyioc \
       "$pyioc_dir/$pyioc.py" && \
    echo "create session pyioc_$pyioc"
done
