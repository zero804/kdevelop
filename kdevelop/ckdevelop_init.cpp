/***************************************************************************
      init.cpp - the init specific part of CKDevelop (constructor,init() ...)
                             -------------------                                         

    begin                : 20 Jul 1998                                        
    copyright            : (C) 1998 by Sandy Meier                         
    email                : smeier@rz.uni-potsdam.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include <iostream.h>
#include <kmsgbox.h>
#include <kaccel.h>
#include <qtoolbutton.h>
#include "./kwrite/kwdoc.h"
#include "ckdevelop.h"
#include "cclassview.h"
#include "cdocbrowser.h"
#include "ctoolclass.h"	
#include "kswallow.h"
#include "ctabctl.h"

CKDevelop::CKDevelop(){
  QString filename;
  version = VERSION;
  project=false;// no project
  beep=false; // no beep

  init();
  initConnections();
  initKDlg();    // create the KDialogEditor

  initWhatsThis();


  initProject();
  config->setGroup("Files");
  filename = config->readEntry("browser_file","");
  if(!filename.isEmpty()){
    slotURLSelected(browser_widget,filename,1,"test");
  }
  else{
    slotURLSelected(browser_widget,"file:" + KApplication::kde_htmldir() +
		    "/en/kdevelop/index.html",1,"test");
    
  }

  kdev_caption=kapp->getCaption();
  config->setGroup("General Options");
  bool showOutput=config->readBoolEntry("show_output_view",false);
  if(showOutput)
  	slotViewTOutputView();

  config->setGroup("General Options");
  bool switchKDevelop=config->readBoolEntry("show_kdevelop",true);  // if true, kdevelop, else kdialogedit
  if(switchKDevelop){
    switchToKDevelop();
  }
  else{
    switchToKDlgEdit();
  }
  // initialize properties_view_pos
  if(kdlg_view_menu->isItemChecked(ID_KDLG_VIEW_PROPVIEW)){
    properties_view_pos=kdlg_top_panner->separatorPos();
  }
  else{
    config->setGroup("General Options");
    properties_view_pos=config->readNumEntry("properties_view_pos", 80);
  }


}


void CKDevelop::init(){  
  act_outbuffer_len=0;
  prj = 0;

  QFont font("Fixed",10);
  KApplication *app=KApplication::getKApplication();
  config = app->getConfig();
  config->setGroup("General Options");
  int w = config->readNumEntry("width", 0);
  int h = config->readNumEntry("height", 500);
  if(w==0){
  	w=800;
	  setGeometry(QApplication::desktop()->width()/2-400, QApplication::desktop()->height()/2-h/2, 800, h);
	}
	else
	  resize(w,h);

  // call bar functions to create bars
  // create the main view

  view = new KNewPanner(this,"view",KNewPanner::Horizontal,KNewPanner::Percent,
  			config->readNumEntry("view_panner_pos",80));


  o_tab_view = new CTabCtl(view,"output_tabview","output_widget");
  
  messages_widget = new COutputWidget(kapp,o_tab_view);
  messages_widget->setFocusPolicy(QWidget::NoFocus);
  messages_widget->setReadOnly(TRUE);

  connect(messages_widget,SIGNAL(clicked()),this,SLOT(slotClickedOnMessagesWidget()));

  stdin_stdout_widget = new COutputWidget(kapp,o_tab_view);
  stdin_stdout_widget->setFocusPolicy(QWidget::ClickFocus);
  
  connect(stdin_stdout_widget,SIGNAL(keyPressed(int)),this,SLOT(slotKeyPressedOnStdinStdoutWidget(int)));
  stderr_widget = new COutputWidget(kapp,o_tab_view);
  stderr_widget->setReadOnly(TRUE);
  stderr_widget->setFocusPolicy(QWidget::NoFocus);

  o_tab_view->addTab(messages_widget,i18n("messages"));
  o_tab_view->addTab(stdin_stdout_widget,i18n("stdin/stdout"));
  o_tab_view->addTab(stderr_widget,i18n("stderr"));
  
  //  s_tab_current = 0;
  top_panner = new KNewPanner(view,"top_panner",KNewPanner::Vertical,KNewPanner::Absolute,
  			      config->readNumEntry("top_panner_pos", 213));
  t_tab_view = new CTabCtl(top_panner);
  t_tab_view->setFocusPolicy(QWidget::ClickFocus);
  connect(t_tab_view,SIGNAL(tabSelected(int)),this,SLOT(slotTTabSelected(int)));


  class_tree = new CClassView(t_tab_view,"cv");
  class_tree->setFocusPolicy(QWidget::NoFocus);

  log_file_tree = new CLogFileView(t_tab_view,"lfv");
  log_file_tree->setFocusPolicy(QWidget::NoFocus);

  real_file_tree = new CRealFileView(t_tab_view,"RFV");
  real_file_tree->setFocusPolicy(QWidget::NoFocus);

  doc_tree = new CDocTree(t_tab_view,"DOC",config);
  doc_tree->setFocusPolicy(QWidget::NoFocus);

  t_tab_view->addTab(class_tree,"CV");
  t_tab_view->addTab(log_file_tree,"LFV");
  t_tab_view->addTab(real_file_tree,"RFV");
  t_tab_view->addTab(doc_tree,"DOC");

  connect(class_tree, SIGNAL(selectionChanged()), SLOT(slotClassTreeSelected()));
  connect(class_tree, SIGNAL(selectedFileNew()), SLOT(slotProjectAddNewFile()));
  connect(class_tree, SIGNAL(selectedClassNew()), SLOT(slotProjectNewClass()));
  connect(class_tree, SIGNAL(selectedProjectOptions()), SLOT(slotProjectOptions()));
  connect(class_tree, SIGNAL(selectedViewDeclaration()), SLOT(slotCVViewDeclaration()));
  connect(class_tree, SIGNAL(selectedViewDefinition()), SLOT(slotCVViewDefinition()));
  connect(class_tree, SIGNAL(signalAddMethod( CParsedMethod *)), SLOT(slotCVAddMethod( CParsedMethod * )));
  connect(class_tree, SIGNAL(signalAddAttribute( CParsedAttribute *)), SLOT(slotCVAddAttribute( CParsedAttribute * )));

  connect(log_file_tree, SIGNAL(logFileTreeSelected(QString)), SLOT(slotLogFileTreeSelected(QString)));
  connect(log_file_tree, SIGNAL(selectedNewClass()), SLOT(slotProjectNewClass()));
  connect(log_file_tree, SIGNAL(selectedNewFile()), SLOT(slotProjectAddNewFile()));
  connect(log_file_tree, SIGNAL(selectedFileRemove()), SLOT(slotProjectRemoveFile()));
  connect(log_file_tree, SIGNAL(showFileProperties(QString)),SLOT(slotShowFileProperties(QString)));

  connect(real_file_tree, SIGNAL(fileSelected(QString)), SLOT(slotRealFileTreeSelected(QString)));
  connect(real_file_tree, SIGNAL(showFileProperties(QString)),SLOT(slotShowFileProperties(QString)));

  connect(doc_tree, SIGNAL(fileSelected(QString)), SLOT(slotDocTreeSelected(QString)));


  // the tabbar + tabwidgets for edit and browser
  s_tab_view = new CTabCtl(top_panner);
  s_tab_view->setFocusPolicy(QWidget::ClickFocus);

  connect(s_tab_view,SIGNAL(tabSelected(int)),this,SLOT(slotSTabSelected(int)));

  header_widget = new CEditWidget(kapp,s_tab_view,"header");
  header_widget->setFocusPolicy(QWidget::StrongFocus);

  header_widget->setFont(font);
  header_widget->setName("Untitled.h");
  config->setGroup("KWrite Options");
  header_widget->readConfig(config);
  header_widget->doc()->readConfig(config);

  connect(header_widget, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));
  connect(header_widget, SIGNAL(newStatus()),this, SLOT(slotNewStatus()));
  connect(header_widget, SIGNAL(newUndo()),this, SLOT(slotNewUndo()));
  connect(header_widget, SIGNAL(bufferMenu(const QPoint&)),this,
	  SLOT(slotBufferMenu(const QPoint&)));

  edit_widget=header_widget;
  cpp_widget = new CEditWidget(kapp,s_tab_view,"cpp");
  cpp_widget->setFocusPolicy(QWidget::StrongFocus);
  cpp_widget->setFont(font);
  cpp_widget->setName("Untitled.cpp");
  config->setGroup("KWrite Options");
  cpp_widget->readConfig(config);
  cpp_widget->doc()->readConfig(config);

//  edit_widget->setFocusPolicy(QWidget::StrongFocus);

  connect(cpp_widget, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));
  connect(cpp_widget, SIGNAL(newStatus()),this, SLOT(slotNewStatus()));
  connect(cpp_widget, SIGNAL(newUndo()),this, SLOT(slotNewUndo()));
  connect(cpp_widget, SIGNAL(bufferMenu(const QPoint&)),this,
	  SLOT(slotBufferMenu(const QPoint&)));

  // init the 2 first kedits
  TEditInfo* edit1 = new TEditInfo;
  TEditInfo* edit2 = new TEditInfo;
  edit1->filename = header_widget->getName();
  edit2->filename = cpp_widget->getName();

  browser_widget = new CDocBrowser(s_tab_view,"browser");
  browser_widget->setFocusPolicy(QWidget::StrongFocus);

  prev_was_search_result= false;
  //init
  browser_widget->setDocBrowserOptions();

  connect(browser_widget,SIGNAL(URLSelected(KHTMLView*,const char*,int,const char*)),
	  this,SLOT(slotURLSelected(KHTMLView*,const char*,int,const char*)));
  connect(browser_widget,SIGNAL(documentDone(KHTMLView*)),
	  this,SLOT(slotDocumentDone(KHTMLView*)));

  swallow_widget = new KSwallowWidget(s_tab_view);
  swallow_widget->setFocusPolicy(QWidget::StrongFocus);
//  swallow_widget->setFocusPolicy(QWidget::NoFocus);

  

  s_tab_view->addTab(header_widget,i18n("Header/Resource Files"));
  s_tab_view->addTab(cpp_widget,i18n("C/C++ Files"));
  s_tab_view->addTab(browser_widget,i18n("Documentation-Browser"));
  s_tab_view->addTab(swallow_widget,i18n("Tools"));



  top_panner->activate(t_tab_view,s_tab_view);// activate the top_panner
  view->activate(top_panner,o_tab_view); 

  output_view_pos=view->separatorPos();
  tree_view_pos=top_panner->separatorPos();

  // set the mainwidget
  setView(view);
  initKeyAccel();
  initMenu();
  initToolbar();
  initStatusBar();

  // init General variables like autosave, autoswitch and make-command
  config->setGroup("General Options");
  bAutosave=config->readBoolEntry("Autosave",true);
  saveTimeout=config->readNumEntry("Autosave Timeout",5*60*1000);
  saveTimer=new QTimer(this);
  connect(saveTimer,SIGNAL(timeout()),SLOT(slotFileSaveAll()));
  if(bAutosave){
    saveTimer->start(saveTimeout);
  }
  else{
    saveTimer->stop();
  }
  bAutoswitch=config->readBoolEntry("Autoswitch",true);
  bDefaultCV=config->readBoolEntry("DefaultClassView",true);
  make_cmd=config->readEntry("Make","make");
  //  make_with_cmd=config->readEntry("MakeWith","");

  // initialize output_view_pos
  if(view_menu->isItemChecked(ID_VIEW_OUTPUTVIEW)){
    output_view_pos=view->separatorPos();
  }
  else{
    config->setGroup("General Options");
    output_view_pos=config->readNumEntry("output_view_pos", 80);
  }
  
  // initialize tree_view_pos
  
  if(view_menu->isItemChecked(ID_VIEW_TREEVIEW)){
    tree_view_pos=top_panner->separatorPos();
  }
  else{
    config->setGroup("General Options");
    tree_view_pos=config->readNumEntry("tree_view_pos", 213);
  }

  // the rest of the init for the kedits
  edit1->id = menu_buffers->insertItem(edit1->filename,-2,0);
  edit1->modified=false;
  edit2->id = menu_buffers->insertItem(edit2->filename,-2,0);
  edit2->modified=false;
  edit_infos.append(edit1);
  edit_infos.append(edit2);
}
void CKDevelop::initKeyAccel(){
  accel = new KAccel( this );
  //file menu

  accel->connectItem( KAccel::New, this, SLOT(slotFileNew()) );
  accel->connectItem( KAccel::Open , this, SLOT(slotFileOpen()) );
  accel->connectItem( KAccel::Close , this, SLOT(slotFileClose()) );
  accel->connectItem( KAccel::Save , this, SLOT(slotFileSave()) );
  accel->connectItem( KAccel::Print , this, SLOT(slotFilePrint()) );
  accel->connectItem( KAccel::Quit, this, SLOT(slotFileQuit()) );


  //edit menu

  accel->connectItem( KAccel::Undo , this, SLOT(slotEditUndo()) );

  accel->insertItem( i18n("Redo"), "Redo",IDK_EDIT_REDO );
  accel->connectItem( "Redo" , this, SLOT(slotEditRedo()) );

  accel->connectItem( KAccel::Cut , this, SLOT(slotEditCut()) );
  accel->connectItem( KAccel::Copy , this, SLOT(slotEditCopy()) );
  accel->connectItem( KAccel::Paste , this, SLOT(slotEditPaste()) );


  accel->insertItem( i18n("Search"), "Search",IDK_EDIT_SEARCH );
  accel->connectItem( "Search", this, SLOT(slotEditSearch() ) );

  accel->insertItem( i18n("Repeat Search"), "RepeatSearch",IDK_EDIT_REPEAT_SEARCH );
  accel->connectItem( "RepeatSearch", this, SLOT(slotEditRepeatSearch() ) );

  accel->insertItem( i18n("Replace"), "Replace",IDK_EDIT_REPLACE );
  accel->connectItem( "Replace", this, SLOT(slotEditReplace() ) );

  accel->insertItem( i18n("Indent"), "Indent",IDK_EDIT_INDENT );
  accel->connectItem( "Indent", this, SLOT(slotEditIndent() ) );

  accel->insertItem( i18n("Unindent"), "Unindent",IDK_EDIT_UNINDENT );
  accel->connectItem( "Unindent", this, SLOT(slotEditUnindent() ) );


  //view menu
  accel->insertItem( i18n("Goto Line"), "GotoLine",IDK_VIEW_GOTO_LINE);
  accel->connectItem( "GotoLine", this, SLOT( slotViewGotoLine()) );

  accel->insertItem( i18n("Toogle Tree-View"), "Tree-View",IDK_VIEW_TREEVIEW);
  accel->connectItem( "Tree-View", this, SLOT(slotViewTTreeView()) );
  
  accel->insertItem( i18n("Toogle Output-View"), "Output-View",IDK_VIEW_OUTPUTVIEW);
  accel->connectItem( "Output-View", this, SLOT(slotViewTOutputView()) );
  
  accel->insertItem( i18n("Toogle Statusbar"), "Statusbar",IDK_VIEW_STATUSBAR);
  accel->connectItem( "Statusbar", this, SLOT(slotViewTStatusbar()) );
   
  //build menu 
  accel->insertItem( i18n("Compile File"), "CompileFile", IDK_BUILD_COMPILE_FILE );
  accel->connectItem( "CompileFile", this, SLOT( slotBuildCompileFile()) );

  accel->insertItem( i18n("Make"), "Make", IDK_BUILD_MAKE );
  accel->connectItem( "Make", this, SLOT(slotBuildMake() ) );

//   accel->insertItem( i18n("Make with"), "MakeWith", IDK_BUILD_MAKE_WITH );
//   accel->connectItem( "MakeWith", this, SLOT(slotBuildMakeWith() ) );

  accel->insertItem( i18n("Execute"), "Run", IDK_BUILD_RUN);
  accel->connectItem( "Run", this, SLOT(slotBuildRun() ) );
  accel->insertItem( i18n("Execute with arguments"), "Run_with_args", IDK_BUILD_RUN_WITH_ARGS);
  accel->connectItem( "Run_with_args", this, SLOT(slotBuildRunWithArgs() ) );


  // Tools-menu
  accel->insertItem(i18n("KDevelop/KDialogEdit"),"KDevKDlg",IDK_TOOLS_KDEVKDLG);
  accel->connectItem("KDevKDlg",this,SLOT(switchToKDlgEdit()) );

  // Bookmarks-menu
  accel->insertItem( i18n("Add Bookmark"), "Add_Bookmarks", IDK_BOOKMARKS_ADD);
  accel->connectItem( "Add_Bookmarks", this, SLOT(slotBookmarksAdd() ) );

  accel->insertItem( i18n("Clear Bookmarks"), "Clear_Bookmarks", IDK_BOOKMARKS_CLEAR);
  accel->connectItem( "Clear_Bookmarks", this, SLOT(slotBookmarksClear() ) );

  //Help menu
  accel->insertItem( i18n("Search Marked Text"), "SearchMarkedText",IDK_HELP_SEARCH_TEXT);
  accel->connectItem( "SearchMarkedText", this, SLOT(slotHelpSearchText() ) );

  accel->connectItem( KAccel::Help , this, SLOT(slotHelpContents()) );

  // Tab-Switch
  accel->insertItem( i18n("Show C Sources Window"), "ShowC",IDK_SHOW_C);
  accel->connectItem( "ShowC", this, SLOT(slotShowC() ) );

  accel->insertItem( i18n("Show Header Window"), "ShowHeader",IDK_SHOW_HEADER);
  accel->connectItem( "ShowHeader", this, SLOT(slotShowHeader() ) );

  accel->insertItem( i18n("Show Help Window"), "ShowHelp",IDK_SHOW_HELP);
  accel->connectItem( "ShowHelp", this, SLOT(slotShowHelp() ) );

  accel->insertItem( i18n("Show Tools Window"), "ShowTools",IDK_SHOW_TOOLS);
  accel->connectItem( "ShowTools", this, SLOT(slotShowTools() ) );

  accel->insertItem( i18n("Toggle Last"), "ToggleLast",IDK_TOGGLE_LAST);
  accel->connectItem( "ToggleLast", this, SLOT(slotToggleLast() ) );

  accel->readSettings();
}
void CKDevelop::initMenu(){
  //  KDEVELOP MENUBAR //
  // NOTE: ALL disableCommand(ID_XX) are placed in initKDlgMenuBar() at the end !!!
  // NEW DISABLES HAVE TO BE ADDED THERE AFTER BOTH MENUBARS ARE CREATED COMPLETELY,
  // OTHERWISE KDEVELOP CRASHES !!!

  kdev_menubar=new KMenuBar(this,"KDevelop_menubar");
///////////////////////////////////////////////////////////////////
// File-menu entries

  QPixmap pix;
  file_menu = new QPopupMenu;

  file_menu->insertItem(Icon("filenew.xpm"),i18n("&New"),this,SLOT(slotFileNew()),0,ID_FILE_NEW);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/open.xpm");
  file_menu->insertItem(pix,i18n("&Open..."), this, SLOT(slotFileOpen()),0 ,ID_FILE_OPEN);

  file_menu->insertItem(i18n("&Close"), this, SLOT(slotFileClose()),0,ID_FILE_CLOSE);

  file_menu->insertSeparator();
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/save.xpm");
  file_menu->insertItem(pix,i18n("&Save"), this, SLOT(slotFileSave()),0 ,ID_FILE_SAVE);

  file_menu->insertItem(i18n("Save &As..."), this, SLOT(slotFileSaveAs()),0 ,ID_FILE_SAVE_AS);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/saveall.xpm");
  file_menu->insertItem(pix,i18n("Save All"), this, SLOT(slotFileSaveAll()),0,ID_FILE_SAVE_ALL);
  file_menu->insertSeparator();

  file_menu->insertItem(Icon("fileprint.xpm"),i18n("&Print..."), this, SLOT(slotFilePrint()),0 ,ID_FILE_PRINT);

  file_menu->insertSeparator();
  file_menu->insertItem(i18n("&Quit"),this, SLOT(slotFileQuit()),0 ,ID_FILE_QUIT);

  kdev_menubar->insertItem(i18n("&File"), file_menu);


///////////////////////////////////////////////////////////////////
// Edit-menu entries

  edit_menu = new QPopupMenu;
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/undo.xpm");
  edit_menu->insertItem(pix, i18n("U&ndo"), this, SLOT(slotEditUndo()),0 ,ID_EDIT_UNDO);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/redo.xpm");
  edit_menu->insertItem(pix, i18n("R&edo"), this, SLOT(slotEditRedo()),0 ,ID_EDIT_REDO);
  edit_menu->insertSeparator();

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/cut.xpm");
  edit_menu->insertItem(pix,i18n("C&ut"), this, SLOT(slotEditCut()),0 ,ID_EDIT_CUT);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/copy.xpm");
  edit_menu->insertItem(pix,i18n("&Copy"), this, SLOT(slotEditCopy()),0 ,ID_EDIT_COPY);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/paste.xpm");
  edit_menu->insertItem(pix,i18n("&Paste"), this, SLOT(slotEditPaste()),0 , ID_EDIT_PASTE);
  edit_menu->insertSeparator();
	edit_menu->insertItem(i18n("In&dent"), this,SLOT(slotEditIndent()),0,ID_EDIT_INDENT);
	edit_menu->insertItem(i18n("&Unindent"), this, SLOT(slotEditUnindent()),0,ID_EDIT_UNINDENT);

  edit_menu->insertSeparator();
  edit_menu->insertItem(i18n("&Insert File..."),this, SLOT(slotEditInsertFile()),0,ID_EDIT_INSERT_FILE);

	edit_menu->insertSeparator();
  edit_menu->insertItem(i18n("&Search..."), this, SLOT(slotEditSearch()),0,ID_EDIT_SEARCH);
  edit_menu->insertItem(i18n("&Repeat Search"), this, SLOT(slotEditRepeatSearch()),0,ID_EDIT_REPEAT_SEARCH);
  
  edit_menu->insertItem(i18n("&Replace..."), this, SLOT(slotEditReplace()),0,ID_EDIT_REPLACE);
  edit_menu->insertSeparator();
  edit_menu->insertItem(i18n("Select &All"), this, SLOT(slotEditSelectAll()),0,ID_EDIT_SELECT_ALL);
  edit_menu->insertItem(i18n("Deselect All"), this, SLOT(slotEditDeselectAll()),0,ID_EDIT_DESELECT_ALL);
  edit_menu->insertItem(i18n("Invert Selection"), this, SLOT(slotEditInvertSelection()),0,ID_EDIT_INVERT_SELECTION);
  
  kdev_menubar->insertItem(i18n("&Edit"), edit_menu);



///////////////////////////////////////////////////////////////////
// View-menu entries

  config->setGroup("General Options");
  bViewStatusbar = config->readBoolEntry("show_statusbar",true);

  view_menu = new QPopupMenu;
  view_menu->insertItem(i18n("&Line..."), this,
			SLOT(slotViewGotoLine()),0,ID_VIEW_GOTO_LINE);

  view_menu->insertSeparator();
  view_menu->insertItem(i18n("&Tree-View"),this,
			SLOT(slotViewTTreeView()),0,ID_VIEW_TREEVIEW);
  view_menu->setItemChecked(ID_VIEW_TREEVIEW,config->readBoolEntry("show_tree_view",true));

  view_menu->insertItem(i18n("&Output-View"),this,
			SLOT(slotViewTOutputView()),0,ID_VIEW_OUTPUTVIEW);
  view_menu->setItemChecked(ID_VIEW_OUTPUTVIEW,config->readBoolEntry("show_output_view",true));

  view_menu->insertSeparator();
  view_menu->insertItem(i18n("&Toolbar"),this,
			   SLOT(slotViewTStdToolbar()),0,ID_VIEW_TOOLBAR);
  view_menu->setItemChecked(ID_VIEW_TOOLBAR,config->readBoolEntry("show_std_toolbar", true));

  view_menu->insertItem(i18n("&Browser-Toolbar"),this,
			   SLOT(slotViewTBrowserToolbar()),0,ID_VIEW_BROWSER_TOOLBAR);
  view_menu->setItemChecked(ID_VIEW_BROWSER_TOOLBAR,config->readBoolEntry("show_browser_toolbar",true));

  view_menu->insertItem(i18n("Status&bar"),this,
			   SLOT(slotViewTStatusbar()),0,ID_VIEW_STATUSBAR);

  view_menu->setItemChecked(ID_VIEW_STATUSBAR,bViewStatusbar);

  view_menu->insertSeparator();
  view_menu->insertItem(Icon("reload.xpm"),i18n("&Refresh"),this,
			   SLOT(slotViewRefresh()),0,ID_VIEW_REFRESH);

  kdev_menubar->insertItem(i18n("&View"), view_menu);
  

  ///////////////////////////////////////////////////////////////////
  // Project-menu entries
  
  // submenu for adding projectfiles
  QPopupMenu* p2 = new QPopupMenu;
  p2->insertItem(i18n("&New File..."), this, SLOT(slotProjectAddNewFile()),0,ID_PROJECT_ADD_FILE_NEW);
  p2->insertItem(i18n("&Existing File(s)..."), this,
		 SLOT(slotProjectAddExistingFiles()),0,ID_PROJECT_ADD_FILE_EXIST);
  
  // project-menu
  project_menu = new QPopupMenu;
  project_menu->insertItem(i18n("Application Wizard..."), this, SLOT(slotProjectNewAppl()),0,ID_PROJECT_KAPPWIZARD);
  project_menu->insertItem(i18n("New"), this, SLOT(slotProjectNew()),0, ID_PROJECT_NEW);
  project_menu->insertItem(i18n("&Open..."), this, SLOT(slotProjectOpen()),0,ID_PROJECT_OPEN);
  project_menu->insertItem(i18n("C&lose"),this, SLOT(slotProjectClose()),0,ID_PROJECT_CLOSE);

  project_menu->insertSeparator();
  project_menu->insertItem(i18n("&New Class..."), this,
			   SLOT(slotProjectNewClass()),0,ID_PROJECT_NEW_CLASS);
  project_menu->insertItem(i18n("&Add File(s) to Project"),p2,ID_PROJECT_ADD_FILE);
  //  project_menu->insertItem(i18n("&Remove File from Project"), this,
  //			   SLOT(slotProjectRemoveFile()),0,ID_PROJECT_REMOVE_FILE);
  

  project_menu->insertSeparator();		
  project_menu->insertItem(i18n("&File Properties..."), this, SLOT(slotProjectFileProperties())
			   ,0,ID_PROJECT_FILE_PROPERTIES);
  project_menu->insertItem(i18n("&Options..."), this, SLOT(slotProjectOptions()),0,ID_PROJECT_OPTIONS);
  //  project_menu->insertSeparator();		

  
  workspaces_submenu = new QPopupMenu;
  //workspaces_submenu->insertItem(i18n("Workspace 1"),ID_PROJECT_WORKSPACES_1);
  //  workspaces_submenu->insertItem(i18n("Workspace 2"),ID_PROJECT_WORKSPACES_2);
  //  workspaces_submenu->insertItem(i18n("Workspace 3"),ID_PROJECT_WORKSPACES_3);
  //  project_menu->insertItem(i18n("Workspaces"),workspaces_submenu,ID_PROJECT_WORKSPACES);
  //  connect(workspaces_submenu, SIGNAL(activated(int)), SLOT(slotProjectWorkspaces(int)));

  kdev_menubar->insertItem(i18n("&Project"), project_menu);


///////////////////////////////////////////////////////////////////
// Build-menu entries

  build_menu = new QPopupMenu;
  build_menu->insertItem(Icon("compfile.xpm"),i18n("Compile File"),
			 this,SLOT(slotBuildCompileFile()),0,ID_BUILD_COMPILE_FILE);

  build_menu->insertItem(Icon("make.xpm"),i18n("&Make"),this,
			 SLOT(slotBuildMake()),0,ID_BUILD_MAKE);

//   build_menu->insertItem(Icon("make.xpm"),i18n("Make &with"),this,
// 			 SLOT(slotBuildMakeWith()),0,ID_BUILD_MAKE_WITH);
//   accel->changeMenuAccel(build_menu,ID_BUILD_MAKE_WITH ,"MakeWith" );

  build_menu->insertItem(Icon("rebuild.xpm"),i18n("&Rebuild all"), this,
			 SLOT(slotBuildRebuildAll()),0,ID_BUILD_REBUILD_ALL);

  build_menu->insertItem(i18n("&Clean/Rebuild all"), this, 
			 SLOT(slotBuildCleanRebuildAll()),0,ID_BUILD_CLEAN_REBUILD_ALL);
  build_menu->insertSeparator();
  build_menu->insertItem(Icon("stop.xpm"),i18n("&Stop Build"), this, SLOT(slotBuildStop()),0,ID_BUILD_STOP);
  build_menu->insertSeparator();

  build_menu->insertItem(Icon("run.xpm"),i18n("&Execute  "),this,SLOT(slotBuildRun()),0,ID_BUILD_RUN);
	build_menu->insertItem(Icon("run.xpm"),i18n("Execute &with Arguments"),this,SLOT(slotBuildRunWithArgs()),0,ID_BUILD_RUN_WITH_ARGS);
  build_menu->insertItem(Icon("debugger.xpm"),i18n("&Debug..."),this,SLOT(slotBuildDebug()),0,ID_BUILD_DEBUG);
  build_menu->insertSeparator();
  build_menu->insertItem(i18n("&DistClean"),this,SLOT(slotBuildDistClean()),0,ID_BUILD_DISTCLEAN);
  build_menu->insertItem(i18n("&Autoconf"),this,SLOT(slotBuildAutoconf()),0,ID_BUILD_AUTOCONF);
  build_menu->insertItem(i18n("C&onfigure"), this, SLOT(slotBuildConfigure()),0,ID_BUILD_CONFIGURE);
  build_menu->insertSeparator();
	build_menu->insertItem(i18n("Execution &arguments"),this,SLOT(slotBuildSetExecuteArgs()),0,ID_BUILD_SET_ARGS);
	build_menu->insertItem(i18n("Make &messages"), this, SLOT(slotBuildMessages()),0, ID_BUILD_MESSAGES);
  build_menu->insertItem(i18n("Make &API-Doc"), this,
			 SLOT(slotBuildAPI()),0,ID_BUILD_MAKE_PROJECT_API);
  build_menu->insertItem(i18n("Make &User-Manual"), this, 
			 SLOT(slotBuildManual()),0,ID_BUILD_MAKE_USER_MANUAL);
  
  kdev_menubar->insertItem(i18n("&Build"), build_menu);



///////////////////////////////////////////////////////////////////
// Tools-menu entries

  tools_menu = new QPopupMenu;
  tools_menu->insertItem(i18n("&KDialogEdit"),this,SLOT(switchToKDlgEdit()),0,ID_TOOLS_KDLGEDIT);
  tools_menu->insertItem(i18n("K&Dbg"),this, SLOT(slotToolsKDbg()),0,ID_TOOLS_KDBG);
  tools_menu->insertItem(i18n("K&Iconedit"),this, SLOT(slotToolsKIconEdit()),0,ID_TOOLS_KICONEDIT);
  tools_menu->insertItem(i18n("K&Translator"),this, SLOT(slotToolsKTranslator()),0,ID_TOOLS_KTRANSLATOR);
  kdev_menubar->insertItem(i18n("&Tools"), tools_menu);

///////////////////////////////////////////////////////////////////
// Options-menu entries
  // submenu for setting printprograms
  QPopupMenu* p3 = new QPopupMenu;
  p3->insertItem(i18n("&A2ps..."), this, SLOT(slotOptionsConfigureA2ps()),0,ID_OPTIONS_PRINT_A2PS);
  p3->insertItem(i18n("&Enscript..."), this,
		  SLOT(slotOptionsConfigureEnscript()),0,ID_OPTIONS_PRINT_ENSCRIPT);

  options_menu = new QPopupMenu;
  options_menu->insertItem(i18n("&Editor..."),this,
			   SLOT(slotOptionsEditor()),0,ID_OPTIONS_EDITOR);
  options_menu->insertItem(i18n("Editor &Colors..."),this,
			   SLOT(slotOptionsEditorColors()),0,ID_OPTIONS_EDITOR_COLORS);
  options_menu->insertItem(i18n("Editor &Defaults..."),this,
			   SLOT(slotOptionsSyntaxHighlightingDefaults())
			   ,0,ID_OPTIONS_SYNTAX_HIGHLIGHTING_DEFAULTS);
  options_menu->insertItem(i18n("&Syntax Highlighting..."),this,
			   SLOT(slotOptionsSyntaxHighlighting()),0,ID_OPTIONS_SYNTAX_HIGHLIGHTING);
  options_menu->insertSeparator();
  options_menu->insertItem(i18n("Documentation &Browser..."),this,
			   SLOT(slotOptionsDocBrowser()),0,ID_OPTIONS_DOCBROWSER);

  options_menu->insertItem(i18n("Configure &Printer..."),p3,ID_OPTIONS_PRINT);
  options_menu->insertSeparator();
  options_menu->insertItem(i18n("&KDevelop Setup..."),this,
			   SLOT(slotOptionsKDevelop()),0,ID_OPTIONS_KDEVELOP);

  kdev_menubar->insertItem(i18n("&Options"), options_menu);
  
  ///////////////////////////////////////////////////////////////////
  // Window-menu entries
  
  menu_buffers = new QPopupMenu;
  kdev_menubar->insertItem(i18n("&Window"), menu_buffers);
  kdev_menubar->insertSeparator();
  
  ///////////////////////////////////////////////////////////////////
  // Bookmarks-menu entries
/*	bookmarks_menu=new QPopupMenu;
	bookmarks_menu->insertItem(i18n("&Set Bookmark..."),edit_widget,SLOT(setBookmark()),0,ID_BOOKMARKS_SET);
	bookmarks_menu->insertItem(i18n("&Add Bookmark..."),edit_widget,SLOT(addBookmark()),0,ID_BOOKMARKS_ADD);
	bookmarks_menu->insertItem(i18n("&Clear Bookmarks"),edit_widget,SLOT(clearBookmarks()),0,ID_BOOKMARKS_CLEAR);
	edit_widget->installBMPopup(bookmarks_menu);
*/
	bookmarks_menu=new QPopupMenu;
	bookmarks_menu->insertItem(i18n("&Set Bookmark..."),this,SLOT(slotBookmarksSet()),0,ID_BOOKMARKS_SET);
	bookmarks_menu->insertItem(i18n("&Add Bookmark..."),this,SLOT(slotBookmarksAdd()),0,ID_BOOKMARKS_ADD);
	bookmarks_menu->insertItem(i18n("&Clear Bookmarks"),this,SLOT(slotBookmarksClear()),0,ID_BOOKMARKS_CLEAR);
	bookmarks_menu->insertSeparator();
  QPopupMenu* header_bookmarks = new QPopupMenu();
	header_widget->installBMPopup(header_bookmarks);
  QPopupMenu* cpp_bookmarks = new QPopupMenu();
	cpp_widget->installBMPopup(cpp_bookmarks);
	
	bookmarks_menu->insertItem(i18n("Header Window"),header_bookmarks,31000);
	bookmarks_menu->insertItem(i18n("C/C++ Window"),cpp_bookmarks,31010);

	kdev_menubar->insertItem(i18n("Book&marks"),bookmarks_menu);

  ///////////////////////////////////////////////////////////////////
  // Help-menu entries
  help_menu = new QPopupMenu();
  help_menu->insertItem(i18n("Back"),this, SLOT(slotHelpBack()),0,ID_HELP_BACK);
  help_menu->insertItem(i18n("Forward"),this, SLOT(slotHelpForward()),0,ID_HELP_FORWARD);
  help_menu->insertSeparator();
  help_menu->insertItem(Icon("lookup.xpm"),i18n("&Search Marked Text"),this,
				 SLOT(slotHelpSearchText()),0,ID_HELP_SEARCH_TEXT);
  help_menu->insertItem(Icon("contents.xpm"),i18n("Search for Help on..."),this,SLOT(slotHelpSearch()),0,ID_HELP_SEARCH);
  help_menu->insertSeparator();
  help_menu->insertItem(Icon("mini/kdehelp.xpm"),i18n("Contents"),this,SLOT(slotHelpContents()),0 ,ID_HELP_CONTENTS);

  help_menu->insertSeparator();
  help_menu->insertItem(i18n("C/C++-Reference"),this,SLOT(slotHelpReference()),0,ID_HELP_REFERENCE);
  help_menu->insertItem(Icon("mini/mini-book1.xpm"),i18n("&Qt-Library"),this, SLOT(slotHelpQtLib()),0,ID_HELP_QT_LIBRARY);
  help_menu->insertItem(Icon("mini/mini-book1.xpm"),i18n("KDE-&Core-Library"),this,
				 SLOT(slotHelpKDECoreLib()),0,ID_HELP_KDE_CORE_LIBRARY);
  help_menu->insertItem(Icon("mini/mini-book1.xpm"),i18n("KDE-&GUI-Library"),this,
				 SLOT(slotHelpKDEGUILib()),0,ID_HELP_KDE_GUI_LIBRARY);
  help_menu->insertItem(Icon("mini/mini-book1.xpm"),i18n("KDE-&KFile-Library"),this,
				 SLOT(slotHelpKDEKFileLib()),0,ID_HELP_KDE_KFILE_LIBRARY);
  help_menu->insertItem(Icon("mini/mini-book1.xpm"),i18n("KDE-&HTML-Library"),this,
				 SLOT(slotHelpKDEHTMLLib()),0,ID_HELP_KDE_HTML_LIBRARY);
  help_menu->insertSeparator();
  help_menu->insertItem(i18n("Project &API-Doc"),this,
				      SLOT(slotHelpAPI()),0,ID_HELP_PROJECT_API);

  help_menu->insertItem(i18n("Project &User-Manual"),this,
				      SLOT(slotHelpManual()),0,ID_HELP_USER_MANUAL);
  //  help_menu->insertItem(i18n("KDevelop Homepage"),this, SLOT(slotHelpHomepage()),0,ID_HELP_HOMEPAGE);
  help_menu->insertSeparator();
  help_menu->insertItem(i18n("About KDevelop..."),this, SLOT(slotHelpAbout()),0,ID_HELP_ABOUT);
  kdev_menubar->insertItem(i18n("&Help"), help_menu);


