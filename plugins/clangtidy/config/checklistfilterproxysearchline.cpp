/*
 * This file is part of KDevelop
 *
 * Copyright 2018 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "checklistfilterproxysearchline.h"

// KF
#include <KLocalizedString>
// Qt
#include <QTimer>
#include <QSortFilterProxyModel>

namespace ClangTidy
{

CheckListFilterProxySearchLine::CheckListFilterProxySearchLine(QWidget *parent)
    : QLineEdit(parent)
    , m_delayTimer(new QTimer(this))
{
    setClearButtonEnabled(true);
    setPlaceholderText(i18nc("@info:placeholder", "Search..."));

    m_delayTimer->setSingleShot(true);
    m_delayTimer->setInterval(300);
    connect(m_delayTimer, &QTimer::timeout,
            this, &CheckListFilterProxySearchLine::updateFilter);
    connect(this, &CheckListFilterProxySearchLine::textChanged,
            m_delayTimer, QOverload<>::of(&QTimer::start));
}


void CheckListFilterProxySearchLine::setFilterProxyModel(QSortFilterProxyModel* filterProxyModel)
{
    m_filterProxyModel = filterProxyModel;
}

void CheckListFilterProxySearchLine::updateFilter()
{
    if (!m_filterProxyModel) {
        return;
    }

    m_filterProxyModel->setFilterFixedString(text());
}

}
