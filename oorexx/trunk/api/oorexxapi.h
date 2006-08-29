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
/* ooRexx Macros                                                              */
/*                                                                            */
/* Header file for ooRexx methods written in C.                               */
/*                                                                            */
/******************************************************************************/
#ifndef RexxNativeAPI_H_INCLUDED
#define RexxNativeAPI_H_INCLUDED

#include "oorexxerrors.h"

BEGIN_EXTERN_C()

/******************************************************************************/
/* Types (for general use)                                                    */
/******************************************************************************/
typedef char *CSTRING;        // pointer to zero-terminated string

typedef void *POINTER;        // pointer to storage area...used for unboxed RexxPointerObjects


// forward defininitions of the context structure types
struct RexxInstance_;
#ifdef __cplusplus
typedef RexxInstance_ RexxInstance;
#else
typedef const struct RexxInstance_ *RexxInstance;
#endif

struct RexxScriptContext_;
#ifdef __cplusplus
typedef RexxScriptContext_ RexxScriptContext;
#else
typedef const struct RexxScriptContext_ *RexxScriptContext;
#endif

struct RexxThreadContext_;
#ifdef __cplusplus
typedef RexxThreadContext_ RexxThreadContext;
#else
typedef const struct RexxThreadContext_ *RexxThreadContext;
#endif

struct RexxMethodContext_;
#ifdef __cplusplus
typedef RexxMethodContext_ RexxMethodContext;
#else
typedef const struct RexxMethodContext_ *RexxMethodContext;
#endif

struct RexxCallContext_;
#ifdef __cplusplus
typedef RexxCallContext_ RexxCallContext;
#else
typedef const struct RexxCallContext_ *RexxCallContext;
#endif

struct RexxExitContext_;
#ifdef __cplusplus
typedef RexxExitContext_ RexxExitContext;
#else
typedef const struct RexxExitContext_ *RexxExitContext;
#endif


typedef struct _ooRexxFunctionEntry
{
    int   style;                     // function call style
	int   reserved1;                 // reserved for future use
	char *name;                      // name of the function
    void *entryPoint;                // resolved function entry point
	int   reserved2;                 // reserved for future use
	int   reserved3;                 // reserved for future use
} ooRexxFunctionEntry;


#define REXX_FUNCTION(s, n, e)	{ s, 0, #n, (void *)e, 0, 0 },

#define REXX_TYPED_FUNCTION(n, e) REXX_FUNCTION(FUNCTION_TYPED_STYLE, n, e)
#define REXX_OBJECT_FUNCTION(n, e) REXX_FUNCTION(FUNCTION_OBJECT_STYLE, n, e)
#define REXX_CLASSIC_FUNCTION(n, e) REXX_FUNCTION(FUNCTION_CLASSIC_STYLE, n, e)
#define REXX_LAST_FUNCTION()        { 0, 0, NULL, (void *)NULL, 0, 0 }

#ifdef __cplusplus
#define REXX_OBJECT_FUNCTION_PROTOTYPE extern "C" RexxReturnCode RexxEntry name(RexxCallContext *, RexxNumber, RexxObjectPtr *, RexxObjectPtr *);
#else
#define REXX_OBJECT_FUNCTION_PROTOTYPE RexxReturnCode RexxEntry name(RexxCallContext *, RexxNumber, RexxObjectPtr *, RexxObjectPtr *);
#endif

#ifdef __cplusplus
#define REXX_CLASSIC_FUNCTION_PROTOTYPE extern "C" unsigned long RexxEntry name(char *, unsigned long, RxString *, char *, RxString *);
#else
#define REXX_CLASSIC_FUNCTION_PROTOTYPE unsigned long RexxEntry name(char *, unsigned long, RxString *, char *, RxString *);
#endif

typedef struct _ooRexxMethodEntry
{
    int   style;                     // function call style
	int   reserved1;                 // reserved for future use
	char *name;                      // name of the function
    void *entryPoint;                // resolved function entry point
	int   reserved2;                 // reserved for future use
	int   reserved3;                 // reserved for future use
} ooRexxMethodEntry;

#define METHOD_TYPED_STYLE 1

#define REXX_METHOD_ENTRY(n, e)	{ METHOD_TYPED_STYLE, 0, #n, (void *)e, 0, 0 },

#define REXX_METHOD(n, e) REXX_METHOD_ENTRY(n, e)
#define REXX_LAST_METHOD()  { 0, 0, NULL, (void *)NULL, 0, 0 }

#define REXX_PACKAGE_API_NO 20051030
#define NO_VERSION_YET NULL

#define STANDARD_PACKAGE_HEADER sizeof(ooRexxPackageEntry), REXX_PACKAGE_API_NO,

