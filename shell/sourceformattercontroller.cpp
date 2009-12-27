/* This file is part of KDevelop
Copyright 2009 Andreas Pakulat <apaku@gmx.de>
Copyright (C) 2008 Cédric Pasteur <cedric.pasteur@free.fr>

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

#include "sourceformattercontroller.h"

#include <QVariant>
#include <QStringList>
#include <QRegExp>
#include <KPluginInfo>
#include <KDebug>

#include <interfaces/icore.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/interfaces/ilanguagesupport.h>
#include <interfaces/isourceformatter.h>
#include "core.h"
#include <ktexteditor/view.h>
#include <project/projectmodel.h>
#include <kio/netaccess.h>
#include <kmessagebox.h>
#include <qfile.h>
#include <interfaces/context.h>
#include <interfaces/contextmenuextension.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>
#include <ktexteditor/document.h>

namespace KDevelop
{

SourceFormatterController::SourceFormatterController(QObject *parent)
		: ISourceFormatterController(parent), m_modelinesEnabled(false)
{
	setObjectName("SourceFormatterController");
    setComponentData(KComponentData("kdevsourceformatter"));
    
	setXMLFile("kdevsourceformatter.rc");

	m_formatTextAction = actionCollection()->addAction("edit_reformat_source");
	m_formatTextAction->setText(i18n("&Reformat Source"));
	m_formatTextAction->setToolTip(i18n("Reformat source using AStyle"));
	m_formatTextAction->setWhatsThis(i18n("<b>Reformat source</b><p>Source reformatting "
	        "functionality using <b>astyle</b> library.</p>"));
	connect(m_formatTextAction, SIGNAL(triggered()), this, SLOT(beautifySource()));

	m_formatFilesAction = actionCollection()->addAction("tools_astyle");
	m_formatFilesAction->setText(i18n("Format files"));
	m_formatFilesAction->setToolTip(i18n("Format file(s) using the current theme"));
	m_formatFilesAction->setWhatsThis(i18n("<b>Format files</b><p>Formatting functionality using <b>astyle</b> library.</p>"));
	connect(m_formatFilesAction, SIGNAL(triggered()), this, SLOT(formatItem()));

	m_formatTextAction->setEnabled(false);
	m_formatFilesAction->setEnabled(true);

	connect(Core::self()->documentController(), SIGNAL(documentActivated(KDevelop::IDocument*)),
	        this, SLOT(activeDocumentChanged(KDevelop::IDocument*)));

	activeDocumentChanged(Core::self()->documentController()->activeDocument());
}

void SourceFormatterController::initialize()
{
	loadPlugins();
	m_rootConfigGroup = KGlobal::config()->group("SourceFormatter");
}

SourceFormatterController::~SourceFormatterController()
{
}

void SourceFormatterController::loadPlugins()
{
 	KDevelop::IPluginController *controller = KDevelop::ICore::self()->pluginController();

	foreach(KDevelop::IPlugin *p,
	        controller->allPluginsForExtension("org.kdevelop.ISourceFormatter")) {
		KPluginInfo info = controller->pluginInfo(p);
		QVariant mimes = info.property("X-KDevelop-SupportedMimeTypes");
		kDebug() << "Found plugin " << info.name() << " for mimes " << mimes << endl;

		QStringList mimeTypes = mimes.toStringList();
		foreach(const QString &s, mimeTypes) {
			QString lang = languageNameFromLanguageSupport(s);
			QList<KDevelop::IPlugin*> &list = m_plugins[lang];
			if (!list.contains(p))
				list.append(p);
			kDebug() << "Loading plugin " << info.name() << " for type " << s
			<< " and lang " << lang << endl;
		}
	}
}

KPluginInfo
SourceFormatterController::languageSupportForMimeType(const QString &name)
{
	QStringList constraints;
	constraints << QString("'%1' in [X-KDevelop-SupportedMimeTypes]").arg(name);

	QList<KPluginInfo> list = KDevelop::IPluginController::queryExtensionPlugins("ILanguageSupport", constraints);
	foreach(KPluginInfo p, list)
		return p;

	return KPluginInfo();
}

QString SourceFormatterController::languageNameFromLanguageSupport(const QString &name)
{
	if (m_languages.contains(name))
		return m_languages[name];

	QString lang = name;
	// we're loading the plugin, find the name from the language support plugin
	KPluginInfo p = languageSupportForMimeType(name);
	if (p.isValid())
		lang = p.property("X-KDevelop-Language").toString();

	m_languages.insert(name, lang);
	return lang;
}

QString SourceFormatterController::languageNameForMimeType(const KMimeType::Ptr &mime)
{
	if (m_languages.contains(mime->name()))
		return m_languages[mime->name()];
	return QString();
}

QString SourceFormatterController::iconForLanguage(const QString &lang)
{
	//  QString mime = mimeTypeForLanguage(lang);
	KDevelop::IPlugin *p = KDevelop::ICore::self()
	        ->pluginController()->pluginForExtension("org.kdevelop.ILanguageSupport", lang);//languageForMimeType(mime);
	if (p) {
		KPluginInfo info = KDevelop::ICore::self()->pluginController()->pluginInfo(p);
		return info.icon();
	}
	return lang;
}

QList<KDevelop::IPlugin*> SourceFormatterController::pluginListForLanguage(const QString &lang)
{
	if (m_plugins.contains(lang))
		return m_plugins[lang];
	return QList<KDevelop::IPlugin*>();
}

ISourceFormatter* SourceFormatterController::activeFormatter()
{
	return m_currentPlugins[m_currentLang];
}

ISourceFormatter* SourceFormatterController::formatterForUrl(const KUrl &url)
{
	KMimeType::Ptr mime = KMimeType::findByUrl(url);
	return formatterForMimeType(mime);
}

ISourceFormatter* SourceFormatterController::formatterForMimeType(const KMimeType::Ptr &mime)
{
	if (!m_languages.contains(mime->name())) //unknown mime type
		return 0;

	setActiveLanguage(languageNameForMimeType(mime));
	kDebug() << "About to format file " << mime->name() << m_currentLang << endl;
	return formatterForLanguage(m_languages[mime->name()]);
}

bool SourceFormatterController::isMimeTypeSupported(const KMimeType::Ptr &mime)
{
	return m_languages.contains(mime->name());
}

ISourceFormatter* SourceFormatterController::formatterForLanguage(const QString &language)
{
	if (m_currentPlugins.contains(language))
		return m_currentPlugins[language];

	// else load the current plugin from config
	KConfigGroup langGroup = m_rootConfigGroup.group(language);
	QString formatterName = langGroup.readEntry("Plugin", "");
	if (formatterName.isEmpty()) { // no formatters defined yet, load the first one
		KDevelop::IPlugin *p = m_plugins[language].first();
		if (!p)
			return 0;
		m_currentPlugins[language] = p->extension<ISourceFormatter>();
	} else
		m_currentPlugins[language] = formatterByName(language, formatterName);

	return m_currentPlugins[language];
}

ISourceFormatter* SourceFormatterController::formatterByName(const QString &language, const QString &name)
{
	QList<KDevelop::IPlugin*> list = m_plugins[language];
	foreach(KDevelop::IPlugin *p, list) {
		if (p) {
			ISourceFormatter *f = p->extension<ISourceFormatter>();
			if (f && (f->name() == name))
				return f;
		}
	}
	return 0;
}

// void SourceFormatterController::setFormatterForLanguage(const QString &lang, ISourceFormatter *formatter)
// {
//     m_currentPlugins[lang] = formatter;
// }

void SourceFormatterController::setActiveLanguage(const QString &lang, QString plugin)
{
	// find the plugin for this language
	if (plugin.isEmpty()) {
		if (m_currentLang == lang)
			return; // no change
		ISourceFormatter *f = formatterForLanguage(lang);
		if (f)
			plugin = f->name();
	} else
		m_currentPlugins[lang] = formatterByName(lang, plugin);
	kDebug() << "Activating language " << lang << " with plugin " << plugin << " == " << m_currentPlugins[lang] << endl;

	if (plugin.isEmpty()) {
		kDebug() << "Cannot find a suitable plugin for language " << lang << endl;
	}

	// update the plugin entry in config
// 	m_rootConfigGroup.group(lang).writeEntry("Plugin", plugin);
	m_activeConfigGroup = m_rootConfigGroup.group(lang).group(plugin);
	m_currentLang = lang;

	// load the current style for this language
	QString styleName = m_activeConfigGroup.readEntry("Style", "");
	if (!styleName.isEmpty() && m_currentPlugins[lang])
		setCurrentStyle(styleName);
}

QStringList SourceFormatterController::languages()
{
	QStringList list;
	QHash<QString, QString>::const_iterator it = m_languages.constBegin();
	for (; it != m_languages.constEnd(); ++it) {
		if (!list.contains(it.value()))
			list.append(it.value());
	}
	return list;
}

QString SourceFormatterController::mimeTypeForLanguage(const QString &lang)
{
	QHash<QString, QString>::const_iterator it = m_languages.constBegin();
	for (; it != m_languages.constEnd(); ++it) {
		if (it.value() == lang)
			return it.key();
	}
	return QString();
}

void SourceFormatterController::loadConfig()
{
	// reload config that may have been modified by config dialog
	m_currentPlugins.clear();
	m_currentLang.clear();

	m_modelinesEnabled = m_rootConfigGroup.readEntry("ModelinesEnabled",
	        QVariant(false)).toBool();
//     // load current plugins and styles
//     foreach(QString l, languages()) {
//         KConfigGroup langGroup = m_configGroup.group(l);
//         QString plugin = langGroup.readEntry("Plugin", "");
//         if(!plugin.isEmpty()) {
//             KConfigGroup pluginGroup = langGroup.group(plugin);
//             m_currentPlugins[l] = formatterByName(l, plugin);
// //             m_currentStyles[l] = pluginGroup.readEntry("Style","");
//         }
//     }
}

void SourceFormatterController::saveConfig()
{
	// save current plugins
	QHash<QString, ISourceFormatter*>::const_iterator it = m_currentPlugins.constBegin();
	for (; it != m_currentPlugins.constEnd(); ++it) {
		ISourceFormatter *f = it.value();
		if(f) {
			QString pluginName = f->name();
			m_rootConfigGroup.group(it.key()).writeEntry("Plugin", pluginName);
		}
	}

	m_rootConfigGroup.writeEntry("ModelinesEnabled", m_modelinesEnabled);
	m_rootConfigGroup.sync();
}

QString SourceFormatterController::currentStyle() const
{
    return m_currentStyle;
// 	return m_activeConfigGroup.readEntry("Style", "");
}

void SourceFormatterController::setCurrentStyle(const QString &style)
{
	if (!m_currentPlugins[m_currentLang]) {
		kDebug() << "currrent plugin is null for style " << m_currentLang << style << endl;
		return;
	}
	kDebug() << "style is " << style << endl;

	if (m_activeConfigGroup.hasKey(style)) { // custom style
		QString content = m_activeConfigGroup.readEntry(style);
		m_currentPlugins[m_currentLang]->setStyle(QString(), content);
	} else // predefined style
		m_currentPlugins[m_currentLang]->setStyle(style);

	m_currentStyle = style;
// 	m_activeConfigGroup.writeEntry("Style", style);
}

void SourceFormatterController::saveStyle(const QString &name, const QString &content)
{
	m_activeConfigGroup.writeEntry(name, content);
}

void SourceFormatterController::renameStyle(const QString &name, const QString &caption)
{
	m_activeConfigGroup.writeEntry("Caption" + name.mid(4), caption);
}

void SourceFormatterController::deleteStyle(const QString &name)
{
	m_activeConfigGroup.deleteEntry(name);
}

QString SourceFormatterController::nameForNewStyle()
{
	//find available number
	int idx = 1;
	QString s = "User" + QString::number(idx);
	while (m_activeConfigGroup.hasKey(s)) {
		++idx;
		s = "User" + QString::number(idx);
	}

	return s;
}

QString SourceFormatterController::indentationMode(const KMimeType::Ptr &mime)
{
	if (mime->is("text/x-c++src") || mime->is("text/x-chdr") ||
	        mime->is("text/x-c++hdr") || mime->is("text/x-csrc") ||
	        mime->is("text/x-java") || mime->is("text/x-csharp"))
		return "cstyle";
	return "none";
}

QString SourceFormatterController::addModelineForCurrentLang(QString input, const KMimeType::Ptr &mime)
{
	if (!m_currentPlugins[m_currentLang] || !m_modelinesEnabled)
		return input;

	QString output;
	QTextStream os(&output, QIODevice::WriteOnly);
	QTextStream is(&input, QIODevice::ReadOnly);

    QString modeline("// kate: ");
	QString length = QString::number(m_currentPlugins[m_currentLang]->indentationLength());
	// add indentation style
	modeline.append("indent-mode ").append(indentationMode(mime)).append("; ");

	ISourceFormatter::IndentationType type = m_currentPlugins[m_currentLang]->indentationType();
	if (type == ISourceFormatter::IndentWithTabs) {
		modeline.append("replace-tabs off; ");
		modeline.append("tab-width ").append(length).append("; ");
	} else {
		modeline.append("space-indent on; ");
		modeline.append("indent-width ").append(length).append("; ");
		if (type == ISourceFormatter::IndentWithSpacesAndConvertTabs)
			modeline.append("replace-tabs on; ");
	}

	kDebug() << "created modeline: " << modeline << endl;

	bool modelinefound = false;
	QRegExp kateModeline("^\\s*//\\s*kate:(.*)$");
	QRegExp knownOptions("\\s*(indent-width|space-indent|tab-width|indent-mode)");
	while (!is.atEnd()) {
		QString line = is.readLine();
		// replace only the options we care about
		if (kateModeline.indexIn(line) >= 0) { // match
			kDebug() << "Found a kate modeline: " << line << endl;
			modelinefound = true;
			QString options = kateModeline.cap(1);
			QStringList optionList = options.split(';', QString::SkipEmptyParts);

			os <<  modeline;
			foreach(const QString &s, optionList) {
				if (knownOptions.indexIn(s) < 0) { // unknown option, add it
					os << s << ";";
					kDebug() << "Found unknown option: " << s << endl;
				}
			}
			os << endl;
		} else
			os << line << endl;
	}

	if (!modelinefound)
		os << modeline << endl;
	return output;
}

