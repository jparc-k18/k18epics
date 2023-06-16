#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate ("db/CAENV895-vme05.substitutions", "host=vme05")
dbLoadTemplate ("db/CAENV895-vme08.substitutions", "host=vme08")
dbLoadTemplate ("db/CAENV895-ctrl2.substitutions", "host=ctrl2")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
