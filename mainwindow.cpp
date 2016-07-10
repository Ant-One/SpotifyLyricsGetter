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
    QString appleScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/GetSpotifyCurrentSong.scpt");
    QProcess osascript;
    osascript.start("osascript", QStringList() << appleScriptPath);
    osascript.waitForFinished();
    osascript.close();
    QMessageBox::information(this, "info", "Spotify interrogé");

    QString lyricsSearcherScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/LyricsSearcher.py");
    QString scrapySearcherCommand("/usr/local/bin/scrapy runspider ");
    QString bashCommand = scrapyCommand + "\"" + lyricsSearcherScriptPath + "\"";
    QMessageBox::information(this, "info", bashCommand);
    QProcess scrapySearcher;
    scrapySearcher.start("bash", QStringList() << "-c" << bashCommand);
    scrapySearcher.waitForFinished();
    scrapySearcher.close();
}