#define OOREXX_GET_PACKAGE(name) \
    BEGIN_EXTERN_C()\
	ooRexxPackageEntry *RexxEntry ooRexxGetPackage(void) { return &name##_module_entry; }\
    END_EXTERN_C()


typedef RexxReturnCode (RexxEntry *RexxPackageLoader(RexxThreadContext *);
typedef RexxReturnCode (RexxEntry *RexxPackageUnloader(RexxThreadContext *);

typedef struct _ooRexxPackageEntry
{
    int size;                      // size of the structure...help compatibility
	int apiVersion;                // version this was compiled with
    char *packageName;             // package identifier
    char  *packageVersion;         // package version #
    RexxPackageLoader loader;      // the package loader
    RexxPackageUnloader unloader;  // the package unloader
    struct _ooRexxFunctionEntry *functions; // functions contained in this package
    struct _ooRexxMethodEntry *methods;   // methods contained in this package
} ooRexxPackageEntry;

END_EXTERN_C()

#ifdef __cplusplus
 class RexxObject;                     /* Object Rexx root object           */
                                       /* reference to a REX object        */
 typedef RexxObject *RexxObjectPtr;
 #define NULLOBJECT ((RexxObjectPtr)NULL)
#else
 typedef void *RexxObjectPtr;          // reference to a REXX object
 #define NULLOBJECT ((RexxObjectPtr)0) // null object
#endif

typedef RexxObjectPtr RexxStringObject;   // Rexx string object
typedef RexxObjectPtr RexxBufferStringObject;   // Rexx string "under construction"
typedef RexxObjectPtr RexxArrayObject;    // Rexx array object
typedef RexxObjectPtr RexxPointerObject;  // Rexx pointer object
typedef RexxObjectPtr RexxIntegerObject;  // Rexx integer wrapper object
typedef RexxObjectPtr RexxClassObject;    // Rexx class object wrapper
typedef RexxObjectPtr RexxMethodObject;   // Rexx method object wrapper
typedef RexxObjectPtr RexxBufferObject;   // Rexx buffer object wrapper
typedef RexxObjectPtr RexxDirectoryObject;// Rexx directory object wrapper
typedef RexxObjectPtr RexxTableObject;    // Rexx table object wrapper
typedef RexxObjectPtr RexxSupplierObject; // Rexx supplier object wrapper
typedef RexxObjectPtr RexxStemObject;     // a Rexx stem object

// argument existence indicator
#define ARGUMENT_EXISTS   0x01
// the argument is a "special" virtual argument derived from context
#define SPECIAL_ARGUMENT  0x02

typedef struct
{
// union containing argument values for each of the passable/returnable
// types from a method/function call.  The arguments are pass/retrieved
// using the appropriate type names, which bypasses any endian issues of
// how different sized values might be stored with a union.
    union
    {
        RexxArrayObject       value_ARGLIST;
        CSTRING               value_NAME;
        RexxClassObject       value_SCOPE;
        POINTER               value_BUFFER;
        POINTER               value_CSELF;
        RexxObjectPtr         value_OSELF;
        RexxObjectPtr         value_RexxObjectPtr;
        int                   value_int;
        RexxNumber            value_RexxNumber;
        double                value_double;
        CSTRING               value_CSTRING;
        RexxClassObject       value_RexxClassObject;
        POINTER               value_POINTER;
        RexxStringObject      value_RexxStringObject;
        RexxUnsignedNumber    value_RexxUnsignedNumber;
        float                 value_float;
        int8_t                value_int8_t;
        int16_t               value_int16_t;
        int32_t               value_int32_t;
        int64_t               value_int64_t;
        uint8_t               value_uint8_t;
        uint16_t              value_uint16_t;
        uint32_t              value_uint32_t;
        uint64_t              value_uint64_t;
        size_t                value_size_t;
        intptr_t              value_intptr_t;
        uintptr_t             value_uintptr_t;
        RexxBoolean           value_RexxBoolean;
        RexxArrayObject       value_RexxArrayObject;
        RexxStemObject        value_RexxStemObject;

        // following just duplicate the non-optional variations...
        // it was difficult (if not impossible) to get the
        // preprocessor to generate a mapped symbol name.
        RexxObjectPtr         value_OPTIONAL_RexxObjectPtr;
        int                   value_OPTIONAL_int;
        RexxNumber            value_OPTIONAL_RexxNumber;
        double                value_OPTIONAL_double;
        CSTRING               value_OPTIONAL_CSTRING;
        RexxClassObject       value_OPTIONAL_RexxClassObject;
        POINTER               value_OPTIONAL_POINTER;
        RexxStringObject      value_OPTIONAL_RexxStringObject;
        RexxUnsignedNumber    value_OPTIONAL_RexxUnsignedNumber;
        float                 value_OPTIONAL_float;
        int8_t                value_OPTIONAL_int8_t;
        int16_t               value_OPTIONAL_int16_t;
        int32_t               value_OPTIONAL_int32_t;
        int64_t               value_OPTIONAL_int64_t;
        uint8_t               value_OPTIONAL_uint8_t;
        uint16_t              value_OPTIONAL_uint16_t;
        uint32_t              value_OPTIONAL_uint32_t;
        uint64_t              value_OPTIONAL_uint64_t;
        size_t                value_OPTIONAL_size_t;
        intptr_t              value_OPTIONAL_intptr_t;
        uintptr_t             value_OPTIONAL_uintptr_t;
        RexxBoolean           value_OPTIONAL_RexxBoolean;
        RexxArrayObject       value_OPTIONAL_RexxArrayObject;
        RexxStemObject        value_OPTIONAL_RexxStemObject;
    } value;

    uint16_t type;            // type of the value
    uint16_t flags;           // argument flags
} ValueDescriptor;



// The initial address environment, passed as a RexxStringPointer value.
#define INITIAL_ADDRESS_ENVIRONMENT "InitialAddress"
// Opaque user data, passed as a POINTER value.
#define APPLICATION_DATA            "ApplicationData"
// External function search path
#define EXTERNAL_CALL_PATH          "ExternalCallPath"
// list of lookup extensions to search for on external calls
#define EXTERNAL_CALL_EXTENSIONS    "ExternalCallPathExt"
// callback function for script external function calls.  Passed as a POINTER value.
#define SCRIPT_EXTERNAL_FUNCTION_CALLBACL "ScriptFunctionCallback"

typedef struct
{
    char *optionName;                // name of the option
    ValueDescriptor extra;           // a value appropriate to the option
} RexxOption;


typedef struct _ConditionData {
  RexxNumber code;                   // full condition code
  RexxNumber rc;                     // return code value
  RexxUnsignedNumber position;       // line number position
  RexxStringObject conditionName;    // name of the condition
  RexxStringObject message;          // fully filled in message
  RexxStringObject errortext;        // major error text
  RexxStringObject program;          // program name
  RexxStringObject description;      // program name
  RexxArrayObject  additional;       // additional information
} ConditionData;

#define INSTANCE_INTERFACE_VERSION 100

typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier

    void        (RexxEntry *Terminate)(RexxInstance  *);
    RexxBoolean (RexxEntry *AttachThread)(RexxInstance *, RexxThreadContext **);
    RexxBoolean (RexxEntry *CreateScriptContext)(RexxInstance *, RexxScriptContext **, , RexxOption *);
    void        (RexxEntry *Halt)(RexxInstance *);
    void        (RexxEntry *SetTrace)(RexxInstance *, RexxBoolean);
} RexxInstanceInterface;

#define THREAD_INTERFACE_VERSION 100

BEGIN_EXTERN_C()

typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier

    void             (RexxEntry *DeleteContext)(RexxScriptContext *);
    void             (RexxEntry *AddVariableBinding)(RexxStringPointer, ValueDescriptor *);
    void             (RexxEntry *RemoveVariableBinding)(RexxStringPointer);
    RexxObjectPtr    (RexxEntry *GetVariableValue)(RexxStringPointer);
} RexxScriptInterface;


typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier

    void             (RexxEntry *DetachThread)(RexxThreadContext *);
    void             (RexxEntry *HaltThread)(RexxThreadContext *);
    void             (RexxEntry *SetThreadTrace)(RexxThreadContext *, RexxBoolean);
    RexxObjectPtr    (RexxEntry *RequestGlobalReference)(RexxThreadContext *, RexxObjectPtr);
    void             (RexxEntry *ReleaseGlobalReference)(RexxThreadContext *, RexxObjectPtr);
    void             (RexxEntry *ReleaseLocalReference)(RexxThreadContext *, RexxObjectPtr);

    RexxObjectPtr  (RexxEntry *SendMessage)(RexxThreadContext *, RexxObjectPtr, RexxStringPointer, RexxArrayObject);
    RexxObjectPtr  (RexxEntry *SendMessage0)(RexxThreadContext *, RexxObjectPtr, RexxStringPointer);
    RexxObjectPtr  (RexxEntry *SendMessage1)(RexxThreadContext *, RexxObjectPtr, RexxStringPointer, RexxObjectPtr);
    RexxObjectPtr  (RexxEntry *SendMessage2)(RexxThreadContext *, RexxObjectPtr, RexxStringPointer, RexxObjectPtr, RexxObjectPtr);

    RexxDirectoryObject (RexxEntry *GetLocalEnvironment)(RexxThreadContext *);
    RexxDirectoryObject (RexxEntry *GetGlobalEnvironment)(RexxThreadContext *);

    RexxBoolean      (RexxEntry *IsSameType)(RexxThreadContext *, RexxObjectPtr, RexxObjectPtr);
    RexxBoolean      (RexxEntry *IsInstanceOf)(RexxThreadContext *, RexxObjectPtr, RexxClassObject);
    RexxClassObject  (RexxEntry *FindClass)(RexxThreadContext *, RexxStringPointer);
    RexxClassObject  (RexxEntry *FindClassFromMethod)(RexxThreadContext *, RexxMethodObject, RexxStringPointer);
    RexxBoolean      (RexxEntry *HasMethod)(RexxThreadContext *, RexxObjectPtr, RexxStringPointer);

    RexxMethodObject (RexxEntry *NewMethod)(RexxThreadContext *, RexxStringPointer, RexxUnsignedNumber);
    RexxDirectoryObject (RexxEntry *GetMethodRoutines)(RexxThreadContext *, RexxMethodObject);
    RexxDirectoryObject (RexxEntry *GetMethodClasses)(RexxThreadContext *, RexxMethodObject);
    RexxDirectoryObject (RexxEntry *GetMethodMethods)(RexxThreadContext *, RexxMethodObject);
    RexxObjectPtr    (RexxEntry *CallMethod)(RexxThreadContext *, RexxMethodObject, RexxArrayObject);
    RexxBufferObject (RexxEntry *SaveMethod)(RexxThreadContext *, RexxMethodObject);
    RexxMethodObject (RexxEntry *LoadMethod)(RexxThreadContext *, RexxStringPointer d, RexxStringLength l);

    RexxObjectPtr    (RexxEntry *NewObject)(RexxThreadContext *);
    RexxObjectPtr    (RexxEntry *NumberToObject)(RexxThreadContext *, RexxNumber);
    RexxObjectPtr    (RexxEntry *UintptrToObject)(RexxThreadContext *, uintptr_t);
    RexxObjectPtr    (RexxEntry *ValueToObject)(RexxThreadContext *, ValueDescriptor *);
    RexxBoolean      (RexxEntry *ObjectToValue)(RexxThreadContext *, RexxObjectPtr, ValueDescriptor *);
    RexxObjectPtr    (RexxEntry *UnsignedNumberToObject)(RexxThreadContext *, RexxUnsignedNumber);
    RexxBoolean      (RexxEntry *ObjectToNumber)(RexxThreadContext *, RexxObjectPtr, RexxNumber *);
    RexxBoolean      (RexxEntry *ObjectToUnsignedNumber)(RexxThreadContext *, RexxObjectPtr, RexxUnsignedNumber *);
    RexxObjectPtr    (RexxEntry *Int64ToObject)(RexxThreadContext *, int64_t);
    RexxObjectPtr    (RexxEntry *UnsignedInt64ToObject)(RexxThreadContext *, uint64_t);
    RexxBoolean      (RexxEntry *ObjectToInt64)(RexxThreadContext *, RexxObjectPtr, int64_t *);
    RexxBoolean      (RexxEntry *ObjectToUnsignedInt64)(RexxThreadContext *, RexxObjectPtr, uint64_t *);
    RexxBoolean      (RexxEntry *ObjectToUintptr)(RexxThreadContext *, RexxObjectPtr, uintptr_t *);
    RexxObjectPtr    (RexxEntry *DoubleToObject)(RexxThreadContext *, double);
    RexxObjectPtr    (RexxEntry *DoubleToObjectWithPrecision)(RexxThreadContext *, double, RexxNumber precision);
    RexxBoolean      (RexxEntry *ObjectToDouble)(RexxThreadContext *, RexxObjectPtr, double *);

    RexxStringObject  (RexxEntry *ObjectToString)(RexxThreadContext *, RexxObjectPtr);
    RexxStringPointer (RexxEntry *ObjectToStringValue)(RexxThreadContext *, RexxObjectPtr);
    RexxStringLength  (RexxEntry *StringGet)(RexxThreadContext *, RexxStringObject, RexxStringLength, RexxStringPointer, RexxStringLength);
    RexxStringLength  (RexxEntry *StringLength)(RexxThreadContext *, RexxStringObject);
    RexxStringPointer (RexxEntry *StringData)(RexxThreadContext *, RexxStringObject);
    RexxStringObject  (RexxEntry *NewString)(RexxThreadContext *, RexxStringPointer, RexxStringLength);
    RexxStringObject  (RexxEntry *NewStringFromAsciiz)(RexxThreadContext *, RexxStringPointer);
    RexxStringObject  (RexxEntry *StringUpper)(RexxThreadContext *, RexxStringObject);
    RexxStringObject  (RexxEntry *StringLower)(RexxThreadContext *, RexxStringObject);
    RexxBoolean       (RexxEntry *IsString)(RexxThreadContext *, RexxObjectPtr);

    RexxBufferStringObject  (RexxEntry *NewBufferString)(RexxThreadContext *, RexxStringLength);
    RexxStringLength  (RexxEntry *BufferStringLength)(RexxThreadContext *, RexxBufferStringObject);
    RexxStringPointer (RexxEntry *BufferStringData)(RexxThreadContext *, RexxBufferStringObject);
    RexxStringObject  (RexxEntry *FinishBufferString)(RexxThreadContext *, RexxBufferStringObject, RexxStringLength);

    void             (RexxEntry *TablePut)(RexxThreadContext *, RexxTableObject, RexxObjectPtr, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *TableGet)(RexxThreadContext *, RexxTableObject, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *TableRemove)(RexxThreadContext *, RexxTableObject, RexxObjectPtr);
    RexxTableObject  (RexxEntry *NewTable)(RexxThreadContext *);
    RexxBoolean      (RexxEntry *IsTable)(RexxThreadContext *, RexxObjectPtr);

    void             (RexxEntry *DirectoryPut)(RexxThreadContext *, RexxDirectoryObject, RexxObjectPtr, RexxStringPointer);
    RexxObjectPtr    (RexxEntry *DirectoryAt)(RexxThreadContext *, RexxDirectoryObject, RexxStringPointer);
    RexxObjectPtr    (RexxEntry *DirectoryRemove)(RexxThreadContext *, RexxDirectoryObject, RexxStringPointer);
    RexxDirectoryObject  (RexxEntry *NewDirectory)(RexxThreadContext *);
    RexxBoolean      (RexxEntry *IsDirectory)(RexxThreadContext *, RexxObjectPtr);

    RexxObjectPtr   (RexxEntry *ArrayAt)(RexxThreadContext *, RexxArrayObject, RexxUnsignedNumber);
    RexxBoolean     (RexxEntry *ArrayHasIndex)(RexxThreadContext *, RexxArrayObject, RexxUnsignedNumber);
    void            (RexxEntry *ArrayPut)(RexxThreadContext *, RexxArrayObject, RexxObjectPtr, RexxUnsignedNumber);
    RexxUnsignedNumber (RexxEntry *ArraySize)(RexxThreadContext *, RexxArrayObject);
    RexxNumber      (RexxEntry *ArrayDimension)(RexxThreadContext *, RexxArrayObject);
    RexxArrayObject (RexxEntry *NewArray)(RexxThreadContext *, RexxUnsignedNumber);
    RexxArrayObject (RexxEntry *ArrayOfOne)(RexxThreadContext *, RexxObjectPtr);
    RexxArrayObject (RexxEntry *ArrayOfTwo)(RexxThreadContext *, RexxObjectPtr, RexxObjectPtr);
    RexxBoolean     (RexxEntry *IsArray)(RexxThreadContext *, RexxObjectPtr);

    RexxStringPointer (RexxEntry *BufferData)(RexxThreadContext *, RexxBufferObject);
    RexxNumber        (RexxEntry *BufferLength)(RexxThreadContext *, RexxBufferObject);
    RexxBufferObject  (RexxEntry *NewBuffer)(RexxThreadContext *, RexxUnsignedNumber);
    RexxBoolean       (RexxEntry *IsBuffer)(RexxThreadContext *, RexxObjectPtr);

    RexxNumber        (RexxEntry *IntegerValue)(RexxThreadContext *, RexxIntegerObject);
    RexxIntegerObject (RexxEntry *NewInteger)(RexxThreadContext *, RexxNumber);
    RexxBoolean       (RexxEntry *IsInteger)(RexxThreadContext *, RexxObjectPtr);

    POINTER           (RexxEntry *PointerValue)(RexxThreadContext *, RexxPointerObject);
    RexxPointerObject (RexxEntry *NewPointer)(RexxThreadContext *, POINTER);
    RexxBoolean       (RexxEntry *IsPointer)(RexxThreadContext *, RexxObjectPtr);

    RexxObjectPtr    (RexxEntry *SupplierValue)(RexxThreadContext *, RexxSupplierObject);
    RexxObjectPtr    (RexxEntry *SupplierIndex)(RexxThreadContext *, RexxSupplierObject);
    RexxBoolean      (RexxEntry *SupplierAvailable)(RexxThreadContext *, RexxSupplierObject);
    void             (RexxEntry *SupplierNext)(RexxThreadContext *, RexxSupplierObject);
    RexxSupplierObject (RexxEntry *NewSupplier)(RexxThreadContext *, RexxArrayObject values, RexxArrayObject names);

    void             (RexxEntry *SetStemElement)(RexxThreadContext *, RexxStemObject, RexxStringPointer, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *GetStemElement)(RexxThreadContext *, RexxStemObject, RexxStringPointer);
    void             (RexxEntry *DropStemElement)(RexxThreadContext *, RexxStemObject, RexxStringPointer);
    void             (RexxEntry *SetStemArrayElement)(RexxThreadContext *, RexxStemObject, RexxUnsignedNumber, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *GetStemArrayElement)(RexxThreadContext *, RexxStemObject, RexxUnsignedNumber);
    void             (RexxEntry *DropStemArrayElement)(RexxThreadContext *, RexxStemObject, RexxUnsignedNumber);
    RexxSupplierObject (RexxEntry *GetAllStemElements)(RexxThreadContext *, RexxStemObject);
    RexxObjectPtr    (RexxEntry *GetStemValue)(RexxThreadContext *, RexxStemObject);
    RexxBoolean      (RexxEntry *IsStem)(RexxThreadContext *, RexxObjectPtr);

    void             (RexxEntry *RaiseException)(RexxThreadContext *, RexxUnsignedNumber);
    void             (RexxEntry *RaiseException1)(RexxThreadContext *, RexxUnsignedNumber, RexxObjectPtr);
    void             (RexxEntry *RaiseException2)(RexxThreadContext *, RexxUnsignedNumber, RexxObjectPtr, RexxObjectPtr);
    void             (RexxEntry *RaiseExceptionArray)(RexxThreadContext *, RexxUnsignedNumber, RexxArrayObject);
    void             (RexxEntry *RaiseCondition)(RexxThreadContext *, RexxStringPointer, RexxStringPointer, RexxArrayObject, RexxObjectPtr);
    RexxBoolean      (RexxEntry *CheckCondition)(RexxThreadContext *);
    RexxDirectoryObject (RexxEntry *GetConditionInfo)(RexxThreadContext *);
    void             (RexxEntry *DecodeConditionInfo)(RexxThreadContext *, RexxDirectoryObject, ConditionData *);
    void             (RexxEntry *ClearCondition)(RexxThreadContext *);

    void             (RexxEntry *AddScript)(RexxScriptContext *, RexxStringPointer, RexxStringPointer, RexxStringLength);
    void             (RexxEntry *RunScript)(RexxScriptContext *, RexxStringPointer, RexxStringPointer, RexxStringLength, RexxNumber, ValueDescriptor *);
    void             (RexxEntry *CallScript)(RexxScriptContext *, RexxStringPointer, RexxNumber, ValueDescriptor *);

    RexxObjectPtr    RexxNil;
    RexxObjectPtr    RexxTrue;
    RexxObjectPtr    RexxFalse;
    RexxStringObject RexxNullString;

} RexxThreadInterface;


#define METHOD_INTERFACE_VERSION 100

typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier

    RexxArrayObject  (RexxEntry *GetArguments)(RexxMethodContext *);
    RexxObjectPtr    (RexxEntry *GetArgument)(RexxMethodContext *, RexxUnsignedNumber);
    RexxStringPointer    (RexxEntry *GetMessageName)(RexxMethodContext *);
    RexxMethodObject (RexxEntry *GetMethod)(RexxMethodContext *);
    RexxObjectPtr    (RexxEntry *GetSelf)(RexxMethodContext *);
    RexxObjectPtr    (RexxEntry *GetSuper)(RexxMethodContext *);
    void             (RexxEntry *SetObjectVariable)(RexxMethodContext *, RexxStringPointer, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *GetObjectVariable)(RexxMethodContext *, RexxStringPointer);
    void             (RexxEntry *DropObjectVariable)(RexxMethodContext *, RexxStringPointer);
    RexxObjectPtr    (RexxEntry *SendSuperMessage)(RexxMethodContext *, RexxStringPointer, RexxArrayObject);
    RexxObjectPtr    (RexxEntry *SendOverrideMessage)(RexxMethodContext *, RexxStringPointer, RexxClassObject, RexxArrayObject);
    void             (RexxEntry *SetGuardOn)(RexxMethodContext *);
    void             (RexxEntry *SetGuardOff)(RexxMethodContext *);
    RexxClassObject  (RexxEntry *FindContextClass)(RexxMethodContext *, RexxStringPointer);
} MethodContextInterface;

#define CALL_INTERFACE_VERSION 100

typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier

    RexxArrayObject  (RexxEntry *GetArguments)(RexxCallContext *);
    RexxObjectPtr    (RexxEntry *GetArgument)(RexxCallContext *, RexxUnsignedNumber);
    void             (RexxEntry *SetContextVariable)(RexxCallContext *, RexxStringPointer, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *GetContextVariable)(RexxCallContext *, RexxStringPointer);
    void             (RexxEntry *DropContextVariable)(RexxCallContext *, RexxStringPointer);
    RexxSupplierObject (RexxEntry *GetAllContextVariables)(RexxCallContext *);
    RexxStemObject   (RexxEntry *ResolveStemVariable)(RexxCallContext *, RexxObjectPtr);
    void             (RexxEntry *InvalidRoutine)(RexxCallContext *);
    RexxNumber       (RexxEntry *GetContextDigits)(RexxCallContext *);
    RexxNumber       (RexxEntry *GetContextFuzz)(RexxCallContext *);
    RexxBoolean      (RexxEntry *GetContextForm)(RexxCallContext *);
} CallContextInterface;

#define EXIT_INTERFACE_VERSION 100

typedef struct
{
    RexxNumber interfaceVersion;    // The interface version identifier
    void             (RexxEntry *SetContextVariable)(RexxExitContext *, RexxStringPointer, RexxObjectPtr);
    RexxObjectPtr    (RexxEntry *GetContextVariable)(RexxExitContext *, RexxStringPointer);
    void             (RexxEntry *DropContextVariable)(RexxExitContext *, RexxStringPointer);
    RexxSupplierObject (RexxEntry *GetAllContextVariables)(RexxExitContext *);
} ExitContextInterface;

END_EXTERN_C()

struct RexxInstance_
{
    RexxInstanceInterface *functions;   // the interface function vector
    void *applicationData;              // creator defined data pointer
#ifdef __cplusplus
    void        Terminate()
    {
        functions->Terminate(this);
    }
    RexxBoolean AttachThread(RexxThreadContext **tc)
    {
        return functions->AttachThread(this, tc);
    }
    RexxBoolean CreateScriptContext(RexxScriptContext **sc)
    {
        return functions->CreateScriptContext(this, sc);
    }
    void Halt()
    {
        functions->Halt(this);
    }
    void SetTrace(RexxBoolean s)
    {
        functions->SetTrace(this, s);
    }
#endif
};


struct RexxScriptContext_
    RexxInstance *instance;             // the owning instance
    RexxScriptInterface *functions;     // the interface function vector
#ifdef __cplusplus
#endif
};


struct RexxThreadContext_
{
    RexxInstance *instance;             // the owning instance
    RexxThreadInterface *functions;     // the interface function vector
#ifdef __cplusplus
    POINTER GetApplicationData()
    {
        return instance->applicationData;
    }
    void DetachThread()
    {
        functions->DetachThread(this);
    }
    void HaltThread()
    {
        functions->HaltThread(this);
    }
    void SetThreadTrace(RexxBoolean s)
    {
        functions->SetThreadTrace(this, s);
    }
    RexxObjectPtr RequestGlobalReference(RexxObjectPtr o)
    {
        return functions->RequestGlobalReference(this, o);
    }
    void      ReleaseGlobalReference(RexxObjectPtr o)
    {
        functions->ReleaseGlobalReference(this, o);
    }
    void      ReleaseLocalReference(RexxObjectPtr o)
    {
        functions->ReleaseLocalReference(this, o);
    }

    RexxObjectPtr  SendMessage(RexxObjectPtr o, RexxStringPointer msg, RexxArrayObject arr)
    {
        return functions->SendMessage(this, o, msg, arr);
    }
    RexxObjectPtr  SendMessage0(RexxObjectPtr o, RexxStringPointer msg)
    {
        return functions->SendMessage0(this, o, msg);
    }
    RexxObjectPtr  SendMessage1(RexxObjectPtr o, RexxStringPointer msg, RexxObjectPtr a1)
    {
        return functions->SendMessage1(this, o, msg, a1);
    }
    RexxObjectPtr  SendMessage2(RexxObjectPtr o, RexxStringPointer msg, RexxObjectPtr a1, RexxObjectPtr a2)
    {
        return functions->SendMessage2(this, o, msg, a1, a2);
    }

    RexxDirectoryObject GetLocalEnvironment()
    {
        return functions->GetLocalEnvironment(this);
    }
    RexxDirectoryObject GetGlobalEnvironment()
    {
        return functions->GetGlobalEnvironment(this);
    }
    RexxBoolean      IsSameType(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return functions->IsSameType(this, o1, o2);
    }
    RexxBoolean      IsInstanceOf(RexxObjectPtr o, RexxClassObject co)
    {
        return functions->IsInstanceOf(this, o, co);
    }
    RexxClassObject  FindClass(RexxStringPointer s)
    {
        return functions->FindClass(this, s);
    }
    RexxClassObject  FindClassFromMethod(RexxMethodObject m, RexxStringPointer n)
    {
        return functions->FindClassFromMethod(this, m, n);
    }
    RexxBoolean      HasMethod(RexxObjectPtr o, RexxStringPointer m)
    {
        return functions->HasMethod(this, o, m);
    }

    RexxMethodObject NewMethod(RexxStringPointer s, RexxUnsignedNumber n)
    {
        return functions->NewMethod(this, s, n);
    }
    RexxDirectoryObject GetMethodRoutines(RexxMethodObject m)
    {
        return functions->GetMethodRoutines(this, m);
    }
    RexxDirectoryObject GetMethodClasses(RexxMethodObject m)
    {
        return functions->GetMethodClasses(this, m);
    }
    RexxDirectoryObject GetMethodMethods(RexxMethodObject m)
    {
        return functions->GetMethodMethods(this, m);
    }
    RexxObjectPtr CallMethod(RexxMethodObject m, RexxArrayObject a)
    {
        return functions->CallMethod(this, m, a);
    }
    RexxBufferObject SaveMethod(RexxMethodObject m)
    {
        return functions->SaveMethod(this, m);
    }
    RexxMethodObject LoadMethod(RexxStringPointer d, RexxStringLength l)
    {
        return functions->LoadMethod(this, d, l);
    }

    RexxObjectPtr    NewObject()
    {
        return functions->NewObject(this);
    }
    RexxObjectPtr    NumberToObject(RexxNumber n)
    {
        return functions->NumberToObject(this, n);
    }
    RexxObjectPtr    UintptrToObject(uintptr_t n)
    {
        return functions->UintptrToObject(this, n);
    }
    RexxObjectPtr    UnsignedNumberToObject(RexxUnsignedNumber n)
    {
        return functions->UnsignedNumberToObject(this, n);
    }
    RexxBoolean      ObjectToNumber(RexxObjectPtr o, RexxNumber *n)
    {
        return functions->ObjectToNumber(this, o, n);
    }
    RexxBoolean      ObjectToUnsignedNumber(RexxObjectPtr o, RexxUnsignedNumber *n)
    {
        return functions->ObjectToUnsignedNumber(this, o, n);
    }
    RexxObjectPtr    Int64ToObject(int64_t i)
    {
        return functions->Int64ToObject(this, i);
    }
    RexxObjectPtr    UnsignedInt64ToObject(uint64_t u)
    {
        return functions->UnsignedInt64ToObject(this, u);
    }
    RexxBoolean      ObjectToInt64(RexxObjectPtr o, int64_t *i)
    {
        return functions->ObjectToInt64(this, o, i);
    }
    RexxBoolean      ObjectToUnsignedInt64(RexxObjectPtr o, uint64_t *u)
    {
        return functions->ObjectToUnsignedInt64(this, o, u);
    }
    RexxBoolean      ObjectToUintptr(RexxObjectPtr o, uintptr_t *n)
    {
        return functions->ObjectToUintptr(this, o, n);
    }
    RexxObjectPtr    DoubleToObject(double d)
    {
        return functions->DoubleToObject(this, d);
    }
    RexxObjectPtr    DoubleToObjectWithPrecision(double d, RexxNumber precision)
    {
        return functions->DoubleToObjectWithPrecision(this, d, precision);
    }
    RexxBoolean      ObjectToDouble(RexxObjectPtr o, double *d)
    {
        return functions->ObjectToDouble(this, o, d);
    }
    RexxObjectPtr    ValueToObject(ValueDescriptor *v)
    {
        return functions->ValueToObject(this, v);
    }
    RexxBoolean      ObjectToValue(RexxObjectPtr o, ValueDescriptor *v)
    {
        return functions->ObjectToValue(this, o, v);
    }
    RexxStringObject  ObjectToString(RexxObjectPtr o)
    {
        return functions->ObjectToString(this, o);
    }
    RexxStringPointer ObjectToStringValue(RexxObjectPtr o)
    {
        return functions->ObjectToStringValue(this, o);
    }
    RexxStringLength  StringGet(RexxStringObject o, RexxStringLength len1, RexxStringPointer s, RexxStringLength len2)
    {
        return functions->StringGet(this, o, len1, s, len2);
    }
    RexxStringLength  StringLength(RexxStringObject o)
    {
        return functions->StringLength(this, o);
    }
    RexxStringPointer StringData(RexxStringObject o)
    {
        return functions->StringData(this, o);
    }
    RexxStringObject  NewString(RexxStringPointer s, RexxStringLength len)
    {
        return functions->NewString(this, s, len);
    }
    RexxStringObject  NewStringFromAsciiz(RexxStringPointer s)
    {
        return functions->NewStringFromAsciiz(this, s);
    }
    RexxStringObject  StringUpper(RexxStringObject s)
    {
        return functions->StringUpper(this, s);
    }
    RexxStringObject  StringLower(RexxStringObject s)
    {
        return functions->StringLower(this, s);
    }
    RexxBoolean       IsString(RexxObjectPtr o)
    {
        return functions->IsString(this, o);
    }

    RexxBufferStringObject  NewBufferString(RexxStringLength len)
    {
        return functions->NewBufferString(this, len);
    }

    RexxStringLength  BufferStringLength(RexxBufferStringObject o)
    {
        return functions->BufferStringLength(this, o);
    }

    RexxStringPointer BufferStringData(RexxBufferStringObject o)
    {
        return functions->BufferStringData(this, o);
    }

    RexxStringObject FinishBufferString(RexxBufferStringObject o, RexxStringLength l)
    {
        return functions->FinishBufferString(this, o, l);
    }

    void             TablePut(RexxTableObject to, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        functions->TablePut(this, to, o1, o2);
    }
    RexxObjectPtr    TableGet(RexxTableObject to, RexxObjectPtr o)
    {

        return functions->TableGet(this, to, o);
    }
    RexxObjectPtr    TableRemove(RexxTableObject to, RexxObjectPtr o)
    {
        return functions->TableRemove(this, to, o);
    }
    RexxTableObject  NewTable()
    {
        return functions->NewTable(this);
    }
    RexxBoolean      IsTable(RexxObjectPtr o)
    {
        return functions->IsTable(this, o);
    }

    void             DirectoryPut(RexxDirectoryObject diro, RexxObjectPtr o, RexxStringPointer s)
    {
        functions->DirectoryPut(this, diro, o, s);
    }
    RexxObjectPtr    DirectoryAt(RexxDirectoryObject to, RexxStringPointer s)
    {
        return functions->DirectoryAt(this, to, s);
    }
    RexxObjectPtr    DirectoryRemove(RexxDirectoryObject to, RexxStringPointer s)
    {
        return functions->DirectoryRemove(this, to, s);
    }
    RexxDirectoryObject  NewDirectory()
    {
        return functions->NewDirectory(this);
    }
    RexxBoolean      IsDirectory(RexxObjectPtr o)
    {
        return functions->IsDirectory(this, o);
    }

