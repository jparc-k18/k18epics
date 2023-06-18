#!/usr/bin/env python3

import datetime
import numpy
import time
import logging
import os

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
def draw(key, option='AL', color=ROOT.kBlack):
  for k, g in graph_dict.items():
    if key == k:
      logger.info(f'draw {key}({g.GetName()})')
      if g.GetN() == 0:
        g.SetPoint(0, 0, 0)
      g.SetLineColor(color)
      g.Draw(option)

#______________________________________________________________________________
def make_canvas():
  global canvas_dict
  c = ROOT.TCanvas('ACC', 'ACC')
  canvas_dict['ACC'] = c
  c.Divide(4, 3)
  c.cd( 1); draw('HDSYS:RUN_NO')
  c.cd( 2); draw("HDSYS:SHOT_NO");
  c.cd( 3); draw("HDSYS:MR_CYCLE");
  c.cd( 4); draw("MRMON:DCCT_073_1:VAL:MRPWR");
  c.cd( 5); draw("HDMON:MR_P3:INTENSITY");
  c.cd( 6); draw("HDMON:SYIM:POWER");
  c.cd( 7); draw("HDMON:SYIM:INTENSITY");
  c.cd( 8); draw("HDMON:BDMPIM:INTENSITY");
  c.cd( 9); draw("MRSLW:SXOPR_D2:VAL:DUTY");
  c.cd(10); draw("MRSLW:SXOPR_D2:VAL:ExtEffi");
  c.cd(11); draw("MRSLW:SXOPR_D2:VAL:SpLen");
  c = ROOT.TCanvas('T1', 'T1')
  canvas_dict['T1'] = c
  c.Divide(2, 2)
  c.cd( 1); draw("HDRGPM:T1IN:MEAN_X");
  c.cd( 2); draw("HDRGPM:T1IN:MEAN_Y");
  c.cd( 3); draw("HDRGPM:T1IN:SIGMA_X");
  c.cd( 4); draw("HDRGPM:T1IN:SIGMA_Y");
  c = ROOT.TCanvas('AIR', 'AIR')
  canvas_dict['AIR'] = c
  c.Divide(4, 3)
  c.cd( 1); draw('AIR:BFT_TENT:TEMP');
  c.cd( 2); draw('AIR:BFT_TENT:HUMI');
  c.cd( 3); draw('AIR:TRG_RACK:TEMP');
  c.cd( 4); draw('AIR:TRG_RACK:HUMI');
  c.cd( 5); draw('AIR:WCU:TEMP');
  c.cd( 6); draw('AIR:WCU:HUMI');
  c.cd( 7); draw('AIR:WCD:TEMP');
  c.cd( 8); draw('AIR:WCD:HUMI');
  c.cd( 9); draw('AIR:BH2_RACK:TEMP');
  c.cd(10); draw('AIR:BH2_RACK:HUMI');
  c = ROOT.TCanvas('ESS', 'ESS')
  canvas_dict['ESS'] = c
  c.Divide(5, 2)
  c.cd( 1); draw("HDESS:K18_ESS1:POS_VMON");
  c.cd( 2); draw("HDESS:K18_ESS1:NEG_VMON");
  c.cd( 3); draw("HDESS:K18_ESS1:POS_IMON");
  c.cd( 4); draw("HDESS:K18_ESS1:NEG_IMON");
  c.cd( 5); draw("HDESS:K18_ESS1:CCG_PMON");
  c.cd( 6); draw("HDESS:K18_ESS2:POS_VMON");
  c.cd( 7); draw("HDESS:K18_ESS2:NEG_VMON");
  c.cd( 8); draw("HDESS:K18_ESS2:POS_IMON");
  c.cd( 9); draw("HDESS:K18_ESS2:NEG_IMON");
  c.cd(10); draw("HDESS:K18_ESS2:CCG_PMON");
  c = ROOT.TCanvas('K18MAG', 'K18MAG')
  canvas_dict['K18MAG'] = c
  c.Divide(1, 2)
  c.cd(1); draw("K18MAG:D4:FLD");
  c.cd(2); draw("K18MAG:S2SD1:FLD");
  c = ROOT.TCanvas('GAS', 'GAS')
  canvas_dict['GAS'] = c
  c.Divide(2, 2)
  c.cd(1); draw("GAS:SDC3:DIFP");
  c.cd(2); draw("GAS:SDC4:DIFP");
  c.cd(3); draw("GAS:SDC5:DIFP");
  c = ROOT.TCanvas('AFT', 'AFT')
  canvas_dict['AFT'] = c
  draw("AFT:X-U1:TEMP", color=ROOT.kRed+1);
  draw("AFT:X-U2:TEMP", color=ROOT.kBlue+1, option='L');
  draw("AFT:Y-U:TEMP", color=ROOT.kGreen+1, option='L');
  draw("AFT:Y-D:TEMP", color=ROOT.kMagenta+1, option='L');
  draw("AFT:X-D1:TEMP", color=ROOT.kOrange+1, option='L');
  draw("AFT:X-D2:TEMP", color=ROOT.kCyan+1, option='L');
  draw("AFT:AIR:TEMP", color=ROOT.kYellow+1, option='L');
  c = ROOT.TCanvas('MPPC_BFT', 'MPPC_BFT')
  canvas_dict['MPPC_BFT'] = c
  c.Divide(2, 2)
  c.cd(1); draw("MPPC:BFT:CH0:VMON");
  c.cd(2); draw("MPPC:BFT:CH0:IMON");
  c.cd(3); draw("MPPC:BFT:CH1:VMON");
  c.cd(4); draw("MPPC:BFT:CH1:IMON");
  c = ROOT.TCanvas('MPPC_AFT', 'MPPC_AFT')
  canvas_dict['MPPC_AFT'] = c
  c.Divide(4, 3)
  c.cd( 1); draw("MPPC:AFT:CH0:VMON");
  c.cd( 2); draw("MPPC:AFT:CH0:IMON");
  c.cd( 3); draw("MPPC:AFT:CH1:VMON");
  c.cd( 4); draw("MPPC:AFT:CH1:IMON");
  c.cd( 5); draw("MPPC:AFT:CH2:VMON");
  c.cd( 6); draw("MPPC:AFT:CH2:IMON");
  c.cd( 7); draw("MPPC:AFT:CH3:VMON");
  c.cd( 8); draw("MPPC:AFT:CH3:IMON");
  c.cd( 9); draw("MPPC:AFT:CH4:VMON");
  c.cd(10); draw("MPPC:AFT:CH4:IMON");
  c.cd(11); draw("MPPC:AFT:CH5:VMON");
  c.cd(12); draw("MPPC:AFT:CH5:IMON");
  c = ROOT.TCanvas('PPS', 'PPS')
  canvas_dict['PPS'] = c
  c.Divide(4, 2)
  c.cd( 1); draw("HDPPS:K18:CNTR1_1S");
  c.cd( 2); draw("HDPPS:K18:CNTR1_INTG_HR");
  c.cd( 3); draw("HDPPS:K18BR:CNTR1_1S");
  c.cd( 4); draw("HDPPS:K18BR:CNTR1_INTG_HR");
  c.cd( 5); draw("RADHD:ORG0201G:VAL:LEVEL");
  c.cd( 6); draw("RADHD:ORG0201N:VAL:LEVEL");
  c.cd( 7); draw("RADHD:ORG0202G:VAL:LEVEL");
  c.cd( 8); draw("RADHD:ORG0202N:VAL:LEVEL");
  c = ROOT.TCanvas('SLIT', 'SLIT')
  canvas_dict['SLIT'] = c
  c.Divide(4, 3)
  c.cd( 1); draw("SLIT:IFH:LEFT");
  c.cd( 2); draw("SLIT:IFH:RIGHT");
  c.cd( 3); draw("SLIT:IFV:LOWER");
  c.cd( 4); draw("SLIT:IFV:UPPER");
  c.cd( 5); draw("SLIT:MOM:LEFT");
  c.cd( 6); draw("SLIT:MOM:RIGHT");
  c.cd( 7);
  c.cd( 8);
  c.cd( 9); draw("SLIT:MASS1:LOWER");
  c.cd(10); draw("SLIT:MASS1:UPPER");
  c.cd(11); draw("SLIT:MASS2:LOWER");
  c.cd(12); draw("SLIT:MASS2:UPPER");
  c = ROOT.TCanvas('HDDAQ', 'HDDAQ')
  canvas_dict['HDDAQ'] = c
  c.Divide(2, 2)
  c.cd( 1); draw("HDDAQ:RUN_NUMBER");
  c.cd( 2); draw("HDDAQ:EVENT_NUMBER");
  c.cd( 3); draw("HDDAQ:UNIX_TIME");
  c.cd( 4); draw("HDDAQ:SPILL_TIME");


