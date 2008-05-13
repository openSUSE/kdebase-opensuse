/*  This file is part of the KDE project
    Copyright (C) 2008 Dirk Mueller <mueller@kde.org>

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

#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "AppsList.h"

const char* const blackList[] = {
    "default_desktop",
    "kbabel",
    "kbabeldict",
    "kbfx",
    "kconf_update",
    "kdeprint",
    "kdesktop",
    "kfm",
    "kicker",
    "knemo",
    "kssl",
    NULL
};

static bool is_blacklisted(const QByteArray& appname)
{
    const char* const* begin = &blackList[0];
    const char* const* end = &blackList[sizeof(blackList)/sizeof(*blackList)]-1;

    // ### binary search
    return qBinaryFind(begin, end, appname.data()) != end;
}

AppList::AppList()
{

}

void AppList::generateAppList()
{
    // list all portable kde3 apps

    QDir kde3path( QDir::homePath() + "/.kde/share/apps");

    QByteArray kdehome = qgetenv("KDEHOME");
    if (kdehome.isEmpty())
        kdehome = QDir::homePath().toUtf8() + "/.kde4/";

    QDir kde4path( QDir::homePath() + "/.kde/share/apps");

    for (QStringListIterator it(kde3path.entryList()); it.hasNext();) {
        QByteArray app = QFile::encodeName(it.next());

        if (app[0] == '.')
            continue;

        qDebug() << "looking at" << app;

        bool blacklisted = is_blacklisted(app);
        if (blacklisted)
            continue;

        qDebug() << "candidate: " << app;
        AppListItem item;
        item.appName = app;
        item.dir = kde3path.path() + "/" + QFile::decodeName(app);
        appLists << item;
    }

}

#include "AppsList.h"


