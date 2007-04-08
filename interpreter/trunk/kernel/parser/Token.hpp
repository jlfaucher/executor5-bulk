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
/* REXX Kernel                                                  Token.hpp  */
/*                                                                            */
/* Primitive Translator Token Class Definitions                               */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* Change history:                                                            */
/*                                                                            */
/* ENG001 - feature 1112                                             02/08/28 */
/*          support USERID BIF                                                */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxToken
#define Included_RexxToken

#define   TERM_EOC     0x00000001u     /* terminate on end of clause        */
#define   TERM_RIGHT   0x00000002u     /* terminate on left paren           */
#define   TERM_SQRIGHT 0x00000004u     /* terminate on left square bracket  */
#define   TERM_TO      0x00000008u     /* terminate on TO keyword           */
#define   TERM_BY      0x00000010u     /* terminate on BY keyword           */
#define   TERM_FOR     0x00000020u     /* terminate on FOR keyword          */
#define   TERM_WHILE   0x00000040u     /* terminate on WHILE/UNTIL keywords */
#define   TERM_COMMA   0x00000080u     /* terminate on comma                */
#define   TERM_WITH    0x00000100u     /* terminate on WITH keyword         */
#define   TERM_THEN    0x00000200u     /* terminate on THEN keyword         */
#define   TERM_KEYWORD 0x10000000u     /* perform keyword terminator checks */
/* terminate on DO keywords          */
#define   TERM_CONTROL (TERM_KEYWORD | TERM_TO | TERM_BY | TERM_FOR | TERM_WHILE | TERM_EOC)
/* terminate on DO conditionals      */
#define   TERM_COND    (TERM_KEYWORD | TERM_WHILE | TERM_EOC)

/* token types */
enum
{
    TOKEN_NULL      = 1201,
    TOKEN_BLANK           ,
    TOKEN_SYMBOL          ,
    TOKEN_LITERAL         ,
    TOKEN_OPERATOR        ,
    TOKEN_EOC             ,
    TOKEN_COMMA           ,
    TOKEN_PREFIX          ,
    TOKEN_LEFT            ,
    TOKEN_RIGHT           ,
    TOKEN_POINT           ,
    TOKEN_COLON           ,
    TOKEN_TILDE           ,
    TOKEN_DTILDE          ,
    TOKEN_SQLEFT          ,
    TOKEN_SQRIGHT         ,
    TOKEN_DCOLON          ,
    TOKEN_CONTINUE        ,
};

/* token extended types - symbols */

enum
{
    SYMBOL_CONSTANT   = 1251,
    SYMBOL_VARIABLE         ,
    SYMBOL_NAME             ,
    SYMBOL_COMPOUND         ,
    SYMBOL_STEM             ,
    SYMBOL_DUMMY            ,
    SYMBOL_DOTSYMBOL        ,
    INTEGER_CONSTANT        ,
    LITERAL_HEX             ,
    LITERAL_BIN             ,
};

enum
{
    OPERATOR_PLUS                         = 1,
    OPERATOR_SUBTRACT                        ,
    OPERATOR_MULTIPLY                        ,
    OPERATOR_DIVIDE                          ,
    OPERATOR_INTDIV                          ,
    OPERATOR_REMAINDER                       ,
    OPERATOR_POWER                           ,
    OPERATOR_ABUTTAL                         ,
    OPERATOR_CONCATENATE                     ,
    OPERATOR_BLANK                           ,
    OPERATOR_EQUAL                           ,
    OPERATOR_BACKSLASH_EQUAL                 ,
    OPERATOR_GREATERTHAN                     ,
    OPERATOR_BACKSLASH_GREATERTHAN           ,
    OPERATOR_LESSTHAN                        ,
    OPERATOR_BACKSLASH_LESSTHAN              ,
    OPERATOR_GREATERTHAN_EQUAL               ,
    OPERATOR_LESSTHAN_EQUAL                  ,
    OPERATOR_STRICT_EQUAL                    ,
    OPERATOR_STRICT_BACKSLASH_EQUAL          ,
    OPERATOR_STRICT_GREATERTHAN              ,
    OPERATOR_STRICT_BACKSLASH_GREATERTHAN    ,
    OPERATOR_STRICT_LESSTHAN                 ,
    OPERATOR_STRICT_BACKSLASH_LESSTHAN       ,
    OPERATOR_STRICT_GREATERTHAN_EQUAL        ,
    OPERATOR_STRICT_LESSTHAN_EQUAL           ,
    OPERATOR_LESSTHAN_GREATERTHAN            ,
    OPERATOR_GREATERTHAN_LESSTHAN            ,
    OPERATOR_AND                             ,
    OPERATOR_OR                              ,
    OPERATOR_XOR                             ,
    OPERATOR_BACKSLASH                       ,

};

