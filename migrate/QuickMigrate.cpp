/*  This file is part of openSUSE
    Copyright (C) 2008 Dirk Mueller <dmueller@suse.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "QuickMigrate.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static const char* const migrate_list[] = {
    "share/apps/kabc/*.vcf",
    "share/apps/akregator/data/feeds.opml",
    "share/apps/kcookiejar/cookies",
    "share/apps/kfile/bookmarks.xml",
    "share/apps/khtml/formcompletions",
    "share/apps/konqueror/bookmarks.xml",
    "share/apps/konsole/bookmarks.xml",
    "share/apps/kopete/contactlist.xml",
    "share/apps/kopete/logs/*",
    "share/apps/krdc/bookmarks.xml",
    "share/apps/kwallet/*.kwl",
    "share/apps/RecentDocuments/*.desktop",
    "share/config/akregatorrc",
    "share/config/colors/*",
    "share/config/cryptodefaults",
    "share/config/jukrc",
    "share/config/katerc",
    "share/config/kateschemarc",
    "share/config/kcookiejarrc",
    "share/config/kcmrandrrc",
    "share/config/kcminputrc",
    "share/config/kcmfonts",
    "share/config/kio_*rc",
    "share/config/klipperrc",
    "share/config/knoderc",
    "share/config/konq_history",
    "share/config/konsolerc",
    "share/config/kopeterc",
    "share/config/kpgprc",
    "share/config/kppprc",
    "share/config/ksslpolicies",
    "share/config/kstarsrc",
    "share/config/ksysguardrc",
    "share/config/kwalletrc",
    NULL
};

static bool makeDir(const QString& dir, int mode)
{
    // we want an absolute path
    if (QDir::isRelativePath(dir))
        return false;

    QString target = dir;
    uint len = target.length();

    // append trailing slash if missing
    if (dir.at(len - 1) != '/')
        target += '/';

    QString base("");
    uint i = 1;

    while( i < len )
    {
        struct stat st;
        int pos = target.indexOf('/', i);
        base += target.mid(i - 1, pos - i + 1);
        QByteArray baseEncoded = QFile::encodeName(base);
        // bail out if we encountered a problem
        if (stat(baseEncoded, &st) != 0)
        {
            // Directory does not exist....
            // Or maybe a dangling symlink ?
            if (lstat(baseEncoded, &st) == 0)
                (void)unlink(baseEncoded); // try removing

            if (mkdir(baseEncoded, static_cast<mode_t>(mode)) != 0) {
                baseEncoded.prepend( "trying to create local folder " );
                perror(baseEncoded.data());
                return false; // Couldn't create it :-(
            }
        }
        i = pos + 1;
    }
    return true;
}

static bool migrateOneFile(const QString& from, const QString& to)
{
    if (QFile::exists(to)) {
        return true;
    }

    QFileInfo fi(from);
    if (!fi.exists() || fi.isSymLink())
        return true;

    QFile f(from);
    if (!f.exists())
        return true;

    qDebug() << "migrating " << from << "->" << to;
    QString todir = QFileInfo(to).path();

    // create destination directory if missing
    if (!QFileInfo(todir).exists())
        makeDir(todir, 0777);

    bool r = f.copy(to);
    if (!r) {
        qDebug() << "copy failed..";
    }
    return r;
}

bool QuickMigrate::migrateMultipleFiles(const QString& filepat)
{
    QFileInfo finfo(filepat);
    QString fpath = finfo.path();
    QString fname = finfo.baseName();


    Q_ASSERT(!fpath.contains('*'));
    Q_ASSERT(fname.contains('*'));

    QDir dir(m_kde3path + fpath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden |
            QDir::Readable);
    dir.setNameFilters(QStringList(fname));

    bool state = true;

    QStringList matches = dir.entryList();
    Q_FOREACH(const QString& match, matches)
        state &= migrateOneFile(m_kde3path + fpath + "/" + match, m_kde4path + fpath + "/" + match);
    return state;
}

QuickMigrate::QuickMigrate()
{
    m_kde3path = QDir::homePath() + "/.kde/";

    QByteArray kdehome = qgetenv("KDEHOME");
    if (kdehome.isEmpty())
        kdehome = QDir::homePath().toUtf8() + "/.kde4/";

    m_kde4path = kdehome;

    if (!m_kde4path.endsWith("/"))
        m_kde4path += "/";

    QFileInfo fi(m_kde4path);

    if (!fi.exists() || !fi.isDir())
        makeDir(m_kde4path, 0700);
}

int QuickMigrate::exec()
{
    int fails = 0;
    for (const char* const* f = &migrate_list[0]; *f; ++f) {

        if (QByteArray(*f).contains('*')) {
           fails += migrateMultipleFiles(*f);
           continue;
        }

        QString oldfile = m_kde3path + *f;

        if (!migrateOneFile(oldfile, m_kde4path + *f))
            ++fails;
    }
    return fails;
}
