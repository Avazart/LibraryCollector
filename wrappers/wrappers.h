#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <QObject>
#include <QTextEdit>
#include <QTreeWidget>

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <algorithm>

#include "treewidgetitem.h"
#include "dependencies.h"
#include "file_functions.h"

#include "fileinfo.h"
#include "dir.h"

class Log: public QObject
{
  Q_OBJECT
public:
  explicit Log(QTextEdit* parent);

  Q_INVOKABLE void addInfo(const QString& text, QString color="black");
  Q_INVOKABLE void addError(const QString& text);

private:
   QTextEdit* textEdit_;
};



class Tree: public QObject
{
  Q_OBJECT
public:
  explicit Tree(QTreeWidget* parent);

  Q_INVOKABLE int addGroup(const QString &groupName);
  Q_INVOKABLE void addLib(int groupIndex, const QString &libName, bool checked= false);
  Q_INVOKABLE void expandGroup(int groupIndex);

  Q_INVOKABLE int groupCount()const;
  Q_INVOKABLE int libCount(int groupIndex)const;

  Q_INVOKABLE QString groupName(int groupIndex)const;
  Q_INVOKABLE QString libName(int groupIndex, int libIndex)const;

  Q_INVOKABLE bool libIsChecked(int groupIndex,int libIndex)const;

private:
   QTreeWidget* treeWidget_;
};



class Utils: public QObject
{
   Q_OBJECT
public:
   Q_INVOKABLE Utils(QObject* parent=0);

   Q_INVOKABLE static bool isSubPath(const QString &dir,
                                     const QString &path);

   Q_INVOKABLE static QString copyFile(const QString &filePath,
                              const QString &destinationDir,
                              const QString &baseDir= QString());

   Q_INVOKABLE static bool patchFile(const QString& fileName,
                                    const QByteArray& valueName,
                                    const QByteArray& newValue);
};



#endif // WRAPPERS_H
