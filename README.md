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

## install CAENHVLIB

```sh
git clone sks@www-online.kek.jp:8022/~/public_html/git/monitor-tools.git
cd monitor-tools/CAENHV
tar zxfv CAENHVWrapper-5.82.tar.gz
cd CAENHVWrapper-5.82
su
./install.sh
```

## make apps

It is noted that EPICS_BASE is defined `/home/sks/k18epics/base`
in epicsApp/configure/RELEASE.

```sh
make
```

## set environment variables

```sh
export PATH=$HOME/k18epics/base/bin/linux-x86_64:$PATH
export LD_LIBRARY_PATH=$HOME/k18epics/base/lib/linux-x86_64:$LD_LIBRARY_PATH
export EPICS_CA_AUTO_ADDR_LIST=no
export EPICS_CA_ADDR_LIST=192.168.30.255
```
