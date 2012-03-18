/*
 * Copyright (C) 2006-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

// Generated, do not edit by hand.

/**
 ** \file ast/scope.hh
 ** \brief Declaration of ast::Scope.
 */

#ifndef AST_SCOPE_HH
# define AST_SCOPE_HH

# include <ast/exp.hh>
# include <urbi/object/fwd.hh>
# include <kernel/config.h>

#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif

namespace ast
{

  /// Scope.
  class Scope : public Exp
  {
  public:
    /// Is there just one child?
    virtual bool single() const;

    /** \name Ctor & dtor.
     ** \{ */
  public:
    /// Construct a Scope node.
    Scope (const loc& location, const rExp& body);
    /// Destroy a Scope node.
    virtual ~Scope ();
    /** \} */

    /// \name Visitors entry point.
    /// \{ */
  public:
    /// Accept a const visitor \a v.
    virtual void accept (ConstVisitor& v) const;
    /// Accept a non-const visitor \a v.
    virtual void accept (Visitor& v);
    /// Evaluate the node in interpreter \a r
    virtual urbi::object::rObject eval(runner::Interpreter& r) const ;
    /// Return the node type
    virtual std::string node_type() const;
    /// \}

#if defined ENABLE_SERIALIZATION
    /// \name Serialization.
    /// \{ */
  public:
    template <typename T>
    Scope(libport::serialize::ISerializer<T>& ser);
    template <typename T>
    void
    serialize(libport::serialize::OSerializer<T>& ser) const;
    /// \}
#endif


    /** \name Accessors.
     ** \{ */
  public:
    /// Return the scoped expression.
    const rExp& body_get () const;
    /// Return the scoped expression.
    rExp& body_get ();
    /// Set the scoped expression.
    void body_set (const rExp&);
    /** \} */

  protected:
    /// The scoped expression.
    rExp body_;
  };

} // namespace ast


#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif
# include <ast/scope.hxx>

#endif // !AST_SCOPE_HH
