QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG(debug, debug | release)
{
LIBS += -L../objects/debug/ -lobjects
}
CONFIG(release, debug | release)
{
LIBS += -L../objects/release/ -lobjects
}

TEMPLATE = app

SOURCES +=  tst_test_lab5.cpp
