/***************************************************************************
 *   Copyright (C) 1999 by Jonas Nordin                                    *
 *   jonas.nordin@syncom.se                                                *
 *   Copyright (C) 2000 by Bernd Gehrmann                                  *
 *   bernd@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _CLASSVIEW_H_
#define _CLASSVIEW_H_

#include <kaction.h>
#include "ParsedClass.h"
#include "ctreehandler.h"
#include "kdevcomponent.h"
#include "classactions.h"


class ClassWidget;
class CClassStore;


class ClassView : public KDevComponent
{
    Q_OBJECT

public:
    ClassView( QObject *parent=0, const char *name=0 );
    ~ClassView();

protected:
    virtual void setupGUI();
    virtual void projectOpened(CProject *prj);
    virtual void projectClosed();
    virtual void languageSupportOpened(KDevLanguageSupport *ls);
    virtual void languageSupportClosed();
    virtual void classStoreOpened(CClassStore *store);
    virtual void classStoreClosed();

private slots:
    void refresh();
    void selectedClass();
    void selectedMethod();
    void switchedDeclImpl();
    void selectedGotoDeclaration();
    void selectedGotoImplementation();
    void selectedGotoClassDeclaration();
    void selectedNewClass();
    void selectedAddMethod();
    void selectedAddAttribute();
    
private:
    ClassWidget *m_widget;

    void setupPopup();
    CParsedClass *getClass(const QString &className);
    void gotoDeclaration(const QString &classname,
                         const QString &declName,
                         THType type);
    void gotoImplementation(const QString &classname,
                            const QString &declName,
                            THType type);

    ClassListAction *classes_action;
    MethodListAction *methods_action;
    DelayedPopupAction *popup_action;
    CClassStore *m_store;
    KDevLanguageSupport *m_langsupport;
    bool m_cv_decl_or_impl;
};

#endif
