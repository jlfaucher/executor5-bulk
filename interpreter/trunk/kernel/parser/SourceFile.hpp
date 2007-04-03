/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/* REXX Kernel                                                  SourceFile.hpp  */
/*                                                                            */
/* Translater Source Class Definitions                                        */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxSource
#define Included_RexxSource

#include "ListClass.hpp"
#include "QueueClass.hpp"
#include "StackClass.hpp"
#include "Token.hpp"
#include "Clause.hpp"

class RexxCompoundVariable;
class RexxStemVariable;
class RexxInstructionIf;
class RexxInstructionForward;
class RexxExpressionMessage;
class RexxInstruction;
class RexxInstructionDo;

#define new_instruction(name, type) this->sourceNewObject(sizeof(RexxInstruction##type), The##type##InstructionBehaviour, KEYWORD_##name)
#define new_variable_instruction(name, type, size) this->sourceNewObject(size, The##type##InstructionBehaviour, KEYWORD_##name)

#define requires_allowed 0x00000001    /* ::REQUIRES directives still valid */
#define _interpret       0x00000002    /* this is interpret translation     */
#define _install         0x00000004    /* installation stuff required       */
#define reclaimed        0x00000008    /* last clause only partially used   */
#define DBCS_scanning    0x00000010    /* need to scan in DBCS mode         */
#define reclaim_possible 0x00000020    /* can re-establish source connect   */
#define no_clause        0x00000040    /* last clause of a block reached    */

class RexxSource : public RexxInternalObject {
    friend class RexxMethod;
 public:
  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void       *operator new(size_t);
  inline void  operator delete(void *) {;}


  // potential line delimiters...we search for either of these, then
  // sort out the combinations.
  static char *lineDelimiters;

