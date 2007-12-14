/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/****************************************************************************/
/* REXX Kernel                                                              */
/*                                                                          */
/* Primitive object declarations                                            */
/****************************************************************************/
                                       /* IMPORTANT NOTE:  The includes in  */
                                       /* this file must be in the same     */
                                       /* order as the class type number    */
                                       /* definitions in RexxCore.H         */
                                       /* Object MUST be included 1st       */
CLASS_EXTERNAL(Object, RexxObject)
CLASS_EXTERNAL(ObjectClass, RexxClass)

CLASS_EXTERNAL(Class, RexxClass)
CLASS_EXTERNAL(ClassClass, RexxClass)

CLASS_EXTERNAL(Array, RexxArray)
CLASS_EXTERNAL(ArrayClass, RexxClass)

CLASS_EXTERNAL(Directory, RexxDirectory)
CLASS_EXTERNAL(DirectoryClass, RexxClass)

CLASS_EXTERNAL_STRING(Integer, RexxInteger)
CLASS_EXTERNAL(IntegerClass, RexxIntegerClass)

CLASS_EXTERNAL(List, RexxList)
CLASS_EXTERNAL(ListClass, RexxClass)

CLASS_EXTERNAL(Message, RexxMessage)
CLASS_EXTERNAL(MessageClass, RexxClass)

CLASS_EXTERNAL(Method, RexxMethod)
CLASS_EXTERNAL(MethodClass,  RexxClass)

CLASS_EXTERNAL_STRING(NumberString, RexxNumberString)
CLASS_EXTERNAL(NumberStringClass, RexxClass)

CLASS_EXTERNAL(Queue, RexxQueue)
CLASS_EXTERNAL(QueueClass, RexxClass)

CLASS_EXTERNAL(Stem, RexxStem)
CLASS_EXTERNAL(StemClass, RexxClass)

CLASS_EXTERNAL_STRING(String, RexxString)
CLASS_EXTERNAL(StringClass, RexxStringClass)

CLASS_EXTERNAL(Supplier, RexxSupplier)
CLASS_EXTERNAL(SupplierClass, RexxClass)

CLASS_EXTERNAL(Table, RexxTable)
CLASS_EXTERNAL(TableClass, RexxClass)

CLASS_EXTERNAL(Relation, RexxRelation)
CLASS_EXTERNAL(RelationClass, RexxClass)

CLASS_EXTERNAL(MutableBuffer, RexxMutableBuffer)
CLASS_EXTERNAL(MutableBufferClass, RexxClass)


CLASS_INTERNAL(NilObject, RexxNilObject)
CLASS_INTERNAL(Behaviour, RexxBehaviour)
CLASS_INTERNAL(RexxSource, RexxSource)
CLASS_INTERNAL(RexxCode, RexxCode)
CLASS_INTERNAL(NativeCode, RexxNativeCode)

CLASS_INTERNAL(Buffer, RexxBuffer)
CLASS_INTERNAL(HashTable, RexxHashTable)
CLASS_INTERNAL(ListTable, RexxListTable)
CLASS_INTERNAL(SmartBuffer, RexxSmartBuffer)

CLASS_INTERNAL(Variable, RexxVariable)
CLASS_INTERNAL(VariableDictionary, RexxVariableDictionary)
CLASS_INTERNAL(VariableTerm, RexxParseVariable)
CLASS_INTERNAL(CompoundVariableTerm, RexxCompoundVariable)
CLASS_INTERNAL(StemVariableTerm, RexxStemVariable)
CLASS_INTERNAL(DotVariableTerm, RexxDotVariable)
CLASS_INTERNAL(IndirectVariableTerm, RexxVariableReference)
CLASS_INTERNAL(FunctionCallTerm, RexxExpressionFunction)
CLASS_INTERNAL(MessageSendTerm, RexxExpressionMessage)
CLASS_INTERNAL(UnaryOperatorTerm, RexxUnaryOperator)
CLASS_INTERNAL(BinaryOperatorTerm, RexxBinaryOperator)
CLASS_INTERNAL(LogicalTerm, RexxExpressionLogical)


