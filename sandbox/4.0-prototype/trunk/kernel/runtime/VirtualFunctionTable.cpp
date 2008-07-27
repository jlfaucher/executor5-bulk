

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
/* REXX Kernel                                                                */
/*                                                                            */
/* Build the table of virtual functions assigned to Rexx class instances      */
/*                                                                            */
/******************************************************************************/

/* -------------------------------------------------------------------------- */
/* --            ==================================================        -- */
/* --            DO NOT CHANGE THIS FILE, ALL CHANGES WILL BE LOST!        -- */
/* --            ==================================================        -- */
/* -------------------------------------------------------------------------- */

#include "RexxCore.h"

   
#include "ObjectClass.hpp"
#include "ClassClass.hpp"
#include "ArrayClass.hpp"
#include "DirectoryClass.hpp"
#include "IntegerClass.hpp"
#include "ListClass.hpp"
#include "MessageClass.hpp"
#include "MethodClass.hpp"
#include "NumberStringClass.hpp"
#include "QueueClass.hpp"
#include "StemClass.hpp"
#include "StringClass.hpp"
#include "SupplierClass.hpp"
#include "TableClass.hpp"
#include "RelationClass.hpp"
#include "MutableBufferClass.hpp"
#include "PointerClass.hpp"
#include "RexxActivity.hpp"
#include "RexxBehaviour.hpp"
#include "RexxBuffer.hpp"
#include "RexxHashTable.hpp"
#include "RexxListTable.hpp"
#include "RexxCode.hpp"
#include "RexxNativeCode.hpp"
#include "Package.hpp"
#include "RexxSmartBuffer.hpp"
#include "StackClass.hpp"
#include "RexxVariable.hpp"
#include "RexxVariableDictionary.hpp"
#include "Clause.hpp"
#include "SourceFile.hpp"
#include "Token.hpp"
#include "RexxInstruction.hpp"
#include "AddressInstruction.hpp"
#include "AssignmentInstruction.hpp"
#include "CallInstruction.hpp"
#include "CommandInstruction.hpp"
#include "ExpressionCompoundVariable.hpp"
#include "DoInstruction.hpp"
#include "ExpressionDotVariable.hpp"
#include "DropInstruction.hpp"
#include "ElseInstruction.hpp"
#include "EndInstruction.hpp"
#include "EndIf.hpp"
#include "ExitInstruction.hpp"
#include "ExposeInstruction.hpp"
#include "ForwardInstruction.hpp"
#include "ExpressionFunction.hpp"
#include "GuardInstruction.hpp"
#include "IfInstruction.hpp"
#include "InterpretInstruction.hpp"
#include "LabelInstruction.hpp"
#include "LeaveInstruction.hpp"
#include "MessageInstruction.hpp"
#include "ExpressionMessage.hpp"
#include "NopInstruction.hpp"
#include "NumericInstruction.hpp"
#include "OptionsInstruction.hpp"
#include "OtherwiseInstruction.hpp"
#include "ParseInstruction.hpp"
#include "ProcedureInstruction.hpp"
#include "QueueInstruction.hpp"
#include "RaiseInstruction.hpp"
#include "ReplyInstruction.hpp"
#include "ReturnInstruction.hpp"
#include "SayInstruction.hpp"
#include "SelectInstruction.hpp"
#include "SignalInstruction.hpp"
#include "ExpressionStem.hpp"
#include "ThenInstruction.hpp"
#include "TraceInstruction.hpp"
#include "ParseTrigger.hpp"
#include "UseInstruction.hpp"
#include "ExpressionVariable.hpp"
#include "IndirectVariableReference.hpp"
#include "RexxCompoundElement.hpp"
#include "ExpressionOperator.hpp"
#include "RootImage.hpp"
#include "WeakReference.hpp"
#include "FunctionActivator.hpp"
#include "DoBlock.hpp"
#include "RexxMemory.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxActivationStack.hpp"
#include "RexxEnvelope.hpp"
#include "RexxInternalStack.hpp"
#include "InterpreterInstance.hpp"
#include "Interpreter.hpp"
#include "ParseTarget.hpp"



