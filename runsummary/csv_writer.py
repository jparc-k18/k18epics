#!/usr/bin/env python3

import csv
import logging
import os

import global_variables as g

logger = logging.getLogger('__main__').getChild(__name__)

#______________________________________________________________________________
def make_title():
  if os.path.isfile(g.run_summary):
    with open(g.run_summary, 'r') as f:
      lines = f.readlines()
      for line in lines:
        titles = line.strip().split(',')
        if titles == g.labels:
          return
  logger.info(f'make title : {g.labels}')
  with open(g.run_summary, 'w') as f:
    w = csv.writer(f, delimiter=',')
    w.writerow(g.labels)

#______________________________________________________________________________
def is_listed(run_number):
  if not os.path.isfile(g.run_summary):
    return False
  with open(g.run_summary, 'r') as f:
    reader = csv.reader(f)
    for row in reader:
      if g.labels[2] == row[2]:
        continue
      if run_number == int(row[2]):
        return True
  return False

#______________________________________________________________________________
def write(values):
  with open(g.run_summary, 'a') as f:
    w = csv.writer(f, delimiter=',')
    w.writerow(values)
