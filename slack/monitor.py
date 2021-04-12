#!/usr/bin/env python3

import epics
import logging
import os
import subprocess
import threading
import time

import slack_api

handler = logging.StreamHandler()
handler.setLevel(logging.INFO)
handler.setFormatter(logging.Formatter(
  "%(asctime)s [%(levelname)s] %(threadName)s : %(message)s"))
logger = logging.getLogger(__name__)
logger.addHandler(handler)
logger.setLevel(logging.INFO)

state = 'IDLE'
monitor_channel = {
  'GAS:TPC:DIFP': {'value': None, 'threshold': [0, 120]},
  'GAS:TPC:STAT': {'value': None, 'threshold': [36.9, 37.1]},
  'GAS:TPC:VALV': {'value': None, 'threshold': [0, 10]},
  'TPC:SPARK': {'value': None, 'threshold': [-1, 10]},
  'CAENHV:CRATE5:SLOT0:CH1:IMon': {'value': None, 'threshold': [-1.0, 325.0]},
  }
monitor_interval = 2 # [s]
alarm_interval = 300 # [s]

#______________________________________________________________________________
def alarm_function():
  logger.info('start')
  while state == 'RUNNING':
    time.sleep(1)
  logger.info('done')

#______________________________________________________________________________
# def caget(caname):
#   command = f'caget -t -w 10 {caname}'
#   proc = subprocess.run(command.split(),
#                         stdout=subprocess.PIPE,
#                         stderr=subprocess.PIPE)
#   ret = proc.stdout.decode().split()
#   if len(ret) > 1:
#     ret = ret[1]
#   else:
#     ret = ret[0]
#   return float(ret)

#______________________________________________________________________________
def monitor_function():
  global monitor_channel
  logger.info('start')
  prev_alarm = 0
  while state == 'RUNNING':
    for key, val in monitor_channel.items():
      val = epics.caget(key, count=2)
      if val is None:
        logger.warning(f'{key} is null')
        continue
      if not isinstance(val, float) and len(val) == 2:
        val = float(val[0])
      val = round(val, 1)
      th = monitor_channel[key]['threshold']
      monitor_channel[key]['value'] = val
      logger.info(f'{key} = {val} (threshold={th})')
      if val <= th[0] or th[1] <= val:
        now = time.time()
        message = f'{key} is {val}'
        logger.warning(message)
        if now - prev_alarm > alarm_interval:
          slack_api.send(message)
          prev_alarm = time.time()
    wait = 0
    while state == 'RUNNING' and wait != monitor_interval:
      time.sleep(1)
      wait += 1
  logger.info('done')

#______________________________________________________________________________
def run():
  ''' run monitoring '''
  global state
  try:
    state = 'RUNNING'
    monitor_thread = threading.Thread(target=monitor_function,
                                      name='monitor')
    monitor_thread.start()
    monitor_thread.join()
    # alarm_thread = threading.Thread(target=alarm_function,
    #                                 name='alarm')
    # alarm_thread.start()
    # alarm_thread.join()
  except KeyboardInterrupt:
    print()
    state = 'IDLE'

#______________________________________________________________________________
if __name__ == '__main__':
  run()
