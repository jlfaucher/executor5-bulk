#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020-2024, Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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
/* Name: SETPROP.REX                                                          */
/* Type: Object REXX Script                                                   */
/*                                                                            */
-- ooRexx script to display/set properties associated with building the ooRexx
--  documentation.
    props = .doc.props
    parse arg theArg
    if theArg = '' then do
        say 'Usage is: setprop prop_name[=prop_value]'
        say '  Omitting =prop_value will display the current value of prop_name'
        say
        say 'The current contents of the properties collection is:'
        strm = .stream~new(props['prop_fn'])
        say strm~arrayin
        strm~close
        exit
    end
    if theArg~pos('=') = 0 then
        say theArg 'is ['props~getProperty(theArg, '')']'
    else do
        parse var theArg start '=' rest
        prop = start~strip
        val = rest~strip
        oldVal = props~getProperty(prop, '')
        if "true false 1 0"~wordPos(oldVal) > 0 then do
            parse value 1 0 with true false
            v_val = value(val)
            if v_val~datatype(O) then
                props~setLogical(prop, v_val)
            else do
                say prop 'must be a logical value; found' val
                exit
            end
        end
        else
            props~setProperty(prop, val)
        call save_props                     -- save the updated properties
        say prop 'was ['oldVal'] and is now ['val']'
    end

::requires doc_props.rex
