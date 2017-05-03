/*
 *  This file is part of the caQtDM Framework, developed at the Paul Scherrer Institut,
 *  Villigen, Switzerland
 *
 *  The caQtDM Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The caQtDM Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the caQtDM Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2010 - 2014
 *
 *  Author:
 *    Anton Mezger
 *  Contact details:
 *    anton.mezger@psi.ch
 */

#if defined(_MSC_VER)
#define NOMINMAX
#include <windows.h>
#include <float.h>
#define isnan _isnan
#define QWT_DLL
#if (_MSC_VER == 1600)
  #define INFINITY (DBL_MAX+DBL_MAX)
  #define NAN (INFINITY-INFINITY)
#endif
#endif

#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include "castripplot.h"
#include <QList>

// increase the array size given by the canvas width to be sure that the whole range is covered
#define MAXIMUMSIZE 5000
#define SOMEMORE 500

class TimeScaleDraw: public QwtScaleDraw
{
public:

    TimeScaleDraw(const QTime &base): baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        // while addsecs has a problem after some 3/4 of a month in seconds, we normalize the seconds to 24 * 3600 seconds
        double day = 24 * 3600;
        double normalized = v - day * floor(v/day);

        QTime upTime = baseTime.addSecs((int) normalized);
        //printf("label = addseconds=%d start plottime=%s labeltime=%s\n", seconds, baseTime.toString().toAscii().constData(),  upTime.toString().toAscii().constData());
        return upTime.toString();
    }

private:
    QTime baseTime;

};

caStripPlot::~caStripPlot() {

    emit timerThreadStop();
    timerThread->wait(200);
    timerThread->deleteLater();
}

caStripPlot::caStripPlot(QWidget *parent): QwtPlot(parent)
{
    initCurves = true;
    timerID = false;
    thisXaxisType = TimeScale;
    thisYaxisScaling = fixedScale;
    HISTORY = 60;
    thisUnits = Second;
    thisPeriod = 60;
    NumberOfCurves = MAXCURVES;
    onInit = true;
    timeInterval = 1.0;
    setAutoReplot(false);
    setAutoFillBackground(true);
    RestartPlot1 = true;
    RestartPlot2 = false;

    // define a grid
    plotGrid = new QwtPlotGrid();
    plotGrid->attach(this);

    plotLayout()->setAlignCanvasToScales(true);

    // define our axis
    if(thisXaxisType == TimeScale) {
        setAxisScale(QwtPlot::xBottom, 0, HISTORY);
    } else {
        setAxisScale(QwtPlot::xBottom, -HISTORY, 0);
    }

    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
    setAxisScale(QwtPlot::yLeft, 0, 1000);

    if(thisXaxisType == TimeScale) {
        // due to time scale we need some distance
        scaleWidget = axisWidget(QwtPlot::xBottom);
        const int fmh = QFontMetrics(scaleWidget->font()).height();
        scaleWidget->setMinBorderDist(fmh * 2, fmh * 2);
        // define time axis
        QTime timeNow= QTime::currentTime();
        setAxisScaleDraw ( QwtPlot::xBottom, new TimeScaleDraw ( timeNow ) );
    }

    // define our curves
    for(int i=0; i< MAXCURVES; i++) {
        curve[i] = new QwtPlotCurve();
        errorcurve[i] = new QwtPlotIntervalCurveNaN();
        fillcurve[i] = new QwtPlotCurveNaN();
        curve[i]->setZ(i);
        fillcurve[i]->setZ(i);
        errorcurve[i]->setZ(i+10);

        curve[i]->attach(this);
        errorcurve[i]->attach(this);
        fillcurve[i]->attach(this);
        showCurve(i, false);

        thisYaxisLimitsMax[i] = 100;
        thisYaxisLimitsMin[i] = 0;
    }

    // default colors and styles
    setTitlePlot("");
    setTitleX("");
    setTitleY("");
    setBackground(Qt::black);
    setForeground(QColor(133, 190, 232));
    setScaleColor(Qt::black);
    setGrid(true);
    setGridColor(Qt::gray);
    for(int i=0; i< MAXCURVES; i++) setStyle(Lines, i);
    setColor(Qt::white, 0);
    setColor(Qt::red, 1);
    setColor(Qt::yellow, 2);
    setColor(Qt::cyan, 3);
    setColor(Qt::magenta, 4);
    setColor(Qt::green, 5);
    setColor(Qt::blue, 6);

    setXaxisEnabled(true);
    setYaxisEnabled(true);
    setLegendEnabled(true);

    setAxisFont(QwtPlot::xBottom, QFont("Arial", 10));
    setAxisFont(QwtPlot::yLeft, QFont("Arial", 10));

    installEventFilter(this);

    // display timer
    Timer = new QTimer(this);
    Timer->setInterval(1000);
    connect(Timer, SIGNAL(timeout()), this, SLOT(TimeOut()));

    // data collection thread
    timerThread = new stripplotthread();
    timerThread->start();
    timerThread->threadSetTimer(100);
    timerThread->setPriority(QThread::HighPriority);
    connect(this, SIGNAL(timerThreadStop()), timerThread, SLOT(runStop()));
    connect(timerThread, SIGNAL(update()), this, SLOT(TimeOutThread()),  Qt::DirectConnection);
}

