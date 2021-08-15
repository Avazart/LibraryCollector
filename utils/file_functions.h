#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H
//---------------------------------------------------------------
#include <QString>
//---------------------------------------------------------------
namespace LibraryCollector
{
  bool isSubPath(const QString& dir,const QString& path);
  QString copyFile(const QString& filePath,
                   const QString& destinationDir ,
                   const QString& baseDir= QString());

  void makeSymLinks(const QString& filePath,const QString &newFilePath);
}
//---------------------------------------------------------------
#endif // FILEFUNCTIONS_H
