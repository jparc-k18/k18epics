#!../../bin/linux-x86_64/K18Monitor

## You may have to change K18Monitor to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/K18Monitor.dbd"
K18Monitor_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadDatabase("db/SHS.db")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX01")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX02")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX03")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX04")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX05")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX06")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX07")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCX08")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TSD09")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TSD10")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TSD11")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TSD12")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC13")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC14")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC15")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC16")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC17")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC18")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC19")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=TCC20")
dbLoadRecords("db/SHS.db", "det_name=CLG,data_type=CI_PS")

dbLoadRecords("db/SHS.db", "det_name=FLD,data_type=HALL")

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=sksHost"
