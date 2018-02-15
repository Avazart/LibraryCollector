#include "wrappers.h"

#include <QDebug>
//-----------------------------------------------------------------------------------------
//  Log
//-----------------------------------------------------------------------------------------
Log::Log(QTextEdit *parent)
  :QObject(parent),
    textEdit_(parent)
{

}

void Log::addInfo(const QString &text,QString color)
{
  textEdit_->append(QString("<span style='color: %2;'>%1</span>").arg(text).arg(color));
  textEdit_->moveCursor(QTextCursor::End);
}

void Log::addError(const QString &text)
{
  addInfo(text,"red");
}
//-----------------------------------------------------------------------------------------
//   Tree
//-----------------------------------------------------------------------------------------
Tree::Tree(QTreeWidget *parent)
  :QObject(parent),
    treeWidget_(parent)
{

}

int Tree::addGroup(const QString &groupName)
{
  TreeWidgetItem *item = new TreeWidgetItem(treeWidget_);
  item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
  item->setText(0,groupName);
  item->setCheckState(0, Qt::Unchecked);
  return treeWidget_->indexOfTopLevelItem(item);
}

void Tree::addLib(int groupIndex, const QString &libName, bool checked)
{
  QTreeWidgetItem *groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return;

  TreeWidgetItem *item = new TreeWidgetItem(groupItem);
  item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
  item->setText(0,libName);
  item->setCheckState(0, checked? Qt::Checked :  Qt::Unchecked);
}

void Tree::expandGroup(int groupIndex)
{
  QTreeWidgetItem *groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return;

  treeWidget_->expandItem(groupItem);
}

int Tree::groupCount() const
{
  return treeWidget_->topLevelItemCount();
}

int Tree::libCount(int groupIndex) const
{
  QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return 0;

  return groupItem->childCount();
}

QString Tree::groupName(int groupIndex) const
{
  QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return QString();

  return groupItem->text(0);
}

QString Tree::libName(int groupIndex, int libIndex) const
{
  QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return QString();

  if(libIndex >= groupItem->childCount())
    return QString();

  QTreeWidgetItem *libItem= groupItem->child(libIndex);
  return libItem->text(0);
}

bool Tree::libIsChecked(int groupIndex, int libIndex) const
{
  QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
  if(groupItem==Q_NULLPTR)
    return false;

  if(libIndex >= groupItem->childCount())
    return false;

  QTreeWidgetItem *libItem= groupItem->child(libIndex);
  return libItem->checkState(0)==Qt::Checked;
}
//--------------------------------------------------------------------------------------
void Tree::clearGroups()
{
  treeWidget_->clear();
  treeWidget_->setColumnCount(1);
}
//--------------------------------------------------------------------------------------
void Tree::clearLibs()
{
  for(int groupIndex=0; groupIndex<treeWidget_->topLevelItemCount(); ++groupIndex)
  {
    QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
    if(groupItem==Q_NULLPTR)
      return;

    foreach(QTreeWidgetItem* child, groupItem->takeChildren())
      delete child;
  }
}
//--------------------------------------------------------------------------------------
void Tree::simplify()
{
  for(int groupIndex=0; groupIndex<treeWidget_->topLevelItemCount(); ++groupIndex)
  {
    QTreeWidgetItem* groupItem = treeWidget_->topLevelItem(groupIndex);
    if(groupItem==Q_NULLPTR)
      return;

    groupItem->setHidden(groupItem->childCount()==0);
  }
}
//--------------------------------------------------------------------------------------
//   Utils
//--------------------------------------------------------------------------------------
Utils::Utils(QObject *parent)
  :QObject(parent)
{

}
//--------------------------------------------------------------------------------------
bool Utils::isSubPath(const QString &dir, const QString &path)
{
  return LibraryCollector::isSubPath(dir,path);
}
//--------------------------------------------------------------------------------------
QString Utils::copyFile(const QString &filePath,
                     const QString &destinationDir,
                     const QString &baseDir)
{
  return LibraryCollector::copyFile(filePath,destinationDir,baseDir);
}
//--------------------------------------------------------------------------------------
template<class ForwardIt1, class ForwardIt2>
ForwardIt1 partical_search(ForwardIt1 first, ForwardIt1 last,
                           ForwardIt2 s_first, ForwardIt2 s_last)
{
  for(;;++first)
  {
    ForwardIt1 it= first;
    for(ForwardIt2 s_it= s_first; ; ++it, ++s_it)
    {
      if(s_it==s_last)
        return first;

      if(it==last)
        return first; // !!!   particle

      if(!(*it==*s_it))
        break;
    }
  }
}
// ---------------------------------------------------------------------------
enum SearchResult { Found, Partical, NotFound };
// ---------------------------------------------------------------------------
SearchResult searchValueName(const QByteArray& buffer,
                             const QByteArray& valueName,
                             int& pos)
{
  QByteArray::const_iterator ci= partical_search(buffer.cbegin()+pos,buffer.cend(),
                                                 valueName.cbegin(),valueName.cend());
  pos= std::distance(buffer.cbegin(),ci);
  if(ci==buffer.cend())
  {
    return NotFound;
  }
  else
  {
    if(buffer.size()-pos<valueName.size())
      return Partical;
    else
      return Found;
  }
}
//--------------------------------------------------------------------------------------
bool Utils::patchFile(const QString &fileName,
                      const QByteArray &valueName,
                      const QByteArray &newValue)
{
  QFile file(fileName);
  if(!file.open(QIODevice::ReadWrite))
    return false;

  qDebug()<<file.isSequential();

  const int bufferSize = 512;
  QByteArray buffer;
  buffer.reserve(bufferSize*2);

  int pos = 0;
  while(!file.atEnd())
  {
    buffer+= file.read(bufferSize);

    SearchResult result= searchValueName(buffer,valueName,pos);
    switch(result)
    {
      case NotFound:
      {
        buffer.clear();
        pos = 0;
        continue;
      }
      case Partical:
      {
        continue;
      }
      case Found:
      {
         int begin= pos+ valueName.size();
         int end = buffer.indexOf('\0',begin);

         while(end==-1)
         {
            buffer += file.read(bufferSize);
            end = buffer.indexOf('\0',begin);
            if(file.atEnd())
              return false;
         }

         const int length= end-begin;
         const int place = file.pos()-(buffer.size()-begin);

         file.seek(place);
         file.write(newValue.leftJustified(length,'\0'));

         qDebug()<< QString(buffer.mid(begin,length));


         return true;
      }
      break;
    }
  }
  return false;
}
