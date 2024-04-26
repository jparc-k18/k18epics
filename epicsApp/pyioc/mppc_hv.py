#!/usr/bin/env python3

import datetime
import epics
import time
import os

os.makedirs("/home/sks/k18epics/epicsApp/pyioc/error_log", exist_ok=True)
error_log = "/home/sks/k18epics/epicsApp/pyioc/error_log/error_mppc_hv.txt"
mppchv_text = '/misc/subdata/MPPC_HV/mppcbias_summary.txt'

interval = 1

if __name__ == '__main__':
  print(datetime.datetime.now(), 'IOC start')
  while True:
    print(datetime.datetime.now())
    start = time.time()
    count = 0
    linenumber = 0
    try:
      with open(mppchv_text, 'r') as f:
        lines = f.readlines()
        for line in lines:
          columns = line.split()
          if len(columns) == 2:
            if columns[0] == "192.168.30.81":
              ch_header = 'MPPC:BFT:'
              if columns[1] =="ON":
                state = 1
              elif columns[1] == "OFF":
                state = 0
              else:
                state = -1
              epics.caput(ch_header+"BOARD", state, wait=1.0)
          if len(columns) == 8:
            if columns[1] =="ON":
              state = 1
            elif columns[1] == "OFF":
              state = 0
            else:
              state = -1
            hv_set = columns[4]
            hv_mon = columns[5]
            i_mon = columns[6]
            temp = columns[7]
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
            print(ch_name+":TEMP", temp, "\n")
    except FileNotFoundError as e:
      error_time = datetime.datetime.now()
      print("\033[31m", error_time)
      print("cannot open the file: mppc_bias_summary.txt\033[0m")
      with open(error_log, 'a') as errf:
        errf.write(str(error_time) + "\n")
        errf.write("Error Type: File not found\n")
        errf.write(f"Detail: {str(e)}\n")
        errf.write("-"*25 + "\n")
      continue
    except PermissionError as e:
      error_time = datetime.datetime.now()
      print("\033[31m", error_time)
      print("permision denied: mppc_bias_summary.txt\033[0m")
      with open(error_log, 'a') as errf:
        errf.write(str(error_time) + "\n")
        errf.write("Error Type: PernissionError\n")
        errf.write(f"Detail: {str(e)}\n")
        errf.write("-"*25 + "\n")
      continue
    except OSError as e:
      error_time = datetime.datetime.now()
      print("\033[31m", error_time)
      print("OS error occured: mppc_bias_summary.txt")
      print("Error code: ", e.errno)
      print("Error msg: ", e.strerror, "\033[0m")
      with open(error_log, 'a') as errf:
        errf.write(str(error_time) + "\n")
        errf.write("Error Type: OSError\n")
        errf.write(f"Detail: {str(e)}\n")
        errf.write("-"*25 + "\n")
      continue
    if interval - (time.time() - start) < 0:
      continue
    print("-"*35)
    time.sleep(interval - (time.time() - start))
