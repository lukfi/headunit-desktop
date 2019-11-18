#include "systemeventhandler.h"

#include <QDebug>
#include <QLoggingCategory>

SystemEventHandler::SystemEventHandler(QMap<QString, PluginInterface*>& plugins) :
    mPlugins(plugins)
{

}

void SystemEventHandler::HandleEvent(QString event, QString eventData)
{
    if (event.toLower() == "key")
    {
        HandleKeyPress(eventData); // keyboard button
    }
    else if (event.toLower() == "btn") // phisical buttons
    {
    }
    else // other messages
    {
        HandlePluginMessage(event, eventData);
    }
}

void SystemEventHandler::HandleKeyPress(QString key)
{
    // TODO: this should map key to function
    PluginInterface* volPlugin = nullptr;
    auto volIter = mPlugins.find("VolumeControl");
    if (volIter != mPlugins.end())
    {
        volPlugin = *volIter;
        volPlugin->eventMessage("KEY", key);
        if (key == QString::number(Qt::Key_Plus))
        {
            qDebug() << "VOL +";
        }
        else if (key == QString::number(Qt::Key_Minus))
        {
            qDebug() << "VOL -";
        }
    }
}

void SystemEventHandler::HandleButtonPress(QString btn)
{
    // TODO: this should map button to function
    PluginInterface* volPlugin = nullptr;
    auto volIter = mPlugins.find("VolumeControl");
    if (volIter != mPlugins.end())
    {
        volPlugin = *volIter;
        if (btn == "ROT0+")
        {
            volPlugin->eventMessage("KEY", QString::number(Qt::Key_Plus));
        }
        if (btn == "ROT0-")
        {
            volPlugin->eventMessage("KEY", QString::number(Qt::Key_Minus));
        }
        if (btn == "0")
        {
            // TODO: toggle
        }
    }
}

void SystemEventHandler::HandlePluginMessage(QString event, QString eventData)
{

}
