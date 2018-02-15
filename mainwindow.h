#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QProcess>
#include <QJSEngine>
#include <QSharedPointer>

#include "wrappers/wrappers.h"
#include "treewidgetitem.h"

namespace Ui {
class MainWindow;
}

class WrapperForJs;

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

    void on_toolButtonFilePath_clicked();
    void on_toolButtonTo_clicked();

    /* Aim */
    void on_toolButtonAim_clicked();
    void on_toolButtonAim_pressed();
    void on_AimRelease(QPoint pos);

    /* Process */
    void processError(QProcess::ProcessError error);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_pushButtonCopyToClipboard_clicked();
    void on_pushButtonClear_clicked();


    void on_toolButtonScript_clicked();

    void on_comboBoxScript_currentIndexChanged(const QString &arg1);

private:
    void clear();
    void loadSettings();
    void saveSettings();
    LibraryCollector::PidType startedProcessId();
    QSharedPointer<QJSEngine> createJSEngine(const QString &scriptFileName);
    QString currentJsScript()const;

private:
  Ui::MainWindow *ui;

  QSharedPointer<QJSEngine> jsEngine_;

  QProcess* process_;
  bool processAlreadyStarted_;

  //QString qtDir_;
  QString systemRoot_;

  /* JS */
  Log* log_;
  Tree* tree_;
  Utils* utils_;
};


#endif // MAINWINDOW_H
