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

#include <QStringList>
#include <QDateTime>
#include <QHash>
#include <QMetaType>
#include <QVector>
#include <QSharedData>

/** @defgroup GitWrap Qt-based wrapper for Git revision control featuring libgit2
 * Wrapper for the LibGit2 library based on Qt4/Qt5.
 */

#ifdef GitWrap_EXPORTS
#	define GITWRAP_API Q_DECL_EXPORT
#else
#	define GITWRAP_API Q_DECL_IMPORT
#endif

#ifndef GW_NO_DEPRECATION
#   define GW_DEPRECATED Q_DECL_DEPRECATED
#else
#   define GW_DEPRECATED
#endif

#if QT_VERSION < 0x050400
// Apparently, this has to be public :-(
// GW_ macros use it
#define qUtf8Printable qPrintable
#endif

#ifndef GW_CPP11
#  ifdef _MSC_VER
#    define     GW_CPP11        1
#  else
#    if __cplusplus < 201103L
#      define   GW_CPP11        0
#    else
#      define   GW_CPP11        1
#    endif
#  endif
#endif

#ifndef _MSC_VER
#  define     GW_CONSTEXPR    constexpr
#else
#  define     GW_CONSTEXPR
#endif

namespace Git
{

    namespace Internal
    {
        // For compat reasons
        template<typename T> class GitPtr;

    }

    class ChangeListConsumer;
    class DiffList;
    class Index;
    class IndexConflict;
    class IndexConflicts;
    class IndexEntry;
    class Object;
    class ObjectId;
    class Blob;
    class BranchRef;
    class Commit;
    class NoteRef;
    class Tag;
    class TagRef;
    class Tree;
    class PatchConsumer;
    class RefName;
    class RefLog;
    class RefLogEntry;
    class RefSpec;
    class Reference;
    class Remote;
    class Repository;
    class RevisionWalker;
    class Signature;
    class Submodule;
    class TreeBuilder;
    class TreeEntry;
    struct ChangeListEntry;

    typedef QVector< ChangeListEntry >  ChangeList;
    typedef QVector< ObjectId >         ObjectIdList;
    typedef QVector< Reference >        ReferenceList;
    typedef QVector< Remote >           RemoteList;
    typedef QVector< Submodule >        SubmoduleList;
    typedef QVector< Commit >           CommitList;

    typedef GW_DEPRECATED CommitList ObjectCommitList;

    /**
     * @enum        Status
     * @ingroup     GitWrap
     * @brief       Represents the modified state for a single file.
     *              The file status is combination of the current HEAD commit,
     *              the repository's index and the working tree.
     *
     * @var         FileInvalidStatus
     *              This status marks an error (all flags cleared).
     *
     * @var         FileIndexNew
     *              The file was added to the index and untracked before.
     *
     * @var         FileIndexModified
     *              The file is modified within the index.
     *
     * @var         FileIndexDeleted
     *              The file was removed from the index.
     *
     * @var         FileIndexRenamed
     *              The file was moved or renamed.
     *
     * @var         FileIndexTypeChange
     *              The file type changed in the index.
     *
     * @var         FileWorkingTreeNew
     *              Marks a new, untracked file in the working tree.
     *
     * @var         FileWorkingTreeModified
     *              The file is modified.
     *
     * @var         FileWorkingTreeDeleted
     *              The tracked file was deleted in the working tree.
     *
     * @var         FileWorkingTreeTypeChange
     *              The file type changed in the working tree.
     *
     * @var         FileIgnored
     *              The file is marked as ignored (i.e. it is filtered in .gitignore).
     *
     * @var         FileUnchanged
     *              The file is completely unchanged.
     *
     * @var         SubmoduleInHead
     *              The superproject's HEAD commit contains the submodule.
     *
     * @var         SubmoduleInIndex
     *              The superproject's contains an entry for the submodule.
     *
     * @var         SubmoduleInConfig
     *              The superproject's .gitmodules file contains
     *              an entry for the submodule.
     *
     * @var         SubmoduleInWorkingTree
     *              The superproject's working tree contains
     *              a folder matching the submodule's name.
     *
     * @var         SubmoduleWorkingTreeUninitialized
     *              The folder for the submodule is empty.
     *
     * @var         SubmoduleWorkingTreeIndexModified
     *              The submodule's index contains entries
     *              (submodule is dirty).
     *
     * @var         SubmoduleWorkingTreeWtModified
     *              The submodule's working tree contains modified files
     *              (submodule is dirty).
     *
     * @var         SubmoduleWorkingTreeUntracked
     *              There are untracked files within the submodule's working tree.
     *
     * @var         SubmoduleIndexNew
     *              The submodule entry was added to the index and untracked before.
     *              (same as Status::FileIndexNew)
     *
     * @var         SubmoduleIndexModified
     *              The submodule index entry is modified.
     *              (same as Status::FileIndexModified)
     *
     * @var         SubmoduleIndexDeleted
     *              The submodule entry was deleted from the index and became untracked.
     *              (same as Status::FileIndexDeleted)
     *
     * @var         SubmoduleWorkingTreeNew
     *              The submodule was added to the working tree.
     *              (same as Status::FileWorkingTreeNew)
     *
     * @var         SubmoduleWorkingTreeModified
     *              The submodule was modified in the working tree.
     *              (same as Status::FileWorkingTreeModified)
     *
     * @var         SubmoduleWorkingTreeDeleted
     *              The submodule's directory was deleted from the working tree.
     *              (same as Status::FileWorkingTreeDeleted)
     */
    enum Status
    {
        FileInvalidStatus           = 0,