    RexxObjectPtr   ArrayAt(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return functions->ArrayAt(this, ao, n);
    }
    RexxBoolean     ArrayHasIndex(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return functions->ArrayHasIndex(this, ao, n);
    }
    void            ArrayPut(RexxArrayObject ao, RexxObjectPtr o, RexxUnsignedNumber n)
    {
        functions->ArrayPut(this, ao, o, n);
    }
    RexxUnsignedNumber ArraySize(RexxArrayObject ao)
    {
        return functions->ArraySize(this, ao);
    }
    RexxNumber ArrayDimension(RexxArrayObject ao)
    {
        return functions->ArrayDimension(this, ao);
    }
    RexxArrayObject    NewArray(RexxUnsignedNumber n)
    {
        return functions->NewArray(this, n);
    }
    RexxArrayObject    ArrayOfOne(RexxObjectPtr o)
    {
        return functions->ArrayOfOne(this, o);
    }
    RexxArrayObject    ArrayOfTwo(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return functions->ArrayOfTwo(this, o1, o2);
    }
    RexxBoolean     IsArray(RexxObjectPtr o)
    {
        return functions->IsArray(this, o);
    }

    RexxStringPointer    BufferData(RexxBufferObject bo)
    {
        return functions->BufferData(this, bo);
    }
    RexxNumber  BufferLength(RexxBufferObject bo)
    {
        return functions->BufferLength(this, bo);
    }
    RexxBufferObject NewBuffer(RexxNumber n)
    {
        return functions->NewBuffer(this, n);
    }
    RexxBoolean      IsBuffer(RexxObjectPtr o)
    {
        return functions->IsBuffer(this, o);
    }

    RexxNumber   IntegerValue(RexxIntegerObject io)
    {
        return functions->IntegerValue(this, io);
    }
    RexxIntegerObject NewInteger(RexxNumber n)
    {
        return functions->NewInteger(this, n);
    }
    RexxBoolean       IsInteger(RexxObjectPtr o)
    {
        return functions->IsInteger(this, o);
    }

    POINTER           PointerValue(RexxPointerObject po)
    {
        return functions->PointerValue(this, po);
    }
    RexxPointerObject NewPointer(POINTER po)
    {
        return functions->NewPointer(this, po);
    }
    RexxBoolean       IsPointer(RexxObjectPtr o)
    {
        return functions->IsPointer(this, o);
    }

    RexxObjectPtr    SupplierValue(RexxSupplierObject so)
    {
        return functions->SupplierValue(this, so);
    }
    RexxObjectPtr    SupplierIndex(RexxSupplierObject so)
    {
        return functions->SupplierIndex(this, so);
    }
    RexxBoolean      SupplierAvailable(RexxSupplierObject so)
    {
        return functions->SupplierAvailable(this, so);
    }
    void             SupplierNext(RexxSupplierObject so)
    {
        functions->SupplierNext(this, so);
    }
    RexxSupplierObject NewSupplier(RexxArrayObject values, RexxArrayObject names)
    {
        return functions->NewSupplier(this, values, names);
    }

    void             SetStemElement(RexxStemObject so, RexxStringPointer s, RexxObjectPtr o)
    {
        functions->SetStemElement(this, so, s, o);
    }
    RexxObjectPtr    GetStemElement(RexxStemObject so, RexxStringPointer s)
    {
        return functions->GetStemElement(this, so, s);
    }
    void DropStemElement(RexxStemObject so, RexxStringPointer s)
    {
        functions->DropStemElement(this, so, s);
    }
    void             SetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n, RexxObjectPtr o)
    {
        functions->SetStemArrayElement(this, so, n, o);
    }
    RexxObjectPtr    GetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        return functions->GetStemArrayElement(this, so, n);
    }
    void DropStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        functions->DropStemArrayElement(this, so, n);
    }
    RexxSupplierObject GetAllStemElements(RexxStemObject so)
    {
        return functions->GetAllStemElements(this, so);
    }
    RexxObjectPtr    GetStemValue(RexxStemObject so)
    {
        return functions->GetStemValue(this, so);
    }
    RexxBoolean      IsStem(RexxObjectPtr o)
    {
        return functions->IsStem(this, o);
    }

    void RaiseException(RexxUnsignedNumber n)
    {
        functions->RaiseException(this, n);
    }
    void RaiseException1(RexxUnsignedNumber n, RexxObjectPtr o)
    {
        functions->RaiseException1(this, n, o);
    }
    void RaiseException2(RexxUnsignedNumber n, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        functions->RaiseException2(this, n, o1, o2);
    }
    void RaiseExceptionArray(RexxUnsignedNumber n, RexxArrayObject ao)
    {
        functions->RaiseExceptionArray(this, n, ao);
    }
    void RaiseCondition(RexxStringPointer s1, RexxStringPointer s2, RexxArrayObject ao, RexxObjectPtr o)
    {
        functions->RaiseCondition(this, s1, s2, ao, o);
    }
    RexxBoolean      CheckCondition()
    {
        return functions->CheckCondition(this);
    }
    RexxDirectoryObject GetConditionInfo()
    {
        return functions->GetConditionInfo(this);
    }
    void             DecodeConditionInfo(RexxDirectoryObject diro, ConditionData *c)
    {
        functions->DecodeConditionInfo(this, diro, c);
    }
    void             ClearCondition()
    {
        functions->ClearCondition(this);
    }

    RexxObjectPtr    NilObject()
    {
        return functions->RexxNil;
    }
    RexxObjectPtr    TrueObject()
    {
        return functions->RexxTrue;
    }
    RexxObjectPtr    FalseObject()
    {
        return functions->RexxFalse;
    }
    RexxStringObject NullString()
    {
        return functions->RexxNullString;
    }
#endif
};