  RexxSource(RexxString *, RexxArray *);
  inline RexxSource(RESTORETYPE restoreType) { ; };
  void        initBuffered(RexxObject *, stringsize_t, stringsize_t);
  void        initFile();
  bool        reconnect();
  void        setReconnect();
  void        setBufferedSource(RexxBuffer *newSource);
  void        interpretLine(size_t);
  void        comment();
  void        DBCScomment();
  void        needVariable(RexxToken *);
  void        needVariableOrDotSymbol(RexxToken *);
  bool        terminator(int, RexxObject *);
  int         subKeyword(RexxToken *);
  int         keyword(RexxToken *);
  int         builtin(RexxToken *);
  int         resolveBuiltin(RexxString *);
  int         condition(RexxToken *);
  int         parseOption(RexxToken *);
  int         keyDirective(RexxToken *);
  int         subDirective(RexxToken *);
  int         precedence(RexxToken *);
  void        nextLine();
  void        position(size_t, size_t);
  void        live();
  void        liveGeneral();
  void        flatten(RexxEnvelope *);
  size_t      sourceSize();
  RexxString *get(size_t);
  void        nextClause();
  RexxToken  *sourceNextToken(RexxToken *);
  RexxString *traceBack(LocationInfo *, size_t, bool);
  RexxString *extract(LocationInfo *);
  RexxArray  *extractSource(LocationInfo *);
  void        startLocation(LocationInfo *);
  void        endLocation(LocationInfo *);
  bool        nextSpecial(stringchar_t, LocationInfo *);
  int         locateToken(RexxToken *);
  void        globalSetup();
  RexxString *packLiteral(stringsize_t, stringsize_t, int);
  RexxMethod *method();
  RexxMethod *interpretMethod(RexxDirectory *);
  RexxMethod *interpret(RexxString *, RexxDirectory *, size_t);
  void        checkDirective();
  RexxObject *toss(RexxObject *);
  void        cleanup();
  void        mergeRequired(RexxSource *);
  RexxMethod *resolveRoutine(RexxString *);
  RexxClass  *resolveClass(RexxString *, RexxActivation *);
  RexxClass  *resolveClass(RexxActivity *activity, RexxString *className);
  void        processInstall(RexxActivation *);
  RexxMethod *translate(RexxDirectory *);
  void        resolveDependencies();
  void        completeClass();
  void        directive();
  void        flushControl(RexxInstruction *);
  RexxMethod *translateBlock(RexxDirectory *);
  RexxInstruction *instruction(ProtectedObject &);
  RexxVariableBase *addVariable(RexxString *);
  RexxStemVariable  *addStem(RexxString *);
  RexxCompoundVariable *addCompound(RexxString *);
  void        expose(RexxString *);
  RexxString *commonString(RexxString *);
  RexxObject *addText(RexxToken *);
  void        addClause(RexxInstruction *);
  void        addLabel(RexxInstruction *, RexxString *);
  RexxInstruction *findLabel(RexxString *);
  void        setGuard();
  RexxArray  *getGuard();
  void        addBlock(void);
  RexxVariableBase *getRetriever(RexxString *);
  RexxObject *constantExpression();
  RexxObject *parenExpression(RexxToken *);
  RexxObject *expression(int, ProtectedObject &);
  RexxObject *subExpression(int, ProtectedObject &);
  size_t      argList(RexxToken *, int);
  RexxArray  *argArray(RexxToken *, int);
  RexxObject *function(RexxToken *, RexxToken *, int);
  RexxObject *collectionMessage(RexxToken *, RexxObject *, int);
  RexxToken  *getToken(int, int);
  RexxObject *message(RexxObject *, int, int);
  RexxObject *messageTerm();
  RexxObject *messageSubterm(int);
  RexxObject *subTerm(int);
  void        pushTerm(RexxObject *);
  void        popTerm(ProtectedObject &);
  void        popNTerms(size_t, ProtectedObject &);
  void        isExposeValid();
  RexxArray  *words(RexxString *);
  void        errorCleanup();
  void        reportError(wholenumber_t);
  void        reportError(wholenumber_t, RexxObject *);
  void        reportError(wholenumber_t, RexxObject *, RexxObject *);
  void        reportError(wholenumber_t, RexxObject *, RexxObject *, RexxObject *);
  void        reportErrorLine(wholenumber_t, RexxInstruction *);
  void        reportErrorPosition(wholenumber_t, RexxToken *);
  void        reportTokenError(wholenumber_t, RexxToken *);
  void        reportBlockError(RexxInstruction *);
  static RexxSource *classNewBuffered(RexxString *, RexxBuffer *);
  static RexxSource *classNewFile(RexxString *);
  RexxObject *sourceNewObject(size_t, RexxBehaviour *, int);
  void        parseTraceSetting(RexxString *, int *, int *);
  size_t      processVariableList(int);
  RexxObject *parseConditional(int *, int);
  bool        terminator(int, RexxToken *);
  bool        traceable(void);
  int         resolveKeyword(stringchar_t *name, size_t length, KWDTABLE *table, size_t tableSize);

  inline bool installed() { return (flags&_install) == 0; }
  inline void        install(RexxActivation *activation) { if (this->flags&_install) this->processInstall(activation); };
  inline void        addReference(RexxObject *reference) { this->calls->addLast(reference); }
  inline void        pushDo(RexxInstruction *localinstruction) { this->control->pushRexx((RexxObject *)localinstruction); }
  inline RexxInstruction *popDo() { return (RexxInstruction *)(this->control->pullRexx()); };
  inline RexxInstruction *topDo() { return (RexxInstruction *)(this->control->peek()); };
  inline void        setProgramName(RexxString *name) { OrefSet(this, this->programName, name); };
  inline RexxString *getProgramName() { return this->programName; }
  inline RexxDirectory *getMethods() { return this->methods; };
  inline void        pushOperator(RexxToken *operatorToken) { this->operators->pushRexx((RexxObject *)operatorToken); };
  inline RexxToken  *popOperator() { return (RexxToken *)(this->operators->pullRexx()); };
  inline RexxToken  *topOperator() { return (RexxToken *)(this->operators->peek()); };
  inline void        reclaimClause()  { this->flags |= reclaimed; };
  inline bool        atEnd(void) { return (!(this->flags&reclaimed) && (this->line_number > (this->line_count))); };
                                       /* handy defines to easy coding      */
  inline RexxToken * nextToken() { return this->clause->next(); }
  inline RexxToken * nextReal()  { return this->clause->nextRealToken(); }
  inline void        previousToken() { this->clause->previous(); }
  inline void        firstToken() { this->clause->firstToken(); }
  inline void        trimClause() { this->clause->trim(); }

