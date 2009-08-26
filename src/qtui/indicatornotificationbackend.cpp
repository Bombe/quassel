/***************************************************************************
*   Copyright (C) 2005-09 Canonical Ltd                                   *
*   author: aurelien.gateau@canonical.com                                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) version 3.                                           *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "indicatornotificationbackend.h"

#include <qindicateserver.h>
#include <qindicateindicatormessage.h>

#include "clientsettings.h"
#include "mainwin.h"
#include "qtui.h"

class Indicator : public QIndicate::IndicatorMessage {
public:
  uint lastNotificationId;
};

IndicatorNotificationBackend::IndicatorNotificationBackend(QObject *parent)
  : AbstractNotificationBackend(parent)
{
  NotificationSettings notificationSettings;
  _enabled = notificationSettings.value("Indicator/Enabled", true).toBool();

  notificationSettings.notify("Indicator/Enabled", this, SLOT(enabledChanged(const QVariant &)));

  _server = QIndicate::Server::defaultInstance();
  _server->setType("message.im");
  _server->setDesktopFile(DESKTOP_FILE);
  if (_enabled) {
    _server->show();
  }
}

IndicatorNotificationBackend::~IndicatorNotificationBackend() {
  qDeleteAll(_indicatorHash);
}

void IndicatorNotificationBackend::notify(const Notification &notification) {
  if(!_enabled) {
    return;
  }
  if (notification.type != Highlight && notification.type != PrivMsg) {
    return;
  }
  Indicator *indicator = _indicatorHash.value(notification.bufferId);
  if(!indicator) {
    indicator = new Indicator;
    _indicatorHash.insert(notification.bufferId, indicator);
  }
  indicator->lastNotificationId = notification.notificationId;
  indicator->setProperty("sender", notification.sender);
  indicator->setProperty("time", QTime::currentTime());
  qDebug() << "FIXME icon";
  indicator->show();
}

void IndicatorNotificationBackend::close(uint notificationId) {
  // If we find an indicator whose lastNotificationId is notificationId, we
  // delete it

  IndicatorHash::Iterator
    it = _indicatorHash.begin(),
    end = _indicatorHash.end();
  for (; it != end; ++it) {
    if (it.value()->lastNotificationId == notificationId) {
      break;
    }
  }
  if (it == end) {
    // Not found, maybe there was no indicator for this notification or another
    // notification happened after
    return;
  }
  _indicatorHash.erase(it);
  delete it.value();
}

void IndicatorNotificationBackend::enabledChanged(const QVariant &v) {
  _enabled = v.toBool();
  if (_enabled) {
    _server->show();
    // TODO: Create indicators for existing highlighted buffers?
  } else {
    _server->hide();
    qDeleteAll(_indicatorHash);
  }
}

SettingsPage *IndicatorNotificationBackend::createConfigWidget() const {
  return new ConfigWidget();
}

/***************************************************************************/

IndicatorNotificationBackend::ConfigWidget::ConfigWidget(QWidget *parent)
: SettingsPage("Internal", "IndicatorNotification", parent)
{
  ui.setupUi(this);

  connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(widgetChanged()));
}

IndicatorNotificationBackend::ConfigWidget::~ConfigWidget() {
}

void IndicatorNotificationBackend::ConfigWidget::widgetChanged() {
  bool changed = enabled != ui.enabled->isChecked();

  if(changed != hasChanged()) setChangedState(changed);
}

bool IndicatorNotificationBackend::ConfigWidget::hasDefaults() const {
  return true;
}

void IndicatorNotificationBackend::ConfigWidget::defaults() {
  ui.enabled->setChecked(false);
  widgetChanged();
}

void IndicatorNotificationBackend::ConfigWidget::load() {
  NotificationSettings s;
  enabled = s.value("Indicator/Enabled", false).toBool();

  ui.enabled->setChecked(enabled);
  setChangedState(false);
}

void IndicatorNotificationBackend::ConfigWidget::save() {
  NotificationSettings s;
  s.setValue("Indicator/Enabled", ui.enabled->isChecked());
  load();
}