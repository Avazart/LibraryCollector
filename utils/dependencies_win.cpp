#include "dependencies.h"

#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include <QFileInfo>
#include <QDebug>
//---------------------------------------------------------------------
QString processFilePathByPid(LibraryCollector::PidType pid,
                             QString& error)
{
  HANDLE  process=
    OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, pid);
  if(!process)
  {
    error=
      QObject::tr("Can not open process with pid=%1.").arg(pid);
    return QString();
  }

  wchar_t bufFileName[MAX_PATH+1]= {0};
  DWORD rc= GetModuleFileNameEx(process,NULL,bufFileName,MAX_PATH);
  CloseHandle(process);
  if(!rc)
  {
    error=
       QObject::tr("Can not get module filename.");
    return QString();
  }

  error.clear();
  return QString::fromWCharArray(bufFileName);
}
//------------------------------------------------------------------
LibraryCollector::PidType
   LibraryCollector::processIdByFilePath(const QString& processFilePath,
                                         QString &error)
{
  QFileInfo processFileInfo(processFilePath);

  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);

  HANDLE hProcessSnap=
      CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

  if(hProcessSnap == INVALID_HANDLE_VALUE)
  {
    error=
      QObject::tr("CreateToolhelp32Snapshot() return %1.")
        .arg("INVALID_HANDLE_VALUE");
    return -1;
  }

  if(Process32First(hProcessSnap, &pe32))
  {
    do
    {
      QString fileName= QString::fromWCharArray(pe32.szExeFile);
      if(fileName==processFileInfo.fileName() && //совпадают имена процессов
         // и совпадают полные пути к файлу (c учетом разделителей)
         QFileInfo(processFilePathByPid(pe32.th32ProcessID,error))==processFileInfo)
      {
        CloseHandle(hProcessSnap);
        error.clear();
        return  pe32.th32ProcessID; // return DWORD
      }
    }
    while ( Process32Next(hProcessSnap, &pe32) );
  }
  CloseHandle(hProcessSnap);
  error=
    QObject::tr("Can not find process \"%1\".").arg(processFilePath);
  return -1;
}
//------------------------------------------------------------------
QStringList
     LibraryCollector::librariesByPid(LibraryCollector::PidType pid,
                                      QString &error)
{
  QStringList modules;
  MODULEENTRY32 mod;
  mod.dwSize=  sizeof(MODULEENTRY32);

  HANDLE hSnap;
  hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
  if(hSnap ==  INVALID_HANDLE_VALUE)
  {
    error=
      QObject::tr("CreateToolhelp32Snapshot() return %1.")
        .arg("INVALID_HANDLE_VALUE");
    return modules;
  }
  else
  {
    if (Module32First(hSnap, &mod))
    {
      modules.push_back(QString::fromWCharArray(mod.szExePath));
      while (Module32Next(hSnap, &mod))
      {
        modules.push_back(QString::fromWCharArray(mod.szExePath));
      }
    }
    CloseHandle(hSnap);
  }
  error.clear();
  return modules;
}
//---------------------------------------------------------------------
QString LibraryCollector::processFilePathByPoint(long x, long y, QString& error)
{
  POINT point={x,y};

  HWND someWnd= WindowFromPoint(point);
  if(!someWnd)
  {
    error= QObject::tr("Can not find window.");
    return QString();
  }

  HWND rootWnd= GetAncestor(someWnd,GA_ROOT);
  if(!rootWnd)
  {
    error= QObject::tr("Can not get root window.");
    return QString();
  }

  DWORD procId;
  if(!GetWindowThreadProcessId(rootWnd,&procId))
  {
    error= QObject::tr("Can not get window thread pid.");
    return QString();
  }

  return processFilePathByPid(procId,error);
}
//---------------------------------------------------------------------

