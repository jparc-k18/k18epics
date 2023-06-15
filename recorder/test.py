#!/usr/bin/env python3

import datetime
import numpy
import time
import logging

import epics
import ROOT

logging.basicConfig(
  level=logging.DEBUG,
  format="%(asctime)s [%(levelname)s] %(message)s",
)
logger = logging.getLogger(__name__)

logging_interval = 30
newfile_interval = 14400 # 14400

val_dict = dict()
graph_dict = dict()

#______________________________________________________________________________
def update_connection(pvname=None, conn=None, **kws):
  state = "connect" if conn else "disconnect"
  logger.info(f'{state} {pvname}')

#______________________________________________________________________________
def update_value(pvname=None, value=None, timestamp=None, **kws):
  logger.debug(f'update value {pvname:20} {value}')

#______________________________________________________________________________
def run():
  pv_list = list()
  unix_time = numpy.zeros(1, dtype=float)
  with open('param/channel_list.txt') as f:
    for line in f.readlines():
      vals = line.split()
      if len(vals) == 0 or vals[0] == '#':
        continue
      pvname = vals[0]
      pv = epics.PV(pvname,
                    callback=update_value,
                    connection_callback=update_connection,
                    connection_timeout=0.1)
      pv_list.append(pv)
      logger.info(f'add {pvname}')
  last = time.time()
  while True:
    unix_time[0] = time.time()
    for pv in pv_list:
      logger.debug(f'{pv.pvname} {pv.value} {pv.timestamp}')
    if unix_time[0] - last > newfile_interval:
      last = unix_time[0]
    while time.time() - unix_time[0] < logging_interval:
      time.sleep(0.01)

#______________________________________________________________________________
if __name__ == '__main__':
  run()
