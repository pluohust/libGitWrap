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

#include "libGitWrap/GitWrap.hpp"


namespace Git
{

    /**
     * @ingroup     GitWrap
     * @brief       Encapsulates a result
     *
     * A result object represents either the success or failure (including an error text) of a call
     * to a GitWrap API.
     *
     * See @ref GitWrapErrorHandling on examples of how to handle result objects correctly.
     *
     */
    class GITWRAP_API Result
    {
    public:
        Result();
        Result( const Result& other );
        explicit Result( int resultCode );
        ~Result();
        Result& operator=( const Result& other );
        Result& operator=( int );

    public:
        operator bool() const;

        int errorCode() const;
        int errorClass() const;
        const QString& errorText() const;

    public:
        void clear();

    public:
        void setError(const char* szErrorText, int code);
        void setError(int resultCode);
        void setInvalidObject();

    private:
        int     mCode;
        int     mClass;
        QString mText;
    };

    /**
     * @brief       Default Constructor
     *
     * Creates a valid Result object
     */
    inline Result::Result()
        : mCode( 0 )
        , mClass( 0 )
    {
    }

    /**
     * @brief       Copy Constructor
     * @param[in]   other   The result to copy
     */
    inline Result::Result( const Result& other )
    {
        if( ( mCode = other.mCode ) < 0 )
        {
            mClass = other.mClass;
            mText = other.mText;
        }
    }

    /**
     * @brief   Destructor
     */
    inline Result::~Result()
    {
    }

    /**
     * @brief       Assignment operator
     * @param[in]   other   The result to asign
     * @return      A reference to this result
     */
    inline Result& Result::operator=( const Result& other )
    {
        if( ( mCode = other.mCode ) < 0 )
        {
            mClass = other.mClass;
            mText = other.mText;
        }

        return *this;
    }

    /**
     * @brief       Implicit cast to bool
     *
     * Automatically casts a result to `bool`.
     *
     * @return      `true`, if this result object contains no error (i.e. it was freshly created or
     *              the last operation did succeed) or `false` if the last operation had an error.
     *
     * If this returns `false` use errorCode(), errorClass() and errorText() to get the details.
     *
     */
    inline Result::operator bool() const
    {
        return mCode == 0;
    }

    /**
     * @brief       Access the error code
     * @return      Error code as it was reported by `libgit2`. In most cases this will simply be
     *              -1.
     */
    inline int Result::errorCode() const
    {
        return mCode;
    }

    inline int Result::errorClass() const
    {
        return mClass;
    }

    /**
     * @brief       Access the error text
     * @return      Error text as it was reported by `libgit2`. This text is not localized.
     */
    inline const QString& Result::errorText() const
    {
        return mText;
    }

}

/**
  * @ingroup    GitWrap
  * @def        GW_CHECK_RESULT
  * @brief      Check, if a given @ref Git::Result object is valid.
  *
  *             The current function will exit with the defined return value,
  *             when the result is invalid.
  */
#define GW_CHECK_RESULT(result, returns) \
    do { \
        if (!(result)) { \
            qDebug("File %s, line %d:\nGit returned error code (%d) \"%s\"", \
            __FILE__, __LINE__, (result).errorCode(), qUtf8Printable((result).errorText())); \
            return (returns); \
        } \
    } while (0)
