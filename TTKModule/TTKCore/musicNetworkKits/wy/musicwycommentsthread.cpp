#include "musicwycommentsthread.h"
#include "musicdownloadquerywythread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicWYSongCommentsThread::MusicWYSongCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicWYSongCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWYThread *query = new MusicDownLoadQueryWYThread(this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWYSongCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_SG_COMMIT_N_URL, false).arg(m_rawData["songID"].toInt()),
               MusicUtils::Algorithm::mdII(WY_COMMIT_NDT_URL, false).arg(m_rawData["songID"].toInt()).arg(m_pageSize).arg(m_pageSize*offset));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYSongCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
            {
                m_pageTotal = value["total"].toLongLong();
                QVariantList comments = value["comments"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    QVariantMap user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_avatarUrl = user["avatarUrl"].toString();

                    comment.m_likedCount = QString::number(value["likedCount"].toLongLong());
                    comment.m_time = QString::number(value["time"].toLongLong());
                    comment.m_content = value["content"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicWYPlaylistCommentsThread::MusicWYPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicWYPlaylistCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYPlaylistCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicWYPlaylistCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_PL_COMMIT_N_URL, false).arg(m_rawData["songID"].toInt()),
               MusicUtils::Algorithm::mdII(WY_COMMIT_NDT_URL, false).arg(m_rawData["songID"].toInt()).arg(m_pageSize).arg(m_pageSize*offset));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYPlaylistCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
            {
                m_pageTotal = value["total"].toLongLong();
                QVariantList comments = value["comments"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    QVariantMap user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_avatarUrl = user["avatarUrl"].toString();

                    comment.m_likedCount = QString::number(value["likedCount"].toLongLong());
                    comment.m_time = QString::number(value["time"].toLongLong());
                    comment.m_content = value["content"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
