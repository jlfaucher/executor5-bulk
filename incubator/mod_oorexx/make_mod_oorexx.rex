#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (C) W. David Ashley 2004-2008. All Rights Reserved.              */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* find out the operating system */
parse source source_str
parse var source_str os .
if substr(os, 1, 7) = 'Windows' then os = 'WINDOWS'
if os = 'WIN32' then os = 'WINDOWS' /* for Regina */
if os = 'UNIX' then os = 'LINUX'    /* for Regina */
select
   when os = 'OS/2' then do
      say 'Error: OS/2 is not supported by Mod_ooRexx.'
      return 8
      end
   when os = 'LINUX' then nop
   when os = 'AIX' then nop
   when os = 'WINDOWS' then nop
   otherwise do
      say 'Error: Unknown operating system.'
      return 12
      end
   end

/* get any command line arguments */
parse arg arguments
if arguments~upper() = 'HELP' then do
   call helpmsg
   return 0
   end

/* now call the appropriate functions for making Mod_ooRexx */
select
   when os = 'LINUX' then do
      'cp ./makefile.linux ./makefile'
      'make'
      'cp mod_oorexx.so ./bin'
      'make cleanall'
      'rm ./makefile'
      end
   when os = 'AIX' then do
      'cp ./makefile.aix ./makefile'
      'make'
      'cp mod_oorexx.so ./bin'
      'make cleanall'
      'rm ./makefile'
      end
   when os = 'WINDOWS' then do
      'nmake /F makefile.nt'
      'copy mod_oorexx.dll .\bin'
      'nmake /F makefile.nt cleanall'
      end
   otherwise nop
   end

return 0


helpmsg:
say
say 'Syntax: rexx make_mod_oorexx.rex [help]'
say
say 'The help argument will give you this message.'
say
return

