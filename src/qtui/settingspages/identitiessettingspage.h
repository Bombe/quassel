/***************************************************************************
 *   Copyright (C) 2005-09 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#ifndef IDENTITIESSETTINGSPAGE_H
#define IDENTITIESSETTINGSPAGE_H

#include "clientidentity.h"
#include "settingspage.h"

#include "ui_identitiessettingspage.h"
#include "ui_createidentitydlg.h"
#include "ui_saveidentitiesdlg.h"
#include "ui_nickeditdlg.h"

#ifdef HAVE_SSL
#include <QSslCertificate>
#include <QSslKey>
#endif

class QAbstractItemModel;

class IdentitiesSettingsPage : public SettingsPage {
  Q_OBJECT

public:
  IdentitiesSettingsPage(QWidget *parent = 0);

  bool aboutToSave();

public slots:
  void save();
  void load();

#ifdef HAVE_SSL
protected:
  virtual bool eventFilter(QObject *watched, QEvent *event);
#endif

private slots:
  void coreConnectionStateChanged(bool);
  void clientIdentityCreated(IdentityId);
  void clientIdentityUpdated();
  void clientIdentityRemoved(IdentityId);

  void on_identityList_currentIndexChanged(int index);

  void on_addIdentity_clicked();
  void on_deleteIdentity_clicked();
  void on_renameIdentity_clicked();

  void on_addNick_clicked();
  void on_deleteNick_clicked();
  void on_renameNick_clicked();
  void on_nickUp_clicked();
  void on_nickDown_clicked();

#ifdef HAVE_SSL
  void on_continueUnsecured_clicked();
  void on_clearOrLoadKeyButton_clicked();
  void on_clearOrLoadCertButton_clicked();
#endif
  void widgetHasChanged();
  void setWidgetStates();

#ifdef HAVE_SSL
  void sslDragEnterEvent(QDragEnterEvent *event);
  void sslDropEvent(QDropEvent *event, bool isCert);
#endif

private:
  Ui::IdentitiesSettingsPage ui;

  QHash<IdentityId, CertIdentity *> identities;
  IdentityId currentId;

  QList<IdentityId> changedIdentities;  // for setting the widget changed state
  QList<IdentityId> deletedIdentities;

  bool _editSsl;

  void insertIdentity(CertIdentity *identity);
  void removeIdentity(Identity *identity);
  void renameIdentity(IdentityId id, const QString &newName);
  void displayIdentity(CertIdentity *, bool dontsave = false);
  void saveToIdentity(CertIdentity *);

#ifdef HAVE_SSL
  QSslKey keyByFilename(const QString &filename);
  void showKeyState(const QSslKey &key);
  QSslCertificate certByFilename(const QString &filename);
  void showCertState(const QSslCertificate &cert);
#endif

  bool testHasChanged();
};

class CreateIdentityDlg : public QDialog {
  Q_OBJECT

public:
  CreateIdentityDlg(QAbstractItemModel *model, QWidget *parent = 0);

  QString identityName() const;
  IdentityId duplicateId() const;

private slots:
  void on_identityName_textChanged(const QString &text);

private:
  Ui::CreateIdentityDlg ui;
};

class SaveIdentitiesDlg : public QDialog {
  Q_OBJECT

public:
  SaveIdentitiesDlg(const QList<CertIdentity *> &toCreate, const QList<CertIdentity *> &toUpdate, const QList<IdentityId> &toRemove, QWidget *parent = 0);

private slots:
  void clientEvent();

private:
  Ui::SaveIdentitiesDlg ui;

  int numevents, rcvevents;
};

class NickEditDlg : public QDialog {
  Q_OBJECT

public:
  NickEditDlg(const QString &oldnick, const QStringList &existing = QStringList(), QWidget *parent = 0);

  QString nick() const;

private slots:
  void on_nickEdit_textChanged(const QString &);

private:
  Ui::NickEditDlg ui;

  QString oldNick;
  QStringList existing;

};

#endif
