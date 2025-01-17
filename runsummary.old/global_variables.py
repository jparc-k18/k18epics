#!/usr/bin/env python3

import os

labels = [
  'StartTime',
  'StopTime',
  'RunNumber',
  'RunTitle',
  'EventNumber',
  'Recorder',
  'Bytes',
  'TrigParam',
  'BeamMom',
  'Beam/Spill',
  'K-Beam/Spill',
  'Pi-Beam/Spill',
  'Beam',
  'K-Beam',
  'Pi-Beam',
  'SpillNumber',
  'DAQ-Eff',
  'BeamxDAQEff',
  'DAQ-Duty',
  'MR-Power',
  'SX-Duty',
  'K1.8D1',
  'K1.8D4',
  'K1.8D4s',
  'S2SQ1',
  'S2SQ2',
  'S2SD1',
  'BLineSM1',
  'ESS1_POS',
  'ESS1_NEG',
  'ESS2_POS',
  'ESS2_NEG',
  'TOFSelector',
  'TrigA-ON',
  'TrigA-PS',
  'TrigA-Gate',
  'TrigA',
  'TrigB-ON',
  'TrigB-PS',
  'TrigB-Gate',
  'TrigB',
  'TrigC-ON',
  'TrigC-PS',
  'TrigC-Gate',
  'TrigC',
  'TrigD-ON',
  'TrigD-PS',
  'TrigD-Gate',
  'TrigD',
  'TrigE-ON',
  'TrigE-PS',
  'TrigE-Gate',
  'TrigE',
  'TrigF-ON',
  'TrigF-PS',
  'TrigF-Gate',
  'TrigF',
]

data_path = '/misc/hdd1/e70_2024may'
subdata_path = '/misc/subdata'
# run_summary = 'tmp.csv'
run_summary = os.path.join(subdata_path, 'E70RunSummary2024May-online.csv')
