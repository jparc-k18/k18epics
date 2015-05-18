#!/bin/bash

cd /home/sks/epics/epicsApp

#Accelerator & HD beam line status
screen -S HD -d -m bin/linux-x86_64/K18Monitor iocBoot/iocK18Monitor/hd.cmd

#K18 magnets & separator status
screen -S K18 -d -m bin/linux-x86_64/K18Monitor iocBoot/iocK18Monitor/k18.cmd

#HBJ status
screen -S HBJ -d -m bin/linux-x86_64/K18Monitor iocBoot/iocK18Monitor/hbj.cmd