/* token extended types - operators */

/* token extended types - instruction keywords */
enum
{
    KEYWORD_ADDRESS          = 1,
    KEYWORD_ARG                 ,
    KEYWORD_CALL                ,
    KEYWORD_DO                  ,
    KEYWORD_DROP                ,
    KEYWORD_EXIT                ,
    KEYWORD_IF                  ,
    KEYWORD_INTERPRET           ,
    KEYWORD_ITERATE             ,
    KEYWORD_LEAVE               ,
    KEYWORD_METHOD              ,
    KEYWORD_NOP                 ,
    KEYWORD_NUMERIC             ,
    KEYWORD_OPTIONS             ,
    KEYWORD_PARSE               ,
    KEYWORD_PROCEDURE           ,
    KEYWORD_PULL                ,
    KEYWORD_PUSH                ,
    KEYWORD_QUEUE               ,
    KEYWORD_REPLY               ,
    KEYWORD_RETURN              ,
    KEYWORD_SAY                 ,
    KEYWORD_SELECT              ,
    KEYWORD_SIGNAL              ,
    KEYWORD_TRACE               ,
    KEYWORD_VAR                 ,
    KEYWORD_GUARD               ,
    KEYWORD_USE                 ,
    KEYWORD_INITPROC            ,
    KEYWORD_EXPOSE              ,
    KEYWORD_RAISE               ,
    KEYWORD_ELSE                ,
    KEYWORD_THEN                ,
    KEYWORD_END                 ,
    KEYWORD_OTHERWISE           ,
    KEYWORD_IFTHEN              ,
    KEYWORD_WHENTHEN            ,
    KEYWORD_WHEN                ,
    KEYWORD_ASSIGNMENT          ,
    KEYWORD_COMMAND             ,
    KEYWORD_MESSAGE             ,
    KEYWORD_LABEL               ,
    KEYWORD_ENDIF               ,
    KEYWORD_BLOCK               ,
    KEYWORD_FIRST               ,
    KEYWORD_LAST                ,
    KEYWORD_ENDELSE             ,
    KEYWORD_ENDTHEN             ,
    KEYWORD_ENDWHEN             ,
    KEYWORD_REQUIRES            ,
    KEYWORD_CLASS               ,
    KEYWORD_INSTRUCTION         ,
    KEYWORD_FORWARD             ,
    KEYWORD_PACKAGE             ,
    KEYWORD_LOOP                ,

};


/* token extended types - instruction option keywords */

enum
{
    SUBKEY_ARG         = 1,
    SUBKEY_BY             ,
    SUBKEY_DIGITS         ,
    SUBKEY_END            ,
    SUBKEY_ELSE           ,
    SUBKEY_ENGINEERING    ,
    SUBKEY_EXPOSE         ,
    SUBKEY_FOR            ,
    SUBKEY_FOREVER        ,
    SUBKEY_FORM           ,
    SUBKEY_FUZZ           ,
    SUBKEY_LINEIN         ,
    SUBKEY_LOWER          ,
    SUBKEY_CASELESS       ,
    SUBKEY_NAME           ,
    SUBKEY_NOVALUE        ,
    SUBKEY_OFF            ,
    SUBKEY_ON             ,
    SUBKEY_OTHERWISE      ,
    SUBKEY_OVER           ,
    SUBKEY_PULL           ,
    SUBKEY_SCIENTIFIC     ,
    SUBKEY_SOURCE         ,
    SUBKEY_THEN           ,
    SUBKEY_TO             ,
    SUBKEY_UNTIL          ,
    SUBKEY_UPPER          ,
    SUBKEY_VALUE          ,
    SUBKEY_VAR            ,
    SUBKEY_VERSION        ,
    SUBKEY_WHEN           ,
    SUBKEY_WHILE          ,
    SUBKEY_WITH           ,
    SUBKEY_DESCRIPTION    ,
    SUBKEY_ADDITIONAL     ,
    SUBKEY_RESULT         ,
    SUBKEY_ARRAY          ,
    SUBKEY_RETURN         ,
    SUBKEY_EXIT           ,
    SUBKEY_CONTINUE       ,
    SUBKEY_CLASS          ,
    SUBKEY_MESSAGE        ,
    SUBKEY_ARGUMENTS      ,
    SUBKEY_LABEL          ,
};


