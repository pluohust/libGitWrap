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

#include "GitWrapPrivate.hpp"
#include "ReferencePrivate.hpp"
#include "ObjectId.hpp"
#include "Repository.hpp"
#include "Reference.hpp"

namespace Git
{

    namespace Internal
    {

        ReferencePrivate::ReferencePrivate( const GitPtr< RepositoryPrivate >& repo,
                                            git_reference* ref )
            : RepoObject( repo )
            , mRef( ref )
        {
            Q_ASSERT( ref );
        }

        ReferencePrivate::~ReferencePrivate()
        {
            git_reference_free( mRef );
        }

    }

    Reference::Reference()
    {
    }

    Reference::Reference( Internal::ReferencePrivate* _d )
        : d( _d )
    {
    }

    Reference::Reference( const Reference& other )
        : d( other.d )
    {
    }

    Reference::~Reference()
    {
    }


    Reference& Reference::operator=( const Reference& other )
    {
        d = other.d;
        return * this;
    }

    bool Reference::isValid() const
    {
        return d;
    }

    bool Reference::destroy( Result& result )
    {
        if( !result )
        {
            return false;
        }

        if( !isValid() )
        {
            return false;
        }

        return git_reference_delete( d->mRef );
    }

    QString Reference::name() const
    {
        if( !isValid() )
        {
            GitWrap::lastResult().setInvalidObject();
            return QString();
        }

        return QString::fromUtf8( git_reference_name( d->mRef ) );
    }


    Reference::Type Reference::type( Result& result ) const
    {
        if( !result )
        {
            return Invalid;
        }

        if( !isValid() )
        {
            result.setInvalidObject();
            return Invalid;
        }

        switch( git_reference_type( d->mRef ) )
        {
        case GIT_REF_SYMBOLIC:  return Symbolic;
        case GIT_REF_OID:       return Direct;
        default:
        case GIT_REF_INVALID:   return Invalid;
        }
    }

    ObjectId Reference::objectId( Result& result ) const
    {
        if( !result )
        {
            return ObjectId();
        }

        if( !type( result ) == Direct )	// Does the isValid() check for us, no need to repeat it
        {
            return ObjectId();
        }

        return ObjectId::fromRaw( git_reference_target( d->mRef )->id );
    }

    QString Reference::target( Result& result ) const
    {
        if( !result )
        {
            return QString();
        }

        if( !type( result ) == Symbolic )   // Does the isValid() check for us, no need to repeat it
        {
            return QString();
        }
        return QString::fromUtf8( git_reference_symbolic_target( d->mRef ) );
    }

    Repository Reference::repository( Result& result ) const
    {
        if( !result )
        {
            return Repository();
        }

        if( !d )
        {
            return Repository();
        }

        return Repository( d->repo() );
    }

    Reference Reference::resolved( Result& result )
    {
        if( !result )
        {
            return Reference();
        }

        if( !d )
        {
            result.setInvalidObject();
            return Reference();
        }

        git_reference* ref;
        result = git_reference_resolve( &ref, d->mRef );
        if( !result )
        {
            return Reference();
        }

        return new Internal::ReferencePrivate( d->repo(), ref );
    }

    ObjectId Reference::resolveToObjectId( Result& result )
    {
        Reference resolvedRef = resolved( result );
        if( !result )
        {
            return ObjectId();
        }
        return resolvedRef.objectId( result );
    }

    void Reference::checkout(Result &result) const
    {
        if (  !result )
        {
            return;
        }

        if( !d )
        {
            result.setInvalidObject();
            return;
        }

        git_object *o = NULL;
        result = git_reference_peel( &o, d->mRef, GIT_OBJ_TREE );
        if ( !result )
            return;

        git_checkout_opts opts = GIT_CHECKOUT_OPTS_INIT;
        result = git_checkout_tree( d->repo()->mRepo, o, &opts );
    }

}
