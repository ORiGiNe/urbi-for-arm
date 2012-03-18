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
 ** \file ast/unary.hh
 ** \brief Declaration of ast::Unary.
 */

#ifndef AST_UNARY_HH
# define AST_UNARY_HH

# include <ast/exp.hh>
# include <ast/lvalue.hh>
# include <urbi/object/fwd.hh>
# include <kernel/config.h>

#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif

namespace ast
{

/// Expressions with a expression.
  class Unary : public Exp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /// Construct an Unary node.
    Unary (const loc& location, const rLValue& exp);
    /// Destroy an Unary node.
    virtual ~Unary ();
    /** \} */

#if defined ENABLE_SERIALIZATION
    /// \name Serialization.
    /// \{ */
  public:
    template <typename T>
    Unary(libport::serialize::ISerializer<T>& ser);
    template <typename T>
    void
    serialize(libport::serialize::OSerializer<T>& ser) const;
    /// \}
#endif


    /** \name Accessors.
     ** \{ */
  public:
    /// Return operand.
    const rLValue& exp_get () const;
    /// Return operand.
    rLValue& exp_get ();
    /** \} */

  protected:
    /// Operand.
    rLValue exp_;
  };

} // namespace ast


#if ENABLE_SERIALIZATION
# include <serialize/serialize.hh>
#endif
# include <ast/unary.hxx>

#endif // !AST_UNARY_HH
