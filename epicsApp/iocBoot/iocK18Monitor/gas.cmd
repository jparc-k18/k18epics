#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadDatabase("db/GAS.db")
dbLoadRecords("db/GAS.db", "det_name=SDC2,data_type=DIFP")
dbLoadRecords("db/GAS.db", "det_name=SDC3,data_type=DIFP")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
