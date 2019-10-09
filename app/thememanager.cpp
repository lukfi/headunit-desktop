#include "thememanager.h"

Q_LOGGING_CATEGORY(THEMEMANAGER, "Theme Manager")

ThemeManager::ThemeManager(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent),
    m_engine(engine)
{
#if defined(Q_OS_WIN)
    QString fileName = "themes/default-theme/default-themed.dll";
#else
    QString fileName = "themes/default-theme/default-themed.so";
#endif
    QPluginLoader themeLoader(fileName);

    QQmlExtensionPlugin * theme = static_cast<QQmlExtensionPlugin *>(themeLoader.instance());

    if (!theme) {
        qCDebug(THEMEMANAGER) << "Error loading plugin : " << fileName << themeLoader.errorString();
        return;
    }

    QQmlExtensionPlugin * themePlugin = static_cast<QQmlExtensionPlugin *>(themeLoader.instance());

    if(!themePlugin){
        qCDebug(THEMEMANAGER) << "Error loading theme : " << themeLoader.metaData().value("name") << ", root component is not a valid instance of QQmlExtensionPlugin";
        return;
    }

    qCDebug(THEMEMANAGER) << "Theme loaded : " << fileName;

    const QMetaObject *pluginMeta = themePlugin->metaObject();

    QStringList methods;
    for(int i = pluginMeta->methodOffset(); i < pluginMeta->methodCount(); ++i){
        if(pluginMeta->method(i).methodSignature() == "onEvent(QString,QString)"){
            qDebug() << "Has onEvent";
            connect(this, SIGNAL(themeEvent(QString, QString)), themePlugin, SLOT(onEvent(QString, QString)));
        }
    }

    QJsonObject themeSettings = themeLoader.metaData().value("MetaData").toObject();

    processThemeSettings(themeSettings);

    themePlugin->registerTypes("");
    themePlugin->initializeEngine(engine, "");
}

void ThemeManager::onEvent(QString event, QString eventData) {
    emit themeEvent(event, eventData);
}

void ThemeManager::loadJson(QString path){
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCDebug(THEMEMANAGER) << "Error opening settings theme file " << path;
        return;
    }

    QString jsonString = file.readAll();
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if(jsonDoc.isNull()){
        qCDebug(THEMEMANAGER) << "Error parsing json file " << path << " " << error.errorString();
        return;
    }
    processThemeSettings(jsonDoc.object());
}

void ThemeManager::processThemeSettings(QJsonObject json){
    if(!json.keys().contains("name") || !json.keys().contains("label") || !json.keys().contains("colors") || !json.keys().contains("sizes")){
        qCDebug(THEMEMANAGER) << "Error processing theme settings, missing required field(s)";
        return;
    }
    if(!json.value("colors").isArray() || !json.value("sizes").isArray()){
        qCDebug(THEMEMANAGER) << "Error processing theme settings, \"colors\" or \"sizes\" field is not an array";
        return;
    }

    QQmlPropertyMap *colorsMap = new QQmlPropertyMap();
    HUDStyleSettings << loadSettingsMap("Colors", "Colors", "color", json.value("colors").toArray().toVariantList(), colorsMap);

    QQmlPropertyMap *sizesMap = new QQmlPropertyMap();
    HUDStyleSettings << loadSettingsMap("Sizes", "Sizes", "slider", json.value("sizes").toArray().toVariantList(), sizesMap);

    HUDStyle.insert("Colors", QVariant::fromValue<QQmlPropertyMap *>(colorsMap));
    HUDStyle.insert("Sizes", QVariant::fromValue<QQmlPropertyMap *>(sizesMap));


    QJsonArray settingsJson = json.value("settings").toArray();

    for(QJsonValueRef item : settingsJson){
        if(!item.isObject()){
            qDebug() << "Item is not object";
            continue;
        }

        QJsonObject jsonObject = item.toObject();

        QQmlPropertyMap *map = new QQmlPropertyMap();
        m_settings << new SettingsLoader(jsonObject, map);

        HUDStyle.insert(jsonObject.value("name").toString(), QVariant::fromValue<QQmlPropertyMap *>(map));
        HUDStyleSettings.append(jsonObject.toVariantMap());
    }

    m_engine->rootContext()->setContextProperty("HUDStyle", HUDStyle);
    m_engine->rootContext()->setContextProperty("HUDStyleSettings", HUDStyleSettings);
}


QVariantMap ThemeManager::loadSettingsMap(QString name, QString label, QString type, QVariantList items, QQmlPropertyMap *settingsMap){
    QVariantMap settings({{"label",label}, {"name",name}, {"type","items"}});

    QVariantList itemsList;
    if(type == ""){
        itemsList = items;
    } else {
        itemsList = themeSettingsToSettingsItems(items, type);
    }
    settings.insert("items", itemsList);

    m_settings << new SettingsLoader(QJsonObject::fromVariantMap(settings), settingsMap);

    return settings;
}
QVariantList ThemeManager::themeSettingsToSettingsItems(QVariantList items, QString type){
    QVariantList settingsList;
    for(QVariant itemVariant : items){
        if(itemVariant.type() != static_cast<QVariant::Type>(QMetaType::QVariantMap)){
            qCDebug(THEMEMANAGER) << " : Invalid settings type, skipping";
            continue;
        }
        QVariantMap itemMap = itemVariant.toMap();
        if(!itemMap.keys().contains("name") || !itemMap.keys().contains("label") || !itemMap.keys().contains("defaultValue")){
            qCDebug(THEMEMANAGER) << "Error processing color, missing required field(s), skipping";
            continue;
        }
        QVariantMap item({
                             {"label", itemMap.value("label")},
                             {"name", itemMap.value("name")},
                             {"defaultValue", itemMap.value("defaultValue")},
                             {"type", type}
                         });
        settingsList << item;
    }
    return settingsList;
}

ThemeManager::~ThemeManager(){
    for(SettingsLoader * settings : m_settings){
        delete(settings);
    }
    qDebug() << "Theme manager dead";
}
