#!/usr/bin/env python3

import array
import epics
import logging
import os
import ROOT

import global_variables as g

logger = logging.getLogger('__main__').getChild(__name__)

# epics["K18MAG:D4:FLD"]*0.299792*4.

epics_dir = os.path.join(g.subdata_path, 'epics-recorder')

#______________________________________________________________________________
def caget(pvname):
  return epics.caget(pvname)

#______________________________________________________________________________
def info(run_number):
  info = dict()
  for r in os.listdir(epics_dir):
    r = os.path.join(epics_dir, r)
    if '.root' not in r:
      continue
    f = ROOT.TFile(r)
    t = f.Get('epics')
    d4 = []
    try:
      t.SetBranchStatus('*', 0)
      t.SetBranchStatus('K18MAG_D4_FLD', 1)
      t.SetBranchStatus('HDDAQ_RUN_NUMBER', 1)
      # br_run_number = array.array('d', [0, 0])
      # t.SetBranchAddress('HDDAQ_RUN_NUMBER', br_run_number)
      for i in range(t.GetEntriesFast()):
        t.GetEntry(i)
        logger.info(br_run_number)
        if t.HDDAQ_RUN_NUMBER != run_number:
          continue
        logger.info(f'{r}, {f}')
    except:
      pass
    #   t.K18MAG_D4_FLD
