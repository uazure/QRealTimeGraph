#include "qrealtimegraphseries.h"
#include "qrealtimegraph.h"
#include <QDateTime>
#include <QDebug>


QRealTimeGraphSeries::QRealTimeGraphSeries(QObject *parent):
    QObject(parent)
{


    steps = 120;
    visible = true;


    #if QT_VERSION >= 0x040700
        data10m.reserve(getResolution());
        data1h.reserve(getResolution());
    #endif


}

double QRealTimeGraphSeries::currentTimestamp()
{
    QDateTime dt=QDateTime::currentDateTime();
    double t=dt.toTime_t();
    t+=(dt.toString("zzz").toDouble())/1000;
    return t;
}

int QRealTimeGraphSeries::getResolution() const
{
    return steps;
}

void QRealTimeGraphSeries::setResolution(int steps)
{
    this->steps=steps;
}

const QPen& QRealTimeGraphSeries::getPen() const
{
    return graphPen;
}


const QList<QPointF> & QRealTimeGraphSeries::getData(int interval) const
{
    switch (interval) {
    case QRealTimeGraph::interval10m:
        return data10m;
        break;
    case QRealTimeGraph::interval1h:
        return data1h;
        break;
    default:
        return data1h;
        break;
    }
}

void QRealTimeGraphSeries::setCurrentValue(double value)
{
    double timestamp=QRealTimeGraphSeries::currentTimestamp();

    //check if data requires update based on resolution and steps
    if (data10m.isEmpty() || timestamp-data10m.last().x()>= (double)QRealTimeGraph::interval10m/getResolution()) {
        if (data10m.size()==getResolution()) {
            data10m.removeFirst();
        }
        qDebug()<<"Appending new point to 10m"<<timestamp<<value;
        data10m.append(QPointF(timestamp,value));
    }

    if (data1h.isEmpty() || timestamp-data1h.last().x()>= (double)QRealTimeGraph::interval1h/getResolution()) {
        if (data1h.size()==getResolution()) {
            data1h.removeFirst();
        }
        qDebug()<<"Appending new point to 1h"<<timestamp<<value;
        data1h.append(QPointF(timestamp,value));
    }
}

double QRealTimeGraphSeries::maxValue(int interval) const
{
    const QList<QPointF> *list;
    switch (interval) {
    case QRealTimeGraph::interval10m:
        list=&data10m;
    break;
    case QRealTimeGraph::interval1h:
        list=&data1h;
        break;
    default:
        list=&data1h;
    }
    double max;
    for (int i=0;i<list->size();++i) {
        if (i==0) {
            max=list->at(i).y();
            continue;
        }
        if (max<list->at(i).y()) max=list->at(i).y();
    }
return max;
}

double QRealTimeGraphSeries::minValue(int interval) const
{
    const QList<QPointF> *list;
    switch (interval) {
    case QRealTimeGraph::interval10m:
        list=&data10m;
    break;
    case QRealTimeGraph::interval1h:
        list=&data1h;
        break;
    default:
        list=&data1h;
    }
    double min;
    for (int i=0;i<list->size();++i) {
        if (i==0) {
            min=list->at(i).y();
            continue;
        }
        if (min>list->at(i).y()) min=list->at(i).y();
    }
return min;
}

