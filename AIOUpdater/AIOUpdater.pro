QT  +=  core gui
QT  +=  network
QT  += gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG      += warn_off

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Downloader/downloadtool.cpp \
    Downloader/httpdownload.cpp \
    Downloader/jsonManager.cpp \
    Logging/logmanager.cpp \
    Logging/updatelog.cpp \
    Ui/updaterdialog.cpp \
    Updater/compredecomprefilethread.cpp \
    Updater/externalappswitch.cpp \
    Updater/rollbackmechanism.cpp \
    VersionInfo/appconfig.cpp \
    VersionInfo/httpcommon.cpp \
    VersionInfo/obslinkdto.cpp \
    VersionInfo/sysfile.cpp \
    VersionInfo/updateverquerydto.cpp \
    VersionInfo/upgradeaccess.cpp \
    aioupdater.cpp \
    main.cpp

HEADERS += \
    Downloader/downloadtool.h \
    Downloader/httpdownload.h \
    Downloader/jsonManager.h \
    Logging/MessageHeader.hpp \
    Logging/logmanager.h \
    Logging/updatelog.h \
    Ui/updaterdialog.h \
    Updater/compredecomprefilethread.h \
    Updater/externalappswitch.h \
    Updater/rollbackmechanism.h \
    VersionInfo/appconfig.h \
    VersionInfo/entitybase.h \
    VersionInfo/httpcommon.h \
    VersionInfo/obslinkdto.h \
    VersionInfo/sysfile.h \
    VersionInfo/updateverquerydto.h \
    VersionInfo/upgradeaccess.h \
    VersionInfo/zjsontools.hpp \
    aioupdater.h

FORMS += \
    Ui/updaterdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
