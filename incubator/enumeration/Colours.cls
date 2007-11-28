/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2008 Rexx Language Association. All rights reserved.    */
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
/* Author: Moritz Hoffmann <spambasket@antiguru.de>                           */
/*----------------------------------------------------------------------------*/
::REQUIRES "Enumeration.cls"

::CLASS Colours SUBCLASS Enumeration PUBLIC
::CLASS Red SUBCLASS Colours
::CLASS Blue SUBCLASS Colours
::CLASS Yellow SUBCLASS Colours
::CLASS LightBlue SUBCLASS Blue

::CLASS Direction SUBCLASS Enumeration PUBLIC
::CLASS North SUBCLASS Direction
::CLASS East SUBCLASS Direction
::CLASS South SUBCLASS Direction
::CLASS West SUBCLASS Direction

::CLASS WithContent SUBCLASS Enumeration PUBLIC
::METHOD Init
  expose content
  use arg content
::METHOD getContent
  expose content
  return content
::METHOD setContent
  expose content
  use strict arg content



::METHOD strictEquals
  use strict arg other
  if other~isInstanceOf(self~class~superclass) then
    return self = other & self~getContent = other~getContent
  else return .false

::CLASS AContent SUBCLASS WithContent