struct RexxMethodContext_
{
    RexxThreadContext *threadContext;   // the interpreter instance state
    MethodContextInterface *functions;  // functions available in a method context
    ValueDescriptor *arguments;         // the argument descriptor

#ifdef __cplusplus
    POINTER GetApplicationData()
    {
        return threadContext->GetApplicationData();
    }
    RexxObjectPtr RequestGlobalReference(RexxObjectPtr o)
    {
        return threadContext->RequestGlobalReference(o);
    }
    void      ReleaseGlobalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseGlobalReference(o);
    }
    void      ReleaseLocalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseLocalReference(o);
    }

    RexxObjectPtr  SendMessage(RexxObjectPtr o, RexxStringPointer s, RexxArrayObject ao)
    {
        return threadContext->SendMessage(o, s, ao);
    }
    RexxObjectPtr  SendMessage0(RexxObjectPtr o, RexxStringPointer s)
    {
        return threadContext->SendMessage0(o, s);
    }
    RexxObjectPtr  SendMessage1(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1)
    {
        return threadContext->SendMessage1(o, s, a1);
    }
    RexxObjectPtr  SendMessage2(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1, RexxObjectPtr a2)
    {
        return threadContext->SendMessage2(o, s, a1, a2);
    }

    RexxDirectoryObject GetLocalEnvironment()
    {
        return threadContext->GetLocalEnvironment();
    }
    RexxDirectoryObject GetGlobalEnvironment()
    {
        return threadContext->GetGlobalEnvironment();
    }

    RexxBoolean      IsSameType(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->IsSameType(o1, o2);
    }
    RexxBoolean      IsInstanceOf(RexxObjectPtr o, RexxClassObject co)
    {
        return threadContext->IsInstanceOf(o, co);
    }
    RexxClassObject  FindClass(RexxStringPointer s)
    {
        return threadContext->FindClass(s);
    }
    RexxClassObject  FindClassFromMethod(RexxMethodObject m, RexxStringPointer n)
    {
        return threadContext->FindClassFromMethod(m, n);
    }
    RexxBoolean      HasMethod(RexxObjectPtr o, RexxStringPointer m)
    {
        return threadContext->HasMethod(o, m);
    }

    RexxMethodObject NewMethod(RexxStringPointer s, RexxUnsignedNumber n)
    {
        return threadContext->NewMethod(s, n);
    }
    RexxDirectoryObject GetMethodRoutines(RexxMethodObject m)
    {
        return threadContext->GetMethodRoutines(m);
    }
    RexxDirectoryObject GetMethodClasses(RexxMethodObject m)
    {
        return threadContext->GetMethodClasses(m);
    }
    RexxDirectoryObject GetMethodMethods(RexxMethodObject m)
    {
        return threadContext->GetMethodMethods(m);
    }
    RexxObjectPtr CallMethod(RexxMethodObject m, RexxArrayObject a)
    {
        return threadContext->CallMethod(m, a);
    }
    RexxBufferObject SaveMethod(RexxMethodObject m)
    {
        return threadContext->SaveMethod(m);
    }
    RexxMethodObject LoadMethod(RexxStringPointer d, RexxStringLength l)
    {
        return threadContext->LoadMethod(d, l);
    }

    RexxObjectPtr    NewObject()
    {
        return threadContext->NewObject();
    }
    RexxObjectPtr    NumberToObject(RexxNumber n)
    {
        return threadContext->NumberToObject(n);
    }
    RexxObjectPtr    UintptrToObject(uintptr_t n)
    {
        return threadContext->UintptrToObject(n);
    }
    RexxObjectPtr    ValueToObject(ValueDescriptor *v)
    {
        return threadContext->ValueToObject(v);
    }
    RexxObjectPtr    UnsignedNumberToObject(RexxUnsignedNumber u)
    {
        return threadContext->UnsignedNumberToObject(u);
    }
    RexxBoolean      ObjectToNumber(RexxObjectPtr o, RexxNumber *n)
    {
        return threadContext->ObjectToNumber(o, n);
    }
    RexxBoolean      ObjectToUnsignedNumber(RexxObjectPtr o, RexxUnsignedNumber *n)
    {
        return threadContext->ObjectToUnsignedNumber(o, n);
    }
    RexxObjectPtr    Int64ToObject(int64_t i)
    {
        return threadContext->Int64ToObject(i);
    }
    RexxObjectPtr    UnsignedInt64ToObject(uint64_t u)
    {
        return threadContext->UnsignedInt64ToObject(u);
    }
    RexxBoolean      ObjectToInt64(RexxObjectPtr o, int64_t *i)
    {
        return threadContext->ObjectToInt64(o, i);
    }
    RexxBoolean      ObjectToUnsignedInt64(RexxObjectPtr o, uint64_t *u)
    {
        return threadContext->ObjectToUnsignedInt64(o, u);
    }
    RexxBoolean      ObjectToUintptr(RexxObjectPtr o, uintptr_t *n)
    {
        return threadContext->ObjectToUintptr(o, n);
    }
    RexxObjectPtr    DoubleToObject(double d)
    {
        return threadContext->DoubleToObject(d);
    }
    RexxObjectPtr    DoubleToObjectWithPrecision(double d, RexxNumber precision)
    {
        return threadContext->DoubleToObjectWithPrecision(d, precision);
    }
    RexxBoolean      ObjectToDouble(RexxObjectPtr o, double *d)
    {
        return threadContext->ObjectToDouble(o, d);
    }

    RexxStringObject  ObjectToString(RexxObjectPtr o)
    {
        return threadContext->ObjectToString(o);
    }
    RexxStringPointer ObjectToStringValue(RexxObjectPtr o)
    {
        return threadContext->ObjectToStringValue(o);
    }
    RexxStringLength  StringGet(RexxStringObject o, RexxStringLength n1, RexxStringPointer s, RexxStringLength n2)
    {
        return threadContext->StringGet(o, n1, s, n2);
    }
    RexxStringLength  StringLength(RexxStringObject o)
    {
        return threadContext->StringLength(o);
    }
    RexxStringPointer StringData(RexxStringObject o)
    {
        return threadContext->StringData(o);
    }
    RexxStringObject  NewString(RexxStringPointer s, RexxStringLength len)
    {
        return threadContext->NewString(s, len);
    }
    RexxStringObject  NewStringFromAsciiz(RexxStringPointer s)
    {
        return threadContext->NewStringFromAsciiz(s);
    }
    RexxStringObject  StringUpper(RexxStringObject s)
    {
        return threadContext->StringUpper(s);
    }
    RexxStringObject  StringLower(RexxStringObject s)
    {
        return threadContext->StringLower(s);
    }
    RexxBoolean       IsString(RexxObjectPtr o)
    {
        return threadContext->IsString(o);
    }

    RexxBufferStringObject  NewBufferString(RexxStringLength len)
    {
        return threadContext->NewBufferString(len);
    }

    RexxStringLength  BufferStringLength(RexxBufferStringObject o)
    {
        return threadContext->BufferStringLength(o);
    }

    RexxStringPointer BufferStringData(RexxBufferStringObject o)
    {
        return threadContext->BufferStringData(o);
    }

    RexxStringObject FinishBufferString(RexxBufferStringObject o, RexxStringLength l)
    {
        return threadContext->FinishBufferString(o, l);
    }

    void             TablePut(RexxTableObject to, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->TablePut(to, o1, o2);
    }
    RexxObjectPtr    TableGet(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableGet(to, o);
    }
    RexxObjectPtr    TableRemove(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableRemove(to, o);
    }
    RexxTableObject  NewTable()
    {
        return threadContext->NewTable();
    }
    RexxBoolean      IsTable(RexxObjectPtr o)
    {
        return threadContext->IsTable(o);
    }

    void             DirectoryPut(RexxDirectoryObject diro, RexxObjectPtr o, RexxStringPointer s)
    {
        threadContext->DirectoryPut(diro, o, s);
    }
    RexxObjectPtr    DirectoryAt(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryAt(to, s);
    }
    RexxObjectPtr    DirectoryRemove(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryRemove(to, s);
    }
    RexxDirectoryObject  NewDirectory()
    {
        return threadContext->NewDirectory();
    }
    RexxBoolean      IsDirectory(RexxObjectPtr o)
    {
        return threadContext->IsDirectory(o);
    }

    RexxObjectPtr   ArrayAt(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayAt(ao, n);
    }
    RexxBoolean     ArrayHasIndex(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayHasIndex(ao, n);
    }
    void            ArrayPut(RexxArrayObject ao, RexxObjectPtr o, RexxUnsignedNumber n)
    {
        threadContext->ArrayPut(ao, o, n);
    }
    RexxUnsignedNumber ArraySize(RexxArrayObject ao)
    {
        return threadContext->ArraySize(ao);
    }
    RexxNumber ArrayDimension(RexxArrayObject ao)
    {
        return threadContext->ArrayDimension(ao);
    }
    RexxArrayObject    NewArray(RexxUnsignedNumber n)
    {
        return threadContext->NewArray(n);
    }
    RexxArrayObject    ArrayOfOne(RexxObjectPtr o)
    {
        return threadContext->ArrayOfOne(o);
    }
    RexxArrayObject    ArrayOfTwo(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->ArrayOfTwo(o1, o2);
    }
    RexxBoolean     IsArray(RexxObjectPtr o)
    {
        return threadContext->IsArray(o);
    }

    RexxStringPointer    BufferData(RexxBufferObject bo)
    {
        return threadContext->BufferData(bo);
    }
    RexxNumber  BufferLength(RexxBufferObject bo)
    {
        return threadContext->BufferLength(bo);
    }
    RexxBufferObject NewBuffer(RexxNumber n)
    {
        return threadContext->NewBuffer(n);
    }
    RexxBoolean      IsBuffer(RexxObjectPtr o)
    {
        return threadContext->IsBuffer(o);
    }

    RexxNumber   IntegerValue(RexxIntegerObject io)
    {
        return threadContext->IntegerValue(io);
    }
    RexxIntegerObject NewInteger(RexxNumber n)
    {
        return threadContext->NewInteger(n);
    }
    RexxBoolean       IsInteger(RexxObjectPtr o)
    {
        return threadContext->IsInteger(o);
    }

    POINTER           PointerValue(RexxPointerObject po)
    {
        return threadContext->PointerValue(po);
    }
    RexxPointerObject NewPointer(POINTER p)
    {
        return threadContext->NewPointer(p);
    }
    RexxBoolean       IsPointer(RexxObjectPtr o)
    {
        return threadContext->IsPointer(o);
    }

    RexxObjectPtr    SupplierValue(RexxSupplierObject so)
    {
        return threadContext->SupplierValue(so);
    }
    RexxObjectPtr    SupplierIndex(RexxSupplierObject so)
    {
        return threadContext->SupplierIndex(so);
    }
    RexxBoolean      SupplierAvailable(RexxSupplierObject so)
    {
        return threadContext->SupplierAvailable(so);
    }
    void             SupplierNext(RexxSupplierObject so)
    {
        threadContext->SupplierNext(so);
    }
    RexxSupplierObject NewSupplier(RexxArrayObject values, RexxArrayObject names)
    {
        return threadContext->NewSupplier(values, names);
    }

    void             SetStemElement(RexxStemObject so, RexxStringPointer s, RexxObjectPtr o)
    {
        threadContext->SetStemElement(so, s, o);
    }
    RexxObjectPtr    GetStemElement(RexxStemObject so, RexxStringPointer s)
    {
        return threadContext->GetStemElement(so, s);
    }
    void DropStemElement(RexxStemObject so, RexxStringPointer s)
    {
        threadContext->DropStemElement(so, s);
    }
    void             SetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->SetStemArrayElement(so, n, o);
    }
    RexxObjectPtr    GetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        return threadContext->GetStemArrayElement(so, n);
    }
    void DropStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        threadContext->DropStemArrayElement(so, n);
    }
    RexxSupplierObject GetAllStemElements(RexxStemObject so)
    {
        return threadContext->GetAllStemElements(so);
    }
    RexxObjectPtr    GetStemValue(RexxStemObject so)
    {
        return threadContext->GetStemValue(so);
    }
    RexxBoolean      IsStem(RexxObjectPtr o)
    {
        return threadContext->IsStem(o);
    }

    void RaiseException(RexxUnsignedNumber n)
    {
        threadContext->RaiseException(n);
    }
    void RaiseException1(RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->RaiseException1(n, o);
    }
    void RaiseException2(RexxUnsignedNumber n, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->RaiseException2(n, o1, o2);
    }
    void RaiseExceptionArray(RexxUnsignedNumber n, RexxArrayObject ao)
    {
        threadContext->RaiseExceptionArray(n, ao);
    }
    void RaiseCondition(RexxStringPointer s1, RexxStringPointer s2, RexxArrayObject ao, RexxObjectPtr o)
    {
        threadContext->RaiseCondition(s1, s2, ao, o);
    }
    RexxBoolean      CheckCondition()
    {
        return threadContext->CheckCondition();
    }
    RexxDirectoryObject GetConditionInfo()
    {
        return threadContext->GetConditionInfo();
    }
    void             DecodeConditionInfo(RexxDirectoryObject diro, ConditionData *c)
    {
        threadContext->DecodeConditionInfo(diro, c);
    }
    void             ClearCondition()
    {
        threadContext->ClearCondition();
    }

    RexxObjectPtr    NilObject()
    {
        return threadContext->NilObject();
    }
    RexxObjectPtr    TrueObject()
    {
        return threadContext->TrueObject();
    }
    RexxObjectPtr    FalseObject()
    {
        return threadContext->FalseObject();
    }
    RexxStringObject NullString()
    {
        return threadContext->NullString();
    }

    RexxArrayObject  GetArguments()
    {
        return functions->GetArguments(this);
    }
    RexxObjectPtr    GetArgument(RexxUnsignedNumber n)
    {
        return functions->GetArgument(this, n);
    }
    RexxStringPointer GetMessageName()
    {
        return functions->GetMessageName(this);
    }
    RexxMethodObject GetMethod()
    {
        return functions->GetMethod(this);
    }
    RexxObjectPtr    GetSelf()
    {
        return functions->GetSelf(this);
    }
    RexxObjectPtr    GetSuper()
    {
        return functions->GetSuper(this);
    }
    void             SetObjectVariable(RexxStringPointer s, RexxObjectPtr o)
    {
        functions->SetObjectVariable(this, s, o);
    }
    RexxObjectPtr    GetObjectVariable(RexxStringPointer s)
    {
        return functions->GetObjectVariable(this, s);
    }
    void             DropObjectVariable(RexxStringPointer s)
    {
        functions->DropObjectVariable(this, s);
    }
    RexxObjectPtr    SendSuperMessage(RexxStringPointer s, RexxArrayObject o)
    {
        return functions->SendSuperMessage(this, s, o);
    }
    RexxObjectPtr    SendOverrideMessage(RexxStringPointer s, RexxClassObject c, RexxArrayObject o)
    {
        return functions->SendOverrideMessage(this, s, c, o);
    }
    void             SetGuardOn()
    {
        functions->SetGuardOn(this);
    }
    void             SetGuardOff()
    {
        functions->SetGuardOff(this);
    }
    RexxClassObject FindContextClass(RexxStringPointer n)
    {
        return functions->FindContextClass(this, n);
    }
#endif
};

