#!/usr/bin/env python3

import datetime
import epics
import time

spark_txt = '/misc/data3/E42SubData/spark/spark.txt'
interval = 60

if __name__ == '__main__':
  try:
    print(datetime.datetime.now(), 'IOC start')
    while True:
      start = time.time()
      count = 0
      with open(spark_txt, 'r') as f:
        lines = f.readlines()
        for line in lines:
          columns = line.split()
          if len(columns) == 2:
            spark_time = int(columns[0])
            spark_count = int(columns[1])
            if start - spark_time < interval:
              count += spark_count
              dt = datetime.datetime.fromtimestamp(spark_time)
              print(f'{dt}\t{spark_count}')
      epics.caput('TPC:SPARK', count, wait=1.0)
      time.sleep(interval - (time.time() - start))
  except:
    pass
