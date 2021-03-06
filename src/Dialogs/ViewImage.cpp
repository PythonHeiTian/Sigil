/************************************************************************
**
**  Copyright (C) 2019-2020 Kevin B. Hendricks
**  Copyright (C) 2013      Dave Heiland
**
**  This file is part of Sigil.
**
**  Sigil is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Sigil is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Sigil.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#include <QtCore/QFileInfo>
#include <QApplication>
#include <QGuiApplication>
#include <QtWidgets/QLayout>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineProfile>

#include "MainUI/MainWindow.h"
#include "ResourceObjects/ImageResource.h"
#include "ViewEditors/SimplePage.h"
#include "Misc/Utility.h"
#include "sigil_constants.h"
#include "Dialogs/ViewImage.h"

static QString SETTINGS_GROUP = "view_image";

ViewImage::ViewImage(QWidget *parent)
    :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.webView->setPage(new SimplePage(ui.webView));
    ui.webView->setContextMenuPolicy(Qt::NoContextMenu);
    ui.webView->setFocusPolicy(Qt::NoFocus);
    ui.webView->setAcceptDrops(false);
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    ui.webView->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,false);
#endif

    ReadSettings();
}

ViewImage::~ViewImage()
{
    WriteSettings();
}

void ViewImage::ShowImage(QString path)
{
    ui.webView->page()->profile()->clearHttpCache();
    const QUrl resourceUrl = QUrl::fromLocalFile(path);
    QString html = IMAGE_HTML_BASE_PREVIEW.arg(resourceUrl.toString());
    if (Utility::IsDarkMode()) {
	html = Utility::AddDarkCSS(html);
    }
    ui.webView->page()->setBackgroundColor(Utility::WebViewBackgroundColor());
    ui.webView->setHtml(html, resourceUrl);
    QApplication::processEvents();
}

void ViewImage::ReadSettings()
{
    SettingsStore settings;
    settings.beginGroup(SETTINGS_GROUP);
    // The size of the window and it's full screen status
    QByteArray geometry = settings.value("geometry").toByteArray();

    if (!geometry.isNull()) {
        restoreGeometry(geometry);
    }

    settings.endGroup();
}

void ViewImage::WriteSettings()
{
    SettingsStore settings;
    settings.beginGroup(SETTINGS_GROUP);
    // The size of the window and it's full screen status
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

