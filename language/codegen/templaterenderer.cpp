/*
 * This file is part of KDevelop
 * Copyright 2012 Miha Čančula <miha@noughmad.eu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "templaterenderer.h"
#include "archivetemplateloader.h"
#include "codedescription.h"
#include <interfaces/icore.h>

#include <grantlee/engine.h>
#include <grantlee/metatype.h>
#include <KComponentData>
#include <KStandardDirs>
#include <KUrl>
#include <KDebug>
#include <QFile>

using namespace KDevelop;
using namespace Grantlee;

class NoEscapeStream : public OutputStream
{
public:
    NoEscapeStream();
    explicit NoEscapeStream (QTextStream* stream);

    virtual QString escape (const QString& input) const;
    virtual QSharedPointer< OutputStream > clone (QTextStream* stream) const;
};

NoEscapeStream::NoEscapeStream() : OutputStream()
{

}

NoEscapeStream::NoEscapeStream (QTextStream* stream) : OutputStream (stream)
{

}

QString NoEscapeStream::escape (const QString& input) const
{
    return input;
}

QSharedPointer< OutputStream > NoEscapeStream::clone (QTextStream* stream) const
{
    QSharedPointer<OutputStream> clonedStream = QSharedPointer<OutputStream>( new NoEscapeStream( stream ) );
    return clonedStream;
}

class KDevelop::TemplateRendererPrivate
{
public:
    Engine engine;
    Context context;
};

TemplateRenderer::TemplateRenderer()
    : d(new TemplateRendererPrivate)
{
    d->engine.setSmartTrimEnabled(true);

    addTemplateDirectories(ICore::self()->componentData().dirs()->findDirs("data", "kdevcodegen/templates"));

    foreach (const QString& path, ICore::self()->componentData().dirs()->resourceDirs("lib"))
    {
        d->engine.addPluginPath(path);
    }

    Grantlee::registerMetaType<KDevelop::VariableDescription>();
    Grantlee::registerMetaType<KDevelop::FunctionDescription>();
    Grantlee::registerMetaType<KDevelop::InheritanceDescription>();
    Grantlee::registerMetaType<KDevelop::ClassDescription>();
}

TemplateRenderer::~TemplateRenderer()
{
    delete d;
}

Grantlee::Engine* TemplateRenderer::engine()
{
    return &(d->engine);
}

void TemplateRenderer::addTemplateDirectories (const QStringList& directories)
{
    FileSystemTemplateLoader* loader = new FileSystemTemplateLoader;
    loader->setTemplateDirs(directories);
    d->engine.addTemplateLoader(AbstractTemplateLoader::Ptr(loader));
}

void TemplateRenderer::addArchive (const KArchiveDirectory* directory)
{
    ArchiveTemplateLoader* loader = new ArchiveTemplateLoader(directory);
    d->engine.addTemplateLoader(AbstractTemplateLoader::Ptr(loader));
}

void TemplateRenderer::addVariables (const QVariantHash& variables)
{
    QVariantHash::const_iterator it = variables.constBegin();
    QVariantHash::const_iterator end = variables.constEnd();
    for (; it != end; ++it)
    {
        d->context.insert(it.key(), it.value());
    }
}

void TemplateRenderer::addVariable (const QString& name, const QVariant& value)
{
    d->context.insert(name, value);
}

QString TemplateRenderer::render (const QString& content, const QString& name)
{
    kDebug() << d->context.stackHash(0);
    
    Template t = d->engine.newTemplate(content, name);
    QString ret;
    QTextStream textStream(&ret);
    NoEscapeStream stream(&textStream);

    t->render(&stream, &d->context);
    return ret;
}

QString TemplateRenderer::renderFile (const KUrl& url, const QString& name)
{
    QFile file(url.toLocalFile());
    file.open(QIODevice::ReadOnly);
    
    QString content(file.readAll());
    kDebug() << content;

    return render(content, name);
}


QStringList TemplateRenderer::render (const QStringList& contents)
{
    kDebug() << d->context.stackHash(0);
    QStringList ret;
    foreach (const QString& content, contents)
    {
        ret << render(content);
    }
    return ret;
}

