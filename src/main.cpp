/***************************************************************************
 *   Copyright (C) 2008 by Elad Lahav
 *   elad_lahav@users.sourceforge.net
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ***************************************************************************/

#include <QApplication>
#include <QSettings>
#ifdef __HAIKU__
#include <FindDirectory.h>
#include <Path.h>
#endif
#include "mainwindow.h"

/**
 * Points QSettings at Haiku's per-user settings directory, ~/config/settings.
 * Left to itself, Qt would store settings in its own XDG location, ~/.config,
 * which is not where a Haiku application is expected to keep them.
 */
static void initSettingsPath()
{
#ifdef __HAIKU__
	BPath path;
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK)
		return;
	
	// setPath() only affects the format it is given, while a default
	// constructed QSettings object uses the native one, so make the ini
	// format the default before overriding its location.
	QSettings::setDefaultFormat(QSettings::IniFormat);
	QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
	                   QString::fromUtf8(path.Path()));
#endif
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	QCoreApplication::setOrganizationName("elad_lahav@users.sourceforge.net");
	QCoreApplication::setApplicationName("Quicky");
	
	// Do this before any QSettings object is created.
	initSettingsPath();
	
	MainWindow mainWnd;
	
	// Load the "Welcome" page first, so that there is some current document,
	// in case a file name is not specified on the command line, or it fails
	// to load.
	mainWnd.openHelp();
	
	mainWnd.show();

	// Open a file name specified on the command line.
	if (argc > 1)
		mainWnd.openFile(QApplication::arguments()[1]);
	
	// Start the application's event loop.
	return app.exec();
}

