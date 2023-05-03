#!/usr/bin/env python3

import datetime
import numpy
import time
import logging

import epics
import ROOT

logging.basicConfig(
  # level=logging.DEBUG,
  level=logging.INFO,
  format="%(asctime)s [%(levelname)s] %(message)s",
)
logger = logging.getLogger(__name__)

port = 9090

server = None
graph_dict = dict()
canvas_dict = dict()
max_time = 60*60*24*7 # [s]

#______________________________________________________________________________
def make_canvas(canvas):
  name = canvas.GetName()
  if name == 'AIR':
    canvas.Divide(2, 2)
  elif name == 'K18MAG':
    canvas.Divide(2, 2)
  elif name == 'HDDAQ':
    canvas.Divide(2, 2)
  i = 1
  for key, g in graph_dict.items():
    if name in key:
      canvas.cd(i)
      g.Draw('AL')
      i = i+1

#______________________________________________________________________________
def update_canvases():
  global graph_dict
  # for g in graph_dict.values():
  #   g.GetYaxis().UnZoom()
  # global canvas_dict
  # for c in canvas_dict.values():
  #   c.Modified()
  #   c.Update()

#______________________________________________________________________________
def update_connection(pvname=None, conn=None, **kws):
  state = "connect" if conn else "disconnect"
  logger.info(f'{state} {pvname}')

#______________________________________________________________________________
def update_value(pvname=None, value=None, timestamp=None, **kws):
  logger.debug(f'update value {pvname:20} {value}')
  if pvname in graph_dict:
    g = graph_dict[pvname]
    g.AddPoint(timestamp, value)
    x = numpy.array(g.GetX())
    y = numpy.array(g.GetY())
    g.Set(0)
    # ymin = None
    # ymax = None
    for i in range(len(x)):
      if time.time() - x[i] < max_time:
        g.SetPoint(g.GetN(), x[i], y[i])
        # if ymin is None or y[i] < ymin:
        #   ymin = y[i]
        # if ymax is None or ymax < y[i]:
        #   ymax = y[i]
    ax = g.GetXaxis()
    ax.SetTimeDisplay(1)
    ax.SetTimeFormat('#splitline{%Y/%m/%d}{  %H:%M:%S}')
    ax.SetTimeOffset(0, 'jpn');
    # ax.SetLabelOffset(0.04)
    ax.SetNdivisions(-506)
    # margin = (ymax - ymin)*0.05
    # ay = g.GetYaxis()
    # ay.SetRangeUser(ymin - margin, ymax + margin)

#______________________________________________________________________________
def run():
  global server
  global canvas_dict
  server = ROOT.THttpServer(f'http:{port}')
  server.Restrict('/', 'allow=all')
  server.SetReadOnly(True)
  server.SetJSROOT('https://root.cern.ch/js/latest/')
  server.SetTopName('K1.8 EPICS')
  pv_list = list()
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
      branch_name = pvname.replace(':', '_')
      g = ROOT.TGraph()
      graph_dict[pvname] = g
      g.SetNameTitle(branch_name, f'{branch_name}; ; {pv.units}')
      g.SetLineWidth(2)
      # g.SetLineColor(ROOT.kBlack)
      # g.SetMarkerStyle(8)
      dirname = pvname.split(':')[0]
      server.Register(f'/{dirname}/', g)
      if dirname not in canvas_dict:
        c = ROOT.TCanvas(dirname, dirname)
        canvas_dict[dirname] = c
      logger.info(f'add {pvname}')
  for c in canvas_dict.values():
    make_canvas(c)
  last = time.time()
  try:
    while not ROOT.gSystem.ProcessEvents():
      update_canvases()
      time.sleep(0.001)
      # now = time.time()
      # if now - last > newfile_interval:
      #   last = now
      # while time.time() - now < logging_interval:
      #   time.sleep(0.01)
  except KeyboardInterrupt:
    logger.info('detect signal')
  ROOT.gSystem.Exit(0)

#______________________________________________________________________________
if __name__ == '__main__':
  ROOT.gROOT.SetBatch(True)
  run()