void caStripPlot::defineXaxis(units unit, double period)
{
    double interval;

    if(unit == Millisecond) {
        interval = period / 1000.0;
    } else if(unit == Second) {
        interval = period;
    } else if(unit == Minute) {
        interval = period * 60;
    } else {
        interval = 60;
        printf("\nunknown unit\n");
    }

    // set axis and in case of a time scale define the time axis
    setXaxis(interval, period);
    replot();
}

void caStripPlot::setXaxis(double interval, double period)
{
    // set axis and in case of a time scale define the time axis
    if(thisXaxisType == TimeScale) {
        QTime timeNow= QTime::currentTime();
        timeNow = timeNow.addSecs((int) -interval);
        setAxisScale(QwtPlot::xBottom, 0, interval);
        setAxisScaleDraw ( QwtPlot::xBottom, new TimeScaleDraw (timeNow) );
    } else {
        setAxisScale(QwtPlot::xBottom, -period, 0);
        setAxisScaleDraw(QwtPlot::xBottom, new QwtScaleDraw());
    }
}

void caStripPlot::setYaxisType(yAxisType s)
{
    thisYaxisType = s;
    if(s == log10) {
#if QWT_VERSION < 0x060100
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
#else
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine);
#endif
    } else {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
    }

    replot();
}

void caStripPlot::RescaleCurves(int width, units unit, double period)
{
    HISTORY = width; // equals canvas width

    // set the right interval
    if(unit == Millisecond) {
        timeInterval = period / (double) HISTORY;
        INTERVAL = period / 1000.0;

        // limit update rate
        if(timeInterval < 2) {
            timeInterval = 2;
            double newPeriod = timeInterval * HISTORY;
            INTERVAL = newPeriod / 1000.0;
            setXaxis(INTERVAL, newPeriod);
        }

    } else if(unit == Minute) {
        timeInterval =  1000.0 * period * 60.0 / (double) HISTORY;
        INTERVAL = period * 60;

    } else { // seconds or undefined
        timeInterval = 1000.0 * period / (double) HISTORY;
        INTERVAL = period;

        // limit update rate
        if(timeInterval < 2) {
            timeInterval = 2;
            double newPeriod = timeInterval * HISTORY / 1000.0;
            INTERVAL = newPeriod;
            setXaxis(INTERVAL, newPeriod);
        }
    }

    if(!initCurves) return;
    initCurves = false;

    mutex.lock();

    // initialize the arrays, with nan data
    for(int i=0; i < MAXCURVES; i++) {
        // define interval data for error and fill curves
        rangeData[i].clear();
        fillData[i].clear();
        if(i==0)  base.clear();
        for ( int j = 0; j <  MAXIMUMSIZE; j++ ) {
            rangeData[i].append(QwtIntervalSample(0, QwtInterval(NAN, NAN)));
            fillData[i].append(QPointF(NAN,NAN));
            if(i==0) base.append(QwtIntervalSample(0, QwtInterval(NAN, NAN)));
        }
    }

    // set the data to the curves
    for(int i=0; i < NumberOfCurves; i++) {
        if(thisStyle[i] == FillUnder) {
            fillcurve[i]->setSamplesList(fillData[i]);
            fillcurve[i]->setSamples(fillData[i].toVector());
        }
        errorcurve[i]->setSamplesList(rangeData[i]);
        errorcurve[i]->setSamples(rangeData[i].toVector());
    }

    mutex.unlock();

    // define update rate
    dataCount = 0;

    if(timerID) {
        TimersStart();
    }

    RestartPlot1 = true;
    RestartPlot2 = false;

    replot();
}

