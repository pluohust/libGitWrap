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

#ifndef GIT_TREE_BUILDER_H
#define GIT_TREE_BUILDER_H

#include "libGitWrap/RepoObject.hpp"

namespace Git
{

    namespace Internal
    {
        class TreeBuilderPrivate;
    }

    /**
     * @ingroup     GitWrap
     * @brief       Tool for creating tree objects
     */
    class GITWRAP_API TreeBuilder : public RepoObject
    {
        GW_PRIVATE_DECL(TreeBuilder, RepoObject, public)

    public:
        void clear( Result& result );

        TreeEntry get( Result& result, const QString& name );
        bool insert( const QString& fileName, FileModes type, const ObjectId& oid,
                     Result& result );
        bool remove( Result& result, const QString& fileName );
        ObjectId write( Result& result );
    };

}

Q_DECLARE_METATYPE( Git::TreeBuilder )

#endif
