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

#include "gtest/gtest.h"

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Repository.hpp"

#include "Infra/Fixture.hpp"
#include "Infra/TempRepo.hpp"

typedef Fixture RepositoryFixture;

TEST_F(RepositoryFixture, CanOpen)
{
    TempRepoOpener tempRepo(this, "SimpleRepo1");
    Git::Repository repo(tempRepo);
    ASSERT_TRUE(repo.isValid());

    ASSERT_FALSE(repo.isBare());
    ASSERT_FALSE(repo.isHeadDetached());

    ASSERT_STREQ("SimpleRepo1", qPrintable(repo.name()));
}

TEST_F(RepositoryFixture, CanListBranches)
{
    TempRepoOpener tempRepo(this, "SimpleRepo1");
    Git::Repository repo(tempRepo);
    ASSERT_TRUE(repo.isValid());

    Git::Result r;
    QStringList sl = repo.branchNames(r, true, false);
    ASSERT_TRUE(r);
    ASSERT_EQ(1, sl.count());
    EXPECT_STREQ("master",
                 qPrintable(sl[0]));

    sl = repo.branchNames(r, false, true);
    ASSERT_TRUE(r);
    ASSERT_EQ(0, sl.count());
}

TEST_F(RepositoryFixture, CanDetachHEAD)
{
    TempRepoOpener tempRepo(this, "SimpleRepo1");
    Git::Repository repo(tempRepo);
    ASSERT_TRUE(repo.isValid());

    ASSERT_FALSE(repo.isHeadDetached());

    Git::Result r;
    ASSERT_TRUE(repo.detachHead(r));
    ASSERT_TRUE(r);

    ASSERT_TRUE(repo.isHeadDetached());
}
