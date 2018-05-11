TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    libRadiusClient/clsRadiusClient.cpp \
    libRadiusClient/md5.cpp \
    libRadiusClient/RadiusAddress.cpp \
    libRadiusClient/RadiusAttribute.cpp \
    libRadiusClient/RadiusClientConnection.cpp \
    libRadiusClient/RadiusClientStack.cpp \
    libRadiusClient/RadiusPacket.cpp \
    libRadiusClient/RadiusSecret.cpp \
    server.cpp \
    libRadiusClient/RadiusServerStack.cpp

HEADERS += \
    libRadiusClient/clsRadiusClient.h \
    libRadiusClient/md5.h \
    libRadiusClient/Radius.h \
    libRadiusClient/RadiusAddress.h \
    libRadiusClient/RadiusAttribute.h \
    libRadiusClient/RadiusClientConnection.h \
    libRadiusClient/RadiusClientStack.h \
    libRadiusClient/RadiusPacket.h \
    libRadiusClient/RadiusSecret.h \
    libRadiusClient/config.h \
    libRadiusClient/log.h \
    libRadiusClient/RadiusServerStack.h
