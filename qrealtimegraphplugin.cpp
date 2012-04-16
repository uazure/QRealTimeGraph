#include "qrealtimegraph.h"
#include "qrealtimegraphplugin.h"

#include <QtCore/QtPlugin>

QRealTimeGraphPlugin::QRealTimeGraphPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QRealTimeGraphPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QRealTimeGraphPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QRealTimeGraphPlugin::createWidget(QWidget *parent)
{
    return new QRealTimeGraph(parent);
}

QString QRealTimeGraphPlugin::name() const
{
    return QLatin1String("QRealTimeGraph");
}

QString QRealTimeGraphPlugin::group() const
{
    return QLatin1String("Graph");
}

QIcon QRealTimeGraphPlugin::icon() const
{
    return QIcon();
}

QString QRealTimeGraphPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QRealTimeGraphPlugin::whatsThis() const
{
    return QLatin1String("Widget for displaying history of some real-time data");
}

bool QRealTimeGraphPlugin::isContainer() const
{
    return false;
}

QString QRealTimeGraphPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QRealTimeGraph\" name=\"qRealTimeGraph\">\n</widget>\n");
}

QString QRealTimeGraphPlugin::includeFile() const
{
    return QLatin1String("qrealtimegraph.h");
}

Q_EXPORT_PLUGIN2(qrealtimegraphplugin, QRealTimeGraphPlugin)
