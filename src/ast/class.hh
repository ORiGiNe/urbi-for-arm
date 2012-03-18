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
 ** \file ast/class.hh
 ** \brief Declaration of ast::Class.
 */

#ifndef AST_CLASS_HH
# define AST_CLASS_HH

# include <ast/exp.hh>
# include <ast/exps-type.hh>
# include <ast/lvalue.hh>
# include <urbi/object/fwd.hh>
# include <kernel/config.h>

#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif

namespace ast
{

  /// Class.
  class Class : public Exp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /// Construct a Class node.
    Class (const loc& location, const rLValue& what, exps_type* protos,
           const rExp& content);
    /// Destroy a Class node.
    virtual ~Class ();
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
    Class(libport::serialize::ISerializer<T>& ser);
    template <typename T>
    void
    serialize(libport::serialize::OSerializer<T>& ser) const;
    /// \}
#endif


    /** \name Accessors.
     ** \{ */
  public:
    /// Return lvalue where to store the class.
    const rLValue& what_get () const;
    /// Return lvalue where to store the class.
    rLValue& what_get ();
    /// Return prototypes.
    const exps_type* protos_get () const;
    /// Return prototypes.
    exps_type* protos_get ();
    /// Return class content.
    const rExp& content_get () const;
    /// Return class content.
    rExp& content_get ();
    /** \} */

  protected:
    /// Lvalue where to store the class.
    rLValue what_;
    /// Prototypes.
    exps_type* protos_;
    /// Class content.
    rExp content_;
  };

} // namespace ast


#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif
# include <ast/class.hxx>

#endif // !AST_CLASS_HH
