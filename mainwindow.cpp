#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getLyrics()));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(showAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout(){
    QMessageBox::information(this, "À Propos", "Créé par Antoine Moix en 2016\nSources disponibles (github.com/Ant-One/SpotifyLyricsGetter/tree/OS-X)\nParoles de Musixmatch.com\nFait avec Qt5 (qt.io) et Scrapy(scrapy.org)\nÎcone de \"design.google.com/icons\"\nCe programme est proposé sous les termes de la Unlicense (unlicense.org)");
}

void MainWindow::getLyrics()
{
    QString appleScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/GetSpotifyCurrentSong.scpt");
    QProcess osascript;
    osascript.start("osascript", QStringList() << appleScriptPath);
    osascript.waitForFinished();
    osascript.close();

    QFile songInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/CurrentSong.txt");
    if (!songInfo.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->label->setText("Pas de chanson en cours d'écoute");
        return;
    }
    QTextStream inSongInfo(&songInfo);
    QString lineSongInfo;
    inSongInfo.setCodec("UTF-8");
    lineSongInfo = inSongInfo.readLine().replace("|", " - ");
    ui->label->setText(lineSongInfo);
    songInfo.close();

    QString scrapyCommand("/usr/local/bin/scrapy runspider ");

    QString lyricsSearcherScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/LyricsSearcher.py");
    QString bashCommandSearcher = scrapyCommand + "\"" + lyricsSearcherScriptPath + "\"";
    QProcess scrapySearcher;
    scrapySearcher.start("bash", QStringList() << "-c" << bashCommandSearcher);
    scrapySearcher.waitForFinished();
    scrapySearcher.close();

    QString lyricsGetterScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/LyricsGetter.py");
    QString bashCommandGetter = scrapyCommand + "\"" + lyricsGetterScriptPath + "\"";
    QProcess scrapyGetter;
    scrapyGetter.start("bash", QStringList() << "-c" << bashCommandGetter);
    scrapyGetter.waitForFinished();
    scrapyGetter.close();

    QFile lyrics(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/Lyrics.txt");
    if (!lyrics.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textBrowser->setText("Pas de paroles trouvées");
        return;
    }
    QTextStream inLyrics(&lyrics);
    QString lineLyrics;
    while (!inLyrics.atEnd()) {
        lineLyrics += inLyrics.readLine().append("\n");
    }
    ui->textBrowser->setText(lineLyrics);
    if (lyrics.pos() == 0){
        ui->textBrowser->setText("Pas de paroles trouvées");
        return;
    }

    lyrics.close();
}