  RexxInstruction *addressNew(ProtectedObject &);
  RexxInstruction *assignmentNew(RexxToken *, ProtectedObject &);
  RexxInstruction *callNew(ProtectedObject &);
  RexxInstruction *commandNew(ProtectedObject &);
  RexxInstruction *doNew(ProtectedObject &);
  void        RexxInstructionDoCreate(RexxInstructionDo *);
  RexxInstruction *dropNew(ProtectedObject &);
  RexxInstruction *elseNew(RexxToken *, ProtectedObject &);
  RexxInstruction *endNew(ProtectedObject &);
  RexxInstruction *endIfNew(RexxInstructionIf *, ProtectedObject &);
  RexxInstruction *exitNew(ProtectedObject &);
  RexxInstruction *exposeNew(ProtectedObject &);
  RexxInstruction *forwardNew(ProtectedObject &);
  void        RexxInstructionForwardCreate(RexxInstructionForward *);
  RexxInstruction *guardNew(ProtectedObject &);
  RexxInstruction *ifNew(int, ProtectedObject &);
  RexxInstruction *instructionNew(int, ProtectedObject &);
  RexxInstruction *interpretNew(ProtectedObject &);
  RexxInstruction *labelNew(ProtectedObject &);
  RexxInstruction *leaveNew(int, ProtectedObject &);
  RexxInstruction *messageNew(RexxExpressionMessage *, ProtectedObject &);
  RexxInstruction *messageAssignmentNew(RexxExpressionMessage *, RexxObject *, ProtectedObject &);
  RexxInstruction *nopNew(ProtectedObject &);
  RexxInstruction *numericNew(ProtectedObject &);
  RexxInstruction *optionsNew(ProtectedObject &);
  RexxInstruction *otherwiseNew(RexxToken *, ProtectedObject &);
  RexxInstruction *parseNew(int, ProtectedObject &);
  RexxInstruction *procedureNew(ProtectedObject &);
  RexxInstruction *queueNew(int, ProtectedObject &);
  RexxInstruction *raiseNew(ProtectedObject &);
  RexxInstruction *replyNew(ProtectedObject &);
  RexxInstruction *returnNew(ProtectedObject &);
  RexxInstruction *sayNew(ProtectedObject &);
  RexxInstruction *selectNew(ProtectedObject &);
  RexxInstruction *signalNew(ProtectedObject &);
  RexxInstruction *thenNew(RexxToken *, RexxInstructionIf *, ProtectedObject &);
  RexxInstruction *traceNew(ProtectedObject &);
  RexxInstruction *useNew(ProtectedObject &);
  void saveObject(RexxObject *object) { this->savelist->put(object, object); };
  void removeObj(RexxObject *object) { if (object != OREF_NULL) this->savelist->remove(object); };
  void setSecurityManager(RexxObject *manager) { OrefSet(this, this->securityManager, manager); }
  void packageDirective();
  void routineDirective();
  void requiresDirective();
  void methodDirective();
  void classDirective();
  RexxDirectory *getRoutinesApi();
  RexxDirectory *getMethodsApi();
  RexxDirectory *getClassesApi();
  inline RexxObject *getSecurityManager() { return securityManager; }
  inline static bool isSymbol(stringchar_t c)
  {
      return symbolCharacters[c] != 0;
  }

  inline static stringchar_t symbolCharacter(stringchar_t c)
  {
      return symbolCharacters[c];
  }

protected:

