TEMPLATE = app
CONFIG += c++14

SOURCES += \
        BluezSocket.cpp \
        CachingPurposeVector.cpp \
        Message.cpp \
        PackageEncapsulation.cpp \
        PhoneReceiver.cpp \
        SensorHandler.cpp \
        SignalProcessPurposeVector.cpp \
        main.cpp

HEADERS += \
    BluezSocket.h \
    CachingPurposeVector.h \
    Message.h \
    PackageEncapsulation.h \
    PhoneReceiver.h \
    SensorHandler.h \
    SignalProcessPurposeVector.h \
    define.h

LIBS += -lpthread
