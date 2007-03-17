/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

// Take the name prefix into account.
#define yylex   QMakelex

#include "qmake_parser.hpp"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc.  */


#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace QMake
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
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

#endif

  /// Build a parser object.
  Parser::Parser (QMake::Lexer* lexer_yyarg, QMake::ProjectAST* project_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      lexer (lexer_yyarg),
      project (project_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  Parser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, lexer);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

    {
            foreach( StatementAST* s, (yysemantic_stack_[(1) - (1)].stmtlist))
            {
                project->addStatement( s );
            }
        ;}
    break;

  case 3:

    {
            (yyval.stmtlist).append( (yysemantic_stack_[(2) - (2)].stmt) );
        ;}
    break;

  case 4:

    {
            (yyval.stmtlist).clear();
        ;}
    break;

  case 5:

    {
            (yyval.stmt) = new CommentAST( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 6:

    {
            (yyval.stmt) = new NewlineAST( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 7:

    {
            (yyval.stmt) = (yysemantic_stack_[(1) - (1)].stmt);
        ;}
    break;

  case 8:

    {
            (yyval.stmt) = (yysemantic_stack_[(1) - (1)].stmt);
        ;}
    break;

  case 9:

    {
            (yyval.stmt) = new ScopeAST( (yysemantic_stack_[(3) - (2)].funccall), (yysemantic_stack_[(3) - (3)].value), (yysemantic_stack_[(3) - (1)].value) );
        ;}
    break;

  case 10:

    {
            (yyval.stmt) = (yysemantic_stack_[(1) - (1)].stmt);
        ;}
    break;

  case 11:

    {
            ScopeAST* node = (yysemantic_stack_[(2) - (1)].scope);
            node->setScopeBody( (yysemantic_stack_[(2) - (2)].scopebody) );
            (yyval.stmt) = node;
        ;}
    break;

  case 12:

    {
            (yysemantic_stack_[(6) - (2)].funccall)->setWhitespace((yysemantic_stack_[(6) - (1)].value));
            (yysemantic_stack_[(6) - (5)].funccall)->setWhitespace((yysemantic_stack_[(6) - (4)].value));
            OrAST* node = new OrAST( (yysemantic_stack_[(6) - (2)].funccall), (yysemantic_stack_[(6) - (3)].value), (yysemantic_stack_[(6) - (5)].funccall), (yysemantic_stack_[(6) - (6)].scopebody) );
            (yyval.stmt) = node;
        ;}
    break;

  case 13:

    {
            ScopeAST* node = new ScopeAST( (yysemantic_stack_[(2) - (2)].value), (yysemantic_stack_[(2) - (1)].value) );
            (yyval.scope) = node;
        ;}
    break;

  case 14:

    {
            ScopeAST* node = new ScopeAST( (yysemantic_stack_[(2) - (2)].funccall), "", (yysemantic_stack_[(2) - (1)].value) );
            (yyval.scope) = node;
        ;}
    break;

  case 15:

    {
            ScopeBodyAST* node = new ScopeBodyAST( (yysemantic_stack_[(8) - (1)].value)+(yysemantic_stack_[(8) - (2)].value)+(yysemantic_stack_[(8) - (3)].value)+(yysemantic_stack_[(8) - (4)].value), (yysemantic_stack_[(8) - (5)].stmtlist), (yysemantic_stack_[(8) - (6)].value)+(yysemantic_stack_[(8) - (7)].value)+(yysemantic_stack_[(8) - (8)].value) );
            (yyval.scopebody) = node;
        ;}
    break;

  case 16:

    {
            ScopeBodyAST* node = new ScopeBodyAST( (yysemantic_stack_[(7) - (1)].value)+(yysemantic_stack_[(7) - (2)].value)+(yysemantic_stack_[(7) - (3)].value), (yysemantic_stack_[(7) - (4)].stmtlist), (yysemantic_stack_[(7) - (5)].value)+(yysemantic_stack_[(7) - (6)].value)+(yysemantic_stack_[(7) - (7)].value) );
            (yyval.scopebody) = node;
        ;}
    break;

  case 17:

    {
            ScopeBodyAST* node = new ScopeBodyAST( (yysemantic_stack_[(6) - (1)].value)+(yysemantic_stack_[(6) - (2)].value)+(yysemantic_stack_[(6) - (3)].value), (yysemantic_stack_[(6) - (4)].stmtlist), (yysemantic_stack_[(6) - (5)].value)+(yysemantic_stack_[(6) - (6)].value) );
            (yyval.scopebody) = node;
        ;}
    break;

  case 18:

    {
            ScopeBodyAST* node = new ScopeBodyAST( (yysemantic_stack_[(7) - (1)].value)+(yysemantic_stack_[(7) - (2)].value)+(yysemantic_stack_[(7) - (3)].value)+(yysemantic_stack_[(7) - (4)].value), (yysemantic_stack_[(7) - (5)].stmtlist), (yysemantic_stack_[(7) - (6)].value)+(yysemantic_stack_[(7) - (7)].value) );
            (yyval.scopebody) = node;
        ;}
    break;

  case 19:

    {
            ScopeBodyAST* node = new ScopeBodyAST( (yysemantic_stack_[(3) - (1)].value)+(yysemantic_stack_[(3) - (2)].value), (yysemantic_stack_[(3) - (3)].stmt) );
            (yyval.scopebody) = node;
        ;}
    break;

  case 20:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 21:

    {
            (yyval.value) = (yysemantic_stack_[(2) - (1)].value)+(yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 22:

    {
            FunctionCallAST* node = new FunctionCallAST( (yysemantic_stack_[(4) - (1)].value), (yysemantic_stack_[(4) - (2)].value), (yysemantic_stack_[(4) - (3)].arglist), (yysemantic_stack_[(4) - (4)].value) );
            (yyval.funccall) = node;
        ;}
    break;

  case 23:

    {
            (yyval.funccall) = new FunctionCallAST( (yysemantic_stack_[(3) - (1)].value), (yysemantic_stack_[(3) - (2)].value), QList<FunctionArgAST*>(), (yysemantic_stack_[(3) - (3)].value) );
        ;}
    break;

  case 24:

    {
            (yyval.funccall) = new FunctionCallAST( (yysemantic_stack_[(5) - (1)].value)+(yysemantic_stack_[(5) - (2)].value), (yysemantic_stack_[(5) - (3)].value), (yysemantic_stack_[(5) - (4)].arglist), (yysemantic_stack_[(5) - (5)].value) );
        ;}
    break;

  case 25:

    {
            (yyval.funccall) = new FunctionCallAST( (yysemantic_stack_[(4) - (1)].value)+(yysemantic_stack_[(4) - (2)].value), (yysemantic_stack_[(4) - (3)].value), QList<FunctionArgAST*>(), (yysemantic_stack_[(4) - (4)].value) );
        ;}
    break;

  case 26:

    {
            (yyval.arglist).append( (yysemantic_stack_[(3) - (3)].fnarg) );
        ;}
    break;

  case 27:

    {
            (yyval.arglist).clear();
            (yyval.arglist).append( (yysemantic_stack_[(1) - (1)].fnarg) );
        ;}
    break;

  case 28:

    {
            (yyval.fnarg) = new SimpleFunctionArgAST( (yysemantic_stack_[(2) - (2)].value), (yysemantic_stack_[(2) - (1)].value) );
        ;}
    break;

  case 29:

    {
            (yyval.fnarg) = new FunctionCallAST( (yysemantic_stack_[(7) - (2)].value), (yysemantic_stack_[(7) - (3)].value)+(yysemantic_stack_[(7) - (4)].value), (yysemantic_stack_[(7) - (5)].arglist), (yysemantic_stack_[(7) - (6)].value)+(yysemantic_stack_[(7) - (7)].value), (yysemantic_stack_[(7) - (1)].value) );
        ;}
    break;

  case 30:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 31:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 32:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 33:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 34:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 35:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 36:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 37:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 38:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 39:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 40:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 41:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 42:

    {
            (yyval.value) += (yysemantic_stack_[(2) - (2)].value);
        ;}
    break;

  case 43:

    {
            (yyval.value) += (yysemantic_stack_[(4) - (2)].value)+(yysemantic_stack_[(4) - (3)].value)+(yysemantic_stack_[(4) - (4)].value);
        ;}
    break;

  case 44:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 45:

    {
            (yyval.value) = (yysemantic_stack_[(3) - (1)].value)+(yysemantic_stack_[(3) - (2)].value)+(yysemantic_stack_[(3) - (3)].value);
        ;}
    break;

  case 46:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 47:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 48:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 49:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 50:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 51:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 52:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 53:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 54:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 55:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 56:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 57:

    {
            (yyval.stmt) = new AssignmentAST( (yysemantic_stack_[(6) - (2)].value), (yysemantic_stack_[(6) - (3)].value), (yysemantic_stack_[(6) - (4)].values), (yysemantic_stack_[(6) - (5)].value), (yysemantic_stack_[(6) - (6)].value), (yysemantic_stack_[(6) - (1)].value) );
        ;}
    break;

  case 58:

    {
            (yyval.stmt) = new AssignmentAST( (yysemantic_stack_[(5) - (2)].value), (yysemantic_stack_[(5) - (3)].value), QStringList(), (yysemantic_stack_[(5) - (4)].value),  (yysemantic_stack_[(5) - (5)].value), (yysemantic_stack_[(5) - (1)].value) );
        ;}
    break;

  case 59:

    {
            (yyval.stmt) = new AssignmentAST( (yysemantic_stack_[(5) - (2)].value), (yysemantic_stack_[(5) - (3)].value), (yysemantic_stack_[(5) - (4)].values), "", (yysemantic_stack_[(5) - (5)].value), (yysemantic_stack_[(5) - (1)].value) );
        ;}
    break;

  case 60:

    {
            (yyval.stmt) = new AssignmentAST( (yysemantic_stack_[(4) - (2)].value), (yysemantic_stack_[(4) - (3)].value), QStringList(), "", (yysemantic_stack_[(4) - (4)].value), (yysemantic_stack_[(4) - (1)].value) );
        ;}
    break;

  case 61:

    {
            (yyval.values).append( (yysemantic_stack_[(3) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(3) - (3)].value) );
        ;}
    break;

  case 62:

    {
            (yyval.values).append( (yysemantic_stack_[(3) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(3) - (3)].value) );
        ;}
    break;

  case 63:

    {
            (yyval.values).append( (yysemantic_stack_[(4) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (3)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (4)].value) );
        ;}
    break;

  case 64:

    {
            (yyval.values).append( (yysemantic_stack_[(4) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (3)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (4)].value) );
        ;}
    break;

  case 65:

    {

            (yyval.values).append( (yysemantic_stack_[(4) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (3)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (4)].value) );
        ;}
    break;

  case 66:

    {

            (yyval.values).append( (yysemantic_stack_[(4) - (2)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (3)].value) );
            (yyval.values).append( (yysemantic_stack_[(4) - (4)].value) );
        ;}
    break;

  case 67:

    {
            (yyval.values) = QStringList();
            (yyval.values).append( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 68:

    {
            (yyval.values) = QStringList();
            (yyval.values).append( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 69:

    {
            (yyval.values) = QStringList();
            (yyval.values).append( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 70:

    {
            (yyval.values) = QStringList();
            (yyval.values).append( (yysemantic_stack_[(1) - (1)].value) );
        ;}
    break;

  case 71:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 72:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 73:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 74:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 75:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 76:

    {
            (yyval.value) = (yysemantic_stack_[(1) - (1)].value);
        ;}
    break;

  case 77:

    {
            (yyval.value) = "";
        ;}
    break;

  case 78:

    {
            (yyval.value) = (yysemantic_stack_[(2) - (1)].value)+(yysemantic_stack_[(2) - (2)].value);
        ;}
    break;


    /* Line 675 of lalr1.cc.  */

	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
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

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
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
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -65;
  const short int
  Parser::yypact_[] =
  {
       -65,    20,    44,   -65,   -65,    24,   -65,   -65,   -65,   -65,
      45,   -65,     9,   -65,   -65,   -65,    77,   173,    47,    43,
     -65,   -65,    -8,   129,    88,   -65,   -65,   -65,   -65,   -65,
     156,    55,   -65,     1,    45,   -65,   -65,   -65,    36,   -65,
     -65,    57,   -65,   -65,   -65,   153,     3,   -65,     0,   -65,
      99,    19,   129,   -65,   -65,   -14,   -65,    45,    66,    45,
     -65,    61,    45,   -65,   -65,   -65,   123,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -65,    45,   -65,    46,    69,
      45,   137,   129,   -65,   -65,    11,   -65,    16,   -65,   -65,
      12,    86,   -65,   -65,   -65,   123,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -65,   -65,    81,   141,   -65,
     -65,   -65,   -65,   -65,    45,    76,   -65,    84,    87,   -65,
     -65,    45,   -65
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         4,     0,    77,     1,    76,     0,     6,     3,     8,    10,
      77,     7,     0,     5,    78,    11,     0,     0,     0,     0,
      20,    13,    14,    77,     0,    71,    72,    73,    75,    74,
       0,     0,    21,    77,    77,     9,    19,     4,     0,    60,
      69,     0,    68,    70,    67,     0,    77,    23,     0,    27,
       0,     0,    77,     4,    58,     0,    59,    77,     0,    77,
      25,     0,    77,    22,    44,    54,     0,    53,    55,    50,
      51,    49,    52,    56,    47,    48,    77,    46,    28,     0,
      77,     0,    77,    61,    62,     0,    57,     0,    24,    26,
       0,     0,    33,    42,    39,     0,    38,    40,    35,    36,
      34,    37,    41,    31,    32,    30,    12,    17,     0,    63,
      65,    64,    66,    45,    77,     0,    16,    18,     0,    43,
      15,    77,    29
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -65,   -65,   -34,    80,   -65,   -65,   -65,    30,   -65,    67,
     -45,    59,   -64,   -65,   -65,   -65,    -2,   -65
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Parser::yydefgoto_[] =
  {
        -1,     1,     2,     7,     8,     9,    10,    15,    21,    22,
      48,    49,    78,    11,    45,    30,    50,    13
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -3;
  const signed char
  Parser::yytable_[] =
  {
        12,    61,    90,    52,     4,    34,     4,    62,    16,    83,
      35,    63,    47,    17,    60,    92,    84,    93,    94,    82,
       3,    12,    95,   113,    96,    97,    98,    99,   100,   101,
      18,   115,    51,   102,   109,    19,   103,   104,    20,   111,
      79,   110,    14,   105,    -2,    19,   112,     4,     4,    92,
      81,    93,    94,    33,    53,    85,    95,    87,    96,    97,
      98,    99,   100,   101,     5,    46,     6,   102,    62,   118,
     103,   104,    88,    31,    91,    54,    32,   105,    16,    92,
     108,    93,    94,    23,    86,    24,    95,   119,    96,    97,
      98,    99,   100,   101,    62,    31,   114,   102,   121,   116,
     103,   104,   120,    36,    64,    65,    37,   105,    38,    66,
     106,    67,    68,    69,    70,    71,    72,     0,    80,   122,
      73,    89,     0,    74,    75,     0,     0,    76,    64,    65,
      77,     0,     4,    66,     0,    67,    68,    69,    70,    71,
      72,    17,     0,     0,    73,    17,   107,    74,    75,     5,
     117,     6,     0,     0,    77,     0,    55,     0,    18,     0,
       0,     0,    18,    19,     0,     0,    20,    19,     0,     0,
      20,    56,    57,    58,    39,    40,    41,     0,     0,    42,
      59,     0,     0,    43,     0,    25,    44,    26,    27,    28,
      29
  };

  /* YYCHECK.  */
  const signed char
  Parser::yycheck_[] =
  {
         2,    46,    66,    37,     3,    13,     3,     7,    10,    23,
      18,    11,    11,     4,    11,     3,    30,     5,     6,    53,
       0,    23,    10,    11,    12,    13,    14,    15,    16,    17,
      21,    95,    34,    21,    23,    26,    24,    25,    29,    23,
      21,    30,    18,    31,     0,    26,    30,     3,     3,     3,
      52,     5,     6,    10,    18,    57,    10,    59,    12,    13,
      14,    15,    16,    17,    20,    10,    22,    21,     7,   114,
      24,    25,    11,    26,    76,    18,    29,    31,    80,     3,
      82,     5,     6,     6,    18,     8,    10,    11,    12,    13,
      14,    15,    16,    17,     7,    26,    10,    21,    11,    18,
      24,    25,    18,    23,     5,     6,    18,    31,    20,    10,
      80,    12,    13,    14,    15,    16,    17,    -1,    51,   121,
      21,    62,    -1,    24,    25,    -1,    -1,    28,     5,     6,
      31,    -1,     3,    10,    -1,    12,    13,    14,    15,    16,
      17,     4,    -1,    -1,    21,     4,     9,    24,    25,    20,
       9,    22,    -1,    -1,    31,    -1,     3,    -1,    21,    -1,
      -1,    -1,    21,    26,    -1,    -1,    29,    26,    -1,    -1,
      29,    18,    19,    20,    18,    19,    20,    -1,    -1,    23,
      27,    -1,    -1,    27,    -1,    12,    30,    14,    15,    16,
      17
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    33,    34,     0,     3,    20,    22,    35,    36,    37,
      38,    45,    48,    49,    18,    39,    48,     4,    21,    26,
      29,    40,    41,     6,     8,    12,    14,    15,    16,    17,
      47,    26,    29,    10,    13,    18,    35,    18,    20,    18,
      19,    20,    23,    27,    30,    46,    10,    11,    42,    43,
      48,    48,    34,    18,    18,     3,    18,    19,    20,    27,
      11,    42,     7,    11,     5,     6,    10,    12,    13,    14,
      15,    16,    17,    21,    24,    25,    28,    31,    44,    21,
      41,    48,    34,    23,    30,    48,    18,    48,    11,    43,
      44,    48,     3,     5,     6,    10,    12,    13,    14,    15,
      16,    17,    21,    24,    25,    31,    39,     9,    48,    23,
      30,    23,    30,    11,    10,    44,    18,     9,    42,    11,
      18,    11,    48
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    32,    33,    34,    34,    35,    35,    35,    35,    35,
      35,    36,    37,    38,    38,    39,    39,    39,    39,    39,
      40,    40,    41,    41,    41,    41,    42,    42,    43,    43,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    45,    45,    45,
      45,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    47,    47,    47,    47,    47,    48,    48,    49
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     2,     0,     1,     1,     1,     1,     3,
       1,     2,     6,     2,     2,     8,     7,     6,     7,     3,
       1,     2,     4,     3,     5,     4,     3,     1,     2,     7,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     4,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     5,     5,
       4,     3,     3,     4,     4,     4,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "WS", "VARIABLE", "DOLLAR", "COLON",
  "COMMA", "LCURLY", "RCURLY", "LPAREN", "RPAREN", "EQUAL", "OR", "PLUSEQ",
  "MINUSEQ", "TILDEEQ", "STAREQ", "NEWLINE", "CONT", "COMMENT", "EXCLAM",
  "EMPTYLINE", "VAR_VALUE", "QMVARIABLE", "SHELLVARIABLE", "FUNCTIONNAME",
  "CONT_COMMENT", "FUNCTIONCALL", "SCOPENAME", "QUOTED_VAR_VALUE",
  "FNVALUE", "$accept", "project", "statements", "statement", "scope",
  "or_op", "scope_head", "scope_body", "scope_name", "functioncall",
  "functionargs", "functionarg", "fnvalue", "variable_assignment",
  "values", "op", "ws", "comment", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        33,     0,    -1,    34,    -1,    34,    35,    -1,    -1,    49,
      -1,    22,    -1,    45,    -1,    36,    -1,    48,    41,    18,
      -1,    37,    -1,    38,    39,    -1,    48,    41,    13,    48,
      41,    39,    -1,    48,    40,    -1,    48,    41,    -1,    48,
       8,    20,    18,    34,    48,     9,    18,    -1,    48,     8,
      18,    34,    48,     9,    18,    -1,    48,     8,    18,    34,
      48,     9,    -1,    48,     8,    20,    18,    34,    48,     9,
      -1,    48,     6,    35,    -1,    29,    -1,    21,    29,    -1,
      26,    10,    42,    11,    -1,    26,    10,    11,    -1,    21,
      26,    10,    42,    11,    -1,    21,    26,    10,    11,    -1,
      42,     7,    43,    -1,    43,    -1,    48,    44,    -1,    48,
      28,    48,    10,    42,    11,    48,    -1,    44,    31,    -1,
      44,    24,    -1,    44,    25,    -1,    44,     3,    -1,    44,
      16,    -1,    44,    14,    -1,    44,    15,    -1,    44,    17,
      -1,    44,    12,    -1,    44,     6,    -1,    44,    13,    -1,
      44,    21,    -1,    44,     5,    -1,    44,    10,    44,    11,
      -1,     5,    -1,    10,    44,    11,    -1,    31,    -1,    24,
      -1,    25,    -1,    16,    -1,    14,    -1,    15,    -1,    17,
      -1,    12,    -1,     6,    -1,    13,    -1,    21,    -1,    48,
       4,    47,    46,    20,    18,    -1,    48,     4,    47,    20,
      18,    -1,    48,     4,    47,    46,    18,    -1,    48,     4,
      47,    18,    -1,    46,     3,    23,    -1,    46,     3,    30,
      -1,    46,    19,    48,    23,    -1,    46,    27,    48,    23,
      -1,    46,    19,    48,    30,    -1,    46,    27,    48,    30,
      -1,    30,    -1,    23,    -1,    19,    -1,    27,    -1,    12,
      -1,    14,    -1,    15,    -1,    17,    -1,    16,    -1,     3,
      -1,    -1,    20,    18,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,     9,    11,    13,    15,    17,
      21,    23,    26,    33,    36,    39,    48,    56,    63,    71,
      75,    77,    80,    85,    89,    95,   100,   104,   106,   109,
     117,   120,   123,   126,   129,   132,   135,   138,   141,   144,
     147,   150,   153,   156,   161,   163,   167,   169,   171,   173,
     175,   177,   179,   181,   183,   185,   187,   189,   196,   202,
     208,   213,   217,   221,   226,   231,   236,   241,   243,   245,
     247,   249,   251,   253,   255,   257,   259,   261,   262
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,    74,    74,    83,    88,    93,    97,   101,   105,   109,
     113,   119,   127,   136,   141,   148,   153,   158,   163,   168,
     175,   179,   185,   190,   194,   198,   204,   208,   215,   219,
     225,   229,   233,   237,   241,   245,   249,   253,   257,   261,
     265,   269,   273,   277,   281,   285,   289,   293,   297,   301,
     305,   309,   313,   317,   321,   325,   329,   335,   339,   343,
     347,   353,   358,   363,   369,   375,   382,   389,   394,   399,
     404,   411,   415,   419,   423,   427,   433,   438,   443
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 190;
  const int Parser::yynnts_ = 18;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 32;

  const unsigned int Parser::yyuser_token_number_max_ = 286;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace QMake




namespace QMake
{
    void Parser::error(const location_type& /*l*/, const std::string& m)
    {
        std::cerr << m << std::endl;
    }
}