void caStripPlot::TimersStart()
{
    // change display timer interval, limit to 20 Hz
    int interval = qMax((int)timeInterval, 50);
    Timer->setInterval(interval);
    Timer->start();

    // stop run method of thread and redefine repetition time then start run
    emit timerThreadStop();
    timerThread->wait(200);
    timerThread->threadSetTimer((int) timeInterval);
    timerThread->start();

    // run timeout slot immedialety
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimeOut()));
    timer->setSingleShot(true);
    timer->start(0);
    timer->start(0);
    timer->start(0);
}

// reinitalize plot when resizing
void caStripPlot::resizeEvent ( QResizeEvent * event )
{
    QwtPlot::resizeEvent(event);
    RescaleCurves(canvas()->size().width(), thisUnits, thisPeriod);
    replot();
    if(timerID) RescaleAxis();
}

void caStripPlot:: UpdateScaling()
{
    initCurves = true;
    RescaleCurves(canvas()->size().width(), thisUnits, thisPeriod);
    replot();
    if(timerID) RescaleAxis();
}

QString caStripPlot::legendText(int i)
{
    char min[20], max[20];
    QString MinMax;
    QString titre(savedTitres.at(i));

    // in case of fixed scales, concatenate the limits that are used
    if(thisYaxisScaling == fixedScale) {
        sprintf(min, "%.1f", thisYaxisLimitsMin[i]);
        sprintf(max, "%.1f", thisYaxisLimitsMax[i]);
        ReplaceTrailingZerosByBlancs(min);
        ReplaceTrailingZerosByBlancs(max);
        MinMax.sprintf("[%s,%s]", min, max);
        MinMax = MinMax.simplified(); MinMax.replace( " ", "" );
        titre.append(" ");
        titre.append(MinMax);
    }
    return titre;
}

void caStripPlot::defineCurves(QStringList titres, units unit, double period, int width, int nbCurves)
{
    int min, max;
    NumberOfCurves = nbCurves;
    scaleWidget->getBorderDistHint(min, max);
    savedTitres = titres;

    defineXaxis(unit, period);

    // define curves
    for(int i=0; i < MAXCURVES; i++) {
        curvStyle s = Lines;
        QColor c;

        if(curve[i] != (QwtPlotCurve*) 0) {
            s = getStyle(i);
            c = getColor(i);
            delete curve[i];
        }

        if(errorcurve[i] != (QwtPlotIntervalCurveNaN*) 0) {
            delete errorcurve[i];
        }
        if(fillcurve[i] != (QwtPlotCurve*) 0) {
            delete fillcurve[i];
        }

        if(i < NumberOfCurves) {
            // set title and limits to legend
            QString titre = legendText(i);

            curve[i] = new QwtPlotCurve(titre);
            errorcurve[i] = new QwtPlotIntervalCurveNaN(titre);
            fillcurve[i] = new QwtPlotCurveNaN(titre);
            setStyle(s, i);

            curve[i]->setZ(i);
            curve[i]->attach(this);
            fillcurve[i]->setZ(i);
            fillcurve[i]->attach(this);
            fillcurve[i]->setItemAttribute(QwtPlotItem::Legend, false);
            errorcurve[i]->setZ(i+10);
            errorcurve[i]->attach(this);
            errorcurve[i]->setItemAttribute(QwtPlotItem::Legend, false);

            showCurve(i, false);

            realMax[i] = maxVal[i] = -1000000;
            realMin[i] = minVal[i] =  1000000;
            actVal[i] = realVal[i] = 0;
        }
    }

    showCurve(0, true);

    // draw legend
    if(thisLegendshow) {
        QwtLegend *lgd = new QwtLegend;
        insertLegend(lgd, QwtPlot::BottomLegend);

        // set color on legend texts
        setLegendAttribute(thisScaleColor, QFont("arial",9), COLOR);
    }
    RescaleCurves(width, unit, period);
}