enum
{
/* token extended types - end of clause */
    CLAUSEEND_EOF        = 2301,
    CLAUSEEND_SEMICOLON  = 2302,
    CLAUSEEND_EOL        = 2303,
    CLAUSEEND_NULL       = 2304,

/* directive types */
    DIRECTIVE_REQUIRES = 2401,
    DIRECTIVE_CLASS    = 2402,
    DIRECTIVE_METHOD   = 2403,
    DIRECTIVE_ROUTINE  = 2404,
    DIRECTIVE_PACKAGE  = 2405,

/* directive sub-keywords */
    SUBDIRECTIVE_PUBLIC     = 2501,
    SUBDIRECTIVE_METACLASS        ,
    SUBDIRECTIVE_INHERIT          ,
    SUBDIRECTIVE_PRIVATE          ,
    SUBDIRECTIVE_GUARDED          ,
    SUBDIRECTIVE_CLASS            ,
    SUBDIRECTIVE_EXTERNAL         ,
    SUBDIRECTIVE_SUBCLASS         ,
    SUBDIRECTIVE_UNGUARDED        ,
    SUBDIRECTIVE_MIXINCLASS       ,
    SUBDIRECTIVE_ATTRIBUTE        ,
    SUBDIRECTIVE_PROTECTED        ,

/* condition keywords */
    CONDITION_ANY        = 2601   ,
    CONDITION_ERROR               ,
    CONDITION_FAILURE             ,
    CONDITION_HALT                ,
    CONDITION_NOMETHOD            ,
    CONDITION_NOSTRING            ,
    CONDITION_NOTREADY            ,
    CONDITION_NOVALUE             ,
    CONDITION_PROPAGATE           ,
    CONDITION_SYNTAX              ,
    CONDITION_USER                ,
    CONDITION_LOSTDIGITS          ,
};




/* builtin function codes */
enum
{
    NO_BUILTIN           ,
    BUILTIN_ABBREV       ,
    BUILTIN_ABS          ,
    BUILTIN_ADDRESS      ,
    BUILTIN_ARG          ,
    BUILTIN_B2X          ,
    BUILTIN_BITAND       ,
    BUILTIN_BITOR        ,
    BUILTIN_BITXOR       ,
    BUILTIN_C2D          ,
    BUILTIN_C2X          ,
    BUILTIN_CENTER       ,
    BUILTIN_CENTRE       ,
    BUILTIN_CHANGESTR    ,
    BUILTIN_CHARIN       ,
    BUILTIN_CHAROUT      ,
    BUILTIN_CHARS        ,
    BUILTIN_COMPARE      ,
    BUILTIN_CONDITION    ,
    BUILTIN_COPIES       ,
    BUILTIN_COUNTSTR     ,
    BUILTIN_D2C          ,
    BUILTIN_D2X          ,
    BUILTIN_DATATYPE     ,
    BUILTIN_DATE         ,
    BUILTIN_DBADJUST     ,
    BUILTIN_DBBRACKET    ,
    BUILTIN_DBCENTER     ,
    BUILTIN_DBLEFT       ,
    BUILTIN_DBRIGHT      ,
    BUILTIN_DBRLEFT      ,
    BUILTIN_DBRRIGHT     ,
    BUILTIN_DBTODBCS     ,
    BUILTIN_DBTOSBCS     ,
    BUILTIN_DBUNBRACKET  ,
    BUILTIN_DBVALIDATE   ,
    BUILTIN_DBWIDTH      ,
    BUILTIN_DELSTR       ,
    BUILTIN_DELWORD      ,
    BUILTIN_DIGITS       ,
    BUILTIN_ERRORTEXT    ,
    BUILTIN_FORM         ,
    BUILTIN_FORMAT       ,
    BUILTIN_FUZZ         ,
    BUILTIN_INSERT       ,
    BUILTIN_LASTPOS      ,
    BUILTIN_LEFT         ,
    BUILTIN_LENGTH       ,
    BUILTIN_LINEIN       ,
    BUILTIN_LINEOUT      ,
    BUILTIN_LINES        ,
    BUILTIN_LOWER        ,
    BUILTIN_MAX          ,
    BUILTIN_MIN          ,
    BUILTIN_OVERLAY      ,
    BUILTIN_POS          ,
    BUILTIN_QUEUED       ,
    BUILTIN_RANDOM       ,
    BUILTIN_REVERSE      ,
    BUILTIN_RIGHT        ,
    BUILTIN_RXFUNCADD    ,
    BUILTIN_RXFUNCDROP   ,
    BUILTIN_RXFUNCQUERY  ,
    BUILTIN_SIGN         ,
    BUILTIN_SOURCELINE   ,
    BUILTIN_SPACE        ,
    BUILTIN_STREAM       ,
    BUILTIN_STRIP        ,
    BUILTIN_SUBSTR       ,
    BUILTIN_SUBWORD      ,
    BUILTIN_SYMBOL       ,
    BUILTIN_TIME         ,
    BUILTIN_TRACE        ,
    BUILTIN_TRANSLATE    ,
    BUILTIN_TRUNC        ,
    BUILTIN_UPPER        ,
    BUILTIN_USERID       ,
    BUILTIN_VALUE        ,
    BUILTIN_VAR          ,
    BUILTIN_VERIFY       ,
    BUILTIN_WORD         ,
    BUILTIN_WORDINDEX    ,
    BUILTIN_WORDLENGTH   ,
    BUILTIN_WORDPOS      ,
    BUILTIN_WORDS        ,
    BUILTIN_X2B          ,
    BUILTIN_X2C          ,
    BUILTIN_X2D          ,
    BUILTIN_XRANGE       ,
    BUILTIN_QUEUEEXIT    ,
};

