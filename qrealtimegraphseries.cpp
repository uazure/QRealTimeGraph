#include "qrealtimegraphseries.h"
#include "qrealtimegraph.h"
#include <QDateTime>
#include <QDebug>


QRealTimeGraphSeries::QRealTimeGraphSeries(QRealTimeGraph *graph, const QObject *obj, int channel):
    QObject(graph)
{
    this->chan=channel;
    this->graph=graph;

    #if QT_VERSION >= 0x040700
        data10m.reserve(graph->getResolution());
        data1h.reserve(graph->getResolution());
    #endif

    dataSourceObject=obj;

    visible=true;

}

double QRealTimeGraphSeries::currentTimestamp()
{
    QDateTime dt=QDateTime::currentDateTime();
    double t=dt.toTime_t();
    t+=(dt.toString("zzz").toDouble())/1000;
    return t;
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
    if (data10m.isEmpty() || timestamp-data10m.last().x()>= (double)QRealTimeGraph::interval10m/graph->getResolution()) {
        if (data10m.size()==graph->getResolution()) {
            data10m.removeFirst();
        }
        qDebug()<<"Appending new point to 10m"<<timestamp<<value;
        data10m.append(QPointF(timestamp,value));
    }

    if (data1h.isEmpty() || timestamp-data1h.last().x()>= (double)QRealTimeGraph::interval1h/graph->getResolution()) {
        if (data1h.size()==graph->getResolution()) {
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