void caStripPlot::startPlot()
{
    //change display timer interval
    timerID = true;
    TimersStart();
}

void caStripPlot::addText(double x, double y, char* text, QColor c, int fontsize)
{
    QwtPlotMarker *mY = new QwtPlotMarker();
    QwtText txt;
    txt.setText(text);
    txt.setFont(QFont("Helvetica",fontsize, QFont::Bold));
    txt.setColor(c);
    mY->setLabel(txt);
    mY->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    mY->setXValue(x);
    mY->setYValue(y);
    mY->attach(this);
    replot();
}

// data collection done by timerthread
void caStripPlot::TimeOutThread()
{
    int c, i;
    double elapsedTime = 0.0;
    QwtIntervalSample tmp;
    QPointF tmpP;
    double value = 0.0;
    double interval=0.0;

    if(!timerID) return;

    mutex.lock();

    int dataCountLimit = (int) (HISTORY - 1 + SOMEMORE);

    // we need an exact time scale
    if(RestartPlot1) {
        ftime(&timeStart);
        RestartPlot1 = false;
        RestartPlot2 = true;
    }
    ftime(&timeNow);

    elapsedTime = ((double) timeNow.time + (double) timeNow.millitm / (double)1000) -
            ((double) timeStart.time + (double) timeStart.millitm / (double)1000);

    timeData = INTERVAL + elapsedTime;  // in seconds
    interval = INTERVAL;
/*
    printf("dataCountLimit = %d datacount=%d history=%d interval=%f elapsed=%f siz=%d\n",
           dataCountLimit, dataCount, HISTORY, interval, elapsedTime,  rangeData[0].size());
*/
    // correct value to fit again inside the interval (only for the fixed scale)
    if(thisXaxisType != TimeScale) {
        if(thisUnits == Millisecond) {
            timeData = timeData * 1000.0;
            interval = INTERVAL * 1000.0;
        } else if(thisUnits == Minute) {
            timeData = timeData / 60.0;
            interval = INTERVAL / 60.0;
        } else {
            interval = INTERVAL;
        }
    }

    // shift data back
    if(dataCount > 1) {

        // shift data and our timebase for the fixed scale
        if(thisXaxisType != TimeScale) {
            for (c = 0; c < NumberOfCurves; c++ ) {
                // shift left and cur last
                rangeData[c].prepend(tmp);
                rangeData[c].removeLast();
                if(thisStyle[c] == FillUnder) {
                    fillData[c].prepend(tmpP);
                    fillData[c].removeLast();
                }
            }

            base.prepend(tmp);
            base.removeLast();

            for (i = dataCount; i > 0; i-- ) {
                value = base[i].value - timeData;
                for (c = 0; c < NumberOfCurves; c++ ) {
                    rangeData[c][i].value = value;
                    fillData[c][i].setX(value);
                }
            }

            // shift data
        } else {
            for (c = 0; c < NumberOfCurves; c++ ) {
                rangeData[c].prepend(tmp);
                if(thisStyle[c] == FillUnder) fillData[c].prepend(tmpP);
                rangeData[c].removeLast();
                if(thisStyle[c] == FillUnder) fillData[c].removeLast();
            }
        }

    }

    // update last point
    for (int c = 0; c < NumberOfCurves; c++ ) {
        double valueMin = minVal[c];
        double valueMax = maxVal[c];

        if(thisYaxisType == log10) {
            if(valueMin < 1.e-20) valueMin=1.e-20;
            if(valueMax < 1.e-20) valueMax=1.e-20;
        }

        QwtInterval tmp;
        tmp.setMaxValue( valueMax);
        tmp.setMinValue( valueMin);

        rangeData[c][0] = QwtIntervalSample( timeData, tmp);
        if(thisXaxisType != TimeScale) {
            base[0] = QwtIntervalSample(timeData, tmp);
        }
        if(thisStyle[c] == FillUnder) {
            fillData[c][0] = QPointF(timeData, (valueMax+valueMin)/2);
        }
    }

    // advance data points
    if (dataCount < 2 && dataCount < dataCountLimit) dataCount++;
    else if(dataCount < dataCountLimit) {
        if(thisXaxisType != TimeScale) {
            if(fillData[0][dataCount-1].x() > -interval) dataCount++;
        } else {
            if(elapsedTime < interval) dataCount++;
        }
    }

    // keep max and min of every curve
    for (int c = 0; c < NumberOfCurves; c++ ) {
        maxVal[c] = minVal[c] = actVal[c];
        realMax[c] = realMin[c] = realVal[c];
    }

    // in case of automatic y scale we need the minimum and maximum of our curves
    if(thisYaxisScaling == autoScale) {
        AutoscaleMaxY = -INFINITY;
        AutoscaleMinY = INFINITY;

        for (int c = 0; c < NumberOfCurves; c++ ) {
            for (int counter = 0; counter < dataCount; counter++) {
                QwtIntervalSample P = rangeData[c].at(counter);
                if(!isnan(P.interval.maxValue()) && P.interval.maxValue() > AutoscaleMaxY) AutoscaleMaxY = P.interval.maxValue();
                if(!isnan(P.interval.minValue()) && P.interval.minValue() < AutoscaleMinY) AutoscaleMinY = P.interval.minValue();
            }
        }

        if(AutoscaleMaxY == AutoscaleMinY) {
            AutoscaleMaxY += 0.5;
            AutoscaleMinY -= 0.5;
        } else {
            if(qAbs(AutoscaleMaxY) > 1.e-9) AutoscaleMaxY += (AutoscaleMaxY - AutoscaleMinY)/20.0;
            if(qAbs(AutoscaleMinY) > 1.e-9 && thisYaxisType != log10) AutoscaleMinY -= (AutoscaleMaxY - AutoscaleMinY)/20.0;
        }

        if(thisYaxisType == log10) {
            for (int c = 0; c < NumberOfCurves; c++ ) {
                if(AutoscaleMinY < thisYaxisLimitsMin[c]) AutoscaleMinY = thisYaxisLimitsMin[c];
            }
        }
    }

    mutex.unlock();
}

