/* A Bison parser, made by GNU Bison 2.3b.696-f332.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

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


/* First part of user declarations.  */

/* Line 369 of lalr1.cc  */
#line 38 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"

#include "calc++-parser.hh"

/* User implementation prologue.  */

/* Line 374 of lalr1.cc  */
#line 45 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
/* Unqualified %code blocks.  */
/* Line 375 of lalr1.cc  */
#line 9733 "../../doc/bison.texinfo"

# include "calc++-driver.hh"


/* Line 375 of lalr1.cc  */
#line 54 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  static_cast<void>(0)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
/* Line 458 of lalr1.cc  */
#line 139 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  calcxx_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  calcxx_parser::calcxx_parser (calcxx_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  calcxx_parser::~calcxx_parser ()
  {
  }


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // stack_symbol_type.
  calcxx_parser::stack_symbol_type::stack_symbol_type ()
    : super_type ()
    , state ()
  {
  }

  calcxx_parser::stack_symbol_type::stack_symbol_type (state_type s, const semantic_type& v, const location_type& l)
    : super_type (v, l)
    , state (s)
  {
  }

  int
  calcxx_parser::stack_symbol_type::type_get_ () const
  {
    return yystos_[state];
  }


  template <typename Exact>
  void
  calcxx_parser::yy_destroy_ (const char* yymsg,
                                       symbol_base_type<Exact>& yysym) const
  {
    int yytype = yysym.type_get ();
    YYUSE (yymsg);
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yytype)
      {
       default:
          break;
      }

    // Type destructor.
    switch (yytype)
    {
      case 10: // "identifier"
        yysym.value.template destroy< std::string >();
        break;

      case 11: // "number"
      case 16: // exp
        yysym.value.template destroy< int >();
        break;

      default:
        break;
    }

  }

#if YYDEBUG
  template <typename Exact>
  void
  calcxx_parser::yy_print_ (std::ostream& yyo,
                                     const symbol_base_type<Exact>& yysym) const
  {
    int yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
      {
            case 10: // "identifier"

/* Line 576 of lalr1.cc  */
#line 9782 "../../doc/bison.texinfo"
        { debug_stream () << yysym.value.template as< std::string >(); }
/* Line 576 of lalr1.cc  */
#line 274 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
        break;

            case 11: // "number"

/* Line 576 of lalr1.cc  */
#line 9782 "../../doc/bison.texinfo"
        { debug_stream () << yysym.value.template as< int >(); }
/* Line 576 of lalr1.cc  */
#line 283 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
        break;

            case 16: // exp

/* Line 576 of lalr1.cc  */
#line 9782 "../../doc/bison.texinfo"
        { debug_stream () << yysym.value.template as< int >(); }
/* Line 576 of lalr1.cc  */
#line 292 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
        break;

       default:
          break;
      }
    yyo << ')';
  }
#endif

  void
  calcxx_parser::yypush_ (const char* m, state_type s,
                                   symbol_type& sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (stack_symbol_type (s, semantic_type(), sym.location));
      switch (yystos_[s])
    {
      case 10: // "identifier"
        yystack_[0].value.build< std::string >(sym.value);
        break;

      case 11: // "number"
      case 16: // exp
        yystack_[0].value.build< int >(sym.value);
        break;

      default:
        break;
    }

  }

  void
  calcxx_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (stack_symbol_type (s.state, semantic_type(), s.location));
      switch (yystos_[s.state])
    {
      case 10: // "identifier"
        yystack_[0].value.build< std::string >(s.value);
        break;

      case 11: // "number"
      case 16: // exp
        yystack_[0].value.build< int >(s.value);
        break;

      default:
        break;
    }

  }

  void
  calcxx_parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  calcxx_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  calcxx_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  calcxx_parser::debug_level_type
  calcxx_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  calcxx_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline calcxx_parser::state_type
  calcxx_parser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  calcxx_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  calcxx_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  calcxx_parser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    /* State.  */
    int yyn;
    int yylen = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse().
    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    /* Line 701 of lalr1.cc  */
