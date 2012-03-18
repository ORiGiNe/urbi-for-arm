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
 ** \file ast/local-declaration.hh
 ** \brief Declaration of ast::LocalDeclaration.
 */

#ifndef AST_LOCAL_DECLARATION_HH
# define AST_LOCAL_DECLARATION_HH

# include <ast/local-write.hh>
# include <urbi/object/fwd.hh>
# include <kernel/config.h>

#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif

namespace ast
{

  /// LocalDeclaration.
  class LocalDeclaration : public LocalWrite
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /// Construct a LocalDeclaration node.
    LocalDeclaration (const loc& location, const libport::Symbol& what,
                      const rExp& value);
    /// Destroy a LocalDeclaration node.
    virtual ~LocalDeclaration ();
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
    LocalDeclaration(libport::serialize::ISerializer<T>& ser);
    template <typename T>
    void
    serialize(libport::serialize::OSerializer<T>& ser) const;
    /// \}
#endif


    /** \name Accessors.
     ** \{ */
  public:
    /// Return whether this slot should be constant.
    const bool& constant_get () const;
    /// Set whether this slot should be constant.
    void constant_set (bool);
    /// Return whether this is a list formal argument.
    const bool& list_get () const;
    /// Set whether this is a list formal argument.
    void list_set (bool);
    /** \} */

  protected:
    /// Whether this slot should be constant.
    bool constant_;
    /// Whether this is a list formal argument.
    bool list_;
  };

} // namespace ast


#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif
# include <ast/local-declaration.hxx>

#endif // !AST_LOCAL_DECLARATION_HH
