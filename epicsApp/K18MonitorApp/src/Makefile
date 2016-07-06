TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = K18Monitor
# K18Monitor.dbd will be created and installed
DBD += K18Monitor.dbd

# K18Monitor.dbd will be made up from these files:
K18Monitor_DBD += base.dbd

# Include dbd files from all support applications:
K18Monitor_DBD += devSKSNMR.dbd
K18Monitor_DBD += devD4Probe.dbd
K18Monitor_DBD += devTR700W.dbd
K18Monitor_DBD += devHTTP_k18magnet.dbd
K18Monitor_DBD += devHTTP_aline.dbd
K18Monitor_DBD += devHTTP_sepa.dbd
K18Monitor_DBD += devHTTP_accel.dbd
K18Monitor_DBD += devHTTP_hduser.dbd
K18Monitor_DBD += devAiHTTP.dbd
K18Monitor_DBD += devCAENHV.dbd
K18Monitor_DBD += devZRRX40.dbd
K18Monitor_DBD += devAiZRRX40.dbd
K18Monitor_DBD += devZRRX45.dbd

#K18Monitor_DBD += devHBJ.dbd
#K18Monitor_DBD += devAiHBJ.dbd

# Add all the support libraries needed by this IOC
#K18Monitor_LIBS += xxx

# K18Monitor_registerRecordDeviceDriver.cpp derives from K18Monitor.dbd
K18Monitor_SRCS += K18Monitor_registerRecordDeviceDriver.cpp
K18Monitor_SRCS += devSKSNMR.cc
K18Monitor_SRCS += devD4Probe.cc
K18Monitor_SRCS += devTR700W.cc
K18Monitor_SRCS += devHTTP_k18magnet.cc
K18Monitor_SRCS += devHTTP_aline.cc
K18Monitor_SRCS += devHTTP_sepa.cc
K18Monitor_SRCS += devHTTP_accel.cc
K18Monitor_SRCS += devHTTP_hduser.cc
K18Monitor_SRCS += devAiHTTP.cc
K18Monitor_SRCS += devCAENHV.cc
K18Monitor_SRCS += devAiZRRX40.cc
K18Monitor_SRCS += devZRRX40.cc
K18Monitor_SRCS += devZRRX45.cc

#K18Monitor_SRCS += devHBJ.cc
#K18Monitor_SRCS += devAiHBJ.cc

# Build the main IOC entry point on workstation OSs.
K18Monitor_SRCS_DEFAULT += K18MonitorMain.cpp
K18Monitor_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#K18Monitor_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
K18Monitor_LIBS += $(EPICS_BASE_IOC_LIBS)

# User Link
USR_LDFLAGS += -lcaenhvwrapper

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

