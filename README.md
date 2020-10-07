README
======

## build EPICS base-7.0

```sh
./build-base.sh
```

## install libraries

```sh
dnf install libcurl-devel
dnf install libxml2-devel
```

## make apps

```sh
make
```

## to .bashrc

```sh
export PATH=$HOME/k18epics/base/bin/linux-x86_64:$PATH
export LD_LIBRARY_PATH=$HOME/k18epics/base/lib/linux-x86_64:$LD_LIBRARY_PATH
export EPICS_CA_AUTO_ADDR_LIST=no
export EPICS_CA_ADDR_LIST=192.168.30.255
```
