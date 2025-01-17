#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/SKSD4.db")
dbLoadRecords("db/TR700W.db")
#dbLoadTemplate ("db/K18Magnet.substitutions")
#dbLoadTemplate ("db/ALine.substitutions")
#dbLoadTemplate ("db/Sepa.substitutions")
#dbLoadTemplate ("db/Accel.substitutions")
#dbLoadTemplate ("db/PWO.substitutions")
#dbLoadTemplate ("db/HBJ.substitutions")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