#line 9708 "../../doc/bison.texinfo"
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}
/* Line 701 of lalr1.cc  */
#line 441 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_ = stack_type (0);
    yypush_ (0, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    /* Accept?  */
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
        {
          yyla = yylex (driver);
        }
        catch (const syntax_error& yyexc)
        {
          error (yyexc.location, yyexc.what());
          goto yyerrlab1;
        }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    /* Discard the token being shifted.  */
    yyempty = true;

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    /* Shift the lookahead token.  */
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 action is NOT applied when using
       variants.  */
      switch (yyr1_[yyn])
    {
      case 10: // "identifier"
        yylhs.value.build< std::string >();
        break;

      case 11: // "number"
      case 16: // exp
        yylhs.value.build< int >();
        break;

      default:
        break;
    }


    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
    {
      switch (yyn)
      {
  case 2:
/* Line 828 of lalr1.cc  */
#line 9793 "../../doc/bison.texinfo"
    { driver.result = yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 564 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 3:
/* Line 828 of lalr1.cc  */
#line 9796 "../../doc/bison.texinfo"
    {}
/* Line 828 of lalr1.cc  */
#line 572 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 4:
/* Line 828 of lalr1.cc  */
#line 9797 "../../doc/bison.texinfo"
    {}
/* Line 828 of lalr1.cc  */
#line 580 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 5:
/* Line 828 of lalr1.cc  */
#line 9800 "../../doc/bison.texinfo"
    { driver.variables[yystack_[2].value.as< std::string >()] = yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 588 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 6:
/* Line 828 of lalr1.cc  */
#line 9805 "../../doc/bison.texinfo"
    { yylhs.value.as< int >() = yystack_[2].value.as< int >() + yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 596 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 7:
/* Line 828 of lalr1.cc  */
#line 9806 "../../doc/bison.texinfo"
    { yylhs.value.as< int >() = yystack_[2].value.as< int >() - yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 604 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 8:
/* Line 828 of lalr1.cc  */
#line 9807 "../../doc/bison.texinfo"
    { yylhs.value.as< int >() = yystack_[2].value.as< int >() * yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 612 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 9:
/* Line 828 of lalr1.cc  */
#line 9808 "../../doc/bison.texinfo"
    { yylhs.value.as< int >() = yystack_[2].value.as< int >() / yystack_[0].value.as< int >(); }
/* Line 828 of lalr1.cc  */
#line 620 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 10:
/* Line 828 of lalr1.cc  */
#line 9809 "../../doc/bison.texinfo"
    { std::swap (yylhs.value.as< int >(), yystack_[1].value.as< int >()); }
/* Line 828 of lalr1.cc  */
#line 628 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 11:
/* Line 828 of lalr1.cc  */
#line 9810 "../../doc/bison.texinfo"
    { yylhs.value.as< int >() = driver.variables[yystack_[0].value.as< std::string >()]; }
/* Line 828 of lalr1.cc  */
#line 636 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;

  case 12:
/* Line 828 of lalr1.cc  */
#line 9811 "../../doc/bison.texinfo"
    { std::swap (yylhs.value.as< int >(), yystack_[0].value.as< int >()); }
/* Line 828 of lalr1.cc  */
#line 644 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
    break;


/* Line 828 of lalr1.cc  */
#line 649 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
        default:
          break;
      }
    }
    catch (const syntax_error& yyexc)
    {
      error (yyexc.location, yyexc.what());
      YYERROR;
    }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);

    // Destroy the rhs symbols.
    for (int i = 0; i < yylen; ++i)
      // Destroy a variant which value may have been swapped with
      // yylhs.value (for instance if the action was "std::swap($$,
      // $1)").  The value of yylhs.value (hence possibly one of these
      // rhs symbols) depends on the default contruction for this
      // type.  In the case of pointers for instance, no
      // initialization is done, so the value is junk.  Therefore do
      // not try to report the value of symbols about to be destroyed
      // in the debug trace, it's possibly junk.  Hence yymsg = 0.
      // Besides, that keeps exactly the same traces as with the other
      // Bison skeletons.
      yy_destroy_ (0, yystack_[i]);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    /* Shift the result of the reduction.  */
    yypush_ (0, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        /* Return failure if at end of input.  */
        if (yyla.type == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* $$ was initialized before running the user action.  */
    yy_destroy_ ("Error: discarding", yylhs);
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   /* Each real token shifted decrements this.  */
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      /* Shift the error token.  */
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystack_.size () != 1)
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  calcxx_parser::yysyntax_error_ (state_type yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = 0;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char calcxx_parser::yypact_ninf_ = -5;

  const signed char calcxx_parser::yytable_ninf_ = -1;

  const signed char
  calcxx_parser::yypact_[] =
  {
      -5,     5,     9,    -5,    13,    15,    -5,    -5,     8,    -5,
      -3,    13,    13,    13,    13,    13,    -5,     8,    19,    19,
      -5,    -5
  };

  const unsigned char
  calcxx_parser::yydefact_[] =
  {
       4,     0,     0,     1,     0,    11,    12,     3,     2,    11,
       0,     0,     0,     0,     0,     0,    10,     5,     7,     6,
       8,     9
  };

  const signed char
  calcxx_parser::yypgoto_[] =
  {
      -5,    -5,    -5,    -5,    -4
  };

  const signed char
  calcxx_parser::yydefgoto_[] =
  {
      -1,     1,     2,     7,     8
  };

  const unsigned char
  calcxx_parser::yytable_[] =
  {
      10,    12,    13,    14,    15,     3,    16,    17,    18,    19,
      20,    21,    12,    13,    14,    15,     0,     4,    11,     5,
       6,     4,     0,     9,     6,    14,    15
  };

  const signed char
  calcxx_parser::yycheck_[] =
  {
       4,     4,     5,     6,     7,     0,     9,    11,    12,    13,
      14,    15,     4,     5,     6,     7,    -1,     8,     3,    10,
      11,     8,    -1,    10,    11,     6,     7
  };

  const unsigned char
  calcxx_parser::yystos_[] =
  {
       0,    13,    14,     0,     8,    10,    11,    15,    16,    10,
      16,     3,     4,     5,     6,     7,     9,    16,    16,    16,
      16,    16
  };

  const unsigned char
  calcxx_parser::yyr1_[] =
  {
       0,    12,    13,    14,    14,    15,    16,    16,    16,    16,
      16,    16,    16
  };

  const unsigned char
  calcxx_parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,     3,     3,     3,     3,     3,
       3,     1,     1
  };


#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const calcxx_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\":=\"", "\"-\"", "\"+\"",
  "\"*\"", "\"/\"", "\"(\"", "\")\"", "\"identifier\"", "\"number\"",
  "$accept", "unit", "assignments", "assignment", "exp", 0
  };
#endif

#if YYDEBUG
  const unsigned short int
  calcxx_parser::yyrline_[] =
  {
       0,  9793,  9793,  9796,  9797,  9800,  9805,  9806,  9807,  9808,
    9809,  9810,  9811
  };

  // Print the state stack on the debug stream.
  void
  calcxx_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  calcxx_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
/* Line 1131 of lalr1.cc  */
#line 1022 "/home/build/urbi-sdk_x86_linux_gcc-4_release/x86_linux_lucid_gcc4_release/source/bison/examples/calc++/calc++-parser.cc"
/* Line 1132 of lalr1.cc  */
#line 9812 "../../doc/bison.texinfo"

#line 9821 "../../doc/bison.texinfo"
void
yy::calcxx_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
