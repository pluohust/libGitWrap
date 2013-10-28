/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#include "Base.hpp"

#include "Private/BasePrivate.hpp"

namespace Git
{

    namespace Internal
    {

        BasePrivate::BasePrivate()
        {
        }

        BasePrivate::~BasePrivate()
        {
        }

        const git_oid* BasePrivate::sha(const ObjectId& id)
        {
            return reinterpret_cast<const git_oid*>(id.raw());
        }

    }

    Base::Base()
    {
    }

    Base::Base(Internal::BasePrivate &_d)
        : mData(&_d)
    {
    }

    Base::Base(const Base& other)
        : mData(other.mData)
    {
    }

    Base::~Base()
    {
    }

    Base& Base::operator=(const Base& other)
    {
        mData = other.mData;
        return * this;
    }

    bool Base::operator==(const Base& other) const
    {
        return mData == other.mData;
    }

    bool Base::isValid() const
    {
        return mData;
    }

}
