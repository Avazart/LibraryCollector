#include "dependencies.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "selectwindow_linux.h"
//--------------------------------------------------------------------
My::PidType My::processIdByFilePath(const QString& processFilePath,QString& error)
{
  QDir procDir("/proc");
  QStringList files= procDir.entryList();
  foreach(QString pidFileName,files)
  {
     bool isProcFile= false;
     PidType pid= pidFileName.toInt(&isProcFile);
     if(isProcFile)
     {
       QString filePath= QFile::symLinkTarget("/proc/"+pidFileName+"/exe");
       if(!filePath.isEmpty() && filePath==processFilePath)
       {
         error.clear();
         return pid;
       }
     }
  }
  error=
    QObject::tr("Can not find process \"%1\"")
      .arg(processFilePath);
  return -1;
}
//--------------------------------------------------------------------
QStringList My::librariesByPid(My::PidType pid,QString& error)
{
    QStringList modules;
    QDir procDir("/proc");
    QStringList files= procDir.entryList();
    foreach(QString fileName,files)
    {
        bool isProcFile= false;
        PidType id= fileName.toInt(&isProcFile);
        if(isProcFile && id== pid)
        {
           QFile mapsFile("/proc/"+fileName+"/maps");
           if(!mapsFile.open(QIODevice::ReadOnly| QIODevice::Text))
           {
              error= QObject::tr("Can not open file \"%1\"")
                             .arg(mapsFile.fileName());
              return QStringList();
           }

           // exemples :
           //b75cf000-b75e8000 r--p 00642000 08:05 307931     /opt/Qt/5.4/gcc/lib/libQt5Widgets.so.5.4.1
           //b7606000-b7607000 rw-p 00000000 00:00 0
           //b7779000-b777a000 r-xp 00000000 00:00 0          [vdso]
           //b777a000-b777c000 r--p 00000000 00:00 0          [vvar]

           QRegExp re("[0-9a-f]{8}-[0-9a-f]{8}\\s"
                      "\\D{4}\\s"
                      "[0-9a-f]{8}\\s"
                      "\\d{2}:\\d{2}\\s"
                      "\\d+\\s+"
                      "(/.*\\.so(\\.\\d+){0,3})");

           QTextStream in(&mapsFile);
           QString line= in.readLine();
           while(!line.isNull())
           {
             if(re.exactMatch(line))
                modules.append(re.cap(1));
             line= in.readLine();
           }

           modules.removeDuplicates();
           error.clear();
           return modules;
        }
    }

   error= QObject::tr("Can not find process with pid=%1").arg(pid);
   return QStringList();
}
//--------------------------------------------------------------------
QString processFilePathByPid(My::PidType pid,QString& error)
{
    QDir procDir("/proc");
    QStringList files= procDir.entryList();
    foreach(QString pidFileName,files)
    {
       bool isProcFile= false;
       My::PidType id= pidFileName.toInt(&isProcFile);
       if(isProcFile && id==pid)
       {
         QString processFilePath=
            QFile::symLinkTarget("/proc/"+pidFileName+"/exe");
         if(!processFilePath.isEmpty())
         {
           error.clear();
           return processFilePath;
         }
         else
         {
           error=
             QObject::tr("Can not get simlink target \"proc/%1/exe\"").arg(pid);
           return QString();
         }
       }
    }
    error=
      QObject::tr("Process with pid=%1 not found.").arg(pid);
    return QString();
}
//--------------------------------------------------------------------
QString My::processFilePathBySelectedWindow(QString& error)
{
  My::PidType pid= pidBySelectedWindow(error);
  if(error.isEmpty())
  {
    QString processFilePath= processFilePathByPid(pid,error);
    if(error.isEmpty())
      return QFileInfo(processFilePath).absoluteFilePath();
  }
  return QString();
}
//--------------------------------------------------------------------
