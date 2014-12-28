/*
 * MacGitver
 * Copyright (C) 2014 Sascha Cunz <sascha@macgitver.org>
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

#include <QStringBuilder>

#include "libGitWrap/Operations/CloneOperation.hpp"
#include "libGitWrap/Operations/Private/CloneOperationPrivate.hpp"

#include "libGitWrap/Events/Private/GitEventCallbacks.hpp"

namespace Git
{

    namespace Internal
    {

        CloneOperationPrivate::CloneOperationPrivate(CloneOperation* owner)
            : BaseRemoteOperationPrivate( (*mCloneOpts).remote_callbacks, owner)
        {
            CheckoutOptionsRef coo = mCloneOpts.checkoutOptions();
            (*coo).checkout_strategy = GIT_CHECKOUT_SAFE_CREATE;

            CheckoutCallbacks::initCallbacks( coo, owner );
        }

        void CloneOperationPrivate::run()
        {
            GW_OP_OWNER(CloneOperation);

            git_repository* repo = NULL;

            if (mResult) {
                mResult = git_clone(&repo, GW_StringFromQt(mUrl), GW_StringFromQt(mPath), mCloneOpts);
            }

            git_repository_free(repo);
        }

    }

    CloneOperation::CloneOperation(QObject* parent)
        : BaseRemoteOperation(*new Private(this), parent)
    {
    }

    void CloneOperation::setUrl(const QString& url)
    {
        GW_D(CloneOperation);
        Q_ASSERT(!isRunning());
        d->mUrl = url;
    }

    void CloneOperation::setPath( const QString& path )
    {
        GW_D(CloneOperation);
        Q_ASSERT(!isRunning());
        d->mPath = path;
    }

    void CloneOperation::setBare(bool bare)
    {
        GW_D(CloneOperation);
        Q_ASSERT(!isRunning());
        (*(d->mCloneOpts)).bare = bare;
    }

    void CloneOperation::setDepth(uint depth)
    {
        GW_D(CloneOperation);
        Q_ASSERT( !isRunning() );
        if ( depth > 0 ) {
            // TODO: not implemented in libgit2 api
            d->mResult.setError( "Setting the clone depth is not yet supported.", GIT_EUSER );
            qWarning( "%s: Missing implementation in libgit2 API.", __FUNCTION__ );
        }
    }

    void CloneOperation::setReference(const QString& refName)
    {
        GW_D( CloneOperation );
        Q_ASSERT( !isRunning() );
        (*(d->mCloneOpts)).checkout_branch = refName.isEmpty() ? NULL : GW_StringFromQt( refName );
    }

    void CloneOperation::setRemoteAlias(const QString& alias)
    {
        GW_D( CloneOperation );
        Q_ASSERT( !isRunning() );
        if ( !alias.isEmpty() ) {
            // TODO: not implemented in libgit2 api
            d->mResult.setError( "Setting the Remote-Alias is not yet supported.", GIT_EUSER );
            qWarning( "%s: Missing implementation in libgit2 API.", __FUNCTION__ );
        }
    }

    QString CloneOperation::url() const
    {
        GW_CD(CloneOperation);
        return d->mUrl;
    }

    QString CloneOperation::path() const
    {
        GW_CD(CloneOperation);
        return d->mPath;
    }

    bool CloneOperation::bare() const
    {
        GW_CD(CloneOperation);
        return (*(d->mCloneOpts)).bare;
    }

    uint CloneOperation::depth() const
    {
        return 0;
    }

    QString CloneOperation::reference() const
    {
        GW_CD( CloneOperation );
        return GW_StringToQt( (*(d->mCloneOpts)).checkout_branch );
    }

    QString CloneOperation::remoteAlias() const
    {
        return QString();
    }

}
