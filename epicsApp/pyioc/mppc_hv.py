#!/usr/bin/env python3

import datetime
import epics
import time

mppchv_text = '/misc/subdata/MPPC_HV/mppcbias_summary.txt'
interval = 5

if __name__ == '__main__':
#  try:
    print(datetime.datetime.now(), 'IOC start')
    while True:
      start = time.time()
      count = 0
      linenumber = 0
      with open(mppchv_text, 'r') as f:
        lines = f.readlines()
        print('open file')
        for line in lines:
          columns = line.split()
          if len(columns) == 2:
            if columns[0] == "192.168.30.81":
              ch_header = 'MPPC:BFT:'
              print("ch_header"+str(ch_header))
#            elif columns[0] == "192.168.30.80":
 #             ch_header = 'MPPC:AFT:'
              if columns[1] =="ON":
                state = 1
              elif columns[1] == "OFF":
                state = 0
              else:
                state = -1
              epics.caput(ch_header+"BOARD", state, wait=1.0)
              print("state"+str(state))
          if len(columns) == 8:
            if columns[1] =="ON":
              state = 1
            elif columns[1] == "OFF":
              state = 0
            else:
              state = -1
            hv_set = columns[4]
            print("hv_set"+str(hv_set))
            hv_mon = columns[5]
            print("hv_mon"+str(hv_mon))
            i_mon = columns[6]
            print("i_mon"+str(i_mon))
            temp = columns[7]
            print("temp"+str(temp))
            ch_name = ch_header + columns[0]
            epics.caput(ch_name+':STAT', state, wait=1.0)
            epics.caput(ch_name+':VSET', hv_set, wait=1.0)
            epics.caput(ch_name+':VMON', hv_mon, wait=1.0)
            epics.caput(ch_name+':IMON', i_mon, wait=1.0)
            epics.caput(ch_name+':TEMP', temp, wait=1.0)
            print(ch_name+":STAT", state)
            print(ch_name+":VSET", hv_set)
            print(ch_name+":VMON", hv_mon)
            print(ch_name+":IMON", i_mon)
            print(ch_name+":TEMP", temp)
        time.sleep(interval - (time.time() - start))
 # except:
  #  pass
