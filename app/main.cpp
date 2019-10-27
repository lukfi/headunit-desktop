#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QJsonObject>
#include <QVariantList>
#include <QQmlComponent>
#include <QCommandLineParser>
#include <QThreadPool>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QLoggingCategory>
#include <unistd.h>

#include "pluginmanager.h"
#include "thememanager.h"

Q_DECLARE_LOGGING_CATEGORY(HEADUNIT)

int main(int argc, char *argv[])
{
    setbuf(stdout, nullptr);

    QCoreApplication::setOrganizationName("viktorgino");
    QCoreApplication::setOrganizationDomain("https://github.com/viktorgino/headunit-desktop");
    QCoreApplication::setApplicationName("viktorgino's HeadUnit Desktop");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QLoggingCategory::setFilterRules("");

    QQmlApplicationEngine *engine = new QQmlApplicationEngine();

    QVariantList menuItems;
    QVariantList configItems;

    QCommandLineParser parser;
    parser.setApplicationDescription("helper");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption pluginsOption(QStringList() << "p" << "plugins",
    QCoreApplication::translate("main", "Plugins to enable (defaults to all)"),
    QCoreApplication::translate("main", "plugins")
    );
    parser.addOption(pluginsOption);

    parser.process(app);
    QString p = parser.value(pluginsOption);
    bool whitelist = parser.isSet(pluginsOption);
    QStringList plugins;
    if (whitelist) {
        plugins = p.split(" ",QString::SkipEmptyParts);
    }

    PluginManager pluginManager(engine, whitelist, plugins);
    ThemeManager themeManager(engine);

    QObject::connect(&pluginManager, &PluginManager::themeEvent, &themeManager, &ThemeManager::onEvent);
    QObject::connect(&themeManager, &ThemeManager::pluginEvent, &pluginManager, &PluginManager::onEvent);

    int ret = app.exec();

    return ret;
}