CLASS_INTERNAL(Instruction, RexxInstruction)
CLASS_INTERNAL(AddressInstruction, RexxInstructionAddress)
CLASS_INTERNAL(AssignmentInstruction, RexxInstructionAssignment)
CLASS_INTERNAL(CallInstruction, RexxInstructionCall)
CLASS_INTERNAL(CommandInstruction, RexxInstructionCommand)
CLASS_INTERNAL(DoInstruction,RexxInstructionDo)
CLASS_INTERNAL(DropInstruction, RexxInstructionDrop)
CLASS_INTERNAL(ElseInstruction, RexxInstructionElse)
CLASS_INTERNAL(EndInstruction, RexxInstructionEnd)
CLASS_INTERNAL(EndIfInstruction, RexxInstructionEndIf)
CLASS_INTERNAL(ExitInstruction, RexxInstructionExit)
CLASS_INTERNAL(ExposeInstruction, RexxInstructionExpose)
CLASS_INTERNAL(ForwardInstruction, RexxInstructionForward)
CLASS_INTERNAL(GuardInstruction, RexxInstructionGuard)
CLASS_INTERNAL(IfInstruction, RexxInstructionIf)
CLASS_INTERNAL(InterpretInstruction, RexxInstructionInterpret)
CLASS_INTERNAL(LabelInstruction, RexxInstructionLabel)
CLASS_INTERNAL(LeaveInstruction, RexxInstructionLeave)
CLASS_INTERNAL(MessageInstruction, RexxInstructionMessage)
CLASS_INTERNAL(NopInstruction, RexxInstructionNop)
CLASS_INTERNAL(NumericInstruction, RexxInstructionNumeric)
CLASS_INTERNAL(OptionsInstruction, RexxInstructionOptions)
CLASS_INTERNAL(OtherwiseInstruction, RexxInstructionOtherWise)
CLASS_INTERNAL(ParseInstruction, RexxInstructionParse)
CLASS_INTERNAL(ProcedureInstruction, RexxInstructionProcedure)
CLASS_INTERNAL(QueueInstruction, RexxInstructionQueue)
CLASS_INTERNAL(RaiseInstruction, RexxInstructionRaise)
CLASS_INTERNAL(ReplyInstruction, RexxInstructionReply)
CLASS_INTERNAL(ReturnInstruction, RexxInstructionReturn)
CLASS_INTERNAL(SayInstruction, RexxInstructionSay)
CLASS_INTERNAL(SelectInstruction, RexxInstructionSelect)
CLASS_INTERNAL(SignalInstruction, RexxInstructionSignal)
CLASS_INTERNAL(ThenInstruction, RexxInstructionThen)
CLASS_INTERNAL(TraceInstruction, RexxInstructionTrace)
CLASS_INTERNAL(UseInstruction, RexxInstructionUseStrict)

CLASS_INTERNAL(CompoundElement, RexxCompoundElement)
CLASS_INTERNAL(ParseTrigger, RexxTrigger)

CLASS_INTERNAL(Memory, RexxObject)
CLASS_INTERNAL(InternalStack, RexxInternalStack)
CLASS_INTERNAL(Stack, RexxStack)
CLASS_INTERNAL(Activity, RexxActivity)
CLASS_INTERNAL(Activation, RexxActivation)
CLASS_INTERNAL(NativeActivation, RexxNativeActivation)
CLASS_INTERNAL(ActivationFrameBuffer, RexxActivationFrameBuffer)
CLASS_INTERNAL(Envelope, RexxEnvelope)
CLASS_INTERNAL(Clause, RexxClause)
CLASS_INTERNAL(Token, RexxToken)
CLASS_INTERNAL(DoBlock, RexxDoBlock)

