

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
/* REXX  Support                                                              */
/*                                                                            */
/* Defines for mapping class ids to behaviours                                */
/*                                                                            */
/*        -- DO NOT CHANGE THIS FILE, ALL CHANGES WILL BE LOST! --            */
/******************************************************************************/

#ifndef PrimitiveBehaviourNames_Included
#define PrimitiveBehaviourNames_Included

    
#define TheObjectBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Object]))
#define TheObjectClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ObjectClass]))
#define TheClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Class]))
#define TheClassClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ClassClass]))
#define TheArrayBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Array]))
#define TheArrayClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ArrayClass]))
#define TheDirectoryBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Directory]))
#define TheDirectoryClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_DirectoryClass]))
#define TheIntegerBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Integer]))
#define TheIntegerClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_IntegerClass]))
#define TheListBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_List]))
#define TheListClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ListClass]))
#define TheMessageBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Message]))
#define TheMessageClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MessageClass]))
#define TheMethodBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Method]))
#define TheMethodClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MethodClass]))
#define TheNumberStringBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NumberString]))
#define TheNumberStringClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NumberStringClass]))
#define TheQueueBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Queue]))
#define TheQueueClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_QueueClass]))
#define TheStemBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Stem]))
#define TheStemClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_StemClass]))
#define TheStringBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_String]))
#define TheStringClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_StringClass]))
#define TheSupplierBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Supplier]))
#define TheSupplierClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_SupplierClass]))
#define TheTableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Table]))
#define TheTableClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_TableClass]))
#define TheRelationBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Relation]))
#define TheRelationClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_RelationClass]))
#define TheMutableBufferBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MutableBuffer]))
#define TheMutableBufferClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MutableBufferClass]))
#define ThePointerBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Pointer]))
#define ThePointerClassBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_PointerClass]))
#define TheActivityManagerBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ActivityManager]))
#define TheBehaviourBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Behaviour]))
#define TheBufferBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Buffer]))
#define TheHashTableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_HashTable]))
#define TheListTableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ListTable]))
#define TheRexxCodeBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_RexxCode]))
#define TheNativeCodeBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NativeCode]))
#define ThePackageBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Package]))
#define ThePackageManagerBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_PackageManager]))
#define TheSmartBufferBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_SmartBuffer]))
#define TheStackBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Stack]))
#define TheVariableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Variable]))
#define TheVariableDictionaryBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_VariableDictionary]))
#define TheClauseBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Clause]))
#define TheSourceBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Source]))
#define TheTokenBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Token]))
#define TheInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Instruction]))
#define TheAddressInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_AddressInstruction]))
#define TheAssignmentInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_AssignmentInstruction]))
#define TheCallInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_CallInstruction]))
#define TheCommandInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_CommandInstruction]))
#define TheCompoundVariableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_CompoundVariable]))
#define TheDoInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_DoInstruction]))
#define TheDotVariableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_DotVariable]))
#define TheDropInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_DropInstruction]))
#define TheElseInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ElseInstruction]))
#define TheEndInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_EndInstruction]))
#define TheEndIfInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_EndIfInstruction]))
#define TheExitInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ExitInstruction]))
#define TheExposeInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ExposeInstruction]))
#define TheForwardInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ForwardInstruction]))
#define TheFunctionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Function]))
#define TheGuardInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_GuardInstruction]))
#define TheIfInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_IfInstruction]))
#define TheInterpretInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_InterpretInstruction]))
#define TheLabelInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_LabelInstruction]))
#define TheLeaveInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_LeaveInstruction]))
#define TheMessageInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MessageInstruction]))
#define TheMessageSendBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MessageSend]))
#define TheNopInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NopInstruction]))
#define TheNumericInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NumericInstruction]))
#define TheOptionsInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_OptionsInstruction]))
#define TheOtherwiseInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_OtherwiseInstruction]))
#define TheParseInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ParseInstruction]))
#define TheProcedureInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ProcedureInstruction]))
#define TheQueueInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_QueueInstruction]))
#define TheRaiseInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_RaiseInstruction]))
#define TheReplyInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ReplyInstruction]))
#define TheReturnInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ReturnInstruction]))
#define TheSayInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_SayInstruction]))
#define TheSelectInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_SelectInstruction]))
#define TheSignalInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_SignalInstruction]))
#define TheStemVariableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_StemVariable]))
#define TheThenInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ThenInstruction]))
#define TheTraceInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_TraceInstruction]))
#define TheParseTriggerBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ParseTrigger]))
#define TheUseInstructionBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_UseInstruction]))
#define TheParseVariableBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ParseVariable]))
#define TheVariableReferenceBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_VariableReference]))
#define TheCompoundElementBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_CompoundElement]))
#define TheOperatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Operator]))
#define TheUnaryOperatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_UnaryOperator]))
#define TheBinaryOperatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_BinaryOperator]))
#define TheRootImageBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_RootImage]))
#define TheWeakReferenceBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_WeakReference]))
#define TheNativeActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NativeActivator]))
#define TheClassicNativeActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ClassicNativeActivator]))
#define TheObjectNativeActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ObjectNativeActivator]))
#define TheTypedNativeActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_TypedNativeActivator]))
#define TheMethodActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_MethodActivator]))
#define TheRexxFunctionActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_RexxFunctionActivator]))
#define TheWeakRexxActivatorBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_WeakRexxActivator]))
#define TheDoBlockBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_DoBlock]))
#define TheMemoryBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Memory]))
#define TheActivationBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Activation]))
#define TheActivityBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Activity]))
#define TheNativeActivationBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_NativeActivation]))
#define TheActivationFrameBufferBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ActivationFrameBuffer]))
#define TheEnvelopeBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Envelope]))
#define TheInternalStackBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_InternalStack]))
#define TheInterpreterInstanceBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_InterpreterInstance]))
#define TheInterpreterBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_Interpreter]))
#define TheParseTargetBehaviour      ((RexxBehaviour *)(&RexxBehaviour::primitiveBehaviours[T_ParseTarget]))


/* -------------------------------------------------------------------------- */
/* --            ==================================================        -- */
/* --            DO NOT CHANGE THIS FILE, ALL CHANGES WILL BE LOST!        -- */
/* --            ==================================================        -- */
/* -------------------------------------------------------------------------- */
#endif
    
