/***************************************************************************
                          krpmview.cpp  -  description
                             -------------------
    begin                : Die Jul  9 13:09:32 CEST 2002
    copyright            : (C) 2002 by Adrian Schröter
    email                : adrian@suse.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlayout.h>
#include <kprocess.h>
#include <krun.h>
#include <time.h>
#include <QTextDocument>
#include <ktextbrowser.h>

#define _RPM_4_4_COMPAT 1
#include <rpm/rpmlegacy.h>
#include <KLocalizedString>
#include <K4AboutData>
#include <kcomponentdata.h>
#include <kgenericfactory.h>

#include "krpmview.h"
#include "krpmview_factory.h"

// typedef KParts::GenericFactory<KRPMViewPart> KRPMViewFactory;
K_EXPORT_COMPONENT_FACTORY( libkrpmview, KRPMViewFactory );

KRPMViewPart::KRPMViewPart( QWidget *parentWidget,
            QObject *parent, const QStringList &)
 : KParts::ReadOnlyPart( parent )
{
  setComponentData( KRPMViewPart::componentData() );

  // Create the tree widget, and set it as the part's widget
  box = new QWidget(parentWidget);

  // button widget
  QWidget *bwidget    = new QWidget( box );
  QVBoxLayout *Layout = new QVBoxLayout( bwidget );
  Layout->setSpacing( 6 );
  Layout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding ) );

  QHBoxLayout *LayoutInner = new QHBoxLayout();
  LayoutInner->setSpacing( 6 );
  LayoutInner->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );

  PushButtonPackage = new QPushButton( bwidget );
  PushButtonPackage->setText( i18n( "Install Package with YaST" ) );
  LayoutInner->addWidget( PushButtonPackage );
  LayoutInner->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );

  PushButtonDir = new QPushButton( bwidget );
  PushButtonDir->setEnabled(false);
  PushButtonDir->setText( i18n( "Use Directory as Source with YaST" ) );
  LayoutInner->addWidget( PushButtonDir );
  LayoutInner->addItem(new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));
  Layout->addLayout( LayoutInner );
  Layout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
  bwidget->setMaximumHeight( PushButtonPackage->fontInfo().pixelSize() * 4 );
  PushButtonDir->setHidden(true);


  // create the tab box
  tab = new QTabBar( box );
  browserDescription = new KTextBrowser( tab, "browser" );
  browserTechnicalData = new KTextBrowser( tab, "technicaldata" );
  browserDependencies = new KTextBrowser( tab, "dependencies" );
  browserChangelog   = new KTextBrowser( tab, "changelog" );
  browserFilelist    = new KTextBrowser( tab, "filelist" );
  browserTechnicalData->hide();
  browserDependencies->hide();
  browserChangelog->hide();
  browserFilelist->hide();

  tab->addTab( i18n("Description") );
  tab->addTab( i18n("Technical Data") );
  tab->addTab( i18n("Dependencies") );
  tab->addTab( i18n("File List") );
  tab->addTab( i18n("Change Log") );
  connect( tab, SIGNAL(selected(int)), this, SLOT(selectedTab(int)) );

  QVBoxLayout *boxlayout = new QVBoxLayout;
  boxlayout->addWidget(bwidget);
  boxlayout->addWidget(tab);
  boxlayout->addWidget(browserDescription);
  boxlayout->addWidget(browserTechnicalData);
  boxlayout->addWidget(browserDependencies);
  boxlayout->addWidget(browserChangelog);
  boxlayout->addWidget(browserFilelist);
  box->setLayout(boxlayout);

  setWidget( box );

  connect( browserDescription, SIGNAL(urlClick(const QString&)), this, SLOT(urlClick(const QString&)) );
  connect( PushButtonPackage, SIGNAL(clicked()), this, SLOT(install_package()) );
  connect( PushButtonDir, SIGNAL(clicked()), this, SLOT(use_directory()) );

  // Action creation code will go here
  setXMLFile( QLatin1String( "krpmview.rc") );
}

void KRPMViewPart::selectedTab(int t)
{
  browserDescription->hide();
  browserDependencies->hide();
  browserTechnicalData->hide();
  browserChangelog->hide();
  browserFilelist->hide();
  switch( t ){
    case 0:
     browserDescription->show();
     break;
    case 1:
     browserTechnicalData->show();
     break;
    case 2:
     browserDependencies->show();
     break;
    case 3:
     browserFilelist->show();
     break;
    case 4:
     browserChangelog->show();
     break;
  }
}

QString KRPMViewPart::createDependencyList(const Header &h, const QString &caption, int TAGNAME, int TAGVERSION, int TAGFLAGS, bool strongState)
{
  QString result;
  void *tmpVoid = 0;
  rpm_count_t nEntries;
  bool captionAdded = false;

  if (headerGetEntry(h, (rpmTag)TAGNAME, NULL, &tmpVoid, &nEntries)) {
     const char **files = (const char **)tmpVoid;
     headerGetEntry(h, (rpmTag)TAGVERSION, NULL, &tmpVoid, NULL);
     const char **version = (const char **)tmpVoid;
     headerGetEntry(h, (rpmTag)TAGFLAGS, NULL, &tmpVoid, NULL);
     const uint *flags = (const uint *)tmpVoid;
     for (int i = 0; i < nEntries; i++){
        if (((flags[i] & RPMSENSE_MISSINGOK) == RPMSENSE_MISSINGOK) == strongState) {
          if (!captionAdded) {
            result += QLatin1String("<h3>")+caption+QLatin1String("</h3>");
            captionAdded = true;
          }
          result += QString::fromLatin1(files[i]);
          if (version[i] && *version[i]) {
            result += QLatin1String(" ");
            if (flags[i] & RPMSENSE_LESS)
              result += QLatin1String("<");
            if (flags[i] & RPMSENSE_GREATER)
              result += QLatin1String(">");
            if (flags[i] & RPMSENSE_EQUAL)
              result += QLatin1String("=");
            result += QLatin1String(" ") + QString::fromLatin1(version[i]);
          }
          result += QLatin1String("<br>");
       }
     }
  }
  return result;
}

bool KRPMViewPart::openFile()
{
  QString changelog, filelist, temp, technicaldata, dependencies;
  rpm_count_t nfiles;
  rpm_count_t numchangelog;
  int i;
  FD_t fd;
  Header h;
  rpmts ts;

  fd = Fopen(localFilePath().toAscii(), "r.ufdio");
  if (fd == 0 || Ferror(fd))
     return false;

  ts = rpmtsCreate();
  rpmtsSetVSFlags(ts, rpmVSFlags(_RPMVSF_NODIGESTS|_RPMVSF_NOSIGNATURES|RPMVSF_NOHDRCHK));

  if (rpmReadPackageFile(ts, fd, "krpmview", &h) != RPMRC_OK) {
     rpmtsFree(ts);
     Fclose(fd);
     return false;
  }
  rpmtsFree(ts);

  const char *name;
  const char *version;
  const char *release;
  const char *summary;
  const char *description;
  const char *url;
  const char *vendor;


  headerNVR(h, &name, &version, &release);
  void *tmpVoid = 0;
  if (!headerGetEntry(h, RPMTAG_DESCRIPTION, NULL, &tmpVoid, NULL))
    description=0;
  else
    description=(const char*)tmpVoid;

  if (!headerGetEntry(h, RPMTAG_SUMMARY, NULL, &tmpVoid, NULL))
    summary=0;
  else
    summary=(const char*)tmpVoid;

  if (!headerGetEntry(h, RPMTAG_VENDOR, NULL, &tmpVoid, NULL))
    vendor=0;
  else
    vendor=(const char*)tmpVoid;

  if (!headerGetEntry(h, RPMTAG_URL, NULL, &tmpVoid, NULL))
    url=0;
  else
    url=(const char*)tmpVoid;

  if (headerGetEntry(h, RPMTAG_CHANGELOGTEXT, NULL, &tmpVoid, &numchangelog)) {
      const char **changelogtext=(const char**)tmpVoid;
      headerGetEntry(h, RPMTAG_CHANGELOGNAME, NULL, &tmpVoid, NULL);
      const char **changelogname=(const char**)tmpVoid;
      headerGetEntry(h, RPMTAG_CHANGELOGTIME, NULL, &tmpVoid, NULL);
      const uint_32 * changelogtime=(const uint_32 *)tmpVoid;
      for (i = 0; i < numchangelog; i++)
        {
          time_t t = changelogtime[i];
	  temp = QString::fromLatin1(changelogname[i]);
          temp.sprintf("* %.24s %s\n\n%s\n\n", ctime(&t), changelogname[i], changelogtext[i]);
          changelog += temp;
        }
  }
  const char **files = NULL;
  if (headerGetEntry(h, RPMTAG_FILENAMES, NULL, &tmpVoid, &nfiles))
     files = (const char **)tmpVoid;
  else if (headerGetEntry(h, RPMTAG_BASENAMES, NULL, &tmpVoid, &nfiles))
     files = (const char **)tmpVoid;
  if (files != NULL ) {
     headerGetEntry(h, RPMTAG_DIRNAMES, NULL, &tmpVoid, 0);
     const char **dirs = (const char **)tmpVoid;
     headerGetEntry(h, RPMTAG_DIRINDEXES, NULL, &tmpVoid, 0);
     int *idx = (int *)tmpVoid;
     for (i = 0; i < nfiles; i++){
        temp.sprintf("%s%s\n", dirs[idx[i]], files[i]);
        filelist += temp;
     }
  }

  if (headerGetEntry(h, RPMTAG_LICENSE, NULL, &tmpVoid, NULL))
      technicaldata += i18n("License: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_GROUP, NULL, &tmpVoid, NULL))
      technicaldata += i18n("RPM group: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_DISTRIBUTION, NULL, &tmpVoid, NULL))
      technicaldata += i18n("Distribution: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_PACKAGER, NULL, &tmpVoid, NULL))
      technicaldata += i18n("Packager: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_VENDOR, NULL, &tmpVoid, NULL))
      technicaldata += i18n("Vendor: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_BUILDTIME, NULL, &tmpVoid, NULL)) {
      const uint_32 * buildtime=(const uint_32 *)tmpVoid;
      time_t t=buildtime[0];
      temp.sprintf("%.24s", ctime(&t));
      technicaldata += i18n("Build time: %1\n").arg(temp);
  }

  if (headerGetEntry(h, RPMTAG_BUILDHOST, NULL, &tmpVoid, NULL))
      technicaldata += i18n("Build host: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  if (headerGetEntry(h, RPMTAG_SIZE, NULL, &tmpVoid, NULL)) {
      const uint *size = (const uint *)tmpVoid;
      temp.sprintf("%d", *size);
      technicaldata += i18n("Size: %1\n").arg(temp);
  }
  if (headerGetEntry(h, RPMTAG_SOURCERPM, NULL, &tmpVoid, NULL))
      technicaldata += i18n("Source RPM: %1\n").arg(QLatin1String((const char *)(tmpVoid)));

  dependencies += createDependencyList(h, i18n("Provides"), RPMTAG_PROVIDENAME, RPMTAG_PROVIDEVERSION, RPMTAG_PROVIDEFLAGS, false);

  dependencies += createDependencyList(h, i18n("Requires"), RPMTAG_REQUIRENAME, RPMTAG_REQUIREVERSION, RPMTAG_REQUIREFLAGS, false);

  dependencies += createDependencyList(h, i18n("Conflicts"), RPMTAG_CONFLICTNAME, RPMTAG_CONFLICTVERSION, RPMTAG_CONFLICTFLAGS, false);

  dependencies += createDependencyList(h, i18n("Obsoletes"), RPMTAG_OBSOLETENAME, RPMTAG_OBSOLETEVERSION, RPMTAG_OBSOLETEFLAGS, false);

  dependencies += createDependencyList(h, i18n("Recommends"), RPMTAG_SUGGESTNAME, RPMTAG_SUGGESTVERSION, RPMTAG_SUGGESTFLAGS, true);

  dependencies += createDependencyList(h, i18n("Suggests"), RPMTAG_SUGGESTNAME, RPMTAG_SUGGESTVERSION, RPMTAG_SUGGESTFLAGS, false);

  dependencies += createDependencyList(h, i18n("Enhances"), RPMTAG_ENHANCENAME, RPMTAG_ENHANCEVERSION, RPMTAG_ENHANCEFLAGS, false);

  dependencies += createDependencyList(h, i18n("Supplements"), RPMTAG_ENHANCENAME, RPMTAG_ENHANCEVERSION, RPMTAG_ENHANCEFLAGS, true);

  Fclose(fd);

  QString text;
  text =  QLatin1String("<h2>") + Qt::convertFromPlainText(QString::fromLatin1(name) +
          QLatin1String(" - ") + QString::fromLatin1(summary)) +
      QLatin1String("</h2><h3>") + 
          QLatin1String(" Version: ") + QString::fromLatin1(version) 
          + QLatin1String("-") + QString::fromLatin1(release) + QLatin1String("</h3>");
  text += QLatin1String("<p>") + i18n("Project Page: ") + QLatin1String("<a href=") + QString::fromLocal8Bit(url) 
      + QLatin1String(">") + QString::fromLocal8Bit(url) + QLatin1String("</a>");
//  text += "<p>" + i18n("Vendor: ") + QStyleSheet::convertFromPlainText(QString::fromLocal8Bit(vendor));
  text += QLatin1String("<p><p>");
  text += Qt::convertFromPlainText(QString::fromLocal8Bit(description));
  browserDescription->setText( text );
  browserDescription->setNotifyClick(true);
  browserDependencies->setText( dependencies );
  browserTechnicalData->setText( technicaldata );
  browserChangelog->setText( changelog );
  browserFilelist->setText( filelist );

  if ( KParts::ReadOnlyPart::url().isLocalFile() )
    PushButtonDir->setEnabled(true);
  else
    PushButtonDir->setEnabled(false);

  // Some feedback for the user
  emit setStatusBarText( i18n("Parsing complete.") );
  return true;
}

void KRPMViewPart::urlClick(const QString &url)
{
  KRun::runUrl( url, QLatin1String("text/html"), 0 );
}

void KRPMViewPart::install_package()
{
  KProcess p;
  p << QLatin1String("kdesu") << QLatin1String("-n") 
      << QLatin1String("--attach") << QString::number(widget()->window()->winId()) << QLatin1String("--") 
      << QLatin1String("/usr/share/krpmview/setup_temp_source") << localFilePath();
  p.execute();
}

void KRPMViewPart::use_directory()
{
  KProcess p;
  p << QLatin1String("kdesu") << QLatin1String("-n")
      << QLatin1String("--attach") << QString::number(widget()->window()->winId()) << QLatin1String("--") 
      << QLatin1String("/usr/bin/kde_add_yast_source.sh");
  p <<
      KParts::ReadOnlyPart::url().path().left(KParts::ReadOnlyPart::url().path().lastIndexOf(QLatin1Char('/')));

  p.execute();
}

K4AboutData* KRPMViewPart::createAboutData()
{

//
//    KAboutData* aboutData = new KAboutData(
//        QStringLiteral("krpmview"),
//        xi18nc("@title", "<application>KDE RPM Viewer</application>"),
//        "0.2",
//        i18nc("@title", "View the content of RPM archives and use YaST to install them"),
//        KAboutLicense::GPL,
//        i18nc("@info:credit", "© 2003 Novell Inc."));
//    aboutData->setOrganizationDomain(QByteArray("kde.org"));
//    aboutData->setProductName(QByteArray("krpmview"));

//    aboutData->addAuthor(i18nc("@info:credit", "Adrian Schröter"), i18nc("@info:credit", "Original Author"), QStringLiteral("adrian@suse.de"));
//    aboutData->addAuthor(i18nc("@info:credit", "Raymond Wooninck"), i18nc("@info:credit", "Maintainer"), QStringLiteral("tittiatcoke@gmail.com"));
//    aboutData->setHomepage(QStringLiteral("https://www.opensuse.org"));
//
//    return aboutData;

    K4AboutData* about = new K4AboutData( "krpmview", 0, ki18n("krpmview"),
            "0.1",
            ki18n( "Viewer for RPM files"),
            K4AboutData::License_GPL,
            ki18n("(C) 2003 Novell Inc."),
            ki18n( "KRPMView views the content of RPM archives "
                       "and can use YaST to install them" ) );
    about->addAuthor( ki18n("Adrian Schröter"),
                      ki18n( "Current Maintainer" ),
                      "adrian@suse.de",
                      "http://www.suse.de" );

    return about;

}

#include "krpmview.moc"
