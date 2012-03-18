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
 ** \file ast/serializer-generator.hh
 ** \brief Declaration of ast::Cloner.
 */

#ifndef AST_SERIALIZER_GENERATOR_HH
# define AST_SERIALIZER_GENERATOR_HH


# include <list>
# include <sstream>

# include <libport/foreach.hh>

# include <ast/ast.hh>
# include <ast/visitor.hh>
# include <ast/symbols-type.hh>

namespace ast
{
  /// Serialize an ast in Dot format
  class SerializerPrinter: public ast::ConstVisitor
  {
  public:
    typedef ast::ConstVisitor super_type;

    SerializerPrinter(std::ostream& output);
    virtual ~SerializerPrinter();
    virtual void operator()(const ast::Ast* n);

  protected:
    virtual void startDictionary(const char* name);
    virtual void stopDictionary();
    virtual void startList(const char* name);
    virtual void stopList();
    virtual void showElement(const char* name, const char* value);
    void separator(const char* name);
    template<typename T> void element(const char* name, const T&value)
    {
      std::stringstream s;
      s << value;
      showElement(name, s.str().c_str());
    }
  private:
    // Import overloaded virtual functions.
    using super_type::visit;
        CONST_VISITOR_VISIT_NODES(
                              (And)
                              (Assign)
                              (Assignment)
                              (At)
                              (Binding)
                              (Break)
                              (Call)
                              (CallMsg)
                              (Catch)
                              (Class)
                              (Continue)
                              (Declaration)
                              (Decrementation)
                              (Dictionary)
                              (Do)
                              (Emit)
                              (Event)
                              (Finally)
                              (Float)
                              (Foreach)
                              (If)
                              (Implicit)
                              (Incrementation)
                              (List)
                              (Local)
                              (LocalAssignment)
                              (LocalDeclaration)
                              (Match)
                              (MetaArgs)
                              (MetaCall)
                              (MetaExp)
                              (MetaId)
                              (MetaLValue)
                              (Nary)
                              (Noop)
                              (OpAssignment)
                              (Pipe)
                              (Property)
                              (PropertyWrite)
                              (Return)
                              (Routine)
                              (Scope)
                              (Stmt)
                              (String)
                              (Subscript)
                              (TaggedStmt)
                              (This)
                              (Throw)
                              (Try)
                              (Unscope)
                              (Watch)
                              (While)
                             )

    std::ostream& output_;
    unsigned id_;
    std::list<std::pair<unsigned, std::string> > ids_;
    bool root_;

    template<typename T>
    void recurse(const T& c);

    template<typename T>
    void recurse(const T* c);
    enum NType {
      LISTSTART,
      LISTCONT,
      DICTIONARY
    };
    std::vector<NType> stack_;
    void fill(size_t n);
  };
}

#endif
