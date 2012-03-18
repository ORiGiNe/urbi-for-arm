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
 ** \file ast/fwd.hh
 ** \brief Forward declarations of all AST classes
 ** (needed by the visitors).
 */

#ifndef AST_FWD_HH
# define AST_FWD_HH

# include <boost/ptr_container/ptr_vector.hpp>

# include <libport/fwd.hh>
# include <libport/hash.hh>
# include <libport/separate.hh>


# include <boost/preprocessor/array.hpp>
# include <boost/preprocessor/repeat.hpp>
# include <boost/preprocessor/seq/for_each.hpp>

# include <libport/intrusive-ptr.hh>
# include <libport/typelist.hh>

namespace ast
{

  class And;
  typedef libport::intrusive_ptr<And> rAnd;
  typedef libport::intrusive_ptr<const And> rConstAnd;
  class Assign;
  typedef libport::intrusive_ptr<Assign> rAssign;
  typedef libport::intrusive_ptr<const Assign> rConstAssign;
  class Assignment;
  typedef libport::intrusive_ptr<Assignment> rAssignment;
  typedef libport::intrusive_ptr<const Assignment> rConstAssignment;
  class Ast;
  typedef libport::intrusive_ptr<Ast> rAst;
  typedef libport::intrusive_ptr<const Ast> rConstAst;
  class At;
  typedef libport::intrusive_ptr<At> rAt;
  typedef libport::intrusive_ptr<const At> rConstAt;
  class Binding;
  typedef libport::intrusive_ptr<Binding> rBinding;
  typedef libport::intrusive_ptr<const Binding> rConstBinding;
  class Break;
  typedef libport::intrusive_ptr<Break> rBreak;
  typedef libport::intrusive_ptr<const Break> rConstBreak;
  class Call;
  typedef libport::intrusive_ptr<Call> rCall;
  typedef libport::intrusive_ptr<const Call> rConstCall;
  class CallMsg;
  typedef libport::intrusive_ptr<CallMsg> rCallMsg;
  typedef libport::intrusive_ptr<const CallMsg> rConstCallMsg;
  class Catch;
  typedef libport::intrusive_ptr<Catch> rCatch;
  typedef libport::intrusive_ptr<const Catch> rConstCatch;
  class Class;
  typedef libport::intrusive_ptr<Class> rClass;
  typedef libport::intrusive_ptr<const Class> rConstClass;
  class Composite;
  typedef libport::intrusive_ptr<Composite> rComposite;
  typedef libport::intrusive_ptr<const Composite> rConstComposite;
  class Continue;
  typedef libport::intrusive_ptr<Continue> rContinue;
  typedef libport::intrusive_ptr<const Continue> rConstContinue;
  class Declaration;
  typedef libport::intrusive_ptr<Declaration> rDeclaration;
  typedef libport::intrusive_ptr<const Declaration> rConstDeclaration;
  class Decrementation;
  typedef libport::intrusive_ptr<Decrementation> rDecrementation;
  typedef libport::intrusive_ptr<const Decrementation> rConstDecrementation;
  class Dictionary;
  typedef libport::intrusive_ptr<Dictionary> rDictionary;
  typedef libport::intrusive_ptr<const Dictionary> rConstDictionary;
  class Do;
  typedef libport::intrusive_ptr<Do> rDo;
  typedef libport::intrusive_ptr<const Do> rConstDo;
  class Emit;
  typedef libport::intrusive_ptr<Emit> rEmit;
  typedef libport::intrusive_ptr<const Emit> rConstEmit;
  class Event;
  typedef libport::intrusive_ptr<Event> rEvent;
  typedef libport::intrusive_ptr<const Event> rConstEvent;
  class Exp;
  typedef libport::intrusive_ptr<Exp> rExp;
  typedef libport::intrusive_ptr<const Exp> rConstExp;
  class Finally;
  typedef libport::intrusive_ptr<Finally> rFinally;
  typedef libport::intrusive_ptr<const Finally> rConstFinally;
  class Flavored;
  typedef libport::intrusive_ptr<Flavored> rFlavored;
  typedef libport::intrusive_ptr<const Flavored> rConstFlavored;
  class Float;
  typedef libport::intrusive_ptr<Float> rFloat;
  typedef libport::intrusive_ptr<const Float> rConstFloat;
  class Foreach;
  typedef libport::intrusive_ptr<Foreach> rForeach;
  typedef libport::intrusive_ptr<const Foreach> rConstForeach;
  class If;
  typedef libport::intrusive_ptr<If> rIf;
  typedef libport::intrusive_ptr<const If> rConstIf;
  class Implicit;
  typedef libport::intrusive_ptr<Implicit> rImplicit;
  typedef libport::intrusive_ptr<const Implicit> rConstImplicit;
  class Incrementation;
  typedef libport::intrusive_ptr<Incrementation> rIncrementation;
  typedef libport::intrusive_ptr<const Incrementation> rConstIncrementation;
  class LValue;
  typedef libport::intrusive_ptr<LValue> rLValue;
  typedef libport::intrusive_ptr<const LValue> rConstLValue;
  class LValueArgs;
  typedef libport::intrusive_ptr<LValueArgs> rLValueArgs;
  typedef libport::intrusive_ptr<const LValueArgs> rConstLValueArgs;
  class List;
  typedef libport::intrusive_ptr<List> rList;
  typedef libport::intrusive_ptr<const List> rConstList;
  class Local;
  typedef libport::intrusive_ptr<Local> rLocal;
  typedef libport::intrusive_ptr<const Local> rConstLocal;
  class LocalAssignment;
  typedef libport::intrusive_ptr<LocalAssignment> rLocalAssignment;
  typedef libport::intrusive_ptr<const LocalAssignment> rConstLocalAssignment;
  class LocalDeclaration;
  typedef libport::intrusive_ptr<LocalDeclaration> rLocalDeclaration;
  typedef libport::intrusive_ptr<const LocalDeclaration> rConstLocalDeclaration;
  class LocalWrite;
  typedef libport::intrusive_ptr<LocalWrite> rLocalWrite;
  typedef libport::intrusive_ptr<const LocalWrite> rConstLocalWrite;
  class Match;
  typedef libport::intrusive_ptr<Match> rMatch;
  typedef libport::intrusive_ptr<const Match> rConstMatch;
  class MetaArgs;
  typedef libport::intrusive_ptr<MetaArgs> rMetaArgs;
  typedef libport::intrusive_ptr<const MetaArgs> rConstMetaArgs;
  class MetaCall;
  typedef libport::intrusive_ptr<MetaCall> rMetaCall;
  typedef libport::intrusive_ptr<const MetaCall> rConstMetaCall;
  class MetaExp;
  typedef libport::intrusive_ptr<MetaExp> rMetaExp;
  typedef libport::intrusive_ptr<const MetaExp> rConstMetaExp;
  class MetaId;
  typedef libport::intrusive_ptr<MetaId> rMetaId;
  typedef libport::intrusive_ptr<const MetaId> rConstMetaId;
  class MetaLValue;
  typedef libport::intrusive_ptr<MetaLValue> rMetaLValue;
  typedef libport::intrusive_ptr<const MetaLValue> rConstMetaLValue;
  class Nary;
  typedef libport::intrusive_ptr<Nary> rNary;
  typedef libport::intrusive_ptr<const Nary> rConstNary;
  class Noop;
  typedef libport::intrusive_ptr<Noop> rNoop;
  typedef libport::intrusive_ptr<const Noop> rConstNoop;
  class OpAssignment;
  typedef libport::intrusive_ptr<OpAssignment> rOpAssignment;
  typedef libport::intrusive_ptr<const OpAssignment> rConstOpAssignment;
  class Pipe;
  typedef libport::intrusive_ptr<Pipe> rPipe;
  typedef libport::intrusive_ptr<const Pipe> rConstPipe;
  class Property;
  typedef libport::intrusive_ptr<Property> rProperty;
  typedef libport::intrusive_ptr<const Property> rConstProperty;
  class PropertyAction;
  typedef libport::intrusive_ptr<PropertyAction> rPropertyAction;
  typedef libport::intrusive_ptr<const PropertyAction> rConstPropertyAction;
  class PropertyWrite;
  typedef libport::intrusive_ptr<PropertyWrite> rPropertyWrite;
  typedef libport::intrusive_ptr<const PropertyWrite> rConstPropertyWrite;
  class Return;
  typedef libport::intrusive_ptr<Return> rReturn;
  typedef libport::intrusive_ptr<const Return> rConstReturn;
  class Routine;
  typedef libport::intrusive_ptr<Routine> rRoutine;
  typedef libport::intrusive_ptr<const Routine> rConstRoutine;
  class Scope;
  typedef libport::intrusive_ptr<Scope> rScope;
  typedef libport::intrusive_ptr<const Scope> rConstScope;
  class Stmt;
  typedef libport::intrusive_ptr<Stmt> rStmt;
  typedef libport::intrusive_ptr<const Stmt> rConstStmt;
  class String;
  typedef libport::intrusive_ptr<String> rString;
  typedef libport::intrusive_ptr<const String> rConstString;
  class Subscript;
  typedef libport::intrusive_ptr<Subscript> rSubscript;
  typedef libport::intrusive_ptr<const Subscript> rConstSubscript;
  class TaggedStmt;
  typedef libport::intrusive_ptr<TaggedStmt> rTaggedStmt;
  typedef libport::intrusive_ptr<const TaggedStmt> rConstTaggedStmt;
  class This;
  typedef libport::intrusive_ptr<This> rThis;
  typedef libport::intrusive_ptr<const This> rConstThis;
  class Throw;
  typedef libport::intrusive_ptr<Throw> rThrow;
  typedef libport::intrusive_ptr<const Throw> rConstThrow;
  class Try;
  typedef libport::intrusive_ptr<Try> rTry;
  typedef libport::intrusive_ptr<const Try> rConstTry;
  class Unary;
  typedef libport::intrusive_ptr<Unary> rUnary;
  typedef libport::intrusive_ptr<const Unary> rConstUnary;
  class Unscope;
  typedef libport::intrusive_ptr<Unscope> rUnscope;
  typedef libport::intrusive_ptr<const Unscope> rConstUnscope;
  class Watch;
  typedef libport::intrusive_ptr<Watch> rWatch;
  typedef libport::intrusive_ptr<const Watch> rConstWatch;
  class While;
  typedef libport::intrusive_ptr<While> rWhile;
  typedef libport::intrusive_ptr<const While> rConstWhile;
  class Write;
  typedef libport::intrusive_ptr<Write> rWrite;
  typedef libport::intrusive_ptr<const Write> rConstWrite;
  typedef TYPELIST_52(And, Assign, Assignment, At, Binding, Break, Call, CallMsg, Catch, Class, Continue, Declaration, Decrementation, Dictionary, Do, Emit, Event, Finally, Float, Foreach, If, Implicit, Incrementation, List, Local, LocalAssignment, LocalDeclaration, Match, MetaArgs, MetaCall, MetaExp, MetaId, MetaLValue, Nary, Noop, OpAssignment, Pipe, Property, PropertyWrite, Return, Routine, Scope, Stmt, String, Subscript, TaggedStmt, This, Throw, Try, Unscope, Watch, While) Nodes;


