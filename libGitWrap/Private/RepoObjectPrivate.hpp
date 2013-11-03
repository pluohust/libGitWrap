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

#ifndef GIT_REPO_OBJECT_H
#define GIT_REPO_OBJECT_H

#include "libGitWrap/Base.hpp"

#include "libGitWrap/Private/BasePrivate.hpp"
#include "libGitWrap/Private/RepositoryPrivate.hpp"

namespace Git
{
    namespace Internal
    {

        /**
         * @internal
         * @ingroup GitWrap
         * @brief The RepoObject class
         *
         */
        class RepoObjectPrivate : public BasePrivate
        {
        protected:
            RepoObjectPrivate();
            RepoObjectPrivate(const RepositoryPrivate::Ptr& repo);

        public:
            RepositoryPrivate::Ptr repo() const;

        protected:
            RepositoryPrivate::Ptr mRepo;
        };

    }

}

#endif

