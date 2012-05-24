#ifndef QREALTIMEGRAPHSERIES_H
#define QREALTIMEGRAPHSERIES_H
#include <QObject>
#include <QPointF>
#include <QList>
#include <QPen>

class QRealTimeGraph;




class QRealTimeGraphSeries:public QObject
{
    Q_OBJECT

public:
    QRealTimeGraphSeries (QRealTimeGraph *graph,const QObject *obj, int channel=0);


    /** set current value for the series. getCurrentTimestamp will be used to get timestamp
      \sa QRealTimeGraphSeries::getCurrentTimestamp()
      */
    virtual void setCurrentValue(double value);

    //returns channel index of the obect
    int channel() const {return chan;};
    //returns pointer to data source object
    const QObject * object() const {return dataSourceObject;};
    void setPen(const QPen &pen) {graphPen=pen;};
    void setColor(const QColor &color) {graphPen.setColor(color);};
    const QString & title() const {return graphTitle;};
    void setTitle(const QString & title) {graphTitle=title;}
    bool isVisible() {return visible;};
    void setVisible(bool visibility) {visible=visibility;};

    //returns list of points for specified interval;
    virtual const QList<QPointF> & getData (int interval) const;

    virtual double maxValue(int interval) const;
    virtual double minValue(int interval) const;

    //returns current timestamp as double with milisecond accuracy
    static double currentTimestamp();

protected:
    QList<QPointF> data10m;
    QList<QPointF> data1h;
    QRealTimeGraph *graph;

private:
    int chan;
    QObject const *dataSourceObject;
    QPen graphPen;
    QString graphTitle;
    bool visible;


};

#endif // QREALTIMEGRAPHSERIES_H