// display the curves
void caStripPlot::TimeOut()
{
    double elapsedTime = 0.0;

    if(!timerID) return;

    mutex.lock();

    // in case of restart plot, get start time and for the running time scale the new scale
    if(RestartPlot2) {
        RestartPlot2 = false;
        ftime(&plotStart);
        if(thisXaxisType == TimeScale) {
            QTime timeNow= QTime::currentTime();
            timeNow = timeNow.addSecs((int) -INTERVAL);
            setAxisScale(QwtPlot::xBottom, 0, INTERVAL);
            setAxisScaleDraw (QwtPlot::xBottom, new TimeScaleDraw (timeNow));
        }
    }

    // get actual time
    ftime(&timeNow);

    // get time since restart
    elapsedTime = ((double) timeNow.time + (double) timeNow.millitm / (double)1000) -
            ((double) plotStart.time + (double) plotStart.millitm / (double)1000);

    // change scale base in case of running time scale
    if(thisXaxisType == TimeScale) {
        timeData = INTERVAL + elapsedTime;
        setAxisScale(QwtPlot::xBottom, timeData - INTERVAL, timeData);
    }

    // set data into the curves
    for (int c = 0; c < NumberOfCurves; c++ ) {
        if(thisStyle[c] == FillUnder) {
            fillcurve[c]->setSamplesList(fillData[c]);
            fillcurve[c]->setSamples(fillData[c].toVector());
        }
        errorcurve[c]->setSamplesList(rangeData[c]);
        errorcurve[c]->setSamples(rangeData[c].toVector());
/*
        if(c==0) {
            printf("-----------------------\n");
            for(int j=0; j!=rangeData[c].size();j++) {
                printf("%d %f %f\n", j, rangeData[c][j].value, rangeData[c][j].interval.maxValue());
                if(isnan(rangeData[c][j].interval.maxValue())) break;
            }
        }
*/
    }

    // in case of autoscale adjust the vertical scale
    if(thisYaxisScaling == autoScale) setAxisScale(QwtPlot::yLeft, AutoscaleMinY, AutoscaleMaxY);

    // replot
    replot();

    mutex.unlock();
}

