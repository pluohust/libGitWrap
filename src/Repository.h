/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <QStringList>

#include "GitWrap.h"

namespace Git
{

	class Submodule;
	class Reference;
	class Error;
	class DiffList;
	class Index;
	class Object;
	class ObjectCommit;
	class ObjectTag;
	class ObjectBlob;
	class ObjectTree;
	class ObjectId;
	class RevisionWalker;
	class Remote;

	namespace Internal
	{
		class RepositoryPrivate;
	}

	typedef QHash< QString, ObjectId > ResolvedRefs;

    /**
	 * @ingroup		GitWrap
	 * @brief		A git repository
	 *
     */
	class GITWRAP_API Repository
	{
	public:
		explicit Repository( Internal::RepositoryPrivate* _d );
		Repository();
		Repository( const Repository& other );
		Repository& operator=( const Repository& other );

	public:
		~Repository();

	public:
		static Repository create( const QString& path,
								  bool bare,
								  Result& result = GitWrap::lastResult() );

		static QString discover( const QString& startPath,
								 bool acrossFs = false,
								 const QStringList& ceilingDirs = QStringList(),
								 Result& result = GitWrap::lastResult() );

		static Repository open( const QString& path,
								Result& result = GitWrap::lastResult() );

		bool isValid() const;
		bool isBare() const;

		QString basePath() const;
		QString gitPath() const;

		QStringList allReferences();
		QStringList allBranches();
		QString currentBranch();
		QStringList branches( bool local = true, bool remote = false );
		QStringList allTags();

		ResolvedRefs allResolvedRefs();

		bool renameBranch( const QString& oldName, const QString& newName, bool force = false );

		Index index();

		StatusHash statusHash();

		Reference HEAD();

		Object lookup( const ObjectId& id, ObjectType ot = otAny );
		ObjectCommit lookupCommit( const ObjectId& id );
		ObjectTree lookupTree( const ObjectId& id );
		ObjectBlob lookupBlob( const ObjectId& id );
		ObjectTag lookupTag( const ObjectId& id );

		bool shouldIgnore( const QString& filePath ) const;

		RevisionWalker newWalker();

		QStringList allRemotes() const;
		Remote remote( const QString& remoteName ) const;
		Remote createRemote( const QString& remoteName, const QString& url,
							 const QString& fetchSpec );

		DiffList diffCommitToCommit( ObjectCommit oldCommit, ObjectCommit newCommit );
		DiffList diffTreeToTree( ObjectTree oldTree, ObjectTree newTree );
		DiffList diffIndexToTree( ObjectTree oldTree );
		DiffList diffTreeToWorkingDir( ObjectTree oldTree );
		DiffList diffIndexToWorkingDir();

		QList< Error > recentErrors();

		QList< Submodule > submodules();
		Submodule submodule( const QString& name );

	private:
		Internal::GitPtr< Internal::RepositoryPrivate > d;
	};

}

#endif
