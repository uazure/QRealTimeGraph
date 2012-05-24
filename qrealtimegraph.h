#ifndef QREALTIMEGRAPH_H
#define QREALTIMEGRAPH_H

#include <QtGui/QWidget>
#include <QMultiMap>

class QRealTimeGraphSeries;

class QRealTimeGraph : public QWidget
{
    Q_OBJECT

public:
    enum Interval {
        interval10m=600,
        interval1h=3600,
        //interval12h=43200,
    };

    QRealTimeGraph(QWidget *parent = 0);

/** set color for line representing data of obj and channel
  \sa setPen()
  */
    void setColor(const QColor &color,const QObject *obj,int channel=0);

    /** set pen for line representing data of obj and channel. Setting pen gives more control on the representation than setColor()
      \sa setColor()
      */
    void setPen(const QPen &pen,const QObject *obj,int channel=0);

    /** set title for the data series
        \sa setGraphTitle(), setColor(), setPen() */
    void setTitle(const QString &title, const QObject *obj,int channel=0);

    QString getTitle(const QObject *obj, int channel=0) const;


    /** clears values for specified obj and channel
      \sa clearAllValues()
      */
    void clearValues(const QObject *obj,int channel=0);

    /** clear ALL values
      */
    void clearAllValues();

    /** removes item from graph but keeps it data
      \sa showItem()
      \sa hideAll()
      */
    void hideItem(const QObject *obj,int channel=0);

    /** clears graph from all data lines
      This can be useful when it's required to show some data.
      So first you can call hideAll() and then showItem()
      \sa hideItem()
      \sa showItem()
      \sa showAll()
      */
    void hideAll();

    /** adds data line of the obj/channel to the graph
      \sa showAll()
      \sa hideAll()
      \sa hideItem()
      */
    void showItem(const QObject *obj,int channel=0);

    /** adds all available data curves to graph
      \sa showItem()
      \sa hideItem()
      \sa hideAll()
      */
    void showAll();


    /** Resolution is number of steps to keep for data representation on time interval
        Default: 120 steps (5 seconds for 10m interval, 30 seconds for 1h interval etc.)
      */
    void setResolution(int steps);
    int getResolution() const {return steps;}

    void setInterval(Interval interval) {currentInterval=interval;};
    Interval interval() const {return currentInterval;};
//    QList<Interval> intervals() const;
//    QList<QString> intervalStringList() const;


    QSize sizeHint() const {return QSize(300,200);}
    QSize minimumSizeHint() const {return QSize(100,100);}
    virtual void paintEvent(QPaintEvent *);
    virtual void paintGrid(QPainter &painter);
    virtual void paintData(QPainter &painter);


public slots:
    /** slot that shoule be connected to QObject that is emitting its current value
      obj - optional pointer to the sender. If not specified then sender() will be used.
        note: obj SHOULD be specified if called directly (i.e. when not triggered by signal)
      channel - optional channel id of the obj. Specyfing channel allows to have many data sets for one QObject
      Ex: torrent can have download and upload speed, so channel=0 can be used for download graph and channel=1 for upload.

    \sa setColor(), setPen()
      */
    void setCurrentValue(const double value, const int channel=0, const QObject *obj=NULL);

    /** Detaches series for obj and channel from graph releasing the memory
      obj can be ommited if signal-slot connection used
//FIXME:  if channel is not specified or channel=0 then all series for the object are released
      */
    void detach(const QObject * obj=NULL,int channel=0);

    /** set title for the graph.
        \sa setTitle() */
    void setGraphTitle(const QString &title) {graphTitle=title;}
    const QString & getGraphTitle() const {return graphTitle;}

    void setUnit(const QString &unit) {graphUnit=unit;}
    const QString & getUnit() const {return graphUnit;}

    //transforms plot coordinates to widget coordinates
    QPointF transform (const QPointF &point) const;
    //performs inverse transformation from widget coordinates to plot coordinates
    QPointF invTransform (const QPointF &point) const;


protected:

    const QRealTimeGraphSeries * getSeries(const QObject *obj,int channel=0) const;
    QRealTimeGraphSeries * getSeries(const QObject *obj,int channel=0);

    QMap<const QObject*,QMap<int,QRealTimeGraphSeries*> > seriesMap;
    QMap<QRealTimeGraphSeries *,bool> visibleMap;
    int steps;
    Interval currentInterval;

private:
    QString graphTitle;
    QString graphUnit;
    //plot area in widget coordinates
    QRectF plotArea;



};

#endif
