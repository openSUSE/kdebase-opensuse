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

#ifndef QuickMigrate_h
#define QuickMigrate_h

#include <QtCore/QString>

class QuickMigrate {

public:

    QuickMigrate();

    int exec();
private:
    bool migrateMultipleFiles(const QString& fpattern);

    QString m_kde3path;
    QString m_kde4path;
};

#endif
