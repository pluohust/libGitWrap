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

#pragma once

#include "libGitWrap/Result.hpp"

#include "libGitWrap/Private/GitWrapPrivate.hpp"

#include "libGitWrap/Operations/Private/WorkerThread.hpp"

namespace Git
{

    class BaseOperation;

    namespace Internal
    {

        /**
         * @internal
         * @ingroup     GitWrap
         *
         */
        class BaseOperationPrivate : public Worker
        {
        public:
            BaseOperationPrivate(BaseOperation* owner);
            ~BaseOperationPrivate();

        public:
            BaseOperation*          mOwner;
            bool                    mBackgroundMode;

            Result                  mResult;
            WorkerThread*           mThread;
        };

    }

}

#define GW_OP_OWNER(CLS) \
    CLS* owner = static_cast<CLS*>(mOwner)

#define GW_OP_D(CLASS) \
    Private* d = static_cast<Private*>(mData.data()); \
    ensureThisIsNotConst()

#define GW_OP_CD(CLASS) \
    const CLASS::Private* d = static_cast<const CLASS::Private*>(mData.constData())
