#include "fileinfo.h"

#include <QDebug>

FileInfo::FileInfo()
   :QObject()
{

}

FileInfo::FileInfo(const QString &file)
  :QObject(),
    fileInfo_(file)
{

}

FileInfo::FileInfo(const QDir &dir, const QString &file)
  :QObject(),
    fileInfo_(dir,file)
{

}

FileInfo::FileInfo(const FileInfo &fileinfo)
  :QObject(),
   fileInfo_(fileinfo.fileInfo_)
{

}

FileInfo::~FileInfo()
{
}

FileInfo &FileInfo::operator=(const FileInfo &fileinfo)
{
  fileInfo_ = fileinfo.fileInfo_;
  return *this;
}

bool FileInfo::operator==(const FileInfo &fileinfo) const
{
  return fileInfo_ == fileinfo.fileInfo_;
}

bool FileInfo::operator!=(const FileInfo &fileinfo) const
{
  return fileInfo_ != fileinfo.fileInfo_;
}

void FileInfo::setFile(const QString &file)
{
  fileInfo_.setFile(file);
}

void FileInfo::setFile(const QDir &dir, const QString &file)
{
  fileInfo_.setFile(dir,file);
}

bool FileInfo::exists() const
{
  return fileInfo_.exists();
}

bool FileInfo::exists(const QString &file)
{
  return QFileInfo::exists(file);
}

void FileInfo::refresh()
{
  fileInfo_.refresh();
}

QString FileInfo::filePath() const
{
  return fileInfo_.filePath();
}

QString FileInfo::absoluteFilePath() const
{
  return fileInfo_.absoluteFilePath();
}

QString FileInfo::canonicalFilePath() const
{
  return fileInfo_.canonicalFilePath();
}

QString FileInfo::fileName() const
{
  return fileInfo_.fileName();
}

QString FileInfo::baseName() const
{
  return fileInfo_.baseName();
}

QString FileInfo::completeBaseName() const
{
  return fileInfo_.completeBaseName();
}

QString FileInfo::suffix() const
{
  return fileInfo_.suffix();
}

QString FileInfo::bundleName() const
{
  return fileInfo_.bundleName();
}

QString FileInfo::completeSuffix() const
{
  return fileInfo_.completeSuffix();
}

QString FileInfo::path() const
{
  return fileInfo_.path();
}

QString FileInfo::absolutePath() const
{
  return fileInfo_.absolutePath();
}

QString FileInfo::canonicalPath() const
{
  return fileInfo_.canonicalPath();
}


//Dir FileInfo::dir() const
//{
//  return fileInfo_.dir();
//}

//Dir FileInfo::absoluteDir() const
//{
//  return fileInfo_.absoluteDir();
//}

bool FileInfo::isReadable() const
{
  return fileInfo_.isReadable();
}

bool FileInfo::isWritable() const
{
  return fileInfo_.isWritable();
}

bool FileInfo::isExecutable() const
{
  return fileInfo_.isExecutable();
}

bool FileInfo::isHidden() const
{
  return fileInfo_.isHidden();
}

bool FileInfo::isNativePath() const
{
  return fileInfo_.isNativePath();
}

bool FileInfo::isRelative() const
{
  return fileInfo_.isRelative();
}

bool FileInfo::isAbsolute() const
{
  return fileInfo_.isAbsolute();
}

bool FileInfo::makeAbsolute()
{
  return fileInfo_.makeAbsolute();
}

bool FileInfo::isFile() const
{
  return fileInfo_.isFile();
}

bool FileInfo::isDir() const
{
  return fileInfo_.isDir();
}

bool FileInfo::isSymLink() const
{
  return fileInfo_.isSymLink();
}

bool FileInfo::isRoot() const
{
  return fileInfo_.isRoot();
}

bool FileInfo::isBundle() const
{
  return fileInfo_.isBundle();
}

QString FileInfo::readLink() const
{
  return fileInfo_.readLink();
}

QString FileInfo::symLinkTarget() const
{
  return fileInfo_.symLinkTarget();
}

QString FileInfo::owner() const
{
  return fileInfo_.owner();
}

uint FileInfo::ownerId() const
{
  return fileInfo_.ownerId();
}

QString FileInfo::group() const
{
  return fileInfo_.group();
}

uint FileInfo::groupId() const
{
  return fileInfo_.groupId();
}

qint64 FileInfo::size() const
{
  return fileInfo_.size();
}

QDateTime FileInfo::created() const
{
  return fileInfo_.created();
}

QDateTime FileInfo::lastModified() const
{
  return fileInfo_.lastModified();
}

QDateTime FileInfo::lastRead() const
{
  return fileInfo_.lastRead();
}

bool FileInfo::caching() const
{
  return fileInfo_.caching();
}

void FileInfo::setCaching(bool on)
{
  fileInfo_.setCaching(on);
}