  uint32_t  flags;                     /* parsing flags                     */
  RexxArray  *sourceArray;             /* source lines for this code        */
  RexxString *programName;             /* name of the source program        */
  RexxObject *securityManager;         /* source execution time security    */
  stringchar_t * current;              /* current working line              */
  RexxClause *clause;                  /* current clause being created      */
  RexxBuffer *sourceBuffer;            /* contiguous buffered source        */
  RexxBuffer *sourceIndices;           /* line locations within buffer      */
  size_t current_length;               /* length of current line            */
  size_t line_count;                   /* size of source array              */
  size_t line_number;                  /* current line position             */
  size_t line_offset;                  /* current offset with in the line   */
  size_t interpret_adjust;             /* INTERPRET adjustment              */

                                       /* start of directives section       */

  RexxDirectory *routines;             /* routines found on directives      */
  RexxDirectory *public_routines;      /* PUBLIC routines directive routines*/
  RexxArray     *requires;             /* requires directives               */
  RexxArray     *packages;             /* PACKAGE  directives               */
  RexxArray     *classes;              /* classes found on directives       */
                                       /* all public installed classes      */
  RexxDirectory *installed_public_classes;
  RexxDirectory *installed_classes;    /* entire list of installed classes  */
  RexxDirectory *merged_public_classes;/* entire merged set of classes      */
                                       /* all public required routines      */
  RexxDirectory *merged_public_routines;
  RexxDirectory *methods;              /* methods found on directives       */
                                       /* start of global parsing section   */

  RexxObjectTable *savelist;           /* saved objects                     */
  RexxDirectory   *literals;           /* root of associated literal list   */
  RexxDirectory   *strings;            /* common pool of created strings    */
  RexxQueue       *control;            /* queue of control structures       */
  RexxQueue       *terms;              /* stack of expression terms         */
  RexxQueue       *subTerms;           /* stack for arguments lists, et al. */
  RexxQueue       *operators;          /* stack of expression terms         */
  RexxDirectory   *class_dependencies; /* dependencies between classes      */
  RexxArray       *active_class;       /* currently active ::CLASS directive*/

                                       /* start of block parsing section    */

  RexxInstruction *first;              /* first instruction of parse tree   */
  RexxInstruction *last;               /* last instruction of parse tree    */
  RexxInstruction *currentInstruction; /* current "protected" instruction   */
  RexxDirectory   *variables;          /* root of associated variable list  */
  RexxDirectory   *labels;             /* root of associated label list     */
  RexxObjectTable *guard_variables;    /* exposed variables in guard list   */
  RexxDirectory   *exposed_variables;  /* root of exposed variables list    */
  RexxList        *calls;              /* root of call list                 */
  size_t           currentstack;       /* current expression stack depth    */
  size_t           maxstack;           /* maximum stack depth               */
  size_t           variableindex;      /* current variable index slot       */

                                       // character type lookup table
  static stringchar_t RexxSource::symbolCharacters[];



/*********************************************************************/
/*                                                                   */
/* Table of directive instructions used for translation              */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE Directives[];
  static size_t   Directivescount;

/*********************************************************************/
/*                                                                   */
/* Table of keyword instructions used for translation                */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE KeywordInstructions[];     /* language keyword table     */
  static size_t   KeywordInstructionscount;  /* language keyword table size*/

/*********************************************************************/
/*                                                                   */
/* Table of instruction subkeywords used for translation             */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE SubKeywords[];         /* language keyword table     */
  static size_t   SubKeywordscount;      /* language keyword table size*/

/*********************************************************************/
/*                                                                   */
/* Table of built-in functions used for translation                  */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE BuiltinFunctions[];    /* built-in function table    */
  static size_t   BuiltinFunctionscount; /* builtin function table size*/

/*********************************************************************/
/*                                                                   */
/* Table of condition keywords used for translation                  */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE ConditionKeywords[];   /* condition option table     */
  static size_t   ConditionKeywordscount;/* condition option table size*/

/*********************************************************************/
/*                                                                   */
/* Table of parse options used for translation                       */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE ParseOptions[];        /* parse option table         */
  static size_t   ParseOptionscount;     /* parse option table size    */

/*********************************************************************/
/*                                                                   */
/* Table of directive subkeywords used for translation               */
/*                                                                   */
/*********************************************************************/

  static KWDTABLE SubDirectives[];/* language directive subkeywords    */
  static size_t   SubDirectivescount;    /* language directive         */
};
#endif
