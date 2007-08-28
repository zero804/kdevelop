/* This file is part of KDevelop
    Copyright 2006 Roberto Raggi <roberto@kdevelop.org>
    Copyright 2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#include <identifier.h>
#include <languageexport.h>
#include <QtCore/QSet>

#include <QtCore/QList>

#include <ksharedptr.h>

namespace KDevelop
{

class AbstractType;
class IntegralType;
class PointerType;
class ReferenceType;
class FunctionType;
class StructureType;
class ArrayType;

class KDEVPLATFORMLANGUAGE_EXPORT TypeVisitor
{
public:
  virtual ~TypeVisitor ();

  virtual bool preVisit (const AbstractType *) = 0;
  virtual void postVisit (const AbstractType *) = 0;

  ///Return whether sub-types should be visited(same for the other visit functions)
  virtual bool visit(const AbstractType*) = 0;

  virtual void visit (const IntegralType *) = 0;

  virtual bool visit (const PointerType *) = 0;
  virtual void endVisit (const PointerType *) = 0;

  virtual bool visit (const ReferenceType *) = 0;
  virtual void endVisit (const ReferenceType *) = 0;

  virtual bool visit (const FunctionType *) = 0;
  virtual void endVisit (const FunctionType *) = 0;

  virtual bool visit (const StructureType *) = 0;
  virtual void endVisit (const StructureType *) = 0;

  virtual bool visit (const ArrayType *) = 0;
  virtual void endVisit (const ArrayType *) = 0;
};

class KDEVPLATFORMLANGUAGE_EXPORT SimpleTypeVisitor : public TypeVisitor
{
public:
  ///When using SimpleTypeVisitor, this is the only function you must override to collect all types.
  virtual bool visit(const AbstractType*) = 0;

  virtual bool preVisit (const AbstractType *) ;
  virtual void postVisit (const AbstractType *) ;

  virtual void visit (const IntegralType *) ;

  virtual bool visit (const PointerType *) ;
  virtual void endVisit (const PointerType *) ;

  virtual bool visit (const ReferenceType *) ;
  virtual void endVisit (const ReferenceType *) ;

  virtual bool visit (const FunctionType *) ;
  virtual void endVisit (const FunctionType *) ;

  virtual bool visit (const StructureType *) ;
  virtual void endVisit (const StructureType *) ;

  virtual bool visit (const ArrayType *) ;
  virtual void endVisit (const ArrayType *) ;
};

/**
 * A class that can be used to walk through all types that are references from one type, and exchange them with other types.
 * Examples for such types: Base-classes of a class, function-argument types of a function, etc.
 * */
class KDEVPLATFORMLANGUAGE_EXPORT TypeExchanger {
  public:
    virtual ~TypeExchanger() {
    }

    /**
     * By default should return the given type, and can return another type that the given should be replaced with.
     * Types should allow replacing all their held types using this from within their exchangeTypes function.
     * */
    virtual AbstractType* exchange( const AbstractType* ) = 0;
    /**
     * Should member-types be exchanged?(Like the types of a structure's members) If false, only types involved in the identity will be exchanged.
     * */
    virtual bool exchangeMembers() const = 0;
};

class KDEVPLATFORMLANGUAGE_EXPORT AbstractType : public KShared
{
public:
  typedef KSharedPtr<AbstractType> Ptr;

  AbstractType();
  AbstractType(const AbstractType& rhs);
  virtual ~AbstractType ();

  void accept(TypeVisitor *v) const;

  static void acceptType(AbstractType::Ptr type, TypeVisitor *v);

  virtual QString toString() const = 0;

  virtual QString mangled() const;

  ///Should return whether this type's content equals the given one
  virtual bool equals(const AbstractType* rhs) const = 0;
  
  /**
   * Should create a clone of the source-type, with as much data copied as possible without breaking the du-chain.
   * */
  virtual AbstractType* clone() const = 0;

  uint hash() const;

  enum WhichType {
    TypeAbstract,
    TypeIntegral,
    TypePointer,
    TypeReference,
    TypeFunction,
    TypeStructure,
    TypeArray,
    TypeDelayed
  };

  virtual WhichType whichType() const;

  /**
   * Should, like accept0, be implemented by all types that hold references to other types.
   * */
  virtual void exchangeTypes( TypeExchanger* exchanger );

protected:
  virtual void accept0 (TypeVisitor *v) const = 0;

//  template <class T>
//  void deregister(T* that) { TypeSystem::self()->deregisterType(that); }

private:
  class AbstractTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT IntegralType: public AbstractType
{
public:
  typedef KSharedPtr<IntegralType> Ptr;

  IntegralType();
  IntegralType(const IntegralType& rhs);
  IntegralType(const QString& name);
  ~IntegralType();

  const QString& name() const;

  void setName(const QString& name);

  bool operator == (const IntegralType &other) const;

  bool operator != (const IntegralType &other) const;

  virtual QString toString() const;

  //virtual uint hash() const;
  virtual WhichType whichType() const;

  virtual AbstractType* clone() const;
  
