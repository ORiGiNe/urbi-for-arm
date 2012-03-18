/* A Bison parser, made by GNU Bison 2.3b.696-f332.  */

/* Skeleton interface for Bison LALR(1) parsers in C++

   Copyright (C) 2002-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

/* "%code requires" blocks.  */
/* Line 146 of lalr1.cc  */
#line 9681 "../../doc/bison.texinfo"

# include <string>
class calcxx_driver;


/* Line 146 of lalr1.cc  */
#line 48 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.hh"


#include <cassert>
#include <stdexcept>
#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"


namespace yy {
/* Line 156 of lalr1.cc  */
#line 61 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.hh"

  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Whether something is contained.
    bool built;

    /// Empty construction.
    inline
    variant ()
      : built (false)
    {}

    /// Instantiate a \a T in here.
    template <typename T>
    inline T&
    build ()
    {
      assert (!built);
      built = true;
      return *new (buffer.raw) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    inline T&
    build (const T& t)
    {
      assert(!built);
      built = true;
      return *new (buffer.raw) T(t);
    }

    /// Construct and fill.
    template <typename T>
    inline
    variant (const T& t)
      : built (true)
    {
      new (buffer.raw) T(t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    inline T&
    as ()
    {
      assert (built);
      return reinterpret_cast<T&>(buffer.raw);
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    inline const T&
    as () const
    {
      assert(built);
      return reinterpret_cast<const T&>(buffer.raw);
    }

    /// Swap the content with \a other.
    template <typename T>
    inline void
    swap (variant<S>& other)
    {
      std::swap (as<T>(), other.as<T>());
    }

    /// Assign the content of \a other to this.
    /// Destroys \a other.
    template <typename T>
    inline void
    build (variant<S>& other)
    {
      build<T>();
      swap<T>(other);
      other.destroy<T>();
    }

    /// Destroy the stored \a T.
    template <typename T>
    inline void
    destroy ()
    {
      as<T>().~T();
      built = false;
    }

    /// A buffer large enough to store any of the semantic values.
    /// Long double is chosen as it has the strongest alignment
    /// constraints.
    union
    {
      long double align_me;
      char raw[S];
    } buffer;
  };


} // yy
/* Line 156 of lalr1.cc  */
#line 168 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


namespace yy {
/* Line 178 of lalr1.cc  */
#line 191 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.hh"

  /// A Bison parser.
  class calcxx_parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // "identifier"
      char dummy1[sizeof(std::string)];

      // "number"
      // exp
      char dummy2[sizeof(int)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     TOK_END = 0,
     TOK_ASSIGN = 258,
     TOK_MINUS = 259,
     TOK_PLUS = 260,
     TOK_STAR = 261,
     TOK_SLASH = 262,
     TOK_LPAREN = 263,
     TOK_RPAREN = 264,
     TOK_IDENTIFIER = 265,
     TOK_NUMBER = 266
   };

    };

    /// Token type.
    typedef token::yytokentype token_type;

    /// A complete symbol, with its type.
    template <typename Exact>
    struct symbol_base_type
    {
      /// Default constructor.
      inline symbol_base_type ();

      /// Constructor.
      inline symbol_base_type (const location_type& l);
      inline symbol_base_type (const semantic_type& v, const location_type& l);

      /// Return this with its exact type.
      const Exact& self () const;
      Exact& self ();

      /// Return the type of this symbol.
      int type_get () const;

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;
    };

    /// External form of a symbol: its type and attributes.
    struct symbol_type : symbol_base_type<symbol_type>
    {
      /// The parent class.
      typedef symbol_base_type<symbol_type> super_type;

      /// Default constructor.
      inline symbol_type ();

      /// Constructor for tokens with semantic value.
      inline symbol_type (token_type t, const semantic_type& v, const location_type& l);

      /// Constructor for valueless tokens.
      inline symbol_type (token_type t, const location_type& l);

      /// The symbol type.
      int type;

      /// The symbol type.
      inline int type_get_ () const;

      /// The token.
      inline token_type token () const;
    };
    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_ASSIGN (const location_type& l);

    static inline
    symbol_type
    make_MINUS (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_STAR (const location_type& l);

    static inline
    symbol_type
    make_SLASH (const location_type& l);

    static inline
    symbol_type
    make_LPAREN (const location_type& l);

    static inline
    symbol_type
    make_RPAREN (const location_type& l);

    static inline
    symbol_type
    make_IDENTIFIER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NUMBER (const int& v, const location_type& l);


    /// Build a parser object.
    calcxx_parser (calcxx_driver& driver_yyarg);
    virtual ~calcxx_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

  private:
    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate, int yytoken);

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yylhs     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yylhs);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /* Tables.  */
  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.    */
  static const signed char yypact_[];

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE does not specify something else to do.  Zero means the default
     is an error.    */
  static const unsigned char yydefact_[];

  /* YYPGOTO[NTERM-NUM].    */
  static const signed char yypgoto_[];

  /* YYDEFGOTO[NTERM-NUM].    */
  static const signed char yydefgoto_[];

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF, syntax error.    */
  static const unsigned char yytable_[];

  static const signed char yycheck_[];

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.    */
  static const unsigned char yystos_[];

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.    */
  static const unsigned char yyr1_[];

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.    */
  static const unsigned char yyr2_[];


#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);

#if YYDEBUG
  /* YYRLINEYYN -- Source line where rule number YYN was defined.    */
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    static inline token_number_type yytranslate_ (token_type t);

#if YYDEBUG
    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Exact>
    void yy_print_ (std::ostream& yyo,
                    const symbol_base_type<Exact>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param s         The symbol.
    template <typename Exact>
    inline void yy_destroy_ (const char* yymsg,
                             symbol_base_type<Exact>& yysym) const;

  private:
    /// Element of the stack: a state and its attributes.
    struct stack_symbol_type : symbol_base_type<stack_symbol_type>
    {
      /// The parent class.
      typedef symbol_base_type<stack_symbol_type> super_type;

      /// Default constructor.
      inline stack_symbol_type ();

      /// Constructor.
      inline stack_symbol_type (state_type s, const semantic_type& v, const location_type& l);

      /// The state.
      state_type state;

      /// The type (corresponding to \a state).
      inline int type_get_ () const;
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    inline void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    inline void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    enum
    {
      yyeof_ = 0,
      yylast_ = 26,           //< Last index in yytable_.
      yynnts_ = 5,  //< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 3, //< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 12    //< Number of tokens.
    };


    /* User arguments.  */
    calcxx_driver& driver;
  };

  // Symbol number corresponding to token number t.
  calcxx_parser::token_number_type
  calcxx_parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11
    };
    const unsigned int user_token_number_max_ = 266;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  calcxx_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // symbol_base_type.
  template <typename Exact>
  inline
  calcxx_parser::symbol_base_type<Exact>::symbol_base_type ()
    : value()
    , location()
  {
  }

  template <typename Exact>
  inline
  calcxx_parser::symbol_base_type<Exact>::symbol_base_type (const location_type& l)
    : value()
    , location(l)
  {
  }

  template <typename Exact>
  inline
  calcxx_parser::symbol_base_type<Exact>::symbol_base_type (const semantic_type& v, const location_type& l)
    : value(v)
    , location(l)
  {
  }

  template <typename Exact>
  inline
  const Exact&
  calcxx_parser::symbol_base_type<Exact>::self () const
  {
    return static_cast<const Exact&>(*this);
  }

  template <typename Exact>
  inline
  Exact&
  calcxx_parser::symbol_base_type<Exact>::self ()
  {
    return static_cast<Exact&>(*this);
  }

  template <typename Exact>
  inline
  int
  calcxx_parser::symbol_base_type<Exact>::type_get () const
  {
    return self ().type_get_ ();
  }

  // symbol_type.
  inline
  calcxx_parser::symbol_type::symbol_type ()
    : super_type ()
    , type ()
  {
  }

  inline
  calcxx_parser::symbol_type::symbol_type (token_type t, const location_type& l)
    : super_type (l)
    , type (yytranslate_ (t))
  {
  }

  inline
  calcxx_parser::symbol_type::symbol_type (token_type t, const semantic_type& v, const location_type& l)
    : super_type (v, l)
    , type (yytranslate_ (t))
  {
  }

  inline
  int
  calcxx_parser::symbol_type::type_get_ () const
  {
    return type;
  }

  inline
  calcxx_parser::token_type
  calcxx_parser::symbol_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  calcxx_parser::symbol_type
  calcxx_parser::make_END (const location_type& l)
  {
    return symbol_type (token::TOK_END, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_ASSIGN (const location_type& l)
  {
    return symbol_type (token::TOK_ASSIGN, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_MINUS (const location_type& l)
  {
    return symbol_type (token::TOK_MINUS, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::TOK_PLUS, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_STAR (const location_type& l)
  {
    return symbol_type (token::TOK_STAR, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_SLASH (const location_type& l)
  {
    return symbol_type (token::TOK_SLASH, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_LPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_LPAREN, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_RPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_RPAREN, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_IDENTIFIER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_IDENTIFIER, v, l);
  }

  calcxx_parser::symbol_type
  calcxx_parser::make_NUMBER (const int& v, const location_type& l)
  {
    return symbol_type (token::TOK_NUMBER, v, l);
  }



} // yy
/* Line 347 of lalr1.cc  */
#line 739 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.hh"



#endif /* ! defined PARSER_HEADER_H */