  // From visitor.hh
  template <template <typename> class Const>
  class GenVisitor;
  typedef GenVisitor<libport::constify_traits> ConstVisitor;
  typedef GenVisitor<libport::id_traits> Visitor;


// factory.hh.
class Factory;

// event-match.hh.
struct EventMatch;

typedef boost::unordered_map<libport::Symbol, ast::rExp> modifiers_type;
typedef std::pair<ast::rExp, ast::rExp> dictionary_elt_type;
typedef std::vector<dictionary_elt_type> dictionary_elts_type;



#define AST_NODES_SEQ\
  (And)\
  (Assign)\
  (Assignment)\
  (Ast)\
  (At)\
  (Binding)\
  (Break)\
  (Call)\
  (CallMsg)\
  (Catch)\
  (Class)\
  (Composite)\
  (Continue)\
  (Declaration)\
  (Decrementation)\
  (Dictionary)\
  (Do)\
  (Emit)\
  (Event)\
  (Exp)\
  (Finally)\
  (Flavored)\
  (Float)\
  (Foreach)\
  (If)\
  (Implicit)\
  (Incrementation)\
  (LValue)\
  (LValueArgs)\
  (List)\
  (Local)\
  (LocalAssignment)\
  (LocalDeclaration)\
  (LocalWrite)\
  (Match)\
  (MetaArgs)\
  (MetaCall)\
  (MetaExp)\
  (MetaId)\
  (MetaLValue)\
  (Nary)\
  (Noop)\
  (OpAssignment)\
  (Pipe)\
  (Property)\
  (PropertyAction)\
  (PropertyWrite)\
  (Return)\
  (Routine)\
  (Scope)\
  (Stmt)\
  (String)\
  (Subscript)\
  (TaggedStmt)\
  (This)\
  (Throw)\
  (Try)\
  (Unary)\
  (Unscope)\
  (Watch)\
  (While)\
  (Write)\

#define AST_FOR_EACH_NODE(Macro)\
    BOOST_PP_SEQ_FOR_EACH(Macro, , AST_NODES_SEQ)

} // namespace ast

#endif // !AST_FWD_HH