struct RexxCallContext_
{
    RexxThreadContext *threadContext;   // the interpreter instance state
    CallContextInterface *functions;    // functions available in a method context
    ValueDescriptor *arguments;         // the argument descriptor

#ifdef __cplusplus
    POINTER GetApplicationData()
    {
        return threadContext->GetApplicationData();
    }
    RexxObjectPtr RequestGlobalReference(RexxObjectPtr o)
    {
        return threadContext->RequestGlobalReference(o);
    }
    void      ReleaseGlobalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseGlobalReference(o);
    }
    void      ReleaseLocalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseLocalReference(o);
    }
    RexxObjectPtr  SendMessage(RexxObjectPtr o, RexxStringPointer s, RexxArrayObject ao)
    {
        return threadContext->SendMessage(o, s, ao);
    }
    RexxObjectPtr  SendMessage0(RexxObjectPtr o, RexxStringPointer s)
    {
        return threadContext->SendMessage0(o, s);
    }
    RexxObjectPtr  SendMessage1(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1)
    {
        return threadContext->SendMessage1(o, s, a1);
    }
    RexxObjectPtr  SendMessage2(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1, RexxObjectPtr a2)
    {
        return threadContext->SendMessage2(o, s, a1, a2);
    }

    RexxDirectoryObject GetLocalEnvironment()
    {
        return threadContext->GetLocalEnvironment();
    }
    RexxDirectoryObject GetGlobalEnvironment()
    {
        return threadContext->GetGlobalEnvironment();
    }

    RexxBoolean      IsSameType(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->IsSameType(o1, o2);
    }
    RexxBoolean      IsInstanceOf(RexxObjectPtr o, RexxClassObject co)
    {
        return threadContext->IsInstanceOf(o, co);
    }
    RexxClassObject  FindClass(RexxStringPointer s)
    {
        return threadContext->FindClass(s);
    }
    RexxClassObject  FindClassFromMethod(RexxMethodObject m, RexxStringPointer n)
    {
        return threadContext->FindClassFromMethod(m, n);
    }
    RexxBoolean      HasMethod(RexxObjectPtr o, RexxStringPointer m)
    {
        return threadContext->HasMethod(o, m);
    }

    RexxMethodObject NewMethod(RexxStringPointer s, RexxUnsignedNumber n)
    {
        return threadContext->NewMethod(s, n);
    }
    RexxDirectoryObject GetMethodRoutines(RexxMethodObject m)
    {
        return threadContext->GetMethodRoutines(m);
    }
    RexxDirectoryObject GetMethodClasses(RexxMethodObject m)
    {
        return threadContext->GetMethodClasses(m);
    }
    RexxDirectoryObject GetMethodMethods(RexxMethodObject m)
    {
        return threadContext->GetMethodMethods(m);
    }
    RexxObjectPtr CallMethod(RexxMethodObject m, RexxArrayObject a)
    {
        return threadContext->CallMethod(m, a);
    }
    RexxBufferObject SaveMethod(RexxMethodObject m)
    {
        return threadContext->SaveMethod(m);
    }
    RexxMethodObject LoadMethod(RexxStringPointer d, RexxStringLength l)
    {
        return threadContext->LoadMethod(d, l);
    }

    RexxObjectPtr    NewObject()
    {
        return threadContext->NewObject();
    }
    RexxObjectPtr    NumberToObject(RexxNumber n)
    {
        return threadContext->NumberToObject(n);
    }
    RexxObjectPtr    UintptrToObject(uintptr_t n)
    {
        return threadContext->UintptrToObject(n);
    }
    RexxObjectPtr    ValueToObject(ValueDescriptor *v)
    {
        return threadContext->ValueToObject(v);
    }
    RexxObjectPtr    UnsignedNumberToObject(RexxUnsignedNumber u)
    {
        return threadContext->UnsignedNumberToObject(u);
    }
    RexxBoolean      ObjectToNumber(RexxObjectPtr o, RexxNumber *n)
    {
        return threadContext->ObjectToNumber(o, n);
    }
    RexxBoolean      ObjectToUnsignedNumber(RexxObjectPtr o, RexxUnsignedNumber *n)
    {
        return threadContext->ObjectToUnsignedNumber(o, n);
    }
    RexxObjectPtr    Int64ToObject(int64_t i)
    {
        return threadContext->Int64ToObject(i);
    }
    RexxObjectPtr    UnsignedInt64ToObject(uint64_t u)
    {
        return threadContext->UnsignedInt64ToObject(u);
    }
    RexxBoolean      ObjectToInt64(RexxObjectPtr o, int64_t *i)
    {
        return threadContext->ObjectToInt64(o, i);
    }
    RexxBoolean      ObjectToUnsignedInt64(RexxObjectPtr o, uint64_t *u)
    {
        return threadContext->ObjectToUnsignedInt64(o, u);
    }
    RexxBoolean      ObjectToUintptr(RexxObjectPtr o, uintptr_t *n)
    {
        return threadContext->ObjectToUintptr(o, n);
    }
    RexxObjectPtr    DoubleToObject(double d)
    {
        return threadContext->DoubleToObject(d);
    }
    RexxObjectPtr    DoubleToObjectWithPrecision(double d, RexxNumber precision)
    {
        return threadContext->DoubleToObjectWithPrecision(d, precision);
    }
    RexxBoolean      ObjectToDouble(RexxObjectPtr o, double *d)
    {
        return threadContext->ObjectToDouble(o, d);
    }

    RexxStringObject  ObjectToString(RexxObjectPtr o)
    {
        return threadContext->ObjectToString(o);
    }
    RexxStringPointer ObjectToStringValue(RexxObjectPtr o)
    {
        return threadContext->ObjectToStringValue(o);
    }
    RexxStringLength  StringGet(RexxStringObject o, RexxStringLength n1, RexxStringPointer s, RexxStringLength n2)
    {
        return threadContext->StringGet(o, n1, s, n2);
    }
    RexxStringLength  StringLength(RexxStringObject o)
    {
        return threadContext->StringLength(o);
    }
    RexxStringPointer StringData(RexxStringObject o)
    {
        return threadContext->StringData(o);
    }
    RexxStringObject  NewString(RexxStringPointer s, RexxStringLength len)
    {
        return threadContext->NewString(s, len);
    }
    RexxStringObject  NewStringFromAsciiz(RexxStringPointer s)
    {
        return threadContext->NewStringFromAsciiz(s);
    }
    RexxStringObject  StringUpper(RexxStringObject s)
    {
        return threadContext->StringUpper(s);
    }
    RexxStringObject  StringLower(RexxStringObject s)
    {
        return threadContext->StringLower(s);
    }
    RexxBoolean       IsString(RexxObjectPtr o)
    {
        return threadContext->IsString(o);
    }

    RexxBufferStringObject  NewBufferString(RexxStringLength len)
    {
        return threadContext->NewBufferString(len);
    }

    RexxStringLength  BufferStringLength(RexxBufferStringObject o)
    {
        return threadContext->BufferStringLength(o);
    }

    RexxStringPointer BufferStringData(RexxBufferStringObject o)
    {
        return threadContext->BufferStringData(o);
    }

    RexxStringObject FinishBufferString(RexxBufferStringObject o, RexxStringLength l)
    {
        return threadContext->FinishBufferString(o, l);
    }

    void             TablePut(RexxTableObject to, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->TablePut(to, o1, o2);
    }
    RexxObjectPtr    TableGet(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableGet(to, o);
    }
    RexxObjectPtr    TableRemove(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableRemove(to, o);
    }
    RexxTableObject  NewTable()
    {
        return threadContext->NewTable();
    }
    RexxBoolean      IsTable(RexxObjectPtr o)
    {
        return threadContext->IsTable(o);
    }

    void             DirectoryPut(RexxDirectoryObject diro, RexxObjectPtr o, RexxStringPointer s)
    {
        threadContext->DirectoryPut(diro, o, s);
    }
    RexxObjectPtr    DirectoryAt(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryAt(to, s);
    }
    RexxObjectPtr    DirectoryRemove(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryRemove(to, s);
    }
    RexxDirectoryObject  NewDirectory()
    {
        return threadContext->NewDirectory();
    }
    RexxBoolean      IsDirectory(RexxObjectPtr o)
    {
        return threadContext->IsDirectory(o);
    }

    RexxObjectPtr   ArrayAt(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayAt(ao, n);
    }
    RexxBoolean     ArrayHasIndex(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayHasIndex(ao, n);
    }
    void            ArrayPut(RexxArrayObject ao, RexxObjectPtr o, RexxUnsignedNumber n)
    {
        threadContext->ArrayPut(ao, o, n);
    }
    RexxUnsignedNumber ArraySize(RexxArrayObject ao)
    {
        return threadContext->ArraySize(ao);
    }
    RexxNumber ArrayDimension(RexxArrayObject ao)
    {
        return threadContext->ArrayDimension(ao);
    }
    RexxArrayObject    NewArray(RexxUnsignedNumber n)
    {
        return threadContext->NewArray(n);
    }
    RexxArrayObject    ArrayOfOne(RexxObjectPtr o)
    {
        return threadContext->ArrayOfOne(o);
    }
    RexxArrayObject    ArrayOfTwo(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->ArrayOfTwo(o1, o2);
    }
    RexxBoolean     IsArray(RexxObjectPtr o)
    {
        return threadContext->IsArray(o);
    }

    RexxStringPointer    BufferData(RexxBufferObject bo)
    {
        return threadContext->BufferData(bo);
    }
    RexxNumber  BufferLength(RexxBufferObject bo)
    {
        return threadContext->BufferLength(bo);
    }
    RexxBufferObject NewBuffer(RexxNumber n)
    {
        return threadContext->NewBuffer(n);
    }
    RexxBoolean      IsBuffer(RexxObjectPtr o)
    {
        return threadContext->IsBuffer(o);
    }

    RexxNumber   IntegerValue(RexxIntegerObject io)
    {
        return threadContext->IntegerValue(io);
    }
    RexxIntegerObject NewInteger(RexxNumber n)
    {
        return threadContext->NewInteger(n);
    }
    RexxBoolean       IsInteger(RexxObjectPtr o)
    {
        return threadContext->IsInteger(o);
    }

    POINTER           PointerValue(RexxPointerObject po)
    {
        return threadContext->PointerValue(po);
    }
    RexxPointerObject NewPointer(POINTER p)
    {
        return threadContext->NewPointer(p);
    }
    RexxBoolean       IsPointer(RexxObjectPtr o)
    {
        return threadContext->IsPointer(o);
    }

    RexxObjectPtr    SupplierValue(RexxSupplierObject so)
    {
        return threadContext->SupplierValue(so);
    }
    RexxObjectPtr    SupplierIndex(RexxSupplierObject so)
    {
        return threadContext->SupplierIndex(so);
    }
    RexxBoolean      SupplierAvailable(RexxSupplierObject so)
    {
        return threadContext->SupplierAvailable(so);
    }
    void             SupplierNext(RexxSupplierObject so)
    {
        threadContext->SupplierNext(so);
    }
    RexxSupplierObject NewSupplier(RexxArrayObject values, RexxArrayObject names)
    {
        return threadContext->NewSupplier(values, names);
    }

    void             SetStemElement(RexxStemObject so, RexxStringPointer s, RexxObjectPtr o)
    {
        threadContext->SetStemElement(so, s, o);
    }
    RexxObjectPtr    GetStemElement(RexxStemObject so, RexxStringPointer s)
    {
        return threadContext->GetStemElement(so, s);
    }
    void DropStemElement(RexxStemObject so, RexxStringPointer s)
    {
        threadContext->DropStemElement(so, s);
    }
    void             SetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->SetStemArrayElement(so, n, o);
    }
    RexxObjectPtr    GetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        return threadContext->GetStemArrayElement(so, n);
    }
    void DropStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        threadContext->DropStemArrayElement(so, n);
    }
    RexxSupplierObject GetAllStemElements(RexxStemObject so)
    {
        return threadContext->GetAllStemElements(so);
    }
    RexxObjectPtr    GetStemValue(RexxStemObject so)
    {
        return threadContext->GetStemValue(so);
    }
    RexxBoolean      IsStem(RexxObjectPtr o)
    {
        return threadContext->IsStem(o);
    }

    void RaiseException(RexxUnsignedNumber n)
    {
        threadContext->RaiseException(n);
    }
    void RaiseException1(RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->RaiseException1(n, o);
    }
    void RaiseException2(RexxUnsignedNumber n, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->RaiseException2(n, o1, o2);
    }
    void RaiseExceptionArray(RexxUnsignedNumber n, RexxArrayObject ao)
    {
        threadContext->RaiseExceptionArray(n, ao);
    }
    void RaiseCondition(RexxStringPointer s1, RexxStringPointer s2, RexxArrayObject ao, RexxObjectPtr o)
    {
        threadContext->RaiseCondition(s1, s2, ao, o);
    }
    RexxBoolean      CheckCondition()
    {
        return threadContext->CheckCondition();
    }
    RexxDirectoryObject GetConditionInfo()
    {
        return threadContext->GetConditionInfo();
    }
    void             DecodeConditionInfo(RexxDirectoryObject diro, ConditionData *c)
    {
        threadContext->DecodeConditionInfo(diro, c);
    }
    void             ClearCondition()
    {
        threadContext->ClearCondition();
    }

    RexxObjectPtr    NilObject()
    {
        return threadContext->NilObject();
    }
    RexxObjectPtr    TrueObject()
    {
        return threadContext->TrueObject();
    }
    RexxObjectPtr    FalseObject()
    {
        return threadContext->FalseObject();
    }
    RexxStringObject NullString()
    {
        return threadContext->NullString();
    }

    RexxArrayObject  GetArguments()
    {
        return functions->GetArguments(this);
    }
    RexxObjectPtr    GetArgument(RexxUnsignedNumber n)
    {
        return functions->GetArgument(this, n);
    }
    void             SetContextVariable(RexxStringPointer s, RexxObjectPtr o)
    {
        functions->SetContextVariable(this, s, o);
    }
    RexxObjectPtr    GetContextVariable(RexxStringPointer s)
    {
        return functions->GetContextVariable(this, s);
    }
    void             DropContextVariable(RexxStringPointer s)
    {
        functions->DropContextVariable(this, s);
    }
    RexxStemObject ResolveStemVariable(RexxObjectPtr v)
    {
        return functions->ResolveStemVariable(this, v);
    }

    RexxSupplierObject GetAllContextVariables()
    {
        return functions->GetAllContextVariables(this);
    }
    void             InvalidRoutine()
    {
        functions->InvalidRoutine(this);
    }
    RexxNumber GetContextDigits()
    {
        return functions->GetContextDigits(this);
    }
    RexxNumber GetContextFuzz()
    {
        return functions->GetContextFuzz(this);
    }
    RexxBoolean GetContextForm()
    {
        return functions->GetContextForm(this);
    }

#endif
};

