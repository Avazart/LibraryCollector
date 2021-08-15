#include "file_functions.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

namespace LibraryCollector
{
//---------------------------------------------------------------
bool isSubPath(const QString &dir, const QString &path)
{
   if ( dir.isEmpty() ) return false;

   QString nativeDir = QDir::toNativeSeparators(dir);
   QString nativePath = QDir::toNativeSeparators(path);

   if( nativeDir.lastIndexOf(QDir::separator()) != nativeDir.size()-1 )
   {
     nativeDir += QDir::separator();
   }

   return !nativePath.indexOf( nativeDir, Qt::CaseInsensitive );
}
//---------------------------------------------------------------
QString copyFile(const QString &filePath,
                 const QString &destinationDir,
                 const QString &baseDir)
{
  QFile file(filePath);
  QFileInfo info(file);
  QDir fileDir= info.absoluteDir(); // директория DLL-ки

  QDir destDir(destinationDir);    // Куда копируем

  QString newFilePath;
  bool success= false;

  if(baseDir.isNull() || baseDir.isEmpty())
  {
    newFilePath= destinationDir+QDir::separator()+info.fileName();
    success= file.copy(newFilePath);
    return success?newFilePath:QString();
  }

  QDir  relDir(baseDir); // Полный путь к подпапке (относительно папки)
  QString path= relDir.relativeFilePath(fileDir.absolutePath()); //   назначение/plugins/

  //                     path            filename
  // [QTDIR(relDir)] + [plugins/... ] + fileName.dll
  // [destDir ]      + [plugins/... ] + fileName.dll
  newFilePath= destDir.absoluteFilePath(path)+QDir::separator()+info.fileName();
  success= destDir.mkpath(path) && file.copy(newFilePath);

  return success?newFilePath:QString();
}
//---------------------------------------------------------------
void makeSymLinks(const QString& filePath,
                  const QString& newFilePath)
{
  // Ищет симлинки рядом с файлом и создает симлинки
  // с таким же именем на новый файл (после копирования)
  QFileInfo libFileInfo(filePath);
  QFileInfo newLibFileInfo(newFilePath);
  QFileInfoList list= libFileInfo.dir().entryInfoList(QDir::Files);
  foreach (QFileInfo fileInfo, list)
  {
     if(fileInfo.isSymLink() &&
        QFileInfo(fileInfo.symLinkTarget())== libFileInfo )
     {
       QString newSymLinkFileName=
         newLibFileInfo.absolutePath()+QDir::separator()+fileInfo.fileName();
       QFile(newLibFileInfo.fileName()).link(newSymLinkFileName);
     }
  }
}
//------------------End my:: -------------------------------------------
}
