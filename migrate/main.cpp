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

#undef INTERACTIVE_WIZARD

#include <qstring.h>

#ifdef INTERACTIVE_WIZARD
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kapplication.h>
#include <klocale.h>

#include "MigrationWizard.h"
#else
#include "QuickMigrate.h"
#endif

int main (int argc, char *argv[])
{
#ifdef INTERACTIVE_WIZARD
    KAboutData aboutData( "kde4-migrate", "", ki18n( "KDE4 Migrate" ), "0.1",
                        ki18n( "KDE4 Migrate" ), KAboutData::License_GPL,
                        ki18n( "(c) 2008, Novell, Inc." ) );
    aboutData.addAuthor( ki18n("Dirk Müller"), ki18n( "Author" ), "dmueller@suse.de" );
    KCmdLineArgs::init( argc, argv, &aboutData );

    QApplication app;

    MigrationWizard wizard;
    wizard.show();

    return app.exec();
#else
    return QuickMigrate().exec();
#endif
}