struct RexxExitContext_
{
    RexxThreadContext *threadContext;   // the interpreter instance state
    ExitContextInterface *functions;    // functions available in a method context
    ValueDescriptor *arguments;         // the argument descriptor

#ifdef __cplusplus
    POINTER GetApplicationData()
    {
        return threadContext->GetApplicationData();
    }
    RexxObjectPtr RequestGlobalReference(RexxObjectPtr o)
    {
        return threadContext->RequestGlobalReference(o);
    }
    void      ReleaseGlobalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseGlobalReference(o);
    }
    void      ReleaseLocalReference(RexxObjectPtr o)
    {
        threadContext->ReleaseLocalReference(o);
    }

    RexxObjectPtr  SendMessage(RexxObjectPtr o, RexxStringPointer s, RexxArrayObject ao)
    {
        return threadContext->SendMessage(o, s, ao);
    }
    RexxObjectPtr  SendMessage0(RexxObjectPtr o, RexxStringPointer s)
    {
        return threadContext->SendMessage0(o, s);
    }
    RexxObjectPtr  SendMessage1(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1)
    {
        return threadContext->SendMessage1(o, s, a1);
    }
    RexxObjectPtr  SendMessage2(RexxObjectPtr o, RexxStringPointer s, RexxObjectPtr a1, RexxObjectPtr a2)
    {
        return threadContext->SendMessage2(o, s, a1, a2);
    }

    RexxDirectoryObject GetLocalEnvironment()
    {
        return threadContext->GetLocalEnvironment();
    }
    RexxDirectoryObject GetGlobalEnvironment()
    {
        return threadContext->GetGlobalEnvironment();
    }

    RexxBoolean      IsSameType(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->IsSameType(o1, o2);
    }
    RexxBoolean      IsInstanceOf(RexxObjectPtr o, RexxClassObject co)
    {
        return threadContext->IsInstanceOf(o, co);
    }
    RexxClassObject  FindClass(RexxStringPointer s)
    {
        return threadContext->FindClass(s);
    }
    RexxClassObject  FindClassFromMethod(RexxMethodObject m, RexxStringPointer n)
    {
        return threadContext->FindClassFromMethod(m, n);
    }
    RexxBoolean      HasMethod(RexxObjectPtr o, RexxStringPointer m)
    {
        return threadContext->HasMethod(o, m);
    }

    RexxMethodObject NewMethod(RexxStringPointer s, RexxUnsignedNumber n)
    {
        return threadContext->NewMethod(s, n);
    }
    RexxDirectoryObject GetMethodRoutines(RexxMethodObject m)
    {
        return threadContext->GetMethodRoutines(m);
    }
    RexxDirectoryObject GetMethodClasses(RexxMethodObject m)
    {
        return threadContext->GetMethodClasses(m);
    }
    RexxDirectoryObject GetMethodMethods(RexxMethodObject m)
    {
        return threadContext->GetMethodMethods(m);
    }
    RexxObjectPtr CallMethod(RexxMethodObject m, RexxArrayObject a)
    {
        return threadContext->CallMethod(m, a);
    }
    RexxBufferObject SaveMethod(RexxMethodObject m)
    {
        return threadContext->SaveMethod(m);
    }
    RexxMethodObject LoadMethod(RexxStringPointer d, RexxStringLength l)
    {
        return threadContext->LoadMethod(d, l);
    }

    RexxObjectPtr    NewObject()
    {
        return threadContext->NewObject();
    }
    RexxObjectPtr    NumberToObject(RexxNumber n)
    {
        return threadContext->NumberToObject(n);
    }
    RexxObjectPtr    UintptrToObject(uintptr_t n)
    {
        return threadContext->UintptrToObject(n);
    }
    RexxObjectPtr    ValueToObject(ValueDescriptor *v)
    {
        return threadContext->ValueToObject(v);
    }
    RexxObjectPtr    UnsignedNumberToObject(RexxUnsignedNumber u)
    {
        return threadContext->UnsignedNumberToObject(u);
    }
    RexxBoolean      ObjectToNumber(RexxObjectPtr o, RexxNumber *n)
    {
        return threadContext->ObjectToNumber(o, n);
    }
    RexxBoolean      ObjectToUnsignedNumber(RexxObjectPtr o, RexxUnsignedNumber *n)
    {
        return threadContext->ObjectToUnsignedNumber(o, n);
    }
    RexxObjectPtr    Int64ToObject(int64_t i)
    {
        return threadContext->Int64ToObject(i);
    }
    RexxObjectPtr    UnsignedInt64ToObject(uint64_t u)
    {
        return threadContext->UnsignedInt64ToObject(u);
    }
    RexxBoolean      ObjectToInt64(RexxObjectPtr o, int64_t *i)
    {
        return threadContext->ObjectToInt64(o, i);
    }
    RexxBoolean      ObjectToUnsignedInt64(RexxObjectPtr o, uint64_t *u)
    {
        return threadContext->ObjectToUnsignedInt64(o, u);
    }
    RexxBoolean      ObjectToUintptr(RexxObjectPtr o, uintptr_t *n)
    {
        return threadContext->ObjectToUintptr(o, n);
    }
    RexxObjectPtr    DoubleToObject(double d)
    {
        return threadContext->DoubleToObject(d);
    }
    RexxObjectPtr    DoubleToObjectWithPrecision(double d, RexxNumber precision)
    {
        return threadContext->DoubleToObjectWithPrecision(d, precision);
    }
    RexxBoolean      ObjectToDouble(RexxObjectPtr o, double *d)
    {
        return threadContext->ObjectToDouble(o, d);
    }

    RexxStringObject  ObjectToString(RexxObjectPtr o)
    {
        return threadContext->ObjectToString(o);
    }
    RexxStringPointer ObjectToStringValue(RexxObjectPtr o)
    {
        return threadContext->ObjectToStringValue(o);
    }
    RexxStringLength  StringGet(RexxStringObject o, RexxStringLength n1, RexxStringPointer s, RexxStringLength n2)
    {
        return threadContext->StringGet(o, n1, s, n2);
    }
    RexxStringLength  StringLength(RexxStringObject o)
    {
        return threadContext->StringLength(o);
    }
    RexxStringPointer StringData(RexxStringObject o)
    {
        return threadContext->StringData(o);
    }
    RexxStringObject  NewString(RexxStringPointer s, RexxStringLength len)
    {
        return threadContext->NewString(s, len);
    }
    RexxStringObject  NewStringFromAsciiz(RexxStringPointer s)
    {
        return threadContext->NewStringFromAsciiz(s);
    }
    RexxStringObject  StringUpper(RexxStringObject s)
    {
        return threadContext->StringUpper(s);
    }
    RexxStringObject  StringLower(RexxStringObject s)
    {
        return threadContext->StringLower(s);
    }
    RexxBoolean       IsString(RexxObjectPtr o)
    {
        return threadContext->IsString(o);
    }

    RexxBufferStringObject  NewBufferString(RexxStringLength len)
    {
        return threadContext->NewBufferString(len);
    }

    RexxStringLength  BufferStringLength(RexxBufferStringObject o)
    {
        return threadContext->BufferStringLength(o);
    }

    RexxStringPointer BufferStringData(RexxBufferStringObject o)
    {
        return threadContext->BufferStringData(o);
    }

    RexxStringObject FinishBufferString(RexxBufferStringObject o, RexxStringLength l)
    {
        return threadContext->FinishBufferString(o, l);
    }

    void             TablePut(RexxTableObject to, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->TablePut(to, o1, o2);
    }
    RexxObjectPtr    TableGet(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableGet(to, o);
    }
    RexxObjectPtr    TableRemove(RexxTableObject to, RexxObjectPtr o)
    {
        return threadContext->TableRemove(to, o);
    }
    RexxTableObject  NewTable()
    {
        return threadContext->NewTable();
    }
    RexxBoolean      IsTable(RexxObjectPtr o)
    {
        return threadContext->IsTable(o);
    }

    void             DirectoryPut(RexxDirectoryObject diro, RexxObjectPtr o, RexxStringPointer s)
    {
        threadContext->DirectoryPut(diro, o, s);
    }
    RexxObjectPtr    DirectoryAt(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryAt(to, s);
    }
    RexxObjectPtr    DirectoryRemove(RexxDirectoryObject to, RexxStringPointer s)
    {
        return threadContext->DirectoryRemove(to, s);
    }
    RexxDirectoryObject  NewDirectory()
    {
        return threadContext->NewDirectory();
    }
    RexxBoolean      IsDirectory(RexxObjectPtr o)
    {
        return threadContext->IsDirectory(o);
    }

    RexxObjectPtr   ArrayAt(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayAt(ao, n);
    }
    RexxBoolean     ArrayHasIndex(RexxArrayObject ao, RexxUnsignedNumber n)
    {
        return threadContext->ArrayHasIndex(ao, n);
    }
    void            ArrayPut(RexxArrayObject ao, RexxObjectPtr o, RexxUnsignedNumber n)
    {
        threadContext->ArrayPut(ao, o, n);
    }
    RexxUnsignedNumber ArraySize(RexxArrayObject ao)
    {
        return threadContext->ArraySize(ao);
    }
    RexxNumber ArrayDimension(RexxArrayObject ao)
    {
        return threadContext->ArrayDimension(ao);
    }
    RexxArrayObject    NewArray(RexxUnsignedNumber n)
    {
        return threadContext->NewArray(n);
    }
    RexxArrayObject    ArrayOfOne(RexxObjectPtr o)
    {
        return threadContext->ArrayOfOne(o);
    }
    RexxArrayObject    ArrayOfTwo(RexxObjectPtr o1, RexxObjectPtr o2)
    {
        return threadContext->ArrayOfTwo(o1, o2);
    }
    RexxBoolean     IsArray(RexxObjectPtr o)
    {
        return threadContext->IsArray(o);
    }

    RexxStringPointer    BufferData(RexxBufferObject bo)
    {
        return threadContext->BufferData(bo);
    }
    RexxNumber  BufferLength(RexxBufferObject bo)
    {
        return threadContext->BufferLength(bo);
    }
    RexxBufferObject NewBuffer(RexxNumber n)
    {
        return threadContext->NewBuffer(n);
    }
    RexxBoolean      IsBuffer(RexxObjectPtr o)
    {
        return threadContext->IsBuffer(o);
    }

    RexxNumber   IntegerValue(RexxIntegerObject io)
    {
        return threadContext->IntegerValue(io);
    }
    RexxIntegerObject NewInteger(RexxNumber n)
    {
        return threadContext->NewInteger(n);
    }
    RexxBoolean       IsInteger(RexxObjectPtr o)
    {
        return threadContext->IsInteger(o);
    }

    POINTER           PointerValue(RexxPointerObject po)
    {
        return threadContext->PointerValue(po);
    }
    RexxPointerObject NewPointer(POINTER p)
    {
        return threadContext->NewPointer(p);
    }
    RexxBoolean       IsPointer(RexxObjectPtr o)
    {
        return threadContext->IsPointer(o);
    }

    RexxObjectPtr    SupplierValue(RexxSupplierObject so)
    {
        return threadContext->SupplierValue(so);
    }
    RexxObjectPtr    SupplierIndex(RexxSupplierObject so)
    {
        return threadContext->SupplierIndex(so);
    }
    RexxBoolean      SupplierAvailable(RexxSupplierObject so)
    {
        return threadContext->SupplierAvailable(so);
    }
    void             SupplierNext(RexxSupplierObject so)
    {
        threadContext->SupplierNext(so);
    }
    RexxSupplierObject NewSupplier(RexxArrayObject values, RexxArrayObject names)
    {
        return threadContext->NewSupplier(values, names);
    }

    void             SetStemElement(RexxStemObject so, RexxStringPointer s, RexxObjectPtr o)
    {
        threadContext->SetStemElement(so, s, o);
    }
    RexxObjectPtr    GetStemElement(RexxStemObject so, RexxStringPointer s)
    {
        return threadContext->GetStemElement(so, s);
    }
    void DropStemElement(RexxStemObject so, RexxStringPointer s)
    {
        threadContext->DropStemElement(so, s);
    }
    void             SetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->SetStemArrayElement(so, n, o);
    }
    RexxObjectPtr    GetStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        return threadContext->GetStemArrayElement(so, n);
    }
    void DropStemArrayElement(RexxStemObject so, RexxUnsignedNumber n)
    {
        threadContext->DropStemArrayElement(so, n);
    }
    RexxSupplierObject GetAllStemElements(RexxStemObject so)
    {
        return threadContext->GetAllStemElements(so);
    }
    RexxObjectPtr    GetStemValue(RexxStemObject so)
    {
        return threadContext->GetStemValue(so);
    }
    RexxBoolean      IsStem(RexxObjectPtr o)
    {
        return threadContext->IsStem(o);
    }

    void RaiseException(RexxUnsignedNumber n)
    {
        threadContext->RaiseException(n);
    }
    void RaiseException1(RexxUnsignedNumber n, RexxObjectPtr o)
    {
        threadContext->RaiseException1(n, o);
    }
    void RaiseException2(RexxUnsignedNumber n, RexxObjectPtr o1, RexxObjectPtr o2)
    {
        threadContext->RaiseException2(n, o1, o2);
    }
    void RaiseExceptionArray(RexxUnsignedNumber n, RexxArrayObject ao)
    {
        threadContext->RaiseExceptionArray(n, ao);
    }
    void RaiseCondition(RexxStringPointer s1, RexxStringPointer s2, RexxArrayObject ao, RexxObjectPtr o)
    {
        threadContext->RaiseCondition(s1, s2, ao, o);
    }
    RexxBoolean      CheckCondition()
    {
        return threadContext->CheckCondition();
    }
    RexxDirectoryObject GetConditionInfo()
    {
        return threadContext->GetConditionInfo();
    }
    void             DecodeConditionInfo(RexxDirectoryObject diro, ConditionData *c)
    {
        threadContext->DecodeConditionInfo(diro, c);
    }
    void             ClearCondition()
    {
        threadContext->ClearCondition();
    }

    RexxObjectPtr    NilObject()
    {
        return threadContext->NilObject();
    }
    RexxObjectPtr    TrueObject()
    {
        return threadContext->TrueObject();
    }
    RexxObjectPtr    FalseObject()
    {
        return threadContext->FalseObject();
    }
    RexxStringObject NullString()
    {
        return threadContext->NullString();
    }
    void             SetContextVariable(RexxStringPointer s, RexxObjectPtr o)
    {
        functions->SetContextVariable(this, s, o);
    }
    RexxObjectPtr    GetContextVariable(RexxStringPointer s)
    {
        return functions->GetContextVariable(this, s);
    }
    void             DropContextVariable(RexxStringPointer s)
    {
        functions->DropContextVariable(this, s);
    }
    RexxSupplierObject GetAllContextVariables()
    {
        return functions->GetAllContextVariables(this);
    }
#endif
};


/***    Structure for system exit block (RXSYSEXIT) */

typedef struct
{
   RexxCallback       entry_point;    /* subcom enviro for sysexit  */
   RexxNumber         exit_code;      /* sysexit function code      */
}  RexxExitCallback;


RexxReturnCode RexxEntry ooRexxCreateInterpreter(RexxInstance **, RexxThreadContext **, RexxExitCallback *, RexxOption *);


/******************************************************************************/
/* Interface Datatypes (used in macro expansions)                             */
/******************************************************************************/
#define REXX_ARGUMENT_TERMINATOR  0
#define REXX_VALUE_ARGLIST     2
#define REXX_VALUE_NAME        3
#define REXX_VALUE_SCOPE       4
#define REXX_VALUE_BUFFER      5
#define REXX_VALUE_CSELF       6
#define REXX_VALUE_OSELF       7

