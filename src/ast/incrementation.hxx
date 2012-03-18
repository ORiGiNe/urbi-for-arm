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
 ** \file ast/incrementation.hxx
 ** \brief Inline methods of ast::Incrementation.
 */

#ifndef AST_INCREMENTATION_HXX
# define AST_INCREMENTATION_HXX

# include <ast/incrementation.hh>

namespace ast
{


#if defined ENABLE_SERIALIZATION
 template <typename T>
  void
  Incrementation::serialize(libport::serialize::OSerializer<T>& ser) const
  {
    LIBPORT_USE(ser);
    Unary::serialize(ser);
  }

  template <typename T>
  Incrementation::Incrementation(libport::serialize::ISerializer<T>& ser)
    : Unary(ser)
  {
    LIBPORT_USE(ser);
  }
#endif


} // namespace ast

#endif // !AST_INCREMENTATION_HXX
