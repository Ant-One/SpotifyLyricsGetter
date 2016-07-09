#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString scriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/GetSpotifyCurrentSong.scpt");
    QString scriptContent =
            "--Main flow\n"
            "set currentlyPlayingTrack to getCurrentlyPlayingTrack()\n"
            "set UnixPath to POSIX path of ((path to me as text) & \"::\")\n"
            "set HFSPath to POSIX file UnixPath\n"
            "writeToFile(currentlyPlayingTrack, HFSPath & \"CurrentSong.txt\", false)\n\n"
            "-- Method to get the currently playing track\n"
            "on getCurrentlyPlayingTrack()\n"
            "	tell application \"Spotify\"\n"
            "       set currentArtist to name of current track as string\n"
            "       set currentTrack to artist of current track as string\n"
            "       return currentArtist & \"|\" & currentTrack\n"
            "   end tell\n"
            "end getCurrentPlayingTrack\n\n"
            "-- Method to write to file\n"
            "on writeToFile(dataToWrite, targetFile, appendData) -- (string, file path as string, boolean)\n"
                "try\n"
            "       set the targetFile to the targetFile as text\n"
            "       set the openTargetFile to open for access file targetFile with write permission\n"
            "        if appendData is false then set eof of the openTargetFile to 0\n"""
            "       write dataToWrite to the openTargetFile starting at eof\n"
            "       close access the openTargetFile\n"
            "        return true\n"
            "    on error\n"
            "        try\n"
            "            close access file targetFile\n"
            "        end try\n"
            "       return false\n"
            "   end try\n"
            "end writeToFile\n";
    QFileInfo checkFile(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    if (!checkFile.exists())
    {
        QDir().mkdir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    }
    QFile file(scriptPath);
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream << scriptContent;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