// each of the following types have an optional equivalent

#define REXX_VALUE_RexxObjectPtr          11
#define REXX_VALUE_int                    12
#define REXX_VALUE_RexxNumber             13
#define REXX_VALUE_double                 14
#define REXX_VALUE_CSTRING                15
#define REXX_VALUE_POINTER                16
#define REXX_VALUE_RexxStringObject       17
#define REXX_VALUE_RexxUnsignedNumber     18
#define REXX_VALUE_float                  19
#define REXX_VALUE_int8_t                 20
#define REXX_VALUE_int16_t                21
#define REXX_VALUE_int32_t                22
#define REXX_VALUE_int64_t                23
#define REXX_VALUE_uint8_t                24
#define REXX_VALUE_uint16_t               25
#define REXX_VALUE_uint32_t               26
#define REXX_VALUE_size_t                 27
#define REXX_VALUE_intptr_t               28
#define REXX_VALUE_uintptr_t              29
#define REXX_VALUE_RexxBoolean            30
#define REXX_VALUE_RexxArrayObject        31
#define REXX_VALUE_RexxStemObject         32
#define REXX_VALUE_uint64_t               33

#define REXX_OPTIONAL_ARGUMENT                 0x8000

#define REXX_VALUE_OPTIONAL_RexxObjectPtr         (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxObjectPtr)
#define REXX_VALUE_OPTIONAL_int                   (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_int)
#define REXX_VALUE_OPTIONAL_RexxNumber            (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxNumber)
#define REXX_VALUE_OPTIONAL_double                (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_double)
#define REXX_VALUE_OPTIONAL_CSTRING               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_CSTRING)
#define REXX_VALUE_OPTIONAL_POINTER               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_POINTER)
#define REXX_VALUE_OPTIONAL_RexxStringObject      (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxStringObject)
#define REXX_VALUE_OPTIONAL_RexxUnsignedNumber    (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxUnsignedNumber)
#define REXX_VALUE_OPTIONAL_float                 (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_float)
#define REXX_VALUE_OPTIONAL_int8_t                (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_int8_t)
#define REXX_VALUE_OPTIONAL_int16_t               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_int16_t)
#define REXX_VALUE_OPTIONAL_int32_t               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_int32_t)
#define REXX_VALUE_OPTIONAL_int64_t               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_int64_t)
#define REXX_VALUE_OPTIONAL_uint8_t               (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_uint8_t)
#define REXX_VALUE_OPTIONAL_uint16_t              (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_uint16_t)
#define REXX_VALUE_OPTIONAL_uint32_t              (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_uint32_t)
#define REXX_VALUE_OPTIONAL_uint64_t              (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_uint32_t)
#define REXX_VALUE_OPTIONAL_size_t                (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_size_t)
#define REXX_VALUE_OPTIONAL_intptr_t              (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_intptr_t)
#define REXX_VALUE_OPTIONAL_uintptr_t             (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_uintptr_t)
#define REXX_VALUE_OPTIONAL_RexxBoolean           (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxBoolean)
#define REXX_VALUE_OPTIONAL_RexxArrayObject       (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxArrayObject)
#define REXX_VALUE_OPTIONAL_RexxStemObject        (REXX_OPTIONAL_ARGUMENT | REXX_VALUE_RexxStemObject)


#define ARGUMENT_TYPE_ARGLIST    RexxArrayObject
#define ARGUMENT_TYPE_NAME       CSTRING
#define ARGUMENT_TYPE_SCOPE      RexxClassObject
#define ARGUMENT_TYPE_BUFFER     POINTER
#define ARGUMENT_TYPE_CSELF      POINTER
#define ARGUMENT_TYPE_OSELF      RexxObjectPtr

// each of the following types have an optional equivalent

#define ARGUMENT_TYPE_RexxObjectPtr         RexxObjectPtr
#define ARGUMENT_TYPE_int                   int
#define ARGUMENT_TYPE_RexxNumber            RexxNumber
#define ARGUMENT_TYPE_double                double
#define ARGUMENT_TYPE_CSTRING               CSTRING
#define ARGUMENT_TYPE_POINTER               POINTER
#define ARGUMENT_TYPE_RexxStringObject      RexxStringObject
#define ARGUMENT_TYPE_RexxUnsignedNumber    RexxUnsignedNumber
#define ARGUMENT_TYPE_float                 float
#define ARGUMENT_TYPE_int8_t                int8_t
#define ARGUMENT_TYPE_int16_t               int16_t
#define ARGUMENT_TYPE_int32_t               int32_t
#define ARGUMENT_TYPE_int64_t               int64_t
#define ARGUMENT_TYPE_uint8_t               uint8_t
#define ARGUMENT_TYPE_uint16_t              uint16_t
#define ARGUMENT_TYPE_uint32_t              uint32_t
#define ARGUMENT_TYPE_uint64_t              uint64_t
#define ARGUMENT_TYPE_size_t                size_t
#define ARGUMENT_TYPE_intptr_t              intptr_t
#define ARGUMENT_TYPE_uintptr_t             uintptr_t
#define ARGUMENT_TYPE_RexxBoolean           RexxBoolean
#define ARGUMENT_TYPE_RexxArrayObject       RexxArrayObject
#define ARGUMENT_TYPE_RexxStemObject        RexxStemObject

#define ARGUMENT_TYPE_OPTIONAL_RexxObjectPtr         RexxObjectPtr
#define ARGUMENT_TYPE_OPTIONAL_int                   int
#define ARGUMENT_TYPE_OPTIONAL_RexxNumber            RexxNumber
#define ARGUMENT_TYPE_OPTIONAL_double                double
#define ARGUMENT_TYPE_OPTIONAL_CSTRING               CSTRING
#define ARGUMENT_TYPE_OPTIONAL_POINTER               POINTER
#define ARGUMENT_TYPE_OPTIONAL_RexxStringObject      RexxStringObject
#define ARGUMENT_TYPE_OPTIONAL_RexxUnsignedNumber    RexxUnsignedNumber
#define ARGUMENT_TYPE_OPTIONAL_float                 float
#define ARGUMENT_TYPE_OPTIONAL_int8_t                int8_t
#define ARGUMENT_TYPE_OPTIONAL_int16_t               int16_t
#define ARGUMENT_TYPE_OPTIONAL_int32_t               int32_t
#define ARGUMENT_TYPE_OPTIONAL_int64_t               int64_t
#define ARGUMENT_TYPE_OPTIONAL_uint8_t               uint8_t
#define ARGUMENT_TYPE_OPTIONAL_uint16_t              uint16_t
#define ARGUMENT_TYPE_OPTIONAL_uint32_t              uint32_t
#define ARGUMENT_TYPE_OPTIONAL_uint64_t              uint64_t
#define ARGUMENT_TYPE_OPTIONAL_size_t                size_t
#define ARGUMENT_TYPE_OPTIONAL_intptr_t              intptr_t
#define ARGUMENT_TYPE_OPTIONAL_uintptr_t             uintptr_t
#define ARGUMENT_TYPE_OPTIONAL_RexxBoolean           RexxBoolean
#define ARGUMENT_TYPE_OPTIONAL_RexxArrayObject       RexxArrayObject
#define ARGUMENT_TYPE_OPTIONAL_RexxStemObject        RexxStemObject

#define ARGUMENT_TYPE(t) ((t) & ~REXX_OPTIONAL_ARGUMENT)
#define IS_OPTIONAL_ARGUMENT(t) (((t) & REXX_OPTIONAL_ARGUMENT) != 0)


#define argumentExists(i) ((context->arguments[i].flags | ARGUMENT_EXISTS) != 0)
#define argumentOmitted(i) (!argumentExists(i))


#define __type(t)   ARGUMENT_TYPE_##t
#define __arg(p, t) arguments[p].value.value_##t
#define __ret(t, v) arguments[0].value.value_##t = (v); return NULL;
#define __adcl(t, n) __type(t) n
#define __tdcl(t)    REXX_VALUE_##t

#define __methodstub(name) uint16_t * RexxEntry name (RexxMethodContext *context, ValueDescriptor *arguments)

#ifdef __cplusplus
#define __cpp_method_proto(name) extern "C" __methodstub(name);
#else
#define __cpp_method_proto(name) __methodstub(name);
#endif

#define REXX_METHOD_PROTOTYPE(name) __cpp_method_proto(name)

// zero argument method call

#define RexxMethod0(returnType, name) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context);  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, (name##_impl(context)));                                \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context)


// method with one argument
#define RexxMethod1(returnType ,name, t1, n1) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1)));                    \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1))


// method with two arguments
#define RexxMethod2(returnType ,name, t1, n1, t2, n2) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1), __adcl(t2, n2));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2)));                    \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1), __adcl(t2, n2))


// method with three arguments
#define RexxMethod3(returnType ,name, t1, n1, t2, n2, t3, n3) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3))


// method with four arguments
#define RexxMethod4(returnType ,name, t1, n1, t2, n2, t3, n3, t4, n4) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4))


// method with five arguments
#define RexxMethod5(returnType ,name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5))


// method with six arguments
#define RexxMethod6(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxMethodContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6);  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_method_proto(name) \
/* generated calling stub function */ \
__methodstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxMethodContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6))

#define __functionstub(name) uint16_t * RexxEntry name(RexxCallContext *context, ValueDescriptor *arguments)

#ifdef __cplusplus
#define __cpp_function_proto(name) extern "C" __functionstub(name);
#else
#define __cpp_function_proto(name) __functionstub(name);
#endif

#define REXX_TYPED_FUNCTION_PROTOTYPE(name) __cpp_function_proto(name)

// zero argument function call

#define RexxFunction0(returnType, name) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context);  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context));                                 \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context)


// method with one argument
#define RexxFunction1(returnType ,name, t1, n1) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1)));                    \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1))


// method with two arguments
#define RexxFunction2(returnType ,name, t1, n1, t2, n2) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), REXX_ARGUMENT_TERMINATOR };    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(1, t2)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2))


// method with three arguments
#define RexxFunction3(returnType ,name, t1, n1, t2, n2, t3, n3) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3))


// method with four arguments
#define RexxFunction4(returnType ,name, t1, n1, t2, n2, t3, n3, t4, n4) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4))


// method with five arguments
#define RexxFunction5(returnType ,name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5))


// method with six arguments
#define RexxFunction6(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6))

// method with seven arguments
#define RexxFunction7(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7)); \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), __tdcl(t7), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6), __arg(7, t7)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7))


// function with eight arguments
#define RexxFunction8(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adcl(t8, n8));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), __tdcl(t7), __tdcl(t8), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6), __arg(7, t7), __arg(8, t8)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adlc(t8, n8))


// function with nine arguments
#define RexxFunction9(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adcl(t8, n8), __adcl(t9, n9));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), __tdcl(t7), __tdcl(t8), __tdcl(t9), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6), __arg(7, t7), __arg(8, t8), __arg(9, t9)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adlc(t8, n8), __adlc(t9, n9))


// function with ten arguments
#define RexxFunction10(returnType, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10) \
/* forward reference definition for method */ \
__type(returnType) name##_impl (RexxCallContext * context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adcl(t8, n8), __adcl(t9, n9), __adlc(t10, n10));  \
                               \
/* method signature definition */ \
static uint16_t name##_types[] = {__tdcl(returnType), __tdcl(t1), __tdcl(t2), __tdcl(t3), __tdcl(t4), __tdcl(t5), __tdcl(t6), __tdcl(t7), __tdcl(t8), __tdcl(t9), __tdcl(t10), REXX_ARGUMENT_TERMINATOR};    \
\
__cpp_function_proto(name) \
/* generated calling stub function */ \
__functionstub(name) \
{ \
    if (arguments != NULL) /* if no arguments passed, this a signature request */ \
    {                                                                             \
        /* forward to the method implementation */                                \
        __ret(returnType, name##_impl(context, __arg(1, t1), __arg(2, t2), __arg(3, t3), __arg(4, t4), __arg(5, t5), __arg(6, t6), __arg(7, t7), __arg(8, t8), __arg(9, t9), __arg(10, t10)));      \
    }                                                                             \
    return name##_types;     /* return the type signature */                      \
} \
/* the real target method code */  \
__type(returnType) name##_impl(RexxCallContext *context, __adcl(t1, n1), __adcl(t2, n2), __adcl(t3, n3), __adcl(t4, n4), __adcl(t5, n5), __adcl(t6, n6), __adcl(t7, n7), __adlc(t8, n8), __adlc(t9, n9), __adlc(t10, n10))

/******************************************************************************/
/* Types (used in macro expansions and function prototypes)                   */
/******************************************************************************/
typedef RexxObjectPtr OSELF;
typedef void *        CSELF;
typedef void *        BUFFER;
typedef RexxArrayObject ARGLIST;
typedef RexxClassObject SCOPE;
typedef CSTRING         NAME;

#endif