void SourceFormatterController::setModelinesEnabled(bool enable)
{
	m_modelinesEnabled = enable;
}

void SourceFormatterController::cleanup()
{
}


void SourceFormatterController::activeDocumentChanged(IDocument* doc)
{
	bool enabled = false;

	if (doc) {
		KMimeType::Ptr mime = KMimeType::findByUrl(doc->url());
		if (isMimeTypeSupported(mime))
			enabled = true;
	}

	m_formatTextAction->setEnabled(enabled);
}

void SourceFormatterController::beautifySource()
{
	KDevelop::IDocumentController *docController = KDevelop::ICore::self()->documentController();
	KDevelop::IDocument *doc = docController->activeDocument();
	if (!doc)
		return;
	// load the appropriate formatter
	KMimeType::Ptr mime = KMimeType::findByUrl(doc->url());
	ISourceFormatter *formatter = formatterForMimeType(mime);

	bool has_selection = false;
	KTextEditor::View *view = doc->textDocument()->views().first();
	if (view && view->selection())
		has_selection = true;

	if (has_selection) {
		QString original = view->selectionText();

		QString output = formatter->formatSource(view->selectionText(), mime,
												  view->document()->text(KTextEditor::Range(KTextEditor::Cursor(0,0),view->selectionRange().start())),
												  view->document()->text(KTextEditor::Range(view->selectionRange().end(), view->document()->documentRange().end())));

		//remove the final newline character, unless it should be there
		if (!original.endsWith('\n')  && output.endsWith('\n'))
			output.resize(output.length() - 1);
		//there was a selection, so only change the part of the text related to it
		doc->textDocument()->replaceText(view->selectionRange(), output);
	} else
		formatDocument(doc, formatter, mime);
}