void caStripPlot::setYscale(double ymin, double ymax) {
    setAxisScale(QwtPlot::yLeft, ymin, ymax);
    replot();
}

void caStripPlot::RescaleAxis()
{
    int i;
    // rescale axis
    for(i=0; i < NumberOfCurves; i++) {
        setData(realTim[i], realVal[i], i);
    }

    // redraw legend if any
    setLegendAttribute(thisScaleColor, QFont("arial", 9), TEXT);
}

void caStripPlot::setLegendAttribute(QColor c, QFont f, LegendAtttribute SW)
{
    int i;

#if QWT_VERSION < 0x060100
    for(i=0; i < NumberOfCurves; i++) {

        switch (SW) {
        case TEXT:
            if(thisLegendshow) {
                QwtText text;
                text.setText(legendText(i));
                qobject_cast<QwtLegendItem*>(this->legend()->find(curve[i]))->setText(text);
            }
            break;

        case FONT:

            if(getLegendEnabled()) {
                if(legend() != (QwtLegend*) 0) {
                    QList<QWidget *> list =  legend()->legendItems();
                    for (QList<QWidget*>::iterator it = list.begin(); it != list.end(); ++it ) {
                        QWidget *w = *it;
                        w->setFont(f);
                    }
                }
            }
            break;

        case COLOR:
            if(legend() != (QwtLegend*) 0) {
                QList<QWidget *> list =  legend()->legendItems();
                for (QList<QWidget*>::iterator it = list.begin(); it != list.end(); ++it ) {
                    QWidget *w = *it;
                    QPalette palette = w->palette();
                    palette.setColor( QPalette::WindowText, c); // for ticks
                    palette.setColor( QPalette::Text, c);       // for ticks' labels
                    w->setPalette (palette);
                    w->setFont(f);
                }
            }

            break;
        }

    }
#else
    i=0;
    foreach (QwtPlotItem *plt_item, itemList()) {
        if ( plt_item->rtti() == QwtPlotItem::Rtti_PlotCurve ) {
            QwtLegend *lgd = qobject_cast<QwtLegend *>(legend());
            QList<QWidget *> legendWidgets = lgd->legendWidgets(itemToInfo(plt_item));
            if ( legendWidgets.size() == 1 ) {
                QwtLegendLabel *b = qobject_cast<QwtLegendLabel *>(legendWidgets[0]);
                switch (SW) {

                case TEXT:

                    if(thisLegendshow) {

                        QwtText text;
                        text.setText(legendText(i++));
                        //printf("%s %s\n", b->plainText().toAscii().constData(), legendText(i-1).toAscii().constData());
                        b->setText(text);
                        b->update();


                    }
                    break;

                case FONT:
                    //printf("%s %s\n", b->plainText().toAscii().constData(), legendText(i-1).toAscii().constData());

                    b->setFont(f);
                    b->update();

                    break;

                case COLOR:

                    //printf("%s %s\n", b->plainText().toAscii().constData(), legendText(i-1).toAscii().constData());
                    QPalette palette = b->palette();
                    palette.setColor( QPalette::WindowText, c); // for ticks
                    palette.setColor( QPalette::Text, c);       // for ticks' labels
                    b->setPalette(palette);
                    b->update();

                    break;

                }


            }
        }
    }
#endif

}

