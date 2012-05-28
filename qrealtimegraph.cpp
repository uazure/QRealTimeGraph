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
    plotArea.setTopRight(QPointF(QPoint(20,10)));
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
        series=new QRealTimeGraphSeries(this,obj,channel);
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
        visibleMap.remove(series);
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


QPointF QRealTimeGraph::transform(const QPointF &point) const
{
    //point - is the point with plot coordinates
    //plotArea - rectangle with plot (in plot coordinates)
    QPointF retval;
    double vAspect,hAspect;
    hAspect=plotArea.width()/width();
    vAspect=plotArea.height()/height();

    retval.setX((point.x()-plotArea.left())/hAspect);
    retval.setY((point.y()-plotArea.right())/vAspect);
    return retval;
}

QPointF QRealTimeGraph::invTransform(const QPointF &point) const {


}

void QRealTimeGraph::paintData(QPainter &painter) {

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

