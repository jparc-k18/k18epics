#!/usr/bin/env python3

import logging
import logging.config
import os
import subprocess
import time
import yaml

import csv_writer
import epics_getter
import hddaq_getter

logger = logging.getLogger(__name__)

#______________________________________________________________________________
def run(offset=1):
  try:
    while True:
      comment_info = hddaq_getter.comment_info()
      recorder_info = hddaq_getter.recorder_info()
      for n in range(offset, offset + 10000):
        if csv_writer.is_listed(n):
          continue
        if n not in comment_info:
          continue
        c = comment_info[n]
        values = list()
        if 'StopTime' in c:
          values.append(c['StartTime'])
          values.append(c['StopTime'])
          values.append(n)
          values.append(c['Title'])
          if n in recorder_info:
            r = recorder_info[n]
            values.append(r['EventNumber'])
            values.append('ON')
            values.append(r['Bytes'])
          else:
            values.append('')
            values.append('OFF')
            values.append('')
          t = hddaq_getter.trigger(n)
          values.append(t[0])
          e = epics_getter.info(n)
          values.extend(t[1:])
          csv_writer.write(values)
        logger.info(n)
      time.sleep(2)
      #     # values.append(f'{year}/{date} {hms}')
      #     # values.append()
  except KeyboardInterrupt:
    pass

if __name__ == '__main__':
  top_dir = os.path.dirname(os.path.abspath(__file__))
  log_conf = os.path.join(top_dir, 'logging_config.yml')
  with open(log_conf, 'r') as f:
    logging.config.dictConfig(yaml.safe_load(f))
  csv_writer.make_title()
  run(70001)
  # epics_getter.get(70183, 'K18MAG:D4:FLD')
  # hddaq_getter.event_number(70183)
  # hddaq_getter.comment_info()
  # #hddaq_getter.recorder_info()
  # logger.info(hddaq_getter.trigger())
