#ifndef MUSICARTISTFOUNDTABLEWIDGET_H
#define MUSICARTISTFOUNDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicfoundabstractwidget.h"

/*! @brief The class of the artist albums item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistAlbumsItemWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistAlbumsItemWidget(QWidget *parent = 0);

    virtual ~MusicArtistAlbumsItemWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set music item.
     */
    void setMusicItem(const MusicPlaylistItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    MusicPlaylistItem m_itemData;
    QPushButton *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel, *m_updateLabel;

};

class MusicPagingWidgetObject;

/*! @brief The class of the artist mvs found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistMvsFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistMvsFoundWidget(QWidget *parent = 0);

    virtual ~MusicArtistMvsFoundWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;
    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Create the current albums item.
     */
    void createArtistMvsItem(const MusicPlaylistItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);

protected:
    QGridLayout *m_gridLayout;
    bool m_firstInit;
    MusicPagingWidgetObject *m_pagingWidgetObject;

};


/*! @brief The class of the artist albums found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistAlbumsFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistAlbumsFoundWidget(QWidget *parent = 0);

    virtual ~MusicArtistAlbumsFoundWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;
    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Create the current albums item.
     */
    void createArtistAlbumsItem(const MusicPlaylistItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);

protected:
    QGridLayout *m_gridLayout;

};


/*! @brief The class of the artist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicArtistFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query) override;

};


/*! @brief The class of artist music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundWidget(QWidget *parent = 0);

    virtual ~MusicArtistFoundWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Query artist musics is finished.
     */
    void queryArtistFinished();
    /*!
     * Create the current artist info item.
     */
    void createArtistInfoItem(const MusicPlaylistItem &item);
    /*!
     * Set current container index.
     */
    void setCurrentIndex(int index);

protected:
    /*!
     * Create init interface lables.
     */
    void createLabels();
    /*!
     * Init the third widget.
     */
    void initThirdWidget();
    /*!
     * Init the fourth widget.
     */
    void initFourthWidget();

    MusicArtistAlbumsFoundWidget *m_artistAlbums;
    MusicArtistMvsFoundWidget *m_artistMvs;

};

#endif // MUSICARTISTFOUNDTABLEWIDGET_H
