#include "dir.h"

Dir::Dir(const QDir &dir)
  :QObject(),
    dir_(dir)
{

}

Dir::Dir(const Dir &dir)
  :QObject(),
    dir_(dir.dir_)
{

}

Dir::Dir(const QString &path)
  :QObject(),
    dir_(path)
{

}

Dir::Dir(const QString &path,
         const QString &nameFilter,
         Dir::SortFlags sort,
         Dir::Filters filter)
  :QObject(),
   dir_(path,
        nameFilter,
        (QDir::SortFlags)(int)sort,
        (QDir::Filters)(int)filter)
{

}

Dir::~Dir()
{

}

Dir &Dir::operator=(const Dir &dir)
{
  dir_ = dir.dir_;
  return *this;
}

Dir &Dir::operator=(const QString &path)
{
  dir_ = path;
  return *this;
}

void Dir::setPath(const QString &path)
{
  dir_.setPath(path);
}

QString Dir::path() const
{
  return dir_.path();
}

QString Dir::absolutePath() const
{
 return dir_.absolutePath();
}

QString Dir::canonicalPath() const
{
   return dir_.canonicalPath();
}

/*static*/
void Dir::addResourceSearchPath(const QString &path)
{
  return QDir::addResourceSearchPath(path);
}

/*static*/
void Dir::setSearchPaths(const QString &prefix, const QStringList &searchPaths)
{
  QDir::setSearchPaths(prefix,searchPaths);
}

void Dir::addSearchPath(const QString &prefix, const QString &path)
{
  QDir::addSearchPath(prefix,path);
}

/*static*/
QStringList Dir::searchPaths(const QString &prefix)
{
  return QDir::searchPaths(prefix);
}

QString Dir::dirName() const
{
  return dir_.dirName();
}

QString Dir::filePath(const QString &fileName) const
{
   return dir_.filePath(fileName);
}

QString Dir::absoluteFilePath(const QString &fileName) const
{
   return dir_.absoluteFilePath(fileName);
}

QString Dir::relativeFilePath(const QString &fileName) const
{
 return dir_.relativeFilePath(fileName);
}

QString Dir::toNativeSeparators(const QString &pathName)
{
 return QDir::toNativeSeparators(pathName);
}

QString Dir::fromNativeSeparators(const QString &pathName)
{
 return QDir::fromNativeSeparators(pathName);
}

bool Dir::cd(const QString &dirName)
{
  return dir_.cd(dirName);
}

bool Dir::cdUp()
{
 return dir_.cdUp();
}

QStringList Dir::nameFilters() const
{
  return dir_.nameFilters();
}

void Dir::setNameFilters(const QStringList &nameFilters)
{
  dir_.setNameFilters(nameFilters);
}

Dir::Filters Dir::filter() const
{
  Dir::Filters filters_=  (int)dir_.filter();
  return filters_;
}

void Dir::setFilter(Dir::Filters filter)
{
  QDir::Filters filter_=  (int)filter;
  dir_.setFilter(filter_);
}

Dir::SortFlags Dir::sorting() const
{
  Dir::SortFlags sorting_=  (int)dir_.sorting();
  return sorting_;
}

void Dir::setSorting(Dir::SortFlags sort)
{
  QDir::SortFlags sort_= (int)sort;
  dir_.setSorting(sort_);
}

uint Dir::count() const
{
  return dir_.count();
}

bool Dir::isEmpty(Dir::Filters filters) const
{
  QDir::Filters filters_=  (int)filters;
  return dir_.isEmpty(filters_);
}

QString Dir::operator[](int i) const
{
  return dir_[i];
}

/*static*/
QStringList Dir::nameFiltersFromString(const QString &nameFilter)
{
  return QDir::nameFiltersFromString(nameFilter);
}

QStringList Dir::entryList1(Filters filters,Dir::SortFlags sort) const
{
  QDir::Filters filters_=  (int)filters;
  QDir::SortFlags sort_=   (int)sort;
  return dir_.entryList(filters_,sort_);
}

QStringList Dir::entryList2(const QStringList &nameFilters,
                            Dir::Filters filters,
                            Dir::SortFlags sort) const
{
  QDir::Filters filters_=  (int)filters;
  QDir::SortFlags sort_=   (int)sort;
  return dir_.entryList(nameFilters,filters_,sort_);
}

bool Dir::mkdir(const QString &dirName) const
{
   return dir_.mkdir(dirName);
}

bool Dir::rmdir(const QString &dirName) const
{
  return dir_.rmdir(dirName);
}

bool Dir::mkpath(const QString &dirPath) const
{
  return dir_.mkpath(dirPath);
}

bool Dir::rmpath(const QString &dirPath) const
{
  return dir_.rmpath(dirPath);
}

bool Dir::removeRecursively()
{
  return dir_.removeRecursively();
}

bool Dir::isReadable() const
{
  return dir_.isReadable();
}

bool Dir::exists() const
{
  return dir_.exists();
}

bool Dir::isRoot() const
{
  return dir_.isRoot();
}

/*static*/
bool Dir::isRelativePath(const QString &path)
{
  return QDir::isRelativePath(path);
}

/*static*/
bool Dir::isAbsolutePath(const QString &path)
{
    return QDir::isAbsolutePath(path);
}

bool Dir::isRelative() const
{
    return dir_.isRelative();
}

bool Dir::isAbsolute() const
{
  return dir_.isAbsolute();
}

bool Dir::makeAbsolute()
{
   return dir_.makeAbsolute();
}

bool Dir::operator==(const Dir &dir) const
{
  return dir_== dir.dir_;
}

bool Dir::operator!=(const Dir &dir) const
{
  return dir_!= dir.dir_;
}

bool Dir::remove(const QString &fileName)
{
   return  dir_.remove(fileName);
}

bool Dir::rename(const QString &oldName, const QString &newName)
{
  return  dir_.rename(oldName,newName);
}

bool Dir::exists(const QString &name) const
{
 return dir_.exists(name);
}

/*static*/
QChar Dir::separator()
{
  return QDir::separator();
}

/*static*/
bool Dir::setCurrent(const QString &path)
{
  return QDir::setCurrent(path);
}

/*static*/
Dir Dir::current()
{
  return QDir::current();
}

/*static*/
QString Dir::currentPath()
{
  return QDir::currentPath();
}

/*static*/
Dir Dir::home()
{
  return QDir::home();
}

/*static*/
QString Dir::homePath()
{
  return QDir::homePath();
}

/*static*/
Dir Dir::root()
{
  return QDir::root();
}

/*static*/
QString Dir::rootPath()
{
  return QDir::rootPath();
}

/*static*/
Dir Dir::temp()
{
  return QDir::temp();
}

/*static*/
QString Dir::tempPath()
{
  return QDir::tempPath();
}

/*static*/
QString Dir::cleanPath(const QString &path)
{
  return QDir::cleanPath(path);
}

void Dir::refresh() const
{
  dir_.refresh();
}
