#----------------------------------------------------------------------------
#
# Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.ibm.com/developerworks/oss/CPLv1.0.htm
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Rexx Language Association nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#----------------------------------------------------------------------------


MAINTAINERCLEANFILES = Makefile.in

# get version details from our external file
include $(top_srcdir)/oorexx.ver

SUBDIRS = messages
DIST_SUBDIRS = api classes client expression extras instructions messages \
               parser platform RexxClasses runtime script streamLibrary

INCLUDES = @OOREXX_INCLUDES@ \
           -I$(top_srcdir)/kernel/api \
           -I$(top_srcdir)/kernel/classes \
           -I$(top_srcdir)/kernel/expression \
           -I$(top_srcdir)/kernel/instructions \
           -I$(top_srcdir)/kernel/parser \
	   -I$(top_srcdir)/kernel/platform/unix \
           -I$(top_srcdir)/kernel/runtime \
           -I$(top_srcdir)/kernel/streamLibrary \
           -I$(top_srcdir)/shared \
	   -I$(top_srcdir)/shared/unix

unix_32_sources = api/InterpreterLegacy.cpp \
     		  api/LegacyExitHandler.cpp

unix_64_sources =


#################### librexx.so ##########################

pkglib_LTLIBRARIES = librexx.la

#
# Sources for librexx.so
#
librexx_la_SOURCES = $(unix_@OOREXX_SIZEOF_POINTER@_sources) \
                Rxstring.hpp \
		SynchronizedBlock.hpp \
		SystemException.hpp \
		SysUtil.cpp \
		SysUtil.hpp \
		$(top_srcdir)/shared/ClientMessage.cpp \
		$(top_srcdir)/shared/ClientMessage.hpp \
		$(top_srcdir)/shared/Encodings.hpp \
		$(top_srcdir)/shared/ServiceException.hpp \
		$(top_srcdir)/shared/ServiceMessage.cpp \
		$(top_srcdir)/shared/ServiceMessage.hpp \
                platform/unix/APIDefinitions.h \
                platform/unix/DBCSSupport.cpp \
                platform/unix/ErrorMessages.cpp \
                platform/unix/ExternalFunctions.cpp \
                platform/unix/FileSystem.cpp \
                platform/unix/MemorySupport.cpp \
                platform/unix/MiscSystem.cpp \
                platform/unix/PlatformDefinitions.h \
                platform/unix/PlatformTypes.h \
                platform/unix/rexx.cat \
                platform/unix/RexxMain.cpp \
                platform/unix/RexxUtil.cpp \
                platform/unix/SharedMemorySupport.cpp \
                platform/unix/SharedMemorySupport.h \
                platform/unix/SysActivity.cpp \
                platform/unix/SysActivity.hpp \
                platform/unix/SysCriticalSection.cpp \
                platform/unix/SysCriticalSection.hpp \
                platform/unix/SysCSStream.cpp \
                platform/unix/SysCSStream.hpp \
                platform/unix/SysFile.cpp \
                platform/unix/SysFile.hpp \
                platform/unix/SysFileSystem.cpp \
                platform/unix/SysFileSystem.hpp \
                platform/unix/SysInterpreter.cpp \
                platform/unix/SysInterpreter.hpp \
                platform/unix/SysLibrary.cpp \
                platform/unix/SysLibrary.hpp \
                platform/unix/SysLocalAPIManager.cpp \
                platform/unix/SysLocalAPIManager.hpp \
                platform/unix/SysProcess.cpp \
                platform/unix/SysProcess.hpp \
                platform/unix/SysSemaphore.cpp \
                platform/unix/SysSemaphore.hpp \
                platform/unix/SystemCommands.cpp \
                platform/unix/SystemCommands.h \
                platform/unix/SysThread.cpp \
                platform/unix/SysThread.hpp \
                platform/unix/TimeSupport.cpp \
                platform/unix/UseridFunction.cpp \
                platform/unix/ValueFunction.cpp \
                api/CallContextStubs.cpp \
     		api/ContextApi.hpp \
     		api/InterpreterAPI.cpp \
     		api/InterpreterInstanceStubs.cpp \
     		api/MacroSpaceApi.cpp \
     		api/MethodContextStubs.cpp \
     		api/QueuesAPI.cpp \
     		api/RegistrationAPI.cpp \
     		api/RexxAPI.h \
     		api/RexxLegacyAPI.hpp \
     		api/ThreadContextStubs.cpp \
     		api/VariablePool.cpp \
     		classes/ArrayClass.cpp \
     		classes/ArrayClass.hpp \
     		classes/ASCIIDBCSStrings.cpp \
     		classes/ASCIIDBCSStrings.hpp \
     		classes/ClassClass.cpp \
     		classes/ClassClass.hpp \
     		classes/DirectoryClass.cpp \
     		classes/DirectoryClass.hpp \
     		classes/IntegerClass.cpp \
     		classes/IntegerClass.hpp \
     		classes/ListClass.cpp \
     		classes/ListClass.hpp \
     		classes/MessageClass.cpp \
     		classes/MessageClass.hpp \
     		classes/MethodClass.cpp \
     		classes/MethodClass.hpp \
     		classes/MutableBufferClass.cpp \
     		classes/MutableBufferClass.hpp \
     		classes/NumberStringClass.cpp \
     		classes/NumberStringClass.hpp \
     		classes/NumberStringMath.cpp \
     		classes/NumberStringMath.hpp \
     		classes/NumberStringMath2.cpp \
     		classes/ObjectClass.cpp \
     		classes/ObjectClass.hpp \
     		classes/PointerClass.cpp \
     		classes/PointerClass.hpp \
     		classes/QueueClass.cpp \
     		classes/QueueClass.hpp \
     		classes/RelationClass.cpp \
     		classes/RelationClass.hpp \
     		classes/StackClass.cpp \
     		classes/StackClass.hpp \
     		classes/StemClass.cpp \
     		classes/StemClass.hpp \
     		classes/StringClass.cpp \
     		classes/StringClass.hpp \
     		classes/StringClassBit.cpp \
     		classes/StringClassConversion.cpp \
     		classes/StringClassMisc.cpp \
     		classes/StringClassSub.cpp \
     		classes/StringClassUtil.cpp \
     		classes/StringClassWord.cpp \
     		classes/SupplierClass.cpp \
     		classes/SupplierClass.hpp \
     		classes/TableClass.cpp \
     		classes/TableClass.hpp \
     		client/LocalAPIContext.cpp \
     		client/LocalAPIContext.hpp \
     		client/LocalAPIManager.cpp \
     		client/LocalAPIManager.hpp \
     		client/LocalAPISubsystem.hpp \
     		client/LocalMacroSpaceManager.cpp \
     		client/LocalMacroSpaceManager.hpp \
     		client/LocalQueueManager.cpp \
     		client/LocalQueueManager.hpp \
     		client/LocalRegistrationManager.cpp \
     		client/LocalRegistrationManager.hpp \
     		expression/BuiltinFunctions.cpp \
     		expression/BuiltinFunctions.hpp \
     		expression/CommonSysUtils.cpp \
     		expression/CommonSysUtils.h \
     		expression/ExpressionBaseVariable.hpp \
     		expression/ExpressionCompoundVariable.cpp \
     		expression/ExpressionCompoundVariable.hpp \
     		expression/ExpressionDotVariable.cpp \
     		expression/ExpressionDotVariable.hpp \
     		expression/ExpressionFunction.cpp \
     		expression/ExpressionFunction.hpp \
     		expression/ExpressionMessage.cpp \
     		expression/ExpressionMessage.hpp \
     		expression/ExpressionOperator.cpp \
     		expression/ExpressionOperator.hpp \
     		expression/ExpressionStack.cpp \
     		expression/ExpressionStack.hpp \
     		expression/ExpressionStem.cpp \
     		expression/ExpressionStem.hpp \
     		expression/ExpressionVariable.cpp \
     		expression/ExpressionVariable.hpp \
     		expression/IndirectVariableReference.cpp \
     		expression/IndirectVariableReference.hpp \
     		expression/QueueFunctions.cpp \
     		instructions/AddressInstruction.cpp \
     		instructions/AddressInstruction.hpp \
     		instructions/AssignmentInstruction.cpp \
     		instructions/AssignmentInstruction.hpp \
     		instructions/CallInstruction.cpp \
     		instructions/CallInstruction.hpp \
     		instructions/CommandInstruction.cpp \
     		instructions/CommandInstruction.hpp \
     		instructions/DoBlock.cpp \
     		instructions/DoBlock.hpp \
     		instructions/DoInstruction.cpp \
     		instructions/DoInstruction.hpp \
     		instructions/DropInstruction.cpp \
     		instructions/DropInstruction.hpp \
     		instructions/ElseInstruction.cpp \
     		instructions/ElseInstruction.hpp \
     		instructions/EndIf.cpp \
     		instructions/EndIf.hpp \
     		instructions/EndInstruction.cpp \
     		instructions/EndInstruction.hpp \
     		instructions/ExitInstruction.cpp \
     		instructions/ExitInstruction.hpp \
     		instructions/ExposeInstruction.cpp \
     		instructions/ExposeInstruction.hpp \
     		instructions/ForwardInstruction.cpp \
     		instructions/ForwardInstruction.hpp \
     		instructions/GuardInstruction.cpp \
     		instructions/GuardInstruction.hpp \
     		instructions/IfInstruction.cpp \
     		instructions/IfInstruction.hpp \
     		instructions/InterpretInstruction.cpp \
     		instructions/InterpretInstruction.hpp \
     		instructions/LabelInstruction.cpp \
     		instructions/LabelInstruction.hpp \
     		instructions/LeaveInstruction.cpp \
     		instructions/LeaveInstruction.hpp \
     		instructions/MessageInstruction.cpp \
     		instructions/MessageInstruction.hpp \
     		instructions/NopInstruction.cpp \
     		instructions/NopInstruction.hpp \
     		instructions/NumericInstruction.cpp \
     		instructions/NumericInstruction.hpp \
     		instructions/OptionsInstruction.cpp \
     		instructions/OptionsInstruction.hpp \
     		instructions/OtherwiseInstruction.cpp \
     		instructions/OtherwiseInstruction.hpp \
     		instructions/ParseInstruction.cpp \
     		instructions/ParseInstruction.hpp \
     		instructions/ParseTarget.cpp \
     		instructions/ParseTarget.hpp \
     		instructions/ParseTrigger.cpp \
     		instructions/ParseTrigger.hpp \
     		instructions/ProcedureInstruction.cpp \
     		instructions/ProcedureInstruction.hpp \
     		instructions/QueueInstruction.cpp \
     		instructions/QueueInstruction.hpp \
     		instructions/RaiseInstruction.cpp \
     		instructions/RaiseInstruction.hpp \
     		instructions/ReplyInstruction.cpp \
     		instructions/ReplyInstruction.hpp \
     		instructions/ReturnInstruction.cpp \
     		instructions/ReturnInstruction.hpp \
     		instructions/RexxInstruction.cpp \
     		instructions/RexxInstruction.hpp \
     		instructions/SayInstruction.cpp \
     		instructions/SayInstruction.hpp \
     		instructions/SelectInstruction.cpp \
     		instructions/SelectInstruction.hpp \
     		instructions/SignalInstruction.cpp \
     		instructions/SignalInstruction.hpp \
     		instructions/ThenInstruction.cpp \
     		instructions/ThenInstruction.hpp \
     		instructions/TraceInstruction.cpp \
     		instructions/TraceInstruction.hpp \
     		instructions/UseInstruction.cpp \
     		instructions/UseInstruction.hpp \
     		parser/Clause.cpp \
     		parser/Clause.hpp \
     		parser/InstructionParser.cpp \
     		parser/Scanner.cpp \
     		parser/SourceFile.cpp \
     		parser/SourceFile.hpp \
     		parser/Token.cpp \
     		parser/Token.hpp \
     		runtime/ActivationApiContexts.hpp \
     		runtime/ActivationBase.hpp \
     		runtime/ActivityState.hpp \
     		runtime/ActivityTable.cpp \
     		runtime/ActivityTable.hpp \
     		runtime/ArgumentUtilities.hpp \
     		runtime/ClassTypeCodes.h \
     		runtime/DeadObject.cpp \
     		runtime/DeadObject.hpp \
     		runtime/Errors.cpp \
     		runtime/ExitHandler.cpp \
     		runtime/ExitHandler.hpp \
     		runtime/ExternalFunctionSearch.cpp \
     		runtime/FunctionActivator.cpp \
     		runtime/FunctionActivator.hpp \
     		runtime/GlobalData.cpp \
     		runtime/GlobalNames.cpp \
     		runtime/GlobalNames.h \
     		runtime/GlobalNames.hpp \
     		runtime/InternalNativeMethod.h \
     		runtime/InternalPackage.cpp \
     		runtime/Interpreter.cpp \
     		runtime/Interpreter.hpp \
     		runtime/InterpreterInstance.cpp \
     		runtime/InterpreterInstance.hpp \
     		runtime/MemorySegment.cpp \
     		runtime/MemorySegment.hpp \
     		runtime/MemoryStats.cpp \
     		runtime/MemoryStats.hpp \
     		runtime/NativeFunctions.h \
     		runtime/NativeMethods.h \
     		runtime/Numerics.cpp \
     		runtime/Numerics.hpp \
     		runtime/Package.cpp \
     		runtime/Package.hpp \
     		runtime/PrimitiveBehaviourNames.h \
     		runtime/PrimitiveBehaviours.cpp \
     		runtime/RexxActivation.cpp \
     		runtime/RexxActivation.hpp \
     		runtime/RexxActivationStack.cpp \
     		runtime/RexxActivationStack.hpp \
     		runtime/RexxActivity.cpp \
     		runtime/RexxActivity.hpp \
     		runtime/RexxBehaviour.cpp \
     		runtime/RexxBehaviour.hpp \
     		runtime/RexxBuffer.cpp \
     		runtime/RexxBuffer.hpp \
     		runtime/RexxCode.cpp \
     		runtime/RexxCode.hpp \
     		runtime/RexxCollection.cpp \
     		runtime/RexxCollection.hpp \
     		runtime/RexxCompoundElement.cpp \
     		runtime/RexxCompoundElement.hpp \
     		runtime/RexxCompoundTable.cpp \
     		runtime/RexxCompoundTable.hpp \
     		runtime/RexxCompoundTail.cpp \
     		runtime/RexxCompoundTail.hpp \
     		runtime/RexxConstants.cpp \
     		runtime/RexxConstants.hpp \
     		runtime/RexxCore.h \
     		runtime/RexxEnvelope.cpp \
     		runtime/RexxEnvelope.hpp \
     		runtime/RexxHashTable.cpp \
     		runtime/RexxHashTable.hpp \
     		runtime/RexxInternalStack.cpp \
     		runtime/RexxInternalStack.hpp \
     		runtime/RexxListTable.cpp \
     		runtime/RexxListTable.hpp \
     		runtime/RexxLocalVariables.cpp \
     		runtime/RexxLocalVariables.hpp \
     		runtime/RexxMemory.cpp \
     		runtime/RexxMemory.hpp \
     		runtime/RexxNativeActivation.cpp \
     		runtime/RexxNativeActivation.hpp \
     		runtime/RexxNativeCode.cpp \
     		runtime/RexxNativeCode.hpp \
     		runtime/RexxSmartBuffer.cpp \
     		runtime/RexxSmartBuffer.hpp \
     		runtime/RexxThread.cpp \
     		runtime/RexxThread.hpp \
     		runtime/RexxVariable.cpp \
     		runtime/RexxVariable.hpp \
     		runtime/RexxVariableDictionary.cpp \
     		runtime/RexxVariableDictionary.hpp \
     		runtime/RootImage.cpp \
     		runtime/RootImage.hpp \
     		runtime/Setup.cpp \
     		runtime/TranslatedImage.cpp \
     		runtime/Utilities.cpp \
     		runtime/Version.cpp \
     		runtime/VirtualFunctionTable.cpp \
     		runtime/WeakReference.cpp \
     		runtime/WeakReference.hpp \
     		script/ScriptContext.cpp \
     		script/ScriptContext.hpp \
     		streamLibrary/StreamCommandParser.cpp \
     		streamLibrary/StreamCommandParser.h \
     		streamLibrary/StreamNative.cpp \
     		streamLibrary/StreamNative.hpp

#
# Extra sources for librexx.so
#
EXTRA_librexx_la_SOURCES = platform/unix/UnixGencat.xsl \
                           platform/unix/SystemMethods.orx

#
# C++ compiler flags for librexx.so sources
#
librexx_la_CXXFLAGS = -DORX_VER=$(MAJOR) -DORX_REL=$(MINOR) -DORX_MOD=$(MOD_LVL)

#
# linker flags for librexx.so objects
#
librexx_la_LDFLAGS = $(VERSION_INFO) -no-undefined

#
# additional libraries for librexx.so
#
librexx_la_LIBADD = $(top_srcdir)/shared/libshared.la