void caStripPlot::setData(struct timeb now, double Y, int curvIndex)
{
    if(curvIndex < 0 || curvIndex > (MAXCURVES-1)) return;

    mutex.lock();

    realVal[curvIndex] = Y;
    realTim[curvIndex] = now;
    if(Y> realMax[curvIndex]) realMax[curvIndex]  = Y;
    if(Y< realMin[curvIndex]) realMin[curvIndex]  = Y;

    // in case of fixed scales, remap the data to the first curve
    if(thisYaxisScaling == fixedScale) {
        double y0min = thisYaxisLimitsMin[0];
        double y0max = thisYaxisLimitsMax[0];
        double ymin =  thisYaxisLimitsMin[curvIndex];
        double ymax =  thisYaxisLimitsMax[curvIndex];
        actVal[curvIndex] = (y0max - y0min) / (ymax -ymin) * (realVal[curvIndex] - ymin) + y0min;
        minVal[curvIndex] = (y0max - y0min) / (ymax -ymin) * (realMin[curvIndex] - ymin) + y0min;
        maxVal[curvIndex] = (y0max - y0min) / (ymax -ymin) * (realMax[curvIndex] - ymin) + y0min;
        // otherwise keep the data
    } else {
        actVal[curvIndex] = realVal[curvIndex];
        minVal[curvIndex] = realMin[curvIndex];
        maxVal[curvIndex] = realMax[curvIndex];
    }

    mutex.unlock();
}

void caStripPlot::showCurve(int number, bool on)
{
    if(number < 0 || number > (MAXCURVES-1)) return;
    curve[number]->setVisible(on);
    fillcurve[number]->setVisible(on);
    errorcurve[number]->setVisible(on);
    replot();
}

void caStripPlot::setXaxisEnabled(bool show)
{
    thisXshow = show;
    enableAxis(xBottom, show);
    replot();
}

void caStripPlot::setYaxisEnabled(bool show)
{
    thisYshow = show;
    enableAxis(yLeft, show);
    replot();
}


void caStripPlot::setLegendEnabled(bool show)
{
    thisLegendshow = show;
}

void caStripPlot::setTitlePlot(QString const &titel)
{
    thisTitle=titel;
    if(titel.size() != 0) {
        QwtText title(titel);
        title.setFont(QFont("Arial", 11));
        setTitle(title);
        replot();
    }
}

void caStripPlot::setTitleX(QString const &titel)
{
    thisTitleX=titel;
    if(titel.size() != 0) {
        QwtText xAxis(titel);
        xAxis.setFont(QFont("Arial", 11));
        setAxisTitle(xBottom, xAxis);
    }
    replot();
}

void caStripPlot::setTitleY(QString const &titel)
{
    thisTitleY=titel;
    if(titel.size() != 0) {
        QwtText xAxis(titel);
        xAxis.setFont(QFont("Arial", 11));
        setAxisTitle(yLeft, xAxis);
    }
    replot();
}

void caStripPlot::setGrid(bool m)
{
    thisGrid = m;
    if(m) {
        penGrid = QPen(Qt::gray);
        penGrid.setStyle(Qt::DashLine);
        plotGrid->setPen(penGrid);
        plotGrid->setVisible(penGrid.style() != Qt::NoPen);
    } else {
        plotGrid->setVisible(false);
    }
    replot();
}

void caStripPlot::setGridColor(QColor c)
{
    thisGridColor = c;
    penGrid = QPen(c);
    penGrid.setStyle(Qt::DashLine);
    plotGrid->setPen(penGrid);
    plotGrid->setVisible(penGrid.style() != Qt::NoPen);
    replot();
}