#______________________________________________________________________________
def update_canvases():
  pass
  # global graph_dict
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
  if pvname is None or value is None or timestamp is None:
    return
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
    # ay.SetRangeUser(ymin - margin, ymax + margin)
    if pvname == 'AFT:X-U1:TEMP':
      g.SetTitle('AFT TEMP')
      ay = g.GetYaxis()
      ay.SetRangeUser(10, 40)

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
      branch_name = pvname.replace('::', '_').replace(':', '_')
      g = ROOT.TGraph()
      graph_dict[pvname] = g
      g.SetNameTitle(branch_name, f'{branch_name}; ; {pv.units}')
      g.SetLineWidth(2)
      # g.SetLineColor(ROOT.kBlack)
      # g.SetMarkerStyle(8)
      dirname = pvname.split(':')[0]
      server.Register(f'/{dirname}/', g)
      logger.info(f'add {pvname}')
  make_canvas()
  last = time.time()
  try:
    while True:
    # while not ROOT.gSystem.ProcessEvents():
      update_canvases()
      time.sleep(10)
      # for pv in pv_list:
      #   update_value(pv.pvname, pv.value, pv.timestamp)
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
  os.environ['EPICS_CA_ADDR_LIST'] = '192.168.30.255 192.153.109.232'
  ROOT.gROOT.SetBatch(True)
  run()