void RexxBehaviour::buildVirtualFunctionTable()
/******************************************************************************/
/* Function:  This routine will build an array of the virtualFunctions        */
/*            There will be one for each Class.                               */
/******************************************************************************/
{
    char objectBuffer[256];              /* buffer for each object            */
    void *objectPtr;

    objectPtr = objectBuffer;
    // instantiate an instance of each class into the buffer and
    // grab the resulting virtual function table
   
   objectPtr = new (objectPtr) RexxObject(RESTOREIMAGE);
   virtualFunctions[T_Object] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_ObjectClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_Class] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_ClassClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxArray(RESTOREIMAGE);
   virtualFunctions[T_Array] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_ArrayClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxDirectory(RESTOREIMAGE);
   virtualFunctions[T_Directory] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_DirectoryClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInteger(RESTOREIMAGE);
   virtualFunctions[T_Integer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxIntegerClass(RESTOREIMAGE);
   virtualFunctions[T_IntegerClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxList(RESTOREIMAGE);
   virtualFunctions[T_List] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_ListClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxMessage(RESTOREIMAGE);
   virtualFunctions[T_Message] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_MessageClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxMethod(RESTOREIMAGE);
   virtualFunctions[T_Method] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_MethodClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxNumberString(RESTOREIMAGE);
   virtualFunctions[T_NumberString] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_NumberStringClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxQueue(RESTOREIMAGE);
   virtualFunctions[T_Queue] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_QueueClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxStem(RESTOREIMAGE);
   virtualFunctions[T_Stem] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_StemClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxString(RESTOREIMAGE);
   virtualFunctions[T_String] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_StringClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxSupplier(RESTOREIMAGE);
   virtualFunctions[T_Supplier] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_SupplierClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxTable(RESTOREIMAGE);
   virtualFunctions[T_Table] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_TableClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxRelation(RESTOREIMAGE);
   virtualFunctions[T_Relation] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_RelationClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxMutableBuffer(RESTOREIMAGE);
   virtualFunctions[T_MutableBuffer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_MutableBufferClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxPointer(RESTOREIMAGE);
   virtualFunctions[T_Pointer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClass(RESTOREIMAGE);
   virtualFunctions[T_PointerClass] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxActivityManager(RESTOREIMAGE);
   virtualFunctions[T_ActivityManager] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxBehaviour(RESTOREIMAGE);
   virtualFunctions[T_Behaviour] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxBuffer(RESTOREIMAGE);
   virtualFunctions[T_Buffer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxHashTable(RESTOREIMAGE);
   virtualFunctions[T_HashTable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxListTable(RESTOREIMAGE);
   virtualFunctions[T_ListTable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxCode(RESTOREIMAGE);
   virtualFunctions[T_RexxCode] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxNativeCode(RESTOREIMAGE);
   virtualFunctions[T_NativeCode] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) Package(RESTOREIMAGE);
   virtualFunctions[T_Package] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) PackageManager(RESTOREIMAGE);
   virtualFunctions[T_PackageManager] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxSmartBuffer(RESTOREIMAGE);
   virtualFunctions[T_SmartBuffer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxStack(RESTOREIMAGE);
   virtualFunctions[T_Stack] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxVariable(RESTOREIMAGE);
   virtualFunctions[T_Variable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxVariableDictionary(RESTOREIMAGE);
   virtualFunctions[T_VariableDictionary] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxClause(RESTOREIMAGE);
   virtualFunctions[T_Clause] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxSource(RESTOREIMAGE);
   virtualFunctions[T_Source] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxToken(RESTOREIMAGE);
   virtualFunctions[T_Token] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstruction(RESTOREIMAGE);
   virtualFunctions[T_Instruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionAddress(RESTOREIMAGE);
   virtualFunctions[T_AddressInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionAssignment(RESTOREIMAGE);
   virtualFunctions[T_AssignmentInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionCall(RESTOREIMAGE);
   virtualFunctions[T_CallInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionCommand(RESTOREIMAGE);
   virtualFunctions[T_CommandInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxCompoundVariable(RESTOREIMAGE);
   virtualFunctions[T_CompoundVariable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionDo(RESTOREIMAGE);
   virtualFunctions[T_DoInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxDotVariable(RESTOREIMAGE);
   virtualFunctions[T_DotVariable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionDrop(RESTOREIMAGE);
   virtualFunctions[T_DropInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionElse(RESTOREIMAGE);
   virtualFunctions[T_ElseInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionEnd(RESTOREIMAGE);
   virtualFunctions[T_EndInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionEndIf(RESTOREIMAGE);
   virtualFunctions[T_EndIfInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionExit(RESTOREIMAGE);
   virtualFunctions[T_ExitInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionExpose(RESTOREIMAGE);
   virtualFunctions[T_ExposeInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionForward(RESTOREIMAGE);
   virtualFunctions[T_ForwardInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxExpressionFunction(RESTOREIMAGE);
   virtualFunctions[T_Function] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionGuard(RESTOREIMAGE);
   virtualFunctions[T_GuardInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionIf(RESTOREIMAGE);
   virtualFunctions[T_IfInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionInterpret(RESTOREIMAGE);
   virtualFunctions[T_InterpretInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionLabel(RESTOREIMAGE);
   virtualFunctions[T_LabelInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionLeave(RESTOREIMAGE);
   virtualFunctions[T_LeaveInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionMessage(RESTOREIMAGE);
   virtualFunctions[T_MessageInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxExpressionMessage(RESTOREIMAGE);
   virtualFunctions[T_MessageSend] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionNop(RESTOREIMAGE);
   virtualFunctions[T_NopInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionNumeric(RESTOREIMAGE);
   virtualFunctions[T_NumericInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionOptions(RESTOREIMAGE);
   virtualFunctions[T_OptionsInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionOtherwise(RESTOREIMAGE);
   virtualFunctions[T_OtherwiseInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionParse(RESTOREIMAGE);
   virtualFunctions[T_ParseInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionProcedure(RESTOREIMAGE);
   virtualFunctions[T_ProcedureInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionQueue(RESTOREIMAGE);
   virtualFunctions[T_QueueInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionRaise(RESTOREIMAGE);
   virtualFunctions[T_RaiseInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionReply(RESTOREIMAGE);
   virtualFunctions[T_ReplyInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionReturn(RESTOREIMAGE);
   virtualFunctions[T_ReturnInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionSay(RESTOREIMAGE);
   virtualFunctions[T_SayInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionSelect(RESTOREIMAGE);
   virtualFunctions[T_SelectInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionSignal(RESTOREIMAGE);
   virtualFunctions[T_SignalInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxStemVariable(RESTOREIMAGE);
   virtualFunctions[T_StemVariable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionThen(RESTOREIMAGE);
   virtualFunctions[T_ThenInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionTrace(RESTOREIMAGE);
   virtualFunctions[T_TraceInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxTrigger(RESTOREIMAGE);
   virtualFunctions[T_ParseTrigger] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInstructionUse(RESTOREIMAGE);
   virtualFunctions[T_UseInstruction] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxParseVariable(RESTOREIMAGE);
   virtualFunctions[T_ParseVariable] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxVariableReference(RESTOREIMAGE);
   virtualFunctions[T_VariableReference] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxCompoundElement(RESTOREIMAGE);
   virtualFunctions[T_CompoundElement] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxExpressionOperator(RESTOREIMAGE);
   virtualFunctions[T_Operator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxUnaryOperator(RESTOREIMAGE);
   virtualFunctions[T_UnaryOperator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxBinaryOperator(RESTOREIMAGE);
   virtualFunctions[T_BinaryOperator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RootImage(RESTOREIMAGE);
   virtualFunctions[T_RootImage] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) WeakReference(RESTOREIMAGE);
   virtualFunctions[T_WeakReference] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) NativeActivator(RESTOREIMAGE);
   virtualFunctions[T_NativeActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) ClassicNativeActivator(RESTOREIMAGE);
   virtualFunctions[T_ClassicNativeActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) ObjectNativeActivator(RESTOREIMAGE);
   virtualFunctions[T_ObjectNativeActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) TypedNativeActivator(RESTOREIMAGE);
   virtualFunctions[T_TypedNativeActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) MethodActivator(RESTOREIMAGE);
   virtualFunctions[T_MethodActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxFunctionActivator(RESTOREIMAGE);
   virtualFunctions[T_RexxFunctionActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) WeakRexxActivator(RESTOREIMAGE);
   virtualFunctions[T_WeakRexxActivator] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxDoBlock(RESTOREIMAGE);
   virtualFunctions[T_DoBlock] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxMemory(RESTOREIMAGE);
   virtualFunctions[T_Memory] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxActivation(RESTOREIMAGE);
   virtualFunctions[T_Activation] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxActivity(RESTOREIMAGE);
   virtualFunctions[T_Activity] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxNativeActivation(RESTOREIMAGE);
   virtualFunctions[T_NativeActivation] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxActivationFrameBuffer(RESTOREIMAGE);
   virtualFunctions[T_ActivationFrameBuffer] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxEnvelope(RESTOREIMAGE);
   virtualFunctions[T_Envelope] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInternalStack(RESTOREIMAGE);
   virtualFunctions[T_InternalStack] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) InterpreterInstanceObject(RESTOREIMAGE);
   virtualFunctions[T_InterpreterInstance] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxInterpreter(RESTOREIMAGE);
   virtualFunctions[T_Interpreter] = *((void **)objectPtr);
   
   objectPtr = new (objectPtr) RexxTarget(RESTOREIMAGE);
   virtualFunctions[T_ParseTarget] = *((void **)objectPtr);
   
};


/* --            ==================================================        -- */
/* --            DO NOT CHANGE THIS FILE, ALL CHANGES WILL BE LOST!        -- */
/* --            ==================================================        -- */
/* -------------------------------------------------------------------------- */
   
