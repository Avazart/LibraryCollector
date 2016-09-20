#include "mainwindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QCursor>
#include <QRegExp>
#include <QThread>  // #253 QThread::usleep();
#include <QClipboard>

#include "dependencies.h"
#include "file_functions.h"
//------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  processAlreadyStarted_(false)
{
  ui->setupUi(this);
  
  ui->splitter->setStretchFactor(0,1);
  ui->splitter->setStretchFactor(1,0);
  ui->splitter->setSizes(QList<int>()<<20<<50);

  process_= new QProcess(this);

  connect( process_, SIGNAL(error(QProcess::ProcessError)),
           this,    SLOT(processError(QProcess::ProcessError))       );

  connect( process_, SIGNAL(started()),
           this,    SLOT(processStarted())  );

  connect( process_, SIGNAL(finished(int, QProcess::ExitStatus)),
           this,    SLOT(processFinished(int, QProcess::ExitStatus))  );


  connect(ui->toolButtonAim,SIGNAL(leftMouseRelease(QPoint)),
          this, SLOT(on_AimRelease(QPoint)));


  QSettings settings(tr("settings.ini"),QSettings::IniFormat);
  settings.beginGroup(tr("Paths"));

  QStringList args= QApplication::arguments();
  if(args.size()==5)
  {
    QString buildPath= args[1];
    QString buildName= args[2];

    if(buildName=="Отладка") buildName="debug";
    if(buildName=="Выпуск")  buildName="release";

    QString fullName= buildPath+QDir::separator()+
                      buildName+QDir::separator()+
                      args[3]+".exe";

    ui->lineEditFilePath->setText(fullName);// exe %{CurrentProject:BuildPath}\%{CurrentBuild:Name}\%{CurrentProject:FileBaseName}
    ui->lineEditLibsDir->setText(args[4]);  // bin %{CurrentProject:QT_HOST_BINS}

    QDir qtDir(args[4]);
    qtDir.cdUp();
    // QTDIR / plugins
    ui->lineEditPluginsDir->setText(qtDir.absolutePath()+tr("/plugins"));
    // QTDIR / qml
    ui->lineEditQmlDir->setText(qtDir.absolutePath()+tr("/qml"));

    process_->setWorkingDirectory(buildPath);
  }
  else
  {
    QFile file(settings.value("name").toString());

    ui->lineEditFilePath->setText(file.fileName());
    ui->lineEditLibsDir->setText(settings.value("libs").toString());
    ui->lineEditPluginsDir->setText(settings.value("plugins").toString());
    ui->lineEditQmlDir->setText(settings.value("qml").toString());

    process_->setWorkingDirectory(QFileInfo(file).absolutePath());
  }

  ui->lineEditToDir->setText(settings.value("to").toString());
  settings.endGroup();

  settings.beginGroup(tr("Other"));
  ui->checkBoxAddBin->setCheckState(
        (Qt::CheckState)settings.value("addLibsDirToEnvironment").toInt() );
  ui->checkBoxMakeSubdirs->setCheckState(
        (Qt::CheckState)settings.value("makeSubdirs").toInt() );
  settings.endGroup();

#ifdef Q_OS_WIN
  QProcessEnvironment enviroment(QProcessEnvironment::systemEnvironment());
  systemDir_ = enviroment.value(tr("SYSTEMROOT"));
#endif  
}
//----------------------------------------------------------
MainWindow::~MainWindow()
{
  delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{    
  QSettings settings("settings.ini",QSettings::IniFormat);
  settings.beginGroup("Paths");
  settings.setValue("libs",    ui->lineEditLibsDir->text());
  settings.setValue("plugins",ui->lineEditPluginsDir->text());
  settings.setValue("qml", ui->lineEditQmlDir->text());
  settings.setValue("name",   ui->lineEditFilePath->text());
  settings.setValue("to",     ui->lineEditToDir->text());
  settings.endGroup();

  settings.beginGroup("Other");
  settings.setValue("addLibsDirToEnvironment", ui->checkBoxAddBin->checkState());
  settings.setValue("makeSubdirs", ui->checkBoxMakeSubdirs->checkState());
  settings.endGroup();

  if(processAlreadyStarted_)
    process_->terminate();
  QWidget::closeEvent(event);
}
//------------------------------------------------------
void MainWindow::on_pushButtonRun_clicked()
{
  QProcessEnvironment enviroment(QProcessEnvironment::systemEnvironment());

  QChar pathSeparator= ';';
#ifdef Q_OS_WIN
  systemDir_ = enviroment.value("SYSTEMROOT");
  pathSeparator= ';';
#elif defined Q_OS_LINUX
  pathSeparator= ':';
#endif


  if(ui->checkBoxAddBin->checkState()==Qt::Checked)
  {
    QString path= enviroment.value("PATH");
    enviroment.insert("PATH",
                      path+pathSeparator+ui->lineEditLibsDir->text());
  }


  process_->setEnvironment(enviroment.toStringList());
  process_->start("\""+ui->lineEditFilePath->text()+"\"");
}
//-----------------------------------------------------------------------------
void MainWindow::clear()
{  
  ui->treeWidget->clear();
  ui->lineEditLibsDir->clear();
  ui->lineEditPluginsDir->clear();
  ui->lineEditFilePath->clear();
}
//------------------------------------------------------
void MainWindow::addInfo(const QString &text)
{
  ui->textEditLog->append(
    QString("<span style='color: blue;'>%1</span>")
          .arg(text) );
  ui->textEditLog->moveCursor(QTextCursor::End);
}
//------------------------------------------------------
void MainWindow::addError(const QString &text)
{
  ui->textEditLog->append(
     QString("<span style='color: red;'>Error: \"%1\"</span>")
           .arg(text) );
  ui->textEditLog->moveCursor(QTextCursor::End);
}
//------------------------------------------------------
MainWindow::dirType MainWindow::checkDirType(const QString &path)
{
  if(My::isSubPath(ui->lineEditLibsDir->text(),path))
    return dtQtLibs;
  else if(My::isSubPath(ui->lineEditPluginsDir->text(),path))
    return dtQtPlugins;
  else if(My::isSubPath(ui->lineEditQmlDir->text(),path))
    return dtQtQml;

#ifdef Q_OS_WIN
  if(My::isSubPath(systemDir_,path))
    return dtSysLib;
#elif defined Q_OS_LINUX
  if(My::isSubPath("/lib",path))
    return dtSysLib;
  if(My::isSubPath("/usr/lib",path))
    return dtUsrLib;
#endif

  return dtOther;
}
//------------------------------------------------------
void MainWindow::checkOtherLib(const QString &path,TreeWidgetItem *item)
{
    if(My::isMySQLlib(path)
#ifdef Q_OS_WIN
       || My::isMSVClib(path)
#endif
       )
    {
      item->setCheckState(0, Qt::Checked);
      item->parent()->setCheckState(0,Qt::PartiallyChecked);
      ui->treeWidget->expandItem(item->parent());
    }
    else
      item->setCheckState(0, Qt::Unchecked);
}
//------------------------------------------------------
TreeWidgetItem* MainWindow::addItem(const QString& text)
{
  TreeWidgetItem *item = new TreeWidgetItem(ui->treeWidget);
  item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
  item->setText(0,text);
  item->setCheckState(0, Qt::Unchecked);

  return item;
}
//------------------------------------------------------
void MainWindow::on_pushButtonUpdate_clicked()
{
  ui->treeWidget->clear();
  ui->treeWidget->setColumnCount(1);

  My::PidType pid= 0;
  if( !processAlreadyStarted_ )
  {
    QFileInfo info(QFile(ui->lineEditFilePath->text()));
    QString processFilePath= info.filePath();

    if(!info.exists())
      addError(QString(tr("Executable file \"%1\" not exist")).arg(processFilePath));

    QString error;
    pid=  My::processIdByFilePath(processFilePath,error);
    if(!error.isEmpty())
    {
      addError(error);
      on_pushButtonRun_clicked();

      if( process_->waitForStarted() )
      {
#ifdef Q_OS_WIN
        pid=  process_->pid()->dwProcessId;
        WaitForInputIdle(process_->pid()->hProcess,5000);
#elif defined Q_OS_LINUX
        pid=  process_->pid();
#endif
        QThread::usleep(500);
      }
      else return;
    }
  }
  else
  {
#ifdef Q_OS_WIN
    pid=  process_->pid()->dwProcessId;
#elif defined Q_OS_LINUX
    pid=  process_->pid();
#endif
  }

  QString error;
  QStringList modules= My::librariesByPid(pid,error);
  if(!error.isEmpty())
  {
    addError(error);
    return;
  }

  addInfo(QString(tr("Found %1 modules."))
                 .arg(modules.size()) );

  itemQtLibs_   = addItem(tr("QtLibs"));
  itemQtPlugins_= addItem(tr("QtPlugins"));
  itemQtQml_ =    addItem(tr("QtQml"));
#ifdef Q_OS_WIN
  itemSystem_ = addItem(tr("System"));
#elif defined Q_OS_LINUX
  itemUsrLib_= addItem(tr("/usr/lib"));
  itemSysLib_= addItem(tr("/lib"));
#endif
  itemOther_  = addItem(tr("Other"));

  foreach(QString path, modules)
  {
    TreeWidgetItem *item = new TreeWidgetItem;
    item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
    item->setText(0,path);

    switch (checkDirType(path))
    {
      case dtQtLibs:    itemQtLibs_->addChild(item);    break;
      case dtQtPlugins: itemQtPlugins_->addChild(item); break;
      case dtQtQml:     itemQtQml_->addChild(item);     break;
#ifdef Q_OS_WIN
      case dtSysLib:  itemSystem_->addChild(item);  checkOtherLib(path,item); break;
#elif defined Q_OS_LINUX
      case dtSysLib:     itemSysLib_->addChild(item);  checkOtherLib(path,item); break;
      case dtUsrLib:     itemUsrLib_->addChild(item);  checkOtherLib(path,item); break;
#endif
      default:           itemOther_->addChild(item);   checkOtherLib(path,item); break;
    }
  }

  itemQtLibs_->setCheckState(0, Qt::Checked);
  itemQtPlugins_->setCheckState(0, Qt::Checked);
  itemQtQml_->setCheckState(0, Qt::Checked);

  ui->treeWidget->expandItem(itemQtLibs_);
  ui->treeWidget->expandItem(itemQtPlugins_);
  ui->treeWidget->expandItem(itemQtQml_);
}
//------------------------------------------------------
void MainWindow::on_pushButtonCopy_clicked()
{
  QString toDir= ui->lineEditToDir->text();

  QFile exeFile(ui->lineEditFilePath->text());
  QFileInfo exeFileInfo(exeFile);
  QString exeFileDir= exeFileInfo.dir().absolutePath();

  if(toDir.isEmpty())
  {
    toDir= exeFileDir;
  }
  else
  {
    QRegExp reDir("<ExeDir>");    
    QRegExp reName("<BaseName>"); 
    toDir.replace(reDir,exeFileDir);
    toDir.replace(reName,exeFileInfo.baseName());
  }


  QDir dir(toDir);
  if(!dir.exists())
    dir.mkpath(tr("."));

  if(QDir(toDir)!=QDir(exeFileDir))
  {
    exeFile.copy(toDir+QDir::separator()+exeFileInfo.fileName());
  }

  for(int i=0; i<ui->treeWidget->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* topLevalItem = ui->treeWidget->topLevelItem(i);
    for(int j=0; j<topLevalItem->childCount(); ++j)
    {
      QTreeWidgetItem *childItem= topLevalItem->child(j);
      if(childItem->checkState(0)==Qt::Checked)
      {
        QString libFilePath=  childItem->text(0);
        QString relDir= tr("");

        if(topLevalItem == itemQtPlugins_)
          relDir= ui->lineEditPluginsDir->text();
        else if(topLevalItem == itemQtQml_)
          relDir= ui->lineEditQmlDir->text();

        if(ui->checkBoxMakeSubdirs->isChecked())
        {
          if(topLevalItem == itemQtPlugins_)
            toDir= dir.absolutePath()+QDir::separator()+"plugins";
          else if(topLevalItem == itemQtQml_)
            toDir= dir.absolutePath()+QDir::separator()+"qml";
          else
            toDir= dir.absolutePath()+QDir::separator()+"lib";

          QDir subDir(toDir);
          if(!subDir.exists())
            subDir.mkpath(".");
        }

        QString newFilePath= My::copyFile(libFilePath,toDir,relDir);

        if (newFilePath.isEmpty())
        {
          addError(
            QString(tr("Can not copy file: \"%1\" to \"%2\""))
                      .arg(libFilePath)
                      .arg(toDir));
        }

        // Copy "qmldir" files
        if(topLevalItem == itemQtQml_)
        {
          QDir fromQmlDir = QFileInfo(libFilePath).absoluteDir();
          QString qmldirFile = fromQmlDir.absolutePath()+"/qmldir";
          if(QFileInfo(qmldirFile).exists())
          {
             QString qmldirNewFilePath = My::copyFile(qmldirFile,toDir,relDir);
             if(qmldirNewFilePath.isEmpty())
             {
               addError(
                 QString(tr("Can not copy file: \"%1\" to \"%2\""))
                           .arg(qmldirFile)
                           .arg(toDir));
             }
          }
        }

        #ifdef Q_OS_LINUX
          My::makeSymLinks(libFilePath,newFilePath);
        #endif
      }
    }
  }
}
//------------------------------------------------------------
void MainWindow::processError(QProcess::ProcessError )
{
  addError(process_->errorString());
}
//------------------------------------------------------------
void MainWindow::processStarted()
{
  processAlreadyStarted_= true;
  addInfo(tr("Process started."));
}
//-------------------------------------------------------------
void MainWindow::processFinished(int, QProcess::ExitStatus)
{
  processAlreadyStarted_= false;
  addInfo(tr("Process finished."));
}
//-------------------------------------------------------------
void MainWindow::on_toolButtonLibs_clicked()
{
  QString libsDir=
      QFileDialog::getExistingDirectory( this,
                                         tr("Select directory"),
                                         ui->lineEditLibsDir->text(),
                                         QFileDialog::ShowDirsOnly );
  if(!libsDir.isEmpty())
    ui->lineEditLibsDir->setText(libsDir);
}
//-------------------------------------------------------------
void MainWindow::on_toolButtonPlugins_clicked()
{
  QString pluginsDir=
      QFileDialog::getExistingDirectory( this,
                                         tr("Select directory"),
                                         ui->lineEditPluginsDir->text(),
                                         QFileDialog::ShowDirsOnly );

  if(!pluginsDir.isEmpty())
    ui->lineEditPluginsDir->setText(pluginsDir);
}
//----------------------------------------------------------
void MainWindow::on_toolButtonQml_clicked()
{
  QString qmlDir=
      QFileDialog::getExistingDirectory( this,
                                         tr("Select directory"),
                                         ui->lineEditQmlDir->text(),
                                         QFileDialog::ShowDirsOnly );

  if(!qmlDir.isEmpty())
    ui->lineEditQmlDir->setText(qmlDir);
}
//----------------------------------------------------------
void MainWindow::on_toolButtonTo_clicked()
{   
  QString toDir =
      QFileDialog::getExistingDirectory( this,
                                         tr("Select directory"),
                                         ui->lineEditFilePath->text(),
                                         QFileDialog::ShowDirsOnly );

  if(!toDir.isEmpty())
    ui->lineEditToDir->setText(toDir);
}
//------------------------------------------------------------
void MainWindow::on_toolButtonFilePath_clicked()
{
  QString name=
      QFileDialog::getOpenFileName(this,
                                   tr("Select file"),
                                   ui->lineEditFilePath->text(),
                                 #ifdef Q_OS_WIN
                                   tr("Executable(*.exe)")
                                 #elif defined Q_OS_LINUX
                                   tr("Executable(*)")
                                 #endif
                                   );

  if(!name.isEmpty())
    ui->lineEditFilePath->setText(name);
}
//----------------------------------------------------------
void MainWindow::on_AimRelease(QPoint pos)
{
#ifdef Q_OS_WIN
  QString error;
  QString processFilePath=
      My::processFilePathByPoint(pos.x(),pos.y(),error);
  if(error.isEmpty())
    ui->lineEditFilePath->setText(processFilePath);
  else
    addError(error);
  setCursor(Qt::ArrowCursor);
#else
  Q_UNUSED(pos)
#endif
}
//----------------------------------------------------------
void MainWindow::on_toolButtonAim_pressed()
{
#ifdef Q_OS_WIN
  setCursor(Qt::CrossCursor);
#endif
}
//----------------------------------------------------------
void MainWindow::on_toolButtonAim_clicked()
{
#ifdef Q_OS_LINUX
  QString error;
  QString processFilePath=
      My::processFilePathBySelectedWindow(error);
  if(error.isEmpty())
    ui->lineEditFilePath->setText(processFilePath);
  else
    addError(error);
#endif
}
//----------------------------------------------------------
void MainWindow::on_pushButtonCopyToClipboard_clicked()
{
    QStringList list;
    for(int i=0; i<ui->treeWidget->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem* topLevalItem = ui->treeWidget->topLevelItem(i);
      list.append(topLevalItem->text(0));
      for(int j=0; j<topLevalItem->childCount(); ++j)
      {
        QTreeWidgetItem *childItem= topLevalItem->child(j);
        list.append("\t"+childItem->text(0));
      }
    }
    if(!list.empty())
      QApplication::clipboard()->setText(list.join("\n"));
}
//----------------------------------------------------------
void MainWindow::on_pushButtonClear_clicked()
{
  ui->textEditLog->clear();
  ui->treeWidget->clear();
}
//----------------------------------------------------------
