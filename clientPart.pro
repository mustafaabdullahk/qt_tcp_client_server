QT += core gui network widgets
QT += 3dcore-private

TEMPLATE = app
TARGET = client

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

SOURCES += main.cpp\
        client_test.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    client_test.h

FORMS    += mainwindow.ui
