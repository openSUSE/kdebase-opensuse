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


#include "MigrationWizard.h"
#include "AppsList.h"

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>

#include <klocale.h>

static QWizardPage* createIntroPage(QWidget* parent)
{
    // Page1: Intro
    QWizardPage* page = new QWizardPage(parent);
    page->setTitle(i18n("Introduction"));

    QLabel* lbl = new QLabel(page);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);

    lbl->setText(
        i18n("On openSUSE, KDE 3 and KDE 4 use different "
             "configuration folders, to avoid their "
             "configuration to be mixed. By completing this "
             "wizard, your existing KDE 3 configuration will "
             "be converted for KDE 4."));

    lbl->setWordWrap(true);

    page->setLayout(layout);

    return page;
}

static QWizardPage* createCompletePage(QWidget* parent)
{
    // Page1: Intro
    QWizardPage* page = new QWizardPage(parent);
    page->setTitle(i18n("Completed Migration"));

    QLabel* lbl = new QLabel(page);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);

    lbl->setText(
        i18n("Congratulations!"));

    page->setFinalPage(true);
    page->setLayout(layout);

    return page;
}

static QWizardPage* createServicesPage(QWidget* parent)
{
    // Page1: Intro
    QWizardPage* page = new QWizardPage(parent);
    page->setTitle(i18n("KDE 3 Service Migration"));

    QLabel* lbl = new QLabel(page);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);

    lbl->setText(
        i18n("The following KDE3 services have been detected "
             "for migration:"
            ));

    lbl->setWordWrap(true);

    QListWidget* lw = new QListWidget(page);
    layout->addWidget(lw);

    page->setLayout(layout);

    return page;
}

static QWizardPage* createProgressPage(QWidget* parent)
{
    // Page1: Intro
    QWizardPage* page = new QWizardPage(parent);
    page->setTitle(i18n("Migrating..."));

    QLabel* lbl = new QLabel(page);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);

    lbl->setText(
        i18n("Please Wait.."));

    page->setLayout(layout);

    return page;
}


// -----------------------------------------------------------------------------

MigrationAppsPage::MigrationAppsPage(QWidget* _parent)
    : QWizardPage(_parent)
{
    setTitle(i18n("KDE 3 Application Migration"));

    QLabel* lbl = new QLabel(this);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);

    lbl->setText(
        i18n("The following KDE3 applications have been detected "
             "for migration:"
             ));

    lbl->setWordWrap(true);

    mAppsWidget = new QListWidget(this);
    layout->addWidget(mAppsWidget);

    setLayout(layout);
}

void MigrationAppsPage::initializePage()
{
    AppList a;
    a.generateAppList();

    for (QList<AppListItem>::const_iterator it=a.appLists.begin();
         it != a.appLists.end(); ++it) {
        QListWidgetItem* item = new QListWidgetItem(it->appName, mAppsWidget);

        item->setCheckState(Qt::Checked);
    }
}

// -----------------------------------------------------------------------------

MigrationWizard::MigrationWizard()
{
    //----  setup the pages
    addPage(createIntroPage(this));
    //addPage(createServicesPage(this));
    addPage((appsPage = new MigrationAppsPage(this)));
    addPage(createProgressPage(this));
    addPage(createCompletePage(this));

    setWindowTitle(i18n("KDE 3 to KDE 4 configuration migration"));
}

MigrationWizard::~MigrationWizard()
{
}
