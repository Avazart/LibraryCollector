#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>

#include "treewidgetitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *event)Q_DECL_OVERRIDE;

private slots:
    void on_pushButtonRun_clicked();
    void on_pushButtonUpdate_clicked();
    void on_pushButtonCopy_clicked();

    void processError(QProcess::ProcessError error);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_toolButtonFilePath_clicked();
    void on_toolButtonLibs_clicked();
    void on_toolButtonPlugins_clicked();
    void on_toolButtonQml_clicked();

    void on_toolButtonTo_clicked();

    void on_toolButtonAim_clicked();
    void on_toolButtonAim_pressed();
    void on_AimRelease(QPoint pos);

    void on_pushButtonCopyToClipboard_clicked();
    void on_pushButtonClear_clicked();

private:
    enum dirType{ dtQtLibs, dtQtPlugins, dtQtQml, dtSystem, dtUsrLib, dtSysLib, dtOther };
    dirType checkDirType(const QString& path);
    void checkOtherLib(const QString& path, TreeWidgetItem *item);

    TreeWidgetItem * addItem(const QString &text);
    void clear();

    void addInfo(const QString& text);
    void addError(const QString& text);

private:
  Ui::MainWindow *ui;

  QProcess* process_;
  bool processAlreadyStarted_;

  TreeWidgetItem *itemQtLibs_;
  TreeWidgetItem *itemQtPlugins_;
  TreeWidgetItem *itemQtQml_;

#ifdef Q_OS_WIN
  QString systemDir_;
  TreeWidgetItem *itemSystem_;
#elif defined Q_OS_LINUX
  TreeWidgetItem *itemUsrLib_;
  TreeWidgetItem *itemSysLib_;
#endif

  TreeWidgetItem *itemOther_;
};

#endif // MAINWINDOW_H
