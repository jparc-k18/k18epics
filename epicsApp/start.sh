#!/bin/bash

cd /home/sks/epics/epicsApp

# TR700W
screen -S TR700 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor tr700.cmd'

# Separator
screen -S Separator -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor separator.cmd'

# SKS & D4
screen -S SKSD4 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor sksd4.cmd'

# K1.8 beamline magnet
screen -S K18line -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor k18line.cmd'

# Aline magnet
screen -S Aline -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor aline.cmd'

# Accelerator status
screen -S Accel -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor accel.cmd'

# Hadron beam status
screen -S Hadron -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor hadron.cmd'

# XRRX40
screen -S ZRRX40 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor zrrx40.cmd'
screen -S ZRRX45 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor zrrx45.cmd'

# CAEN HV Crate
screen -S CAENHV1 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor caenhv1.cmd'
screen -S CAENHV2 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor caenhv2.cmd'
screen -S CAENHV3 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor caenhv3.cmd'
screen -S CAENHV4 -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor caenhv4.cmd'

screen -S HDDAQ -d -m sh -c 'cd iocBoot/iocK18Monitor; ../../bin/linux-x86_64/K18Monitor hddaq.cmd'