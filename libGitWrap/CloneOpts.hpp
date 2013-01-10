/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GITWRAP_CLONEOPTS_HPP
#define GITWRAP_CLONEOPTS_HPP

#include "IFetchEvents.hpp"

namespace Git
{

    class Repository;
    class CredentialRequest;
    class CloneOpts;

    namespace Internal
    {
        class CloneOptsPrivate;
    }

    class GITWRAP_API CloneEvents : public QObject, private IFetchEvents
    {
        Q_OBJECT
    public:
        CloneEvents( QObject* parent = 0 );

    public:
        CloneOpts options() const;

    signals:
        void askCredentials( CredentialRequest& request );
        void transportProgress( unsigned int totalObjects,
                                unsigned int indexedObjects,
                                unsigned int receivedObjects,
                                size_t receivedBytes );
        void doneDownloading();
        void doneIndexing();
        void error();
        void remoteMessage( const QString& message );
        void updateTip( const QString& branchName,
                        const Git::ObjectId& from,
                        const Git::ObjectId& to );

    private:
        friend class Internal::CloneOptsPrivate;
        operator IFetchEvents*(){ return this; }
        Internal::CloneOptsPrivate*     mOpts;
    };

    class GITWRAP_API CloneOpts
    {
    public:
        CloneOpts( Internal::CloneOptsPrivate* _d );
        CloneOpts();
        CloneOpts( const CloneOpts& other );
        ~CloneOpts();
        CloneOpts& operator=( const CloneOpts& other );

        bool operator==( const CloneOpts& other ) const;

    public:
        void setEvents( CloneEvents* events );
        CloneEvents* events() const;

    public:
        void setUrl( const QString& url );
        void setPath( const QString& path );

    public:
        void setBare( bool bare );
        void setRemoteName( const QByteArray& remoteName );
        void setFetchSpec( const QByteArray& fetchSpec );
        void setPushSpec( const QByteArray& pushSpec );
        void setPushUrl( const QByteArray& pushUrl );

    public:
        QString url() const;
        QString path() const;
        bool bare() const;

        QByteArray remoteName() const;
        QByteArray fetchSpec() const;
        QByteArray pushSpec() const;
        QByteArray pushUrl() const;

    private:
        friend class Repository;
        operator const Internal::CloneOptsPrivate*() const
        {
            return d;
        }

    private:
        Internal::GitPtr< Internal::CloneOptsPrivate > d;
    };

}

#endif
