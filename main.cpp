#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>

void initializeScripts()
{
    QString appleScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/GetSpotifyCurrentSong.scpt");
    QString appleScriptContent =
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

    QString lyricsSearcherScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/LyricsSearcher.py");
    QString lyricsSearcherScriptContent =
            "# coding: utf-8\n"
            "import scrapy\n"
            "from scrapy.http import Request\n"
            "from HTMLParser import HTMLParser\n"
            "import os.path\n"

            "class SearchSpider(scrapy.Spider):\n"
            "    name = \"lyrics\"\n"
            "    allowed_domains = ['musixmatch.com']\n"
            "    file_song = open(os.path.abspath(os.path.join(os.path.dirname(__file__),\"..\")) + \"/Spotify Lyrics Getter/CurrentSong.txt\", \"r\")\n"
            "    info = file_song.read()\n"
            "    file_song.close()\n"
            "    infos = info.split(\"|\")\n"
            "    song = infos[0]\n"
            "    artist = infos[1]\n"
            "    start_urls = [\n"
            "        'https://www.musixmatch.com/search/%s %s' % (song, artist),\n"
            "    ]\n"
            "    def parse(self, response):\n"
            "        song_results = response.css(\".media-card-title a::attr('href')\").extract()\n"
            "        try:\n"
            "            song_url = 'https://www.musixmatch.com' + song_results[0]\n"
            "        except IndexError:\n"
            "            song_url = 'Lyrics not found'\n"
            "        h = HTMLParser()\n"
            "        filename = os.path.abspath(os.path.join(os.path.dirname(__file__),\"..\")) + '/Spotify Lyrics Getter/LyricsURL.txt'\n"
            "        with open(filename, 'wb') as f:\n"
            "            f.write(h.unescape(song_url).encode('utf8'))\n";

    QString lyricsGetterScriptPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/LyricsGetter.py");
    QString lyricsGetterScriptContent =
            "# coding: utf-8\n"
            "import scrapy\n"
            "from scrapy.http import Request\n"
            "from HTMLParser import HTMLParser\n"
            "class LyricsSpider(scrapy.Spider):\n"
            "    name = \"lyrics\"\n"
            "   allowed_domains = ['musixmatch.com']\n"
            "    file_song = open(\"LyricsURL.txt\", \"r\")\n"
            "    url = file_song.read()\n"
            "    file_song.close()\n"
            "    def start_requests(self):\n"
            "        yield Request(self.url, callback=self.parse, headers={\"User-Agent\": \"Mozilla/5.0 (iPhone; CPU iPhone OS 5_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B179 Safari/7534.48.3\"})\n"
            "    def parse(self, response):\n"
            "        h = HTMLParser()\n"
            "        lyrics = response.css('.mxm-lyrics__content::text').extract()\n"
            "        filename = \"Lyrics.txt\"\n"
            "        with open(filename, 'wb') as f:\n"
            "            for lyric in lyrics:\n"
            "                f.write(h.unescape(lyric).encode('utf8'))\n";

    QFileInfo checkFile(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    if (!checkFile.exists())
    {
        QDir().mkdir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    }
    QFile appleScriptFile(appleScriptPath);
    if ( appleScriptFile.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &appleScriptFile);
        stream << appleScriptContent;
    }
    QFile lyricsSearcherScriptFile(lyricsSearcherScriptPath);
    if ( lyricsSearcherScriptFile.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &lyricsSearcherScriptFile);
        stream << lyricsSearcherScriptContent;
    }
    QFile lyricsGetterScriptFile(lyricsGetterScriptPath);
    if ( lyricsGetterScriptFile.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &lyricsGetterScriptFile);
        stream << lyricsGetterScriptContent;
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initializeScripts();
    MainWindow w;
    w.show();

    return a.exec();
}