///////////////////////////////////////////////////////////////////
// connects for the statusbar help
  connect(file_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(p3,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(edit_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(view_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(project_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(p2,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(build_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(tools_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(options_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  connect(help_menu,SIGNAL(highlighted(int)), SLOT(statusCallback(int)));


}

void CKDevelop::initToolbar(){
  QPixmap pix;
  QString  path;
 
//  toolBar()->insertButton(Icon("filenew.xpm"),ID_FILE_NEW, false,i18n("New"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/openprj.xpm");
  toolBar()->insertButton(pix,ID_PROJECT_OPEN, true,i18n("Open Project"));
  toolBar()->insertSeparator();
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/open.xpm");
  toolBar()->insertButton(pix,ID_FILE_OPEN, true,i18n("Open File"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/save.xpm");
  toolBar()->insertButton(pix,ID_FILE_SAVE,true,i18n("Save File"));
/*  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/save_all.xpm");
  toolBar()->insertButton(pix,ID_FILE_SAVE_ALL,true,i18n("Save All"));
*/
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/print.xpm");
  toolBar()->insertButton(pix,ID_FILE_PRINT,false,i18n("Print"));

  QFrame *separatorLine= new QFrame(toolBar());
  separatorLine->setFrameStyle(QFrame::VLine|QFrame::Sunken);
  toolBar()->insertWidget(0,10,separatorLine);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/undo.xpm");
	toolBar()->insertButton(pix,ID_EDIT_UNDO,false,i18n("Undo"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/redo.xpm");
	toolBar()->insertButton(pix,ID_EDIT_REDO,false,i18n("Redo"));

  toolBar()->insertSeparator();
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/cut.xpm");
  toolBar()->insertButton(pix,ID_EDIT_CUT,true,i18n("Cut"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/copy.xpm");
  toolBar()->insertButton(pix,ID_EDIT_COPY, true,i18n("Copy"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/paste.xpm");
  toolBar()->insertButton(pix,ID_EDIT_PASTE, true,i18n("Paste"));

  QFrame *separatorLine1= new QFrame(toolBar());
  separatorLine1->setFrameStyle(QFrame::VLine|QFrame::Sunken);
  toolBar()->insertWidget(0,20,separatorLine1);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/compfile.xpm");
  toolBar()->insertButton(pix,ID_BUILD_COMPILE_FILE, false,i18n("Compile file"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/make.xpm");
  toolBar()->insertButton(pix,ID_BUILD_MAKE, false,i18n("Make"));
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/rebuild.xpm");
  toolBar()->insertButton(pix,ID_BUILD_REBUILD_ALL, false,i18n("Rebuild"));
  toolBar()->insertSeparator();
	
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/debugger.xpm");
	toolBar()->insertButton(pix,ID_BUILD_DEBUG, false, i18n("Debug program"));
  toolBar()->insertSeparator();
	
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/run.xpm");
  toolBar()->insertButton(pix,ID_BUILD_RUN, false,i18n("Run"));

  toolBar()->insertSeparator();

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/stop.xpm");
  toolBar()->insertButton(pix,ID_BUILD_STOP, false,i18n("Stop"));

  QFrame *separatorLine2= new QFrame(toolBar());
  separatorLine2->setFrameStyle(QFrame::VLine|QFrame::Sunken);
  toolBar()->insertWidget(0,30,separatorLine2);

  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/newwidget.xpm");
  toolBar()->insertButton(pix,ID_TOOLS_KDLGEDIT, true,i18n("Switch to the dialogeditor"));

  QFrame *separatorLine3= new QFrame(toolBar());
  separatorLine3->setFrameStyle(QFrame::VLine|QFrame::Sunken);
  toolBar()->insertWidget(0,30,separatorLine3);

  whats_this = new QWhatsThis;  
  QToolButton *btnwhat = whats_this->whatsThisButton(toolBar());
  QToolTip::add(btnwhat, i18n("What's this...?"));
  toolBar()->insertWidget(ID_HELP_WHATS_THIS, btnwhat->sizeHint().width(), btnwhat);
  btnwhat->setFocusPolicy(QWidget::NoFocus);

  connect(toolBar(), SIGNAL(clicked(int)), SLOT(slotToolbarClicked(int)));
  config->setGroup("General Options");
  if(config->readBoolEntry("show_std_toolbar", true)){
    enableToolBar(KToolBar::Show,0);
  }
  else{
    enableToolBar(KToolBar::Hide,0);
  }

  // the second toolbar
  toolBar(ID_BROWSER_TOOLBAR)->insertCombo(i18n("Classes"),TOOLBAR_CLASS_CHOICE,true,SIGNAL(activated(int))
			  ,this,SLOT(slotClassChoiceCombo(int)),true,i18n("choice the class"),160 );
  KCombo* class_combo = toolBar(1)->getCombo(TOOLBAR_CLASS_CHOICE);
  class_combo->setFocusPolicy(QWidget::NoFocus);
  toolBar(ID_BROWSER_TOOLBAR)->insertCombo(i18n("Methods"),TOOLBAR_METHOD_CHOICE,true,SIGNAL(activated(int))
			  ,this,SLOT(slotMethodChoiceCombo(int)),true,i18n("choice the methods"),240 );
  KCombo* choice_combo = toolBar(1)->getCombo(TOOLBAR_METHOD_CHOICE);
  choice_combo->setFocusPolicy(QWidget::NoFocus);
  toolBar(ID_BROWSER_TOOLBAR)->insertSeparator();
  toolBar(ID_BROWSER_TOOLBAR)->insertButton(Icon("back.xpm"),ID_HELP_BACK, false,i18n("Back"));
  toolBar(ID_BROWSER_TOOLBAR)->insertButton(Icon("forward.xpm"),ID_HELP_FORWARD, false,i18n("Forward"));
  toolBar(ID_BROWSER_TOOLBAR)->insertSeparator();

  toolBar(ID_BROWSER_TOOLBAR)->insertButton(Icon("lookup.xpm"), ID_HELP_SEARCH_TEXT,
					    true,i18n("Search Text in Documenation"));
  toolBar(ID_BROWSER_TOOLBAR)->insertButton(Icon("contents.xpm"),ID_HELP_SEARCH,
              true,i18n("Search for Help on..."));
	
  connect(toolBar(ID_BROWSER_TOOLBAR), SIGNAL(clicked(int)), SLOT(slotToolbarClicked(int)));

  if(config->readBoolEntry("show_browser_toolbar", true)){
    enableToolBar(KToolBar::Show,ID_BROWSER_TOOLBAR);
  }
  else{
    enableToolBar(KToolBar::Hide,ID_BROWSER_TOOLBAR);
  }
}

void CKDevelop::initStatusBar(){
  kdev_statusbar= new KStatusBar(this,"KDevelop_statusbar");

  statProg = new KProgress(0,100,0,KProgress::Horizontal,kdev_statusbar,"Progressbar");
  statProg->setBarColor("blue");
  statProg->setBarStyle(KProgress::Solid);  // Solid or Blocked
  statProg->setTextEnabled(false);
  statProg->setBackgroundMode(PaletteBackground);
  statProg->setLineWidth( 1 );                // hehe, this *is* tricky...

  kdev_statusbar->insertItem(i18n("xxxxxxxxxxxxxxxxxxxx"), ID_STATUS_EMPTY);
  kdev_statusbar->insertItem(i18n("Line: 00000 Col: 000"), ID_STATUS_LN_CLM);
  kdev_statusbar->changeItem("", ID_STATUS_EMPTY);
  kdev_statusbar->changeItem("", ID_STATUS_LN_CLM);

  kdev_statusbar->insertItem(i18n(" INS "), ID_STATUS_INS_OVR);
//  kdev_statusbar->insertItem(i18n(" CAPS "), ID_STATUS_CAPS);
  kdev_statusbar->insertItem(i18n("yyyyyyyyyyyyyy"),ID_STATUS_EMPTY_2);
  kdev_statusbar->changeItem("", ID_STATUS_EMPTY_2);

//  kdev_statusbar->insertWidget(statProg,150, ID_STATUS_PROGRESS);
  kdev_statusbar->insertItem(i18n("Welcome to KDevelop!"), ID_STATUS_MSG);
  kdev_statusbar->setInsertOrder(KStatusBar::RightToLeft);
  kdev_statusbar->setAlignment(ID_STATUS_INS_OVR, AlignCenter);

  setStatusBar(kdev_statusbar);
  enableStatusBar();
  if(!bViewStatusbar)
  enableStatusBar();
}

// all the init-stuff
void CKDevelop::initConnections(){
  // connections for the proc -processes
  connect(&search_process,SIGNAL(receivedStdout(KProcess*,char*,int)),
  	  this,SLOT(slotSearchReceivedStdout(KProcess*,char*,int)) );

  connect(&search_process,SIGNAL(receivedStderr(KProcess*,char*,int)),
  	  this,SLOT(slotReceivedStderr(KProcess*,char*,int)) );

  connect(&search_process,SIGNAL(processExited(KProcess*)),
	  this,SLOT(slotSearchProcessExited(KProcess*) )) ;


  connect(&process,SIGNAL(receivedStdout(KProcess*,char*,int)),
  	  this,SLOT(slotReceivedStdout(KProcess*,char*,int)) );

  connect(&process,SIGNAL(receivedStderr(KProcess*,char*,int)),
	  this,SLOT(slotReceivedStderr(KProcess*,char*,int)) );

  connect(&process,SIGNAL(processExited(KProcess*)),
	  this,SLOT(slotProcessExited(KProcess*) )) ;

  // shellprocess
  connect(&shell_process,SIGNAL(receivedStdout(KProcess*,char*,int)),
	  this,SLOT(slotReceivedStdout(KProcess*,char*,int)) );

  connect(&shell_process,SIGNAL(receivedStderr(KProcess*,char*,int)),
	  this,SLOT(slotReceivedStderr(KProcess*,char*,int)) );

  connect(&shell_process,SIGNAL(processExited(KProcess*)),
	  this,SLOT(slotProcessExited(KProcess*) )) ;

  //application process

  connect(&appl_process,SIGNAL(processExited(KProcess*)),
	  this,SLOT(slotProcessExited(KProcess*) ));

  connect(&appl_process,SIGNAL(receivedStdout(KProcess*,char*,int)),
	  this,SLOT(slotApplReceivedStdout(KProcess*,char*,int)) );

  connect(&appl_process,SIGNAL(receivedStderr(KProcess*,char*,int)),
	  this,SLOT(slotApplReceivedStderr(KProcess*,char*,int)) );


  // connect the windowsmenu with a method
  connect(menu_buffers,SIGNAL(activated(int)),this,SLOT(slotMenuBuffersSelected(int)));

  //connect the editor lookup function with slotHelpSText
  connect(cpp_widget,SIGNAL(lookUp(QString)),this,SLOT(slotHelpSearchText(QString)));
  connect(header_widget,SIGNAL(lookUp(QString)),this,SLOT(slotHelpSearchText(QString)));

  // connect Docbrowser rb menu
  connect(browser_widget,SIGNAL(signalURLBack()),this,SLOT(slotHelpBack()));
  connect(browser_widget,SIGNAL(signalURLForward()),this,SLOT(slotHelpForward()));

  connect(browser_widget,SIGNAL(onURL(KHTMLView *, const char *)),this,SLOT(slotURLonURL(KHTMLView *, const char *)));
  connect(browser_widget,SIGNAL(signalSearchText()),this,SLOT(slotHelpSearchText()));

}
void CKDevelop::initProject(){

  config->setGroup("General Options");
  bool bLastProject= config->readBoolEntry("LastProject",true);
  QString filename;
	if(bLastProject){
	  config->setGroup("Files");
  	filename = config->readEntry("project_file","");
	}
	else
		filename="";
  QFile file(filename);
  // cerr << "INITPROJECT: " << filename << endl;
  if (file.exists()){
    if(!(readProjectFile(filename))){
      KMsgBox::message(0,filename,"This is a Project-File from KDevelop 0.1\nSorry,but it's incompatible with KDevelop >= 0.2.\nPlease use only new generated projects!");
      refreshTrees();
    }
    config->setGroup("Files");
    filename = config->readEntry("header_file",i18n("Untitled.h"));
    QFile _file(filename);

    if (QFile::exists(filename)){
      switchToFile(filename);

    }

    filename = config->readEntry("cpp_file", i18n("Untitled.cpp"));
    if (QFile::exists(filename)){
      switchToFile(filename);
    }

  }
  else{
    refreshTrees(); // this refresh only the documentation tab,tree
  }

}


void CKDevelop::setKeyAccel(){
if(bKDevelop){
    accel->disconnectItem(accel->stdAction( KAccel::Open ), kdlgedit, SLOT(slotFileOpen()) );
    accel->disconnectItem(accel->stdAction( KAccel::Close ) , kdlgedit, SLOT(slotFileClose()) );
    accel->disconnectItem(accel->stdAction( KAccel::Save ) , kdlgedit, SLOT(slotFileSave()) );
    accel->disconnectItem(accel->stdAction( KAccel::Undo ), kdlgedit, SLOT(slotEditUndo()) );
    accel->disconnectItem( "Redo" , kdlgedit, SLOT(slotEditRedo()) );
    accel->disconnectItem(accel->stdAction( KAccel::Cut ), kdlgedit, SLOT(slotEditCut()) );
    accel->disconnectItem(accel->stdAction( KAccel::Copy ), kdlgedit, SLOT(slotEditCopy()) );
    accel->disconnectItem(accel->stdAction( KAccel::Paste ), kdlgedit, SLOT(slotEditPaste()) );
    accel->disconnectItem("KDevKDlg",this,SLOT(switchToKDevelop()) );

    accel->connectItem( KAccel::Open , this, SLOT(slotFileOpen()) );
    accel->connectItem( KAccel::Close , this, SLOT(slotFileClose()) );
    accel->connectItem( KAccel::Save , this, SLOT(slotFileSave()) );
    accel->connectItem( KAccel::Undo , this, SLOT(slotEditUndo()) );
    accel->connectItem( "Redo" , this, SLOT(slotEditRedo()) );
    accel->connectItem( KAccel::Cut , this, SLOT(slotEditCut()) );
    accel->connectItem( KAccel::Copy , this, SLOT(slotEditCopy()) );
    accel->connectItem( KAccel::Paste , this, SLOT(slotEditPaste()) );
    accel->connectItem("KDevKDlg",this,SLOT(switchToKDlgEdit()) );

    accel->changeMenuAccel(file_menu, ID_FILE_NEW, KAccel::New );
    accel->changeMenuAccel(file_menu, ID_FILE_OPEN, KAccel::Open );
    accel->changeMenuAccel(file_menu, ID_FILE_CLOSE, KAccel::Close );
    accel->changeMenuAccel(file_menu, ID_FILE_SAVE, KAccel::Save );
    accel->changeMenuAccel(file_menu, ID_FILE_PRINT, KAccel::Print );
    accel->changeMenuAccel(file_menu, ID_FILE_QUIT, KAccel::Quit );

    accel->changeMenuAccel(edit_menu, ID_EDIT_UNDO, KAccel::Undo );
    accel->changeMenuAccel(edit_menu, ID_EDIT_REDO,"Redo" );
    accel->changeMenuAccel(edit_menu, ID_EDIT_CUT, KAccel::Cut );
    accel->changeMenuAccel(edit_menu, ID_EDIT_COPY, KAccel::Copy );
    accel->changeMenuAccel(edit_menu, ID_EDIT_PASTE, KAccel::Paste );
    accel->changeMenuAccel(edit_menu, ID_EDIT_SEARCH,"Search" );
    accel->changeMenuAccel(edit_menu, ID_EDIT_REPEAT_SEARCH,"RepeatSearch" );
    accel->changeMenuAccel(edit_menu, ID_EDIT_REPLACE,"Replace" );
    accel->changeMenuAccel(edit_menu, ID_EDIT_INDENT,"Indent" );
    accel->changeMenuAccel(edit_menu, ID_EDIT_UNINDENT,"Unindent" );

    accel->changeMenuAccel(view_menu,ID_VIEW_GOTO_LINE ,"GotoLine" );
    accel->changeMenuAccel(view_menu,ID_VIEW_TREEVIEW ,"Tree-View" );
    accel->changeMenuAccel(view_menu,ID_VIEW_OUTPUTVIEW,"Output-View" );
    accel->changeMenuAccel(view_menu,ID_VIEW_STATUSBAR,"Statusbar");

    accel->changeMenuAccel(build_menu,ID_BUILD_COMPILE_FILE ,"CompileFile" );
    accel->changeMenuAccel(build_menu,ID_BUILD_MAKE ,"Make" );
    accel->changeMenuAccel(build_menu,ID_BUILD_RUN ,"Run" );
		accel->changeMenuAccel(build_menu,ID_BUILD_RUN_WITH_ARGS,"Run_with_args");

    accel->changeMenuAccel(bookmarks_menu,ID_BOOKMARKS_ADD ,"Add_Bookmarks" );
    accel->changeMenuAccel(bookmarks_menu,ID_BOOKMARKS_CLEAR ,"Clear_Bookmarks" );

    accel->changeMenuAccel(help_menu,ID_HELP_SEARCH_TEXT,"SearchMarkedText" );
    accel->changeMenuAccel(help_menu, ID_HELP_CONTENTS, KAccel::Help );
  }
  else{
    accel->disconnectItem(accel->stdAction( KAccel::Open ), this, SLOT(slotFileOpen()) );
    accel->disconnectItem(accel->stdAction( KAccel::Close ) , this, SLOT(slotFileClose()) );
    accel->disconnectItem(accel->stdAction( KAccel::Save ) , this, SLOT(slotFileSave()) );
    accel->disconnectItem(accel->stdAction( KAccel::Undo ), this, SLOT(slotEditUndo()) );
    accel->disconnectItem( "Redo" , this, SLOT(slotEditRedo()) );
    accel->disconnectItem(accel->stdAction( KAccel::Cut ), this, SLOT(slotEditCut()) );
    accel->disconnectItem(accel->stdAction( KAccel::Copy ), this, SLOT(slotEditCopy()) );
    accel->disconnectItem(accel->stdAction( KAccel::Paste ), this, SLOT(slotEditPaste()) );
    accel->disconnectItem("KDevKDlg",this,SLOT(switchToKDlgEdit()) );

    accel->connectItem( KAccel::Open , kdlgedit, SLOT(slotFileOpen()) );
    accel->connectItem( KAccel::Close , kdlgedit, SLOT(slotFileClose()) );
    accel->connectItem( KAccel::Save , kdlgedit, SLOT(slotFileSave()) );
    accel->connectItem( KAccel::Undo , kdlgedit, SLOT(slotEditUndo()) );
    accel->connectItem( "Redo" , kdlgedit, SLOT(slotEditRedo()) );
    accel->connectItem( KAccel::Cut , kdlgedit, SLOT(slotEditCut()) );
    accel->connectItem( KAccel::Copy , kdlgedit, SLOT(slotEditCopy()) );
    accel->connectItem( KAccel::Paste , kdlgedit, SLOT(slotEditPaste()) );
    accel->connectItem("KDevKDlg",this,SLOT(switchToKDevelop()) );

    accel->changeMenuAccel(kdlg_file_menu, ID_FILE_NEW, KAccel::New );
    accel->changeMenuAccel(kdlg_file_menu, ID_KDLG_FILE_OPEN, KAccel::Open );
    accel->changeMenuAccel(kdlg_file_menu, ID_KDLG_FILE_CLOSE, KAccel::Close );
    accel->changeMenuAccel(kdlg_file_menu, ID_KDLG_FILE_SAVE, KAccel::Save );
    accel->changeMenuAccel(kdlg_file_menu, ID_FILE_QUIT, KAccel::Quit );

    accel->changeMenuAccel(kdlg_edit_menu, ID_KDLG_EDIT_UNDO, KAccel::Undo );
    accel->changeMenuAccel(kdlg_edit_menu, ID_KDLG_EDIT_REDO,"Redo" );
    accel->changeMenuAccel(kdlg_edit_menu, ID_KDLG_EDIT_CUT, KAccel::Cut );
    accel->changeMenuAccel(kdlg_edit_menu, ID_KDLG_EDIT_COPY, KAccel::Copy );
    accel->changeMenuAccel(kdlg_edit_menu, ID_KDLG_EDIT_PASTE, KAccel::Paste );

    accel->changeMenuAccel(kdlg_view_menu,ID_VIEW_TREEVIEW ,"Tree-View" );
    accel->changeMenuAccel(kdlg_view_menu,ID_VIEW_OUTPUTVIEW,"Output-View" );
    accel->changeMenuAccel(kdlg_view_menu,ID_VIEW_STATUSBAR,"Statusbar");

    accel->changeMenuAccel(kdlg_build_menu,ID_BUILD_COMPILE_FILE ,"CompileFile" );
    accel->changeMenuAccel(kdlg_build_menu,ID_BUILD_MAKE ,"Make" );
    accel->changeMenuAccel(kdlg_build_menu,ID_BUILD_RUN ,"Run" );
		accel->changeMenuAccel(kdlg_build_menu,ID_BUILD_RUN_WITH_ARGS,"Run_with_args");

    accel->changeMenuAccel(kdlg_help_menu,ID_HELP_SEARCH_TEXT,"SearchMarkedText" );
    accel->changeMenuAccel(kdlg_help_menu, ID_HELP_CONTENTS, KAccel::Help );
  }
}































































































