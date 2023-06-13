#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/PMX18.db", "ip=192.168.30.45,id_num=3")
dbLoadRecords("db/PMX18.db", "ip=192.168.30.46,id_num=4")
dbLoadRecords("db/PMX18.db", "ip=192.168.30.47,id_num=5")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
