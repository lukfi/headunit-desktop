#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QPluginLoader>
#include <QDir>
#include <QJsonObject>
#include <QDebug>
#include <QVariant>
#include <QHash>
#include <QQmlPropertyMap>
#include <QTextStream>
#include <QJsonParseError>
#include <QLoggingCategory>
#include "../includes/plugininterface.h"
#include "settingsloader.h"

#include "systemeventhandler.h"

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QQmlApplicationEngine *engine, bool filter, QStringList filterList, QObject *parent = nullptr);
    ~PluginManager();
signals:
    void themeEvent(QString, QString); // event for GUI (theme)
public slots:
    void onEvent(QString event, QString eventData); // communication from GUI (theme)
private slots:
    void messageReceived(QString id, QString message); // communication from plugin
private:
    bool loadPlugins(QQmlApplicationEngine *engine, bool filter, QStringList filterList);
    QVariantList menuItems;
    QVariantList configItems;
    QMap<QString, PluginInterface*> plugins;
    QMap<QString, QJsonObject> pluginConfigs;
    QList<QPluginLoader*>pluginLoaders;
    QList<SettingsLoader*>pluginSettings;

    void settingsChanged(QString key, QVariant value);
//    QQmlPropertyMap settings;
    QVariantList settingsItems;

    void loadMenuItems(QQmlApplicationEngine *engine);
    void loadConfigItems(QQmlApplicationEngine *engine);
    void processPluginEvents(QStringList events);
    QVariantMap m_settings;
    QHash<QString, QStringList> connections;

    QStringList m_overlays;

    SystemEventHandler mEventHandler;
};

#endif // PLUGINMANAGER_H
