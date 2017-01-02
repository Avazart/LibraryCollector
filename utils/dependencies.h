#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H
//------------------------------------
#include <QString>
#include <QObject> // tr()
#include <QStringList>
//------------------------------------
#ifdef Q_OS_WIN
  #include <Windows.h> // DWORD
  namespace LibraryCollector
  {
    typedef DWORD PidType;
  }
#elif defined Q_OS_LINUX
  #include <sys/types.h> // pid_t
  namespace LibraryCollector
  {
    typedef pid_t PidType;
  }
#endif
//------------------------------------
namespace LibraryCollector
{
  QStringList librariesByPid(PidType pid,QString& error);
  PidType processIdByFilePath(const QString& processFilePath, QString& error);
#ifdef Q_OS_WIN
  QString processFilePathByPoint(long x, long y, QString& error);
#elif defined Q_OS_LINUX
  QString processFilePathBySelectedWindow(QString& error);
#endif
}
//------------------------------------
#endif
