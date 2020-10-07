#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
## dbLoadTemplate ("db/HulScaler.substitutions", "host=192.168.10.74,port=9000")
dbLoadRecords("db/HulScaler.db", "host=192.168.10.74,port=9000")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
