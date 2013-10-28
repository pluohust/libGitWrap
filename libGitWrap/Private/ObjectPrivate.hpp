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

#ifndef GIT_OBJECT_PRIVATE_H
#define GIT_OBJECT_PRIVATE_H

#include "libGitWrap/Private/RepoObjectPrivate.hpp"

namespace Git
{

    namespace Internal
    {

        /**
         * @internal    GitWrap
         * @brief       The ObjectPrivate class
         *
         */
        class ObjectPrivate : public RepoObjectPrivate
        {
        public:
            ObjectPrivate(RepositoryPrivate* repo, git_object* o);
            ~ObjectPrivate();

        public:
            git_object* mObj;
        };

    }

}

#endif