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

#ifndef GITWRAP_GITWRAP_H
#define GITWRAP_GITWRAP_H

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

namespace Git
{

    class ChangeListConsumer;
    class DiffList;
    class Index;
    class IndexConflict;
    class IndexConflicts;
    class IndexEntry;
    class Object;
    class ObjectId;
    class Blob;
    class Commit;
    class Tag;
    class Tree;
    class PatchConsumer;
    class RefName;
    class RefSpec;
    class Reference;
    class Remote;
    class Repository;
    class RevisionWalker;
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

    enum CheckoutOption
    {
        CheckoutUpdateHEAD          = (1 << 0),
        CheckoutAllowDetachHEAD     = (1 << 1),
        CheckoutForceDetachHEAD     = (1 << 2),
        CheckoutCreateLocalBranch   = (1 << 3),
        CheckoutForce               = (1 << 4),
        CheckoutNone                = 0
    };

    typedef QFlags<CheckoutOption> CheckoutOptions;

    typedef QHash<QString, StatusFlags> StatusHash;

    class Result;

    class GITWRAP_API GitWrap
    {
    public:
        GitWrap();
        ~GitWrap();

    public:
        static Result& lastResult();
    };

}

#endif
