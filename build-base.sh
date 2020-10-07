#!/bin/sh

if [ -s base-7.0 ]; then
    cd base
    git pull --recurse-submodules
else
    git clone --recursive -b 7.0 https://git.launchpad.net/epics-base base-7.0
    ln -s base-7.0 base
    cd base
    target=configure/CONFIG_SITE_ENV
    cp $target configure/CONFIG_SITE_ENV.org
    sed -i -e 's/#EPICS_TZ = "JST-9"/EPICS_TZ = "JST-9"/g' $taget
    sed -i -e 's/EPICS_TZ = "CST6CDT,M3.2.0/2,M11.1.0/2"/#EPICS_TZ = "CST6CDT,M3.2.0/2,M11.1.0/2"/g' $taget
fi

make clean uninstall
make
