/* This file is part of the KDE project
   Copyright (C) 2002 Harald Fernengel <harry@kdevelop.org>
   Copyright (C) 2002 F@lk Brettschneider <falkbr@kdevelop.org>
   Copyright (C) 2003 Roberto Raggi <roberto@kdevelop.org>

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/**
 * The interface to a diff frontend
 */

#ifndef _KDEVDIFFFRONTEND_H_
#define _KDEVDIFFFRONTEND_H_

#include <kurl.h>
#include "kdevplugin.h"


class KDevDiffFrontend : public KDevPlugin
{

public:

    KDevDiffFrontend( const KDevPluginInfo *info, QObject *parent=0, const char *name=0 )
        :KDevPlugin(info, parent, name ? name : "KDevDiffFrontend") {}

    /**
     * displays the patch.
     */
    virtual void showDiff( const QString& diff ) = 0;

    /**
     * displays a patch file
     */
    virtual void openURL( const KURL &url ) = 0;

    /**
     * displays the difference between the two files
     */
    virtual void showDiff( const KURL &url1, const KURL &url2 ) = 0;

};

#endif