        FileIndexNew                = (1u << 0),
        FileIndexModified           = (1u << 1),
        FileIndexDeleted            = (1u << 2),
        FileIndexRenamed            = (1u << 3),
        FileIndexTypeChange         = (1u << 4),

        FileWorkingTreeNew          = (1u << 7),
        FileWorkingTreeModified     = (1u << 8),
        FileWorkingTreeDeleted      = (1u << 9),
        FileWorkingTreeTypeChange   = (1u << 10),

        FileIgnored                 = (1u << 14),
        FileUnchanged               = (1u << 15),

        SubmoduleInHead                     = (1u << 16),
        SubmoduleInIndex                    = (1u << 17),
        SubmoduleInConfig                   = (1u << 18),
        SubmoduleInWorkingTree              = (1u << 19),
        SubmoduleWorkingTreeUninitialized   = (1u << 22),
        SubmoduleWorkingTreeIndexModified   = (1u << 26),
        SubmoduleWorkingTreeWtModified      = (1u << 27),
        SubmoduleWorkingTreeUntracked       = (1u << 28),

        // convenience flags
        SubmoduleIndexNew                   = FileIndexNew,
        SubmoduleIndexModified              = FileIndexModified,
        SubmoduleIndexDeleted               = FileIndexDeleted,
        SubmoduleWorkingTreeNew             = FileWorkingTreeNew,
        SubmoduleWorkingTreeModified        = FileWorkingTreeModified,
        SubmoduleWorkingTreeDeleted         = FileWorkingTreeDeleted
    };
    Q_DECLARE_FLAGS ( StatusFlags, Status )

    enum ObjectType
    {
        otTree,
        otCommit,
        otBlob,
        otTag,

        otAny = -1
    };

    enum FileModes
    {
        UnkownAttr          = 0,
        TreeAttr            = 0040000,
        FileAttr            = 0100644,
        FileExecutableAttr  = 0100755,
        GitLinkAttr         = 0120000,
        SubmoduleAttr       = 0160000
    };

    enum ReferenceTypes
    {
        ReferenceDirect,
        ReferenceSymbolic,

        ReferenceInvalid = -1
    };

    enum ReferenceKinds
    {
        BranchReference,
        TagReference,
        NoteReference,

        UnknownReference
    };

    enum CheckoutMode
    {
        CheckoutDryRun,
        CheckoutSafe,
        CheckoutSafeCreate,
        CheckoutForce
    };

    enum CheckoutFlag
    {
        CheckoutUpdateHEAD              = (1UL <<  0),
        CheckoutAllowDetachHEAD         = (1UL <<  1),
        CheckoutForceDetachHEAD         = (1UL <<  2),
        CheckoutCreateLocalBranch       = (1UL <<  3),

        /* these are LG2 flags: */
        CheckoutAllowConflicts          = (1UL <<  8),
        CheckoutRemoveUntracked         = (1UL <<  9),
        CheckoutRemoveIgnored           = (1UL << 10),
        CheckoutNoRefresh               = (1UL << 11),
        CheckoutUpdateOnly              = (1UL << 12),
        CheckoutDontUpdateIndex         = (1UL << 13),
        CheckoutDisablePathSpecMatch    = (1UL << 14),
        CheckoutSkipLockedDirectories   = (1UL << 15),

        CheckoutNone                    = 0
    };

    typedef QFlags<CheckoutFlag> CheckoutFlags;

    typedef QHash<QString, StatusFlags> StatusHash;

    class Result;

    class GITWRAP_API GitWrap
    {
    public:
        GitWrap(bool autoInit = true);
        ~GitWrap();

    public:
        void init();
        void shutDown();

    private:
        bool mInitialised;
    };

}

// Note, that macros like these are also a good way to hide the nasty administrative stuff from
// Doxygen, later...

#define GW_PRIVATE_DECL_EX(SHARE, CLASS, BASE, SCOPE) \
    public: \
        typedef Internal::SHARE##Private Private; \
        typedef Internal::GitPtr<Private> PrivatePtr; \
    \
    SCOPE: \
        CLASS(Private* _d = nullptr) \
            : BASE((BASE::Private*)(_d)) \
            {} \
        \
        CLASS(const PrivatePtr& _d); \
        \
        CLASS(const CLASS& other) \
            : BASE(other) \
            {}

#define GW_PRIVATE_IMPL(CLASS, BASE) \
    CLASS::CLASS(const PrivatePtr& d) \
        : BASE(d.data()) \
        {}

#define GW_PRIVATE_DECL(CLASS, BASE, SCOPE) \
    GW_PRIVATE_DECL_EX(CLASS, CLASS, BASE, SCOPE)

#define GW_PRIVATE_OBJECT_DECL(CLASS, BASE, SCOPE) \
    GW_PRIVATE_DECL(CLASS, BASE, SCOPE) \
public: \
    enum { ObjectTypeId = ot##CLASS };
