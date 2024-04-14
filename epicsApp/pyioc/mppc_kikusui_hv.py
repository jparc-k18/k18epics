#!/usr/bin/env python3

import datetime
import epics
import time


interval = 5
N=0
if __name__ == '__main__':
#  try:
  print(datetime.datetime.now(), 'IOC start')
  while True:
    for i in range(1,5):
      start = time.time()
      count = 0
      linenumber = 0
      mppchv_text = ""
      mppchv_text = f"/misc/subdata/AFT_MPPCHV/MPPC_HV{i}/mppcbias_summary.txt"
      ch_header = f"MPPC:AFT:CH{i}"
      print(mppchv_text) # debug
      with open(mppchv_text, 'r') as f:
        lines = f.readlines()
        print('open file')
        for line in lines:
          print(line) # debug
          columns = line.split()
          print(columns[0])
          if columns[0] =="ON":
            state = 1
          elif columns[0] == "OFF":
            state = 0
          else:
            state = -1
          print(columns[0])
          hv_set = columns[1]
          print(columns[1])
          hv_mon = columns[2]
          i_mon = columns[3]
          for j in range(0,4):
            print(columns[j])
          epics.caput(ch_header+':STAT', state, wait=1.0)
          epics.caput(ch_header+':VSET', hv_set, wait=1.0)
          epics.caput(ch_header+':VMON', hv_mon, wait=1.0)
          epics.caput(ch_header+':IMON', i_mon, wait=1.0)
          print(ch_header+":STAT", state)
          print(ch_header+":VSET", hv_set)
          print(ch_header+":VMON", hv_mon)
          print(ch_header+":IMON", i_mon)
          N+=1
          print(N)
    time.sleep(interval - (time.time() - start))
 # except:
  #  pass
