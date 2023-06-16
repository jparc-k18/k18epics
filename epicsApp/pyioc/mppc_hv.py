#!/usr/bin/env python3

import datetime
import epics
import time

mppchv_text = '/misc/subdata/MPPC_HV/mppcbias_summary.txt'
interval = 1

if __name__ == '__main__':
  try:
    print(datetime.datetime.now(), 'IOC start')
    while True:
      print('debug')
      start = time.time()
      count = 0
      linenumber = 0
      with open(mppchv_text, 'r') as f:
        lines = f.readlines()
        print('open file')
        for line in lines:
          columns = line.split()
          if len(columns) == 8:
            print('read line')
            linenumber = linenumber+1;
            if linenumber == 1 or linenumber == 2:
              state  = columns[1]
              hv_set = columns[4]
              hv_mon = columns[5]
              i_mon = columns[6]
              temp = columns[7]
              ch_name = 'MPPC:BFT:' + columns[0]
              epics.caput(ch_name+':VSET', hv_set, wait=1.0)
              epics.caput(ch_name+':VMON', hv_mon, wait=1.0)
              epics.caput(ch_name+':IMON', i_mon, wait=1.0)
              epics.caput(ch_name+':TEMP', temp, wait=1.0)
              print(ch_name+":VSET", hv_set)
              print(ch_name+":VMON", hv_mon)
              print(ch_name+":IMON", i_mon)
              print(ch_name+":TEMP", temp)
            if linenumber > 8 and linenumber < 15:
              state  = columns[1]
              hv_set = columns[4]
              hv_mon = columns[5]
              i_mon = columns[6]
              temp = columns[7]
              ch_name = 'MPPC:AFT:' + columns[0]
              epics.caput(ch_name+':VSET', hv_set, wait=1.0)
              epics.caput(ch_name+':VMON', hv_mon, wait=1.0)
              epics.caput(ch_name+':IMON', i_mon, wait=1.0)
              epics.caput(ch_name+':TEMP', temp, wait=1.0)
              print(ch_name+":VSET", hv_set)
              print(ch_name+":VMON", hv_mon)
              print(ch_name+":IMON", i_mon)
              print(ch_name+":TEMP", temp)
        time.sleep(interval - (time.time() - start))
  except:
    pass
