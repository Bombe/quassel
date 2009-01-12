/***************************************************************************
 *   Copyright (C) 2005-09 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
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

#ifndef BUFFERSETTINGS_H
#define BUFFERSETTINGS_H

#include "clientsettings.h"
#include "message.h"
#include "types.h"

class BufferSettings : public ClientSettings {
public:
  BufferSettings(const QString &idString = "__default__");
  BufferSettings(BufferId bufferId);

  inline void setValue(const QString &key, const QVariant &data) { setLocalValue(key, data); }
  inline QVariant value(const QString &key, const QVariant &def = QVariant()) { return localValue(key, def); }

  inline bool hasFilter() { return localValue("hasMessageTypeFilter", false).toBool(); }
  inline int messageFilter() { return localValue("MessageTypeFilter", 0).toInt(); }
  void setMessageFilter(int filter);
  void filterMessage(Message::Type msgType, bool filter);
  void removeFilter();

  inline bool showUserStateIcons() { return localValue("ShowUserStateIcons", true).toBool(); }
  inline void enableUserStateIcons(bool enabled) { setLocalValue("ShowUserStateIcons", enabled); }
};


#endif
