#ifndef DIR_H
#define DIR_H

#include <QObject>
#include <QDir>
#include <QDebug>

class Dir : public QObject
{
  Q_OBJECT
public:
  enum Filter { Dirs        = 0x001,
                Files       = 0x002,
                Drives      = 0x004,
                NoSymLinks  = 0x008,
                AllEntries  = Dirs | Files | Drives,
                TypeMask    = 0x00f,

                Readable    = 0x010,
                Writable    = 0x020,
                Executable  = 0x040,
                PermissionMask    = 0x070,

                Modified    = 0x080,
                Hidden      = 0x100,
                System      = 0x200,

                AccessMask  = 0x3F0,

                AllDirs       = 0x400,
                CaseSensitive = 0x800,
                NoDot         = 0x2000,
                NoDotDot      = 0x4000,
                NoDotAndDotDot = NoDot | NoDotDot,

                NoFilter = -1
  };

  Q_ENUM(Filter)
  Q_DECLARE_FLAGS(Filters, Filter)
  Q_FLAGS(Filters)

  enum SortFlag { Name        = 0x00,
                  Time        = 0x01,
                  Size        = 0x02,
                  Unsorted    = 0x03,
                  SortByMask  = 0x03,

                  DirsFirst   = 0x04,
                  Reversed    = 0x08,
                  IgnoreCase  = 0x10,
                  DirsLast    = 0x20,
                  LocaleAware = 0x40,
                  Type        = 0x80,
                  NoSort = -1
  };

  Q_ENUM(SortFlag)
  Q_DECLARE_FLAGS(SortFlags, SortFlag)
  Q_FLAGS(SortFlags)

  Dir(const QDir &dir);

  Q_INVOKABLE Dir(const Dir &dir);
  Q_INVOKABLE Dir(const QString &path = QString());
  Q_INVOKABLE Dir(const QString &path, const QString &nameFilter,
       SortFlags sort = SortFlags(SortFlag::Name | SortFlag::IgnoreCase),
       Filters filter = Filter::AllEntries);
  ~Dir();

  Q_INVOKABLE Dir &operator=(const Dir & dir);
  Q_INVOKABLE Dir &operator=(const QString &path);
//#ifdef Q_COMPILER_RVALUE_REFS
//  Dir &operator=(Dir &&other) Q_DECL_NOTHROW { swap(other); return *this; }
//#endif

//  void swap(QDir &other) Q_DECL_NOTHROW
//  { qSwap(d_ptr, other.d_ptr); }


  Q_INVOKABLE void setPath(const QString &path);
  Q_INVOKABLE QString path() const;
  Q_INVOKABLE QString absolutePath() const;
  Q_INVOKABLE QString canonicalPath() const;


  Q_INVOKABLE static void setSearchPaths(const QString &prefix, const QStringList &searchPaths);
  Q_INVOKABLE static void addSearchPath(const QString &prefix, const QString &path);
  Q_INVOKABLE static QStringList searchPaths(const QString &prefix);

  Q_INVOKABLE QString dirName() const;
  Q_INVOKABLE QString filePath(const QString &fileName) const;
  Q_INVOKABLE QString absoluteFilePath(const QString &fileName) const;
  Q_INVOKABLE QString relativeFilePath(const QString &fileName) const;

  Q_INVOKABLE static QString toNativeSeparators(const QString &pathName);
  Q_INVOKABLE static QString fromNativeSeparators(const QString &pathName);

  Q_INVOKABLE bool cd(const QString &dirName);
  Q_INVOKABLE bool cdUp();

  Q_INVOKABLE QStringList nameFilters() const;
  Q_INVOKABLE void setNameFilters(const QStringList &nameFilters);

  Q_INVOKABLE Filters filter() const;
  Q_INVOKABLE void setFilter(Filters filter);
  Q_INVOKABLE SortFlags sorting() const;
  Q_INVOKABLE void setSorting(SortFlags sort);

  Q_INVOKABLE uint count() const;
  Q_INVOKABLE  bool isEmpty(Filters filters = Filters(Filter::AllEntries | Filter::NoDotAndDotDot)) const;

  Q_INVOKABLE QString operator[](int i) const;

  Q_INVOKABLE  static QStringList nameFiltersFromString(const QString &nameFilter);

  Q_INVOKABLE  QStringList entryList1(Filters filters = Filter::NoFilter,
                                      SortFlags sort = SortFlag::NoSort) const;

  Q_INVOKABLE  QStringList entryList2(const QStringList &nameFilters,
                         Filters filters = Filter::NoFilter,
                         SortFlags sort = SortFlag::NoSort) const;

//  QFileInfoList entryInfoList(Filters filters = NoFilter, SortFlags sort = NoSort) const;
//  QFileInfoList entryInfoList(const QStringList &nameFilters, Filters filters = NoFilter,
//                              SortFlags sort = NoSort) const;

  Q_INVOKABLE bool mkdir(const QString &dirName) const;
  Q_INVOKABLE bool rmdir(const QString &dirName) const;
  Q_INVOKABLE bool mkpath(const QString &dirPath) const;
  Q_INVOKABLE bool rmpath(const QString &dirPath) const;

  Q_INVOKABLE bool removeRecursively();

  Q_INVOKABLE bool isReadable() const;
  Q_INVOKABLE bool exists() const;
  Q_INVOKABLE bool isRoot() const;

  Q_INVOKABLE static bool isRelativePath(const QString &path);
  Q_INVOKABLE static bool isAbsolutePath(const QString &path);
  Q_INVOKABLE bool isRelative() const;
  Q_INVOKABLE bool isAbsolute() const;
  Q_INVOKABLE bool makeAbsolute();

  Q_INVOKABLE bool operator==(const Dir &dir) const;
  Q_INVOKABLE bool operator!=(const Dir &dir) const;

  Q_INVOKABLE bool remove(const QString &fileName);
  Q_INVOKABLE bool rename(const QString &oldName, const QString &newName);
  Q_INVOKABLE bool exists(const QString &name) const;

//  static QFileInfoList drives();

//  Q_DECL_CONSTEXPR static inline QChar listSeparator() Q_DECL_NOTHROW
//  {
//#if defined(Q_OS_WIN)
//      return QLatin1Char(';');
//#else
//      return QLatin1Char(':');
//#endif
//  }

  Q_INVOKABLE static QChar separator();

  Q_INVOKABLE static bool setCurrent(const QString &path);
  Q_INVOKABLE static Dir current();
  Q_INVOKABLE static QString currentPath();

  Q_INVOKABLE static Dir home();
  Q_INVOKABLE static QString homePath();
  Q_INVOKABLE static Dir root();
  Q_INVOKABLE static QString rootPath();
  Q_INVOKABLE static Dir temp();
  Q_INVOKABLE static QString tempPath();

//#ifndef QT_NO_REGEXP
//  static bool match(const QStringList &filters, const QString &fileName);
//  static bool match(const QString &filter, const QString &fileName);
//#endif

  Q_INVOKABLE static QString cleanPath(const QString &path);
  Q_INVOKABLE void refresh() const;
private:
    QDir dir_;
};


//Q_DECLARE_METATYPE(Dir::Filter)
//Q_DECLARE_METATYPE(Dir::SortFlag)

//Q_DECLARE_METATYPE(Dir::Filters)
//Q_DECLARE_METATYPE(Dir::SortFlags)

//Q_DECLARE_OPERATORS_FOR_FLAGS(Dir::Filters)
//Q_DECLARE_OPERATORS_FOR_FLAGS(Dir::SortFlags)

#endif // DIR_H
