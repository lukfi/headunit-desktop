#pragma once

#include <QString>
#include "../includes/plugininterface.h"

class SystemEventHandler
{
public:
    SystemEventHandler(QMap<QString, PluginInterface*>& plugins);
    void HandleEvent(QString event, QString eventData);

private:
    QMap<QString, PluginInterface*>& mPlugins;
    void HandleKeyPress(QString key);
    void HandleButtonPress(QString btn);
    void HandlePluginMessage(QString event, QString eventData);
};
