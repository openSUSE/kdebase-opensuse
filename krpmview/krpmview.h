/***************************************************************************
                          krpmview.h  -  description
                             -------------------
    begin                : Die Jul  9 13:09:32 CEST 2002
    copyright            : (C) 2002 by Adrian Schr�ter
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
#ifndef __plugin_krpmview_h
#define __plugin_krpmview_h
 
#include <KTextBrowser>
#include <qtabbar.h>
#include <qpushbutton.h>
#include <K4AboutData>

#define _RPM_4_4_COMPAT 1

#include <rpmlib.h>
#include <rpm/rpmts.h>
#include <KParts/ReadOnlyPart>

// Forward declarations, saves time compared to including the full headers
class KAboutData;

class KRPMViewPart : public KParts::ReadOnlyPart
{
  Q_OBJECT
public:
  KRPMViewPart( QWidget *parentWidget, 
                QObject *parent, const QStringList &args );
  virtual ~KRPMViewPart() {}

  // Return information about the part
  static K4AboutData* createAboutData();

protected:
  // Open the file whose path is stored in the member variable m_file
  // and return true on success, false on failure.
  virtual bool openFile();

protected slots:
  void urlClick(const QString &url);
  void install_package();
  void use_directory();
  void selectedTab(int);

private:
  // The component's widget
  KTextBrowser *browser;
  KTextBrowser *browserChangelog;
  KTextBrowser *browserDescription;
  KTextBrowser *browserFilelist;
  KTextBrowser *browserTechnicalData;
  KTextBrowser *browserDependencies;
  QWidget      *box;
  QTabBar      *tab;
  QPushButton  *PushButtonPackage;
  QPushButton  *PushButtonDir;

  QString      createDependencyList(const Header &, const QString &caption, 
                  int TAGNAME, int TAGVERSION, int TAGFLAGS, bool strongState);
};
 
#endif