void SourceFormatterController::formatDocument(KDevelop::IDocument *doc, ISourceFormatter *formatter, const KMimeType::Ptr &mime)
{
	KTextEditor::Document *textDoc = doc->textDocument();

	KTextEditor::Cursor cursor = doc->cursorPosition();
	QString text = formatter->formatSource(textDoc->text(), mime);
	text = addModelineForCurrentLang(text, mime);
	textDoc->setText(text);
	doc->setCursorPosition(cursor);
}

void SourceFormatterController::formatFiles()
{
	if (m_prjItems.isEmpty())
		return;

	//get a list of all files in this folder recursively
	QList<KDevelop::ProjectFolderItem*> folders;
	foreach(KDevelop::ProjectBaseItem *item, m_prjItems) {
		if (!item)
			continue;
		if (item->folder())
			folders.append(item->folder());
		else if (item->file())
			m_urls.append(item->file()->url());
		else if (item->target()) {
			foreach(KDevelop::ProjectFileItem *f, item->fileList())
			m_urls.append(f->url());
		}
	}

	while (!folders.isEmpty()) {
		KDevelop::ProjectFolderItem *item = folders.takeFirst();
		foreach(KDevelop::ProjectFolderItem *f, item->folderList())
		folders.append(f);
		foreach(KDevelop::ProjectTargetItem *f, item->targetList()) {
			foreach(KDevelop::ProjectFileItem *child, f->fileList())
			m_urls.append(child->url());
		}
		foreach(KDevelop::ProjectFileItem *f, item->fileList())
		m_urls.append(f->url());
	}

	formatFiles(m_urls);
}

