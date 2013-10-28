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

#ifndef GIT_DIFFLIST_H
#define GIT_DIFFLIST_H

#include "libGitWrap/RepoObject.hpp"

namespace Git
{

    namespace Internal
    {
        class DiffListPrivate;
    }

    /**
     * @ingroup     GitWrap
     * @brief       List of differences between to objects
     *
     */
    class GITWRAP_API DiffList : public RepoObject
    {
    public:
        typedef Internal::DiffListPrivate Private;

    public:
        explicit DiffList(Internal::DiffListPrivate& _d);

    public:
        DiffList(const DiffList& other);
        DiffList();
        ~DiffList();

    public:
        DiffList& operator=( const DiffList& other );

    public:
        bool mergeOnto( Result& result, DiffList other ) const;

        bool consumePatch( Result& result, PatchConsumer* consumer ) const;
        bool consumeChangeList( Result& result,
                                ChangeListConsumer* consumer ) const;

        ChangeList changeList(Result& result) const;

        bool findRenames( Result& result );
    };

}

Q_DECLARE_METATYPE( Git::DiffList )

#endif
