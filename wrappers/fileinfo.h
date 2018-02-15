#ifndef FILEINFO_H
#define FILEINFO_H

#include <QObject>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

class Dir;

class FileInfo : public QObject
{
  Q_OBJECT
public:
  //explicit QFileInfo(QFileInfoPrivate *d);

  Q_INVOKABLE FileInfo();
  Q_INVOKABLE FileInfo(const QString &file);

  // QFileInfo(const QFile &file);
  Q_INVOKABLE FileInfo(const QDir &dir, const QString &file);
  Q_INVOKABLE FileInfo(const FileInfo &fileinfo);
  ~FileInfo();

  Q_INVOKABLE FileInfo &operator=(const FileInfo &fileinfo);

//#ifdef Q_COMPILER_RVALUE_REFS
//  Q_INVOKABLE QFileInfo &operator=(QFileInfo &&other) Q_DECL_NOTHROW { swap(other); return *this; }
//#endif

  //void swap(QFileInfo &other) Q_DECL_NOTHROW
  //{ qSwap(d_ptr, other.d_ptr); }

  Q_INVOKABLE bool operator==(const FileInfo &fileinfo) const;
  Q_INVOKABLE bool operator!=(const FileInfo &fileinfo) const;

  Q_INVOKABLE void setFile(const QString &file);
  //void setFile(const QFile &file);
  Q_INVOKABLE void setFile(const QDir &dir, const QString &file);
  Q_INVOKABLE bool exists() const;
  Q_INVOKABLE static bool exists(const QString &file);
  Q_INVOKABLE void refresh();

  Q_INVOKABLE QString filePath() const;
  Q_INVOKABLE QString absoluteFilePath() const;
  Q_INVOKABLE QString canonicalFilePath() const;
  Q_INVOKABLE QString fileName() const;
  Q_INVOKABLE QString baseName() const;
  Q_INVOKABLE QString completeBaseName() const;
  Q_INVOKABLE QString suffix() const;
  Q_INVOKABLE QString bundleName() const;
  Q_INVOKABLE QString completeSuffix() const;

  Q_INVOKABLE QString path() const;
  Q_INVOKABLE QString absolutePath() const;
  Q_INVOKABLE QString canonicalPath() const;

// Q_INVOKABLE Dir dir() const;
//  Q_INVOKABLE Dir absoluteDir() const;

  Q_INVOKABLE bool isReadable() const;
  Q_INVOKABLE bool isWritable() const;
  Q_INVOKABLE bool isExecutable() const;
  Q_INVOKABLE bool isHidden() const;
  Q_INVOKABLE bool isNativePath() const;

  Q_INVOKABLE bool isRelative() const;
  Q_INVOKABLE bool isAbsolute() const;
  Q_INVOKABLE bool makeAbsolute();

  Q_INVOKABLE bool isFile() const;
  Q_INVOKABLE bool isDir() const;
  Q_INVOKABLE bool isSymLink() const;
  Q_INVOKABLE bool isRoot() const;
  Q_INVOKABLE bool isBundle() const;

  Q_INVOKABLE QString readLink() const;
  Q_INVOKABLE QString symLinkTarget() const;

  Q_INVOKABLE QString owner() const;
  Q_INVOKABLE uint ownerId() const;
  Q_INVOKABLE QString group() const;
  Q_INVOKABLE uint groupId() const;

  //bool permission(QFile::Permissions permissions) const;
  //QFile::Permissions permissions() const;

  Q_INVOKABLE qint64 size() const;

  Q_INVOKABLE QDateTime created() const;
  Q_INVOKABLE QDateTime lastModified() const;
  Q_INVOKABLE QDateTime lastRead() const;

  Q_INVOKABLE bool caching() const;
  Q_INVOKABLE void setCaching(bool on);

signals:

public slots:

private:
   QFileInfo fileInfo_;
};

#endif // FILEINFO_H
