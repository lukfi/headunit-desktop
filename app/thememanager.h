#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>
#include <QSettings>
#include <QDir>
#include <cstring>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QSettings>
#include <QPluginLoader>
#include <QQmlExtensionPlugin>

#include "settingsloader.h"

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    explicit ThemeManager(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~ThemeManager();
    void IndicateCurrentItem();
private:
    void loadJson(QString path);
    void processThemeSettings(QJsonObject json);
    QVariantMap loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap * settingsMap);
    QVariantList themeSettingsToSettingsItems(QVariantList items, QString type);
    QQmlPropertyMap theme;
    QQmlApplicationEngine *m_engine;
    QVariantMap HUDStyle;
    QVariantList HUDStyleSettings;
    QList<SettingsLoader *>m_settings;
    struct {
        QString event;
        QString data;
    } mCurrentItem;
signals:
    void themeEvent(QString event, QString eventData); // communicaiton to themePlugin
    void pluginEvent(QString event, QString eventData); // event from plugin
public slots:
    void onEvent(QString event, QString eventData);
    void onThemeEvent(QString, QString);
};

#endif // THEMEMANAGER_H
