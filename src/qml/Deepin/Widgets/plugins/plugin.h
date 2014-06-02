#ifndef DWINDOW_PLUGIN_H
#define DWINDOW_PLUGIN_H

#include <QQmlExtensionPlugin>

class DockAppletPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Deepin.Widgets")

public:
    void registerTypes(const char *uri);
};

#endif // DWINDOW_PLUGIN_H

