#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadTemplate("db/MppcBias.substitutions")

dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=BOARD")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH0:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH0:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH0:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH0:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH0:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH1:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH1:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH1:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH1:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=BFT,data_type=CH1:TEMP")

dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=BOARD")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH0:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH0:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH0:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH0:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH0:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH1:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH1:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH1:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH1:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH1:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH2:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH2:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH2:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH2:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH2:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH3:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH3:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH3:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH3:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH3:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH4:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH4:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH4:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH4:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH4:TEMP")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH5:STAT")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH5:VSET")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH5:VMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH5:IMON")
dbLoadRecords("db/MppcBias.db", "det_name=AFT,data_type=CH5:TEMP")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
