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
 ** \file ast/unary.hxx
 ** \brief Inline methods of ast::Unary.
 */

#ifndef AST_UNARY_HXX
# define AST_UNARY_HXX

# include <ast/unary.hh>

namespace ast
{

#if defined ENABLE_SERIALIZATION
 template <typename T>
  void
  Unary::serialize(libport::serialize::OSerializer<T>& ser) const
  {
    LIBPORT_USE(ser);
    Exp::serialize(ser);
    ser.template serialize< rLValue >("exp", exp_);
  }

  template <typename T>
  Unary::Unary(libport::serialize::ISerializer<T>& ser)
    : Exp(ser)
  {
    LIBPORT_USE(ser);
    exp_ = ser.template unserialize< rLValue >("exp");
  }
#endif

  inline const rLValue&
  Unary::exp_get () const
  {
    return exp_;
  }
  inline rLValue&
  Unary::exp_get ()
  {
    return exp_;
  }


} // namespace ast

#endif // !AST_UNARY_HXX
