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
/*                                                                            */
/* Script context object used for external scripting applications             */
/*                                                                            */
/*******************************************************************************/

#include "ScriptContext.hpp"
#include "RexxActivation.hpp"


/**
 * Add a variable binding to the script context.
 *
 * @param name   The name of the bound variable (only simple variables supported).
 * @param value  The variables initial value.
 */
void ScriptContext::addVariableBinding(RexxString *name, RexxObject *value)
{
    boundVariables->set(name, value);
}


/**
 * Update a ScriptContext variable binding.
 *
 * @param name   The name of the target variable.
 * @param value  The new variable value.
 */
void ScriptContext::setBoundValue(RexxString *name, RexxObject *value)
{
    boundVariables->set(name, value);
}


/**
 * Get the current value for a bound variable.
 *
 * @param name   The name of the variable..
 *
 * @return The current variable value.  Returns null if the variable has
 *         been dropped.
 */
RexxObject *Scriptcontext::getBoundValue(RexxString *name)
{
    return boundVariables->realValue(name);

}


/**
 * Remove a variable from the active bindings list.  This variable
 * will be dropped, and the reference removed from the script
 * context.
 *
 * @param name   The target variable name.
 */
void ScriptContext::removeVariableBinding(RexxString *name)
{
    boundValues->removeVariable(name);
}


/**
 * Bind the script context variable bindings into a new activation
 * context.
 *
 * @param context The new activation.
 */
void ScriptContext::bindScriptVariables(RexxActivation *context)
{
    boundValues->bindScriptVariables(context);
}
