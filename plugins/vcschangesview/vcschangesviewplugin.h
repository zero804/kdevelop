/*  This file is part of KDevelop
    Copyright 2010 Aleix Pol <aleixpol@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
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

#ifndef KDEVPLATFORM_PLUGIN_VCSCHANGESVIEWPLUGIN_H
#define KDEVPLATFORM_PLUGIN_VCSCHANGESVIEWPLUGIN_H

#include <interfaces/iplugin.h>
#include <QVariant>

class QModelIndex;

namespace KDevelop
{
    class IProject;
    class IDocument;
    class ProjectChangesModel;
}

class VcsProjectIntegrationPlugin : public KDevelop::IPlugin
{
    Q_OBJECT
    public:
        VcsProjectIntegrationPlugin(QObject *parent, const QVariantList & args);
        KDevelop::ProjectChangesModel* model();
        
    private Q_SLOTS:
        void activated(const QModelIndex& idx);
        
    private:
        KDevelop::ProjectChangesModel* m_model;

        friend class VCSProjectToolViewFactory; // to access activated() slot
};

#endif
