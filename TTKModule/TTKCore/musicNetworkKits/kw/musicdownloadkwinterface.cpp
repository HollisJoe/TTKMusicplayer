#include "musicdownloadkwinterface.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musicalgorithmutils.h"
#include "musictime.h"

#///QJson import
#include "qjson/parser.h"
#include "qalg/qdeswrap.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadKWInterface::readFromMusicLLAttribute(MusicObject::MusicSongInformation *info, const QString &suffix,
                                                        const QString &format, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QDesWrap dess;
    QByteArray parameter = dess.encrypt(MusicUtils::Algorithm::mdII(KW_SONG_ATTR_LL_URL, false).arg(info->m_songId).arg(suffix).arg(format).toUtf8(),
                                        MusicUtils::Algorithm::mdII(_SIGN, ALG_LOW_KEY, false).toUtf8());
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_MV_URL, false).arg(QString(parameter));

    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QByteArray data = reply->readAll();
    if(!data.isEmpty() && !data.contains("res not found"))
    {
        QString text(data);
        QRegExp regx(".*url=(.*)\r\nsig=");
        if(text.indexOf(regx) != -1)
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = regx.cap(1);
            attr.m_bitrate = bitrate;
            attr.m_format = suffix;
            attr.m_size = "-";
            if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
            {
                return;
            }

            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadKWInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &suffix,
                                                          const QString &format, int bitrate)
{
    if(format.contains("MP3128") && bitrate == MB_128 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3192") && bitrate == MB_192 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3H") && bitrate == MB_320 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("ALFLAC") && bitrate == MB_1000 && suffix == "flac")
    {
        readFromMusicLLAttribute(info, "flac", "2000kflac", bitrate);
    }
    else if(format.contains("AL") && bitrate == MB_750 && suffix == "ape")
    {
        readFromMusicLLAttribute(info, "ape", "1000kape", bitrate);
    }
}

void MusicDownLoadKWInterface::readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QString &suffix,
                                                              const QString &format, int bitrate)
{
    if(format.contains("128kmp3") && bitrate == MB_128 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("192kmp3") && bitrate == MB_192 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("320kmp3") && bitrate == MB_320 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_ATTR_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("2000kflac") && bitrate == MB_1000 && suffix == "flac")
    {
        readFromMusicLLAttribute(info, "flac", "2000kflac", bitrate);
    }
    else if(format.contains("1000kape") && bitrate == MB_750 && suffix == "ape")
    {
        readFromMusicLLAttribute(info, "ape", "1000kape", bitrate);
    }
}

void MusicDownLoadKWInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                                          const QString &format, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, "mp3", format, MB_128);
        readFromMusicSongAttribute(info, "mp3", format, MB_192);
        readFromMusicSongAttribute(info, "mp3", format, MB_320);
        readFromMusicSongAttribute(info, "ape", format, MB_750);
        readFromMusicSongAttribute(info, "flac", format, MB_1000);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, "mp3", format, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, "mp3", format, MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, "mp3", format, MB_320);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, "ape", format, MB_750);
            readFromMusicSongAttribute(info, "flac", format, MB_1000);
        }
    }
}

void MusicDownLoadKWInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                                          const QVariantList &format, const QString &quality, bool all)
{
    foreach(const QVariant &var, format)
    {
        const QString fs = var.toString();
        if(all)
        {
            readFromMusicSongAttributePlus(info, "mp3", fs, MB_128);
            readFromMusicSongAttributePlus(info, "mp3", fs, MB_192);
            readFromMusicSongAttributePlus(info, "mp3", fs, MB_320);
            readFromMusicSongAttributePlus(info, "ape", fs, MB_750);
            readFromMusicSongAttributePlus(info, "flac", fs, MB_1000);
        }
        else
        {
            if(quality == QObject::tr("SD"))
            {
                readFromMusicSongAttributePlus(info, "mp3", fs, MB_128);
            }
            else if(quality == QObject::tr("HQ"))
            {
                readFromMusicSongAttributePlus(info, "mp3", fs, MB_192);
            }
            else if(quality == QObject::tr("SQ"))
            {
                readFromMusicSongAttributePlus(info, "mp3", fs, MB_320);
            }
            else if(quality == QObject::tr("CD"))
            {
                readFromMusicSongAttributePlus(info, "ape", fs, MB_750);
                readFromMusicSongAttributePlus(info, "flac", fs, MB_1000);
            }
        }
    }
}

void MusicDownLoadKWInterface::readFromMusicSongPic(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll().replace("lrclist", "'lrclist'").replace("'", "\""), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("songinfo"))
        {
            value = value["songinfo"].toMap();
            info->m_smallPicUrl = value["artpic"].toString();
            if(!info->m_smallPicUrl.contains("http://") && !info->m_smallPicUrl.contains("null"))
            {
                info->m_smallPicUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + info->m_smallPicUrl;
            }

            if(info->m_timeLength.isEmpty())
            {
                info->m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);
            }
        }
    }
}
