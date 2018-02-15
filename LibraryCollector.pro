#-------------------------------------------------
#
# Project created by QtCreator 2014-06-05T13:10:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets qml

TARGET = LibraryCollector
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    treewidgetitem.cpp \
    toolbutton.cpp \
    utils/file_functions.cpp \
    wrappers/fileinfo.cpp \
    wrappers/wrappers.cpp \
    wrappers/dir.cpp

HEADERS  += \
    mainwindow.h\
    treewidgetitem.h \
    toolbutton.h \
    utils/dependencies.h \
    utils/file_functions.h \
    wrappers/wrappers.h \
    wrappers/fileinfo.h \
    wrappers/dir.h


FORMS    += \
    mainwindow.ui

INCLUDEPATH += utils wrappers

win32|win64{
  SOURCES += utils/dependencies_win.cpp
  LIBS+= -lpsapi -lUser32

  RC_FILE=  rc.rc
  OTHER_FILES+= rc.rc
}

linux{
  HEADERS += utils/selectwindow_linux.h
  SOURCES += utils/selectwindow_linux.cpp \
       utils/dependencies_linux.cpp

  LIBS+= -lX11 -lXmu
}
