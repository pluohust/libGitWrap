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

#pragma once

#include "libGitWrap/Private/BasePrivate.hpp"

namespace Git
{

    namespace Internal
    {

        /**
         * @internal
         * @ingroup     GitWrap
         * @brief       The TreeEntryPrivate class
         */
        class TreeEntryPrivate : public BasePrivate
        {
        public:
            TreeEntryPrivate( const git_tree_entry* entry, bool unmanaged = false );
            ~TreeEntryPrivate();

        public:
            const git_tree_entry*   mEntry;
            bool                    mUnmanaged;
        };

    }

}