void SourceFormatterController::formatFiles(KUrl::List &list)
{
	//! \todo IStatus
	for (int fileCount = 0; fileCount < list.size(); fileCount++) {
		// check mimetype
		KMimeType::Ptr mime = KMimeType::findByUrl(list[fileCount]);
		kDebug() << "Checking file " << list[fileCount].pathOrUrl() << " of mime type " << mime->name() << endl;
		ISourceFormatter *formatter = formatterForMimeType(mime);
		if (!formatter) // unsupported mime type
			continue;

		// if the file is opened in the editor, format the text in the editor without saving it
		KDevelop::IDocumentController *docController = KDevelop::ICore::self()->documentController();
		KDevelop::IDocument *doc = docController->documentForUrl(list[fileCount]);
		if (doc) {
			kDebug() << "Processing file " << list[fileCount].pathOrUrl() << "opened in editor" << endl;
			formatDocument(doc, formatter, mime);
			return;
		}

		kDebug() << "Processing file " << list[fileCount].pathOrUrl() << endl;
		QString tmpFile, output;
		if (KIO::NetAccess::download(list[fileCount], tmpFile, 0)) {
			QFile file(tmpFile);
			// read file
			if (file.open(QFile::ReadOnly)) {
				QTextStream is(&file);
				output = formatter->formatSource(is.readAll(), mime);
				file.close();
			} else
				KMessageBox::error(0, i18n("Unable to read %1", list[fileCount].prettyUrl()));

			//write new content
			if (file.open(QFile::WriteOnly | QIODevice::Truncate)) {
				QTextStream os(&file);
				os << addModelineForCurrentLang(output, mime);
				file.close();
			} else
				KMessageBox::error(0, i18n("Unable to write to %1", list[fileCount].prettyUrl()));

			if (!KIO::NetAccess::upload(tmpFile, list[fileCount], 0))
				KMessageBox::error(0, KIO::NetAccess::lastErrorString());

			KIO::NetAccess::removeTempFile(tmpFile);
		} else
			KMessageBox::error(0, KIO::NetAccess::lastErrorString());
	}
}

