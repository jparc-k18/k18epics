#!/usr/bin/env python3

import datetime
import epics
import time

interval = 1
N=0
error_log="error_log/error_mppc_kikusui_hv.txt"

if __name__ == '__main__':
  print(datetime.datetime.now(), 'IOC start')
  while True:
    print(datetime.datetime.now())
    for i in range(1,5):
      start = time.time()
      count = 0
      linenumber = 0
      mppchv_text = ""
      mppchv_text = f"/misc/subdata/AFT_MPPCHV/MPPC_HV{i}/mppcbias_summary.txt"
      ch_header = f"MPPC:AFT:CH{i}"
      # print(mppchv_text) # debug
      try:
        with open(mppchv_text, 'r') as f:
          lines = f.readlines()
          for line in lines:
            columns = line.split()
            if columns[0] =="ON":
              state = 1
            elif columns[0] == "OFF":
              state = 0
            else:
              state = -1
            hv_set = columns[1]
            hv_mon = columns[2]
            i_mon = columns[3]
            epics.caput(ch_header+':STAT', state, wait=1.0)
            epics.caput(ch_header+':VSET', hv_set, wait=1.0)
            epics.caput(ch_header+':VMON', hv_mon, wait=1.0)
            epics.caput(ch_header+':IMON', i_mon, wait=1.0)
            print(ch_header+":STAT", state)
            print(ch_header+":VSET", hv_set)
            print(ch_header+":VMON", hv_mon)
            print(ch_header+":IMON", i_mon, "\n")
      except FileNotFoundError as e:
        error_time = datetime.datetime.now()
        print("\033[31m", error_time)
        print("cannot open the file: MPPC_HV" + str(i) + "/mppc_bias_summary.txt\033[0m")
        with open(error_log, 'a') as errf:
          errf.write(str(error_time) + "\n")
          errf.write("Error Type: File not found\n")
          errf.write(f"Detail: {str(e)}\n")
          errf.write("-"*25 + "\n")
        continue
      except PermissionError as e:
        error_time = datetime.datetime.now()
        print("\033[31m", error_time)
        print("permision denied: MPPC_HV" + str(i) + "/mppc_bias_summary.txt\033[0m")
        with open(error_log, 'a') as errf:
          errf.write(str(error_time) + "\n")
          errf.write("Error Type: PernissionError\n")
          errf.write(f"Detail: {str(e)}\n")
          errf.write("-"*25 + "\n")
        continue
      except OSError as e:
        error_time = datetime.datetime.now()
        print("\033[31m", error_time)
        print("OS error occured: MPPC_HV" + str(i) + "/mppc_bias_summary.txt")
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
