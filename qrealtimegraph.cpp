#include "qrealtimegraph.h"
#include <QtGui/QPainter>
#include <QDebug>
#include "qrealtimegraphseries.h"

QRealTimeGraph::QRealTimeGraph(QWidget *parent) :
    QWidget(parent)
{
    steps=120;
    currentInterval=interval10m;
    plotArea.setBottomLeft(QPointF(0,0));
    plotArea.setTopRight(QPointF(100, 100));
}

void QRealTimeGraph::paintEvent(QPaintEvent *)
{
    qDebug()<<"Paint event";
    QPainter p;
    p.begin(this);

    paintGrid(p);

    paintData(p);


    p.end();
}

void QRealTimeGraph::paintGrid(QPainter &painter) {


}

void QRealTimeGraph::paintData(QPainter &painter) {
    QList<QRealTimeGraphSeries*> visibleSeries = getVisibleSeriesList();
    QRealTimeGraphSeries* series;

    QList<QPointF> data;
    plotArea.setRight(QRealTimeGraphSeries::currentTimestamp());
    plotArea.setLeft(QRealTimeGraphSeries::currentTimestamp()-QRealTimeGraph::interval10m);
    //iterate over visible series and paint them
    for (int i=0; i<visibleSeries.size(); ++i) {
        series = visibleSeries.at(i);
        //set pen from series
        painter.setPen(series->getPen());
        qDebug()<<"Pen for"<<series<<"is"<<series->getPen();
        data = series->getData(QRealTimeGraph::interval10m);
        for (int j=0; j<data.size()-1; ++j) {
            painter.drawLine(transform(data.at(j)), transform(data.at(j+1)));
        }



    }




}

void QRealTimeGraph::setCurrentValue(const double value, const int channel, const QObject *obj)
{
    //check obj
    if (obj==NULL) {
        //try to get sender() as obj
        obj=sender();
    }
    //last try to get valid object address
    if (obj==NULL) {
        qDebug()<<"Object not specified";
        return;
    }

    //check if series for such obj/channel exists
    QRealTimeGraphSeries *series=getSeries(obj,channel);
    if (series==NULL) {
        qDebug()<<"Series does not exists";
        //Creating series object
        series=new QRealTimeGraphSeries(this);
        seriesList.append(series);
        if (seriesMap.contains(obj)) {
            qDebug()<<"SeriesMap contains"<<obj;
            qDebug()<<"Inserting series for channel"<<channel;
            QMap<int,QRealTimeGraphSeries *> chanMap=seriesMap.value(obj);
            chanMap.insert(channel,series);
        } else {
            qDebug()<<"SeriesMap doesn't contain"<<obj;
            qDebug()<<"Creating channelMap";
            QMap<int,QRealTimeGraphSeries *> chanMap;
            qDebug()<<"Populating chanMap with"<<channel<<series;
            chanMap.insert(channel,series);
            qDebug()<<"Populating seriesMap with"<<obj<<chanMap;
            seriesMap.insert(obj,chanMap);
        }

    }
    series->setCurrentValue(value);
}

void QRealTimeGraph::setResolution(int steps)
{
    if (steps>2) this->steps=steps;
}

void QRealTimeGraph::detach(const QObject *obj, int channel)
{
    if (obj==NULL) {
        obj=sender();
    }
    if (obj==NULL) {
        qWarning()<<"Can not detach non-specified object";
        return;
    }
    //remove channel of the object
    QRealTimeGraphSeries *series=getSeries(obj,channel);
    if (series!=NULL) {
        int num = seriesList.removeAll(series);
        if (num > 1) {
            qDebug()<<"Removed"<<num<<"!!! series from seriesList";
        }
        delete series;
    }
    seriesMap[obj].remove(channel);
    if (seriesMap.value(obj).size()==0) {
        seriesMap.remove(obj);
    }
}

const QRealTimeGraphSeries * QRealTimeGraph::getSeries(const QObject *obj, int channel) const
{
    return seriesMap.value(obj).value(channel);
}

QRealTimeGraphSeries * QRealTimeGraph::getSeries(const QObject *obj, int channel)
{
    return seriesMap.value(obj).value(channel);
}


QList<QRealTimeGraphSeries*> QRealTimeGraph::getVisibleSeriesList() const
{
    QList<QRealTimeGraphSeries*> visibleList;
    //append to visible list only visible series of data
    for (int i=0; i<seriesList.size(); ++i) {
        if (seriesList.at(i)->isVisible()) {
            visibleList.append(seriesList.at(i));
        }
    }

    return visibleList;
}


//transform gets point with plot coordinates and transforms it to widget coordinates

QPointF QRealTimeGraph::transform(const QPointF &point) const
{
    //point - is the point with plot coordinates
    //plotArea - rectangle with plot (in plot coordinates)
    QPointF retval;
    double vAspect,hAspect;
    hAspect=plotArea.width()/width();
    vAspect=plotArea.height()/height();

    retval.setX((point.x()-plotArea.left())/hAspect);
    retval.setY((point.y()-plotArea.top())/vAspect);
    qDebug()<<"transform for"<<point<<"results in"<<retval;
    return retval;
}


//TODO: implement transform from widget coords to plot coords
QPointF QRealTimeGraph::invTransform(const QPointF &point) const {


}



void QRealTimeGraph::setColor(const QColor &color, const QObject *obj, int channel) {
    QRealTimeGraphSeries *series=getSeries(obj, channel);
    if (series) {
        series->setColor(color);
    }
}

void QRealTimeGraph::setPen(const QPen &pen, const QObject *obj, int channel)
{
    QRealTimeGraphSeries *series=getSeries(obj, channel);
    if (series) {
        series->setPen(pen);
    }
}

void QRealTimeGraph::setTitle(const QString &title, const QObject *obj, int channel) {
    QRealTimeGraphSeries *series=getSeries(obj, channel);
    if (series) {
        series->setTitle(title);
    }
}

QString QRealTimeGraph::getTitle(const QObject *obj, int channel) const {
    const QRealTimeGraphSeries *series=getSeries(obj,channel);
    if (series) {
        return series->title();
    } else {
        qDebug()<<"No series for object"<<obj<<"channel"<<channel<<"found";
        return QString();
    }
}