QwtPlotCurve::CurveStyle caStripPlot::myStyle(curvStyle s)
{
    QwtPlotCurve::CurveStyle ms = QwtPlotCurve::Lines;
    switch ( s ) {
    case Lines:
        ms = QwtPlotCurve::Lines;  break;
    case FillUnder:
        ms = QwtPlotCurve::Lines;  break;
    default:
        ms = QwtPlotCurve::Lines; break;
    }
    return ms;
}

void caStripPlot::setBackground(QColor c)
{
    thisBackColor = c;
    QPalette canvasPalette(c);
    canvasPalette.setColor(QPalette::Foreground, QColor(133, 190, 232));
    canvas()->setPalette(canvasPalette);
    replot();
}

void caStripPlot::setForeground(QColor c)
{
    thisForeColor = c;
    setPalette(QPalette(c));
    replot();
}

void caStripPlot::setScaleColor(QColor c)
{
    QwtScaleWidget *scaleX =axisWidget(QwtPlot::xBottom);
    QwtScaleWidget *scaleY =axisWidget(QwtPlot::yLeft);
    thisScaleColor = c;
    QPalette palette = scaleX->palette();
    palette.setColor( QPalette::WindowText, c); // for ticks
    palette.setColor( QPalette::Text, c);       // for ticks' labels
    scaleX->setPalette( palette);
    scaleY->setPalette (palette);
    titleLabel()->setPalette(palette);
}

void caStripPlot::setColor(QColor c, int number)
{
    if(number < 0 || number > (MAXCURVES-1)) return;
    thisLineColor[number] = c;

    if(curve[number] != (QwtPlotCurve *) 0) {
        curve[number]->setPen(QPen(c, 0));
    }

    if(fillcurve[number] != (QwtPlotCurveNaN *) 0) {
        fillcurve[number]->setPen(c);
        fillcurve[number]->setBrush(QBrush(c, Qt::SolidPattern));
        fillcurve[number]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    }

    if(errorcurve[number] != (QwtPlotIntervalCurveNaN *) 0) {
        if(thisStyle[number] == FillUnder) {
            errorcurve[number]->setPen(QPen(thisScaleColor,0));
            errorcurve[number]->setBrush(QBrush(thisScaleColor, Qt::SolidPattern));
        } else {
            errorcurve[number]->setPen(QPen(c, 0));
            errorcurve[number]->setBrush(QBrush(c, Qt::SolidPattern));
        }
        errorcurve[number]->setRenderHint( QwtPlotItem::RenderAntialiased, true);
    }
}

void caStripPlot::setStyle(curvStyle s, int number)
{
    if(number < 0 || number > (MAXCURVES-1)) return;
    thisStyle[number] = s;
    setColor(thisLineColor[number], number);
}

/**
 * eliminate stupid zero in value after the period
 */
void caStripPlot::ReplaceTrailingZerosByBlancs(char *asc)
{
    int i;
    int dot = false;
    for (i = 0; i < (int) strlen(asc); i++) {
        if (asc[i] == '.') {
            dot = true;
            break;
        }
    }
    if (dot) {
        for (i = strlen(asc) - 1; i >= 0; i--) {
            if (asc[i] != '0') {
                if(asc[i] == '.') asc[i]=' ';
                break;
            }
            else asc[i] = ' ';
        }
    }
    if (asc[0] == '+') asc[0] = ' ';
    if (asc[1] == '+') asc[1] = ' ';
}


bool caStripPlot::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        int nButton = ((QMouseEvent*) event)->button();
        if(nButton==2) {
            //printf("emit from %s\n", this->objectName().toAscii().constData());
            QPoint p;
            emit ShowContextMenu(p);
        }
    } else if(event->type() == QEvent::Show) {
    }
    return QObject::eventFilter(obj, event);
}

#include "moc_castripplot.cpp"