  virtual bool equals(const AbstractType* rhs) const;

protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class IntegralTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT PointerType: public AbstractType
{
public:
  typedef KSharedPtr<PointerType> Ptr;

  PointerType ();
  PointerType(const PointerType& rhs);
  ~PointerType();

  bool operator != (const PointerType &other) const;
  bool operator == (const PointerType &other) const;
  void setBaseType(AbstractType::Ptr type);
  AbstractType::Ptr baseType () const;

  virtual QString toString() const;

  //virtual uint hash() const;

  virtual WhichType whichType() const;

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;
  
  virtual void exchangeTypes( TypeExchanger* exchanger );
protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class PointerTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT ReferenceType: public AbstractType
{
public:
  typedef KSharedPtr<ReferenceType> Ptr;

  ReferenceType ();
  ReferenceType (const ReferenceType& rhs);
  ~ReferenceType();
  AbstractType::Ptr baseType () const;

  void setBaseType(AbstractType::Ptr baseType);

  bool operator == (const ReferenceType &other) const;

  bool operator != (const ReferenceType &other) const;

  virtual QString toString() const;

  //virtual uint hash() const;

  virtual WhichType whichType() const;

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;
  
  virtual void exchangeTypes( TypeExchanger* exchanger );
protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class ReferenceTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT FunctionType : public AbstractType
{
public:
  typedef KSharedPtr<FunctionType> Ptr;

  enum SignaturePart {
    SignatureWhole, //When this is given to toString(..), a string link "RETURNTYPE (ARGTYPE1, ARGTYPE1, ..)" is returned
    SignatureReturn, //When this is given, only a string that represents the return-type is returned
    SignatureArguments //When this is given, a string that represents the arguments like "(ARGTYPE1, ARGTYPE1, ..)" is returend
  };
  
  FunctionType();
  FunctionType(const FunctionType& rhs);
  ~FunctionType();

  AbstractType::Ptr returnType () const;

  void setReturnType(AbstractType::Ptr returnType);

  const QList<AbstractType::Ptr>& arguments () const;

  void addArgument(AbstractType::Ptr argument);
  void removeArgument(AbstractType::Ptr argument);

  bool operator == (const FunctionType &other) const;

  bool operator != (const FunctionType &other) const;

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;
  
  virtual QString toString() const;

  ///Creates a string that represents the given part of the signature
  virtual QString toString( SignaturePart sigPart ) const;
  
  //virtual uint hash() const;

  virtual WhichType whichType() const;

  virtual void exchangeTypes( TypeExchanger* exchanger );
protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class FunctionTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT StructureType : public AbstractType
{
public:
  StructureType();
  StructureType(const StructureType&);
  ~StructureType();
  typedef KSharedPtr<StructureType> Ptr;

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;
  
  const QList<AbstractType::Ptr>& elements () const;

  bool operator == (const StructureType &other) const;

  bool operator != (const StructureType &other) const;

  virtual void addElement(AbstractType::Ptr element);
  void removeElement(AbstractType::Ptr element);

  void clear();

  virtual QString toString() const;

  //virtual uint hash() const;

  virtual WhichType whichType() const;

  virtual void exchangeTypes( TypeExchanger* exchanger );
protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class StructureTypePrivate* const d;
};

class KDEVPLATFORMLANGUAGE_EXPORT ArrayType : public AbstractType
{
public:
  typedef KSharedPtr<ArrayType> Ptr;

  ArrayType(const ArrayType&);

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;
  
  ArrayType();
  ~ArrayType();

  int dimension () const;

  void setDimension(int dimension);

  AbstractType::Ptr elementType () const;

  void setElementType(AbstractType::Ptr type);

  bool operator == (const ArrayType &other) const;

  bool operator != (const ArrayType &other) const;

  virtual QString toString() const;

  //virtual uint hash() const;

  virtual WhichType whichType() const;

  virtual void exchangeTypes( TypeExchanger* exchanger );
protected:
  virtual void accept0 (TypeVisitor *v) const;

private:
  class ArrayTypePrivate* const d;
};

/**
 * Delayed types can be used for any types that cannot be resolved in the moment they are encountered.
 * They can be used for example in template-classes, or to store the names of unresolved types.
 * In a template-class, many types can not be evaluated at the time they are used, because they depend on unknown template-parameters.
 * Delayed types store the way the type would be searched, and can be used to find the type once the template-paremeters have values.
 * */
class KDEVPLATFORMLANGUAGE_EXPORT DelayedType : public KDevelop::AbstractType
{
public:
  typedef KSharedPtr<DelayedType> Ptr;

  enum Kind {
    Delayed, //The type should be resolved later
    Unresolved //The type could not be resolved
  };
  
  DelayedType();
  virtual ~DelayedType();

  KDevelop::QualifiedIdentifier qualifiedIdentifier() const;
  void setQualifiedIdentifier(const KDevelop::QualifiedIdentifier& identifier);

  virtual QString toString() const;

  virtual AbstractType* clone() const;

  virtual bool equals(const AbstractType* rhs) const;

  Kind kind() const;
  void setKind(Kind kind);
  
  virtual WhichType whichType() const;
  protected:
    virtual void accept0 (KDevelop::TypeVisitor *v) const ;
  private:
    class DelayedTypePrivate* const d;
};

template <class T>
uint qHash(const KSharedPtr<T>& type) { return type->hash(); }

}




#endif // TYPESYSTEM_H

// kate: space-indent on; indent-width 2; tab-width: 4; replace-tabs on; auto-insert-doxygen on
