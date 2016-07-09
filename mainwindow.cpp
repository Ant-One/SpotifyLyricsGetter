#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getCurrentSong()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCurrentSong()
{
    QString pathToScript(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/GetSpotifyCurrentSong.scpt");
    QProcess osascript;
    osascript.start("osascript", QStringList() << pathToScript);
    osascript.waitForFinished();
    osascript.close();
}