typedef struct _KWDTABLE
{             /* keyword table entries             */
    stringchar_t *  name;                       /* keyword name                      */
    size_t length;                     /* keyword name length               */
    int    keyword_code;               /* translated keyword code           */
} KWDTABLE;

typedef KWDTABLE *PKWDTABLE;           /* pointer to a keyword table entry  */
                                       /* size of keyword table             */
#define KWDTABLESIZE(table) (sizeof(table)/sizeof(KWDTABLE))
#define KWD(s, d)           (stringchar_t *)s, (sizeof(s) - 1), d,

class RexxToken : public RexxInternalObject
{
public:
    void      *operator new(size_t);
    inline void  operator delete(void *) {;}
    inline void *operator new(size_t, void *ptr) { return ptr; }
    inline void  operator delete(void *, void *) { ; }

    RexxToken(int, int, RexxString *, LocationInfo *);
    inline RexxToken(RESTORETYPE restoreType)
    {
        ;
    };
    void       live();
    void       liveGeneral();
    void       flatten(RexxEnvelope *);
    void       setStart(size_t, size_t);
    void       setEnd(size_t, size_t);

    inline bool       isVariable(void)
    {
        return(this->subclass == SYMBOL_VARIABLE || this->subclass == SYMBOL_STEM || this->subclass == SYMBOL_COMPOUND);
    };
    inline bool       isLiteral(void)
    {
        return(this->classId == TOKEN_LITERAL);
    };
    inline bool       isConstant(void)
    {
        return(this->classId == TOKEN_SYMBOL && this->subclass != SYMBOL_VARIABLE && this->subclass != SYMBOL_STEM && this->subclass != SYMBOL_COMPOUND);
    };
    inline bool       isSymbol(void)
    {
        return(this->classId == TOKEN_SYMBOL);
    };
    inline void       setNumeric(int localvalue)
    {
        this->numeric = localvalue;
    };
    inline void       getLocation(LocationInfo * locallocation)
    {
        *locallocation = this->location;
    }
    inline void       setLocation(LocationInfo * locallocation)
    {
        this->location = *locallocation;
    }

    LocationInfo location;               /* token source location             */
    RexxString *value;                   /* token string value                */
    int         classId;                 /* class of token                    */
    int         subclass;                /* specialized type of token         */
    int         numeric;                 /* even further specialization       */
};
#endif
