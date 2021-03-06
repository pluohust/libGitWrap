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

#include <QString>
#include <QStringBuilder>
#include <QDir>

#include "Infra/TempDirProvider.hpp"
#include "Infra/Fixture.hpp"

Fixture::Fixture()
{
}

Fixture::~Fixture()
{
}

void Fixture::SetUp()
{
}

void Fixture::TearDown()
{
}

QString Fixture::dataDir() const
{
    return QString::fromLatin1(DATADIR);
}

bool copyFile(const QString& src, const QString& dest)
{
    QFile fInput(src);
    QFile fOutput(dest);

    if (!fInput.open(QFile::ReadOnly)) {
        return false;
    }

    if (!fOutput.open(QFile::WriteOnly)) {
        return false;
    }

    for (;;) {
        char buffer[1024];
        quint64 size = fInput.read(buffer, sizeof(buffer));
        if (size) {
            fOutput.write(buffer, size);
        }
        else {
            return true;
        }
    }
}

bool copyDir(const QString& src, const QString& dest)
{
    QDir srcDir(src);
    foreach (QString subDir, srcDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::Hidden)) {
        QDir destDir(dest);
        destDir.mkdir(subDir);

        if (!copyDir(src % QChar(L'/') % subDir, dest % QChar(L'/') % subDir)) {
            return false;
        }
    }

    foreach (QString file, srcDir.entryList(QDir::Files|QDir::Hidden)) {
        if (!copyFile(src % QChar(L'/') % file, dest % QChar(L'/') % file)) {
            return false;
        }
    }

    return true;
}

QString Fixture::prepareRepo(const char* name)
{
    QString strName = QString::fromUtf8(name);
    QString sourceDir = dataDir() % QChar(L'/') % strName;
    QString destDir = TempDirProvider::get() % QChar(L'/') % strName;

    QDir(TempDirProvider::get()).mkpath(strName);

    if ( !copyDir(sourceDir, destDir) )
        qCritical( "copyDir() failed: src=%s -> dest=%s", qPrintable(sourceDir), qPrintable(destDir) );

    return destDir;
}