KDevelop::ContextMenuExtension SourceFormatterController::contextMenuExtension(KDevelop::Context* context)
{
	KDevelop::ContextMenuExtension ext;
	m_urls.clear();
	m_prjItems.clear();

	if (context->hasType(KDevelop::Context::EditorContext))
		ext.addAction(KDevelop::ContextMenuExtension::EditGroup, m_formatTextAction);
	else if (context->hasType(KDevelop::Context::FileContext)) {
		KDevelop::FileContext* filectx = dynamic_cast<KDevelop::FileContext*>(context);
		m_urls = filectx->urls();
		ext.addAction(KDevelop::ContextMenuExtension::EditGroup, m_formatFilesAction);
	} else if (context->hasType(KDevelop::Context::CodeContext)) {
	} else if (context->hasType(KDevelop::Context::ProjectItemContext)) {
		KDevelop::ProjectItemContext* prjctx = dynamic_cast<KDevelop::ProjectItemContext*>(context);
		m_prjItems = prjctx->items();
		ext.addAction(KDevelop::ContextMenuExtension::ExtensionGroup, m_formatFilesAction);
	}
	return ext;
}



/*
 Code copied from source formatter plugin, unused currently but shouldn't be just thrown away
QString SourceFormatterPlugin::replaceSpacesWithTab(const QString &input, ISourceFormatter *formatter)
{
	QString output(input);
	int wsCount = formatter->indentationLength();
	ISourceFormatter::IndentationType type = formatter->indentationType();

	if (type == ISourceFormatter::IndentWithTabs) {
		// tabs and wsCount spaces to be a tab
		QString replace;
		for (int i = 0; i < wsCount;i++)
			replace += ' ';

		output = output.replace(replace, QChar('\t'));
// 		input = input.remove(' ');
	} else if (type == ISourceFormatter::IndentWithSpacesAndConvertTabs) {
		//convert tabs to spaces
		QString replace;
		for (int i = 0;i < wsCount;i++)
			replace += ' ';

		output = output.replace(QChar('\t'), replace);
	}
	return output;
}

QString SourceFormatterPlugin::addIndentation(QString input, const QString indentWith)
{
	QString output;
	QTextStream os(&output, QIODevice::WriteOnly);
	QTextStream is(&input, QIODevice::ReadOnly);

	while (!is.atEnd())
		os << indentWith << is.readLine() << endl;
	return output;
}
*/

}

#include "sourceformattercontroller.moc"

// kate: indent-mode cstyle; space-indent off; tab-width 4;

