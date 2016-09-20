#ifndef SELECTWINDOW_LINUX_H
#define SELECTWINDOW_LINUX_H
//-------------------------------------------------------------
#include <sys/types.h> // pid_t
#include <QString>
#include <QObject> // tr()
//-------------------------------------------------------------
pid_t pidBySelectedWindow(QString &error);
//-------------------------------------------------------------
#endif // SELECTWINDOW_LINUX_H
