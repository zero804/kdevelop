/* This file is part of the KDE project
   Copyright (C) 2004 Alexander Dymo <adymo@kdevelop.org>

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
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "kdevdesignerintegration.h"
#include "kdevdesignerintegrationiface.h"

class KDevDesignerIntegration::KDevDesignerIntegrationPrivate {
public:
  	KDevDesignerIntegrationIface *m_iface;
};

KDevDesignerIntegration::KDevDesignerIntegration(QObject *parent, const char *name)
 : QObject(parent, name)
{
  dptr = new KDevDesignerIntegrationPrivate();
  
  dptr->m_iface = new KDevDesignerIntegrationIface(this);
}

KDevDesignerIntegration::~KDevDesignerIntegration()
{
  delete dptr;
}

#include "kdevdesignerintegration.moc"
