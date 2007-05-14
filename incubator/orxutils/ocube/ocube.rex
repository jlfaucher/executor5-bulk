#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: OCube application.                                            */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
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


/* program version string */
OCube_version = '1.0.0'
OCube_vdate = '2003-02-18'


/* parse the command line arguments into a .array similar to the way C does */
cmdLine = arg(1)
cmdArgs = .array~new
i = 1
do while cmdLine~length() > 0
   if cmdLine~left(1) = '"' then do
      parse var cmdLine '"' argument '"' cmdLine
      end
   else if cmdLine~left(1) = '{' then do
      parse var cmdLine '{' argument '}' cmdLine
      argument = '{' || argument || '}'
      end
   else do
      parse var cmdLine argument cmdLine
      end
   cmdArgs[i] = argument
   i = i + 1
   end

/* process the files portion of the command line arguments */
if cmdArgs~items < 2 | cmdArgs~items > 6 then do
   say 'Error: Invalid command line.'
   return 12
   end
ProcFileName = cmdArgs[1]
TargetFileName = cmdArgs[2]
if cmdArgs~items >= 3 then do
   BackupFileName = cmdArgs[3]
   z = 4
   end
else do
   BackupFileName = ''
   z = 3
   end

/* process the global options from the command line argument array */
GlobalOpt = .GlobalOpt~new()
do x = z to cmdArgs~items
   cmdArgs[x] = cmdArgs[x]~translate()
   select
      when cmdArgs[x] = '-DEBUG' then GlobalOpt~Debug = .true
      when cmdArgs[x] = '-PAUSE' then GlobalOpt~Pause = .true
      when cmdArgs[x] = '-VERBOSE' then GlobalOpt~Verbose = .true
      when cmdArgs[x] = '-TRUNCINLINES' then GlobalOpt~TruncInLines = .true
      otherwise do
         say 'Error: Invalid command line option.'
         return 12
         end
      end
   end

/* give program banner */
if GlobalOpt~Verbose then do
   say
   say 'OCube by W. David Ashley - dashley@us.ibm.com'
   say 'Version' OCube_version OCube_vdate
   say 'Copyright (C) Rexx Language Association. All Rights Reserved.'
   say
   say 'This software is subject to the terms of the Common Public License. You'
   say 'must accept the terms of this license to use this software. Refer to'
   say 'the http://www.oorexx.org/license.html page for more information.'
   say
   end

/* read in the procedure file */
if ProcFileName~left(1) = '{' then do
   ProcFile = .array~new
   parse var ProcFileName '{' ProcFileName '}'
   ProcFile[1] = ProcFileName
   end
else if ProcFileName~translate = 'QUEUE' then do
   ProcFile = .array~new
   do while queued() > 0
      parse pull procline
      ProcFile[ProcFile~items + 1] = procline
      end
   end
else do
   x = .stream~new(ProcFileName)
   if x~query('exists') = '' then do
      say 'Error:' ProcFileName 'does not exist.'
      return 8
      end
   retc = x~open('read')
   ProcFile = x~arrayin
   if ProcFile[ProcFile~items] = '' then,
   ProcFile~remove(ProcFile~items)
   x~close
   end
if ProcFile~items = 0 then do
   say 'Error: Procedure file empty.'
   return 8
   end

/* read in the target file */
x = .stream~new(TargetFileName)
if x~query('exists') = '' then do
   say 'Error:' TargetFileName 'does not exist.'
   return 8
   end
retc = x~open('read')
TFile = x~arrayin
if TFile[TFile~items] = '' then TFile~remove(self~TFile~items)
x~close

/* create the backup file if necessary */
if BackupFileName <> '' then do
   x = .stream~new(BackupFileName)
   retc = x~open('write replace')
   x~arrayout(TFile)
   x~close
   end

/* convert the target file contents from a .array to a .list */
Lines = .list~new()
do x over TFile
   Lines~insert(x)
   end
if GlobalOpt~TruncInLines then do line over Lines
   /* truncate trailing spaces, tabs, CR and LF */
   do while (' ' || x2c('09') || x2c('0A') || x2c('0D'))~pos(line~right(1)) > 0
      line = line~left(line~length() - 1)
      end
   end

/* process each line of the procedure file */
procline = ''
do p = ProcFile~first() to ProcFile~last
   procline = (procline ProcFile[p])~strip()
   /* check for continuation character */
   if procline~right(1) = ',' then do
      procline = procline~left(procline~length - 1)
      iterate
      end
   retc = ExecProcLine(p, procline, Lines, GlobalOpt)
   if retc > 4 then leave
   procline = ''
   end

/* if no error then overwrite target file with changes */
say
if GlobalOpt~Changes > 0 & retc <= 4 then do
   x = .stream~new(TargetFileName)
   retc = x~open('write replace')
   x~arrayout(Lines~makearray)
   x~close
   say 'Info: Number of changes to the target file was' GlobalOpt~Changes'.'
   end
else do
   say 'Info: No changes made to the target file.'
   end

/* done */
return 0


/*----------------------------------------------------------------------------*/
/* Routine ExecProcStmt - parse and execute a procedure file statement        */
/*----------------------------------------------------------------------------*/

::routine ExecProcLine public
use arg p, procline, Lines, GlobalOpt
/* p         = the procedure file line number         */
/* procline  = the procedure file line to be executed */
/* Lines     = the lines from the input target file   */
/* GlobalOpt = the global options class               */
if GlobalOpt~Verbose | GlobalOpt~Pause then say procline
/* test for a comment line */
procline = procline~strip()
if procline = '' | procline~left(1) = '*' | procline~left(2) = '--' then,
 return 0
/* get the initial token from the procline */
parse var procline Verb procline
Verb = Verb~translate
/* call the class corresponding to the token */
select
   when Verb = 'ONERROR' then do
      x = .ApplyOnError~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'ADDLINE' | Verb = 'AL' then do
      x = .AddLine~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'DELLINE' | Verb = 'DL' then do
      x = .DelLine~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'REPLINE' | Verb = 'RL' then do
      x = .RepLine~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'COMMENTLINE' | Verb = 'CL' then do
      x = .CmtLine~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'ADDSTRING' | Verb = 'AS' then do
      x = .AddString~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'REPSTRING' | Verb = 'RS' then do
      x = .RepString~new(GlobalOpt, p, procline, Lines)
      end
   when Verb = 'DELSTRING' | Verb = 'DS' then do
      x = .DelString~new(GlobalOpt, p, procline, Lines)
      end
   otherwise do
      say 'Error: Line' p': Invalid procedure statement.'
      if GlobalOpt~OnError = 'C' then return 4
      else return 12
      end
   end
if GlobalOpt~Pause then do
   call charout , 'Hit any key when ready to continue: '
   templine = linein()
   end
return x~ErrorCode


/*----------------------------------------------------------------------------*/
/* Routine LexicalParser - simple lexical parser                              */
/*                         This lexical analyzer in based on REXX syntax. It  */
/*                         is a very simple parser and does NOT cover all     */
/*                         possibilities. But it does a good enough job for   */
/*                         support of OCube.                                  */
/*----------------------------------------------------------------------------*/

::routine LexicalParser public
use arg stmt
/* stmt = procedure file statement (or a portion of it) to parse */
/* set up some variables */
alpha = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
ext_alpha = alpha || '_'
numbers = '0123456789'
decnum = numbers || '.'
alphanum = alpha || numbers
ext_alphanum = ext_alpha || numbers
symbols = '(){}[]<>+-*/;:=|&\,.~%'
/* create the token to return */
token = .Token~new()
if stmt = '' then return token
/* strip any leading spaces and tabs from the procedure statement */
do while (' ' || x2c('09'))~pos(stmt~left(1)) > 0
   stmt = stmt~right(stmt~length() - 1)
   end
if stmt = '' then return token
/* strip any trailing white space from the procedure statement */
do while stmt~right(1)~pos(' ' || x2c('09') || x2c('0A') || x2c('0D')) > 0
   stmt = stmt~left(stmt~length() - 1)
   end
if stmt = '' then return token
/* get the first character of the next token */
token~Value = stmt~left(1)
stmt = stmt~substr(2)
/* determine the token type and value based on the first character */
select
   when symbols~pos(token~Value) > 0 then do
      /* get operators */
      /* note: the order is important here! */
      token~Type = 'OPERATOR'
      if token~Value = '|' & stmt~left(1) = '|' then do
         token~value = '||'
         stmt = stmt~substr(2)
         end
      else if token~Value = '&' & stmt~left(1) = '&' then do
         token~Value = '&&'
         stmt = stmt~substr(2)
         end
      else if token~Value = '<' & stmt~left(1) = '=' then do
         token~Value = '<='
         stmt = stmt~substr(2)
         end
      else if token~Value = '<' & stmt~left(1) = '>' then do
         token~Value = '<>'
         stmt = stmt~substr(2)
         end
      else if token~Value = '<' & stmt~left(2) = '<=' then do
         token~Value = '<<='
         stmt = stmt~substr(3)
         end
      else if token~Value = '<' & stmt~left(1) = '<' then do
         token~Value = '<<'
         stmt = stmt~substr(2)
         end
      else if token~Value = '>' & stmt~left(1) = '=' then do
         token~Value = '>='
         stmt = stmt~substr(2)
         end
      else if token~Value = '>' & stmt~left(2) = '>=' then do
         token~Value = '>>='
         stmt = stmt~substr(3)
         end
      else if token~Value = '>' & stmt~left(1) = '>' then do
         token~Value = '>>'
         stmt = stmt~substr(2)
         end
      else if token~Value = '\' & stmt~left(2) = '==' then do
         token~Value = '\=='
         stmt = stmt~substr(3)
         end
      else if token~Value = '\' & stmt~left(1) = '=' then do
         token~Value = '\='
         stmt = stmt~substr(2)
         end
      else if token~Value = '\' & stmt~left(2) = '<<' then do
         token~Value = '\<<'
         stmt = stmt~substr(3)
         end
      else if token~Value = '\' & stmt~left(1) = '<' then do
         token~Value = '\<'
         stmt = stmt~substr(2)
         end
      else if token~Value = '\' & stmt~left(2) = '>>' then do
         token~Value = '\>>'
         stmt = stmt~substr(3)
         end
      else if token~Value = '\' & stmt~left(1) = '>' then do
         token~Value = '\>'
         stmt = stmt~substr(2)
         end
      else if token~Value = '*' & stmt~left(1) = '*' then do
         token~Value = '**'
         stmt = stmt~substr(2)
         end
      else if token~Value = '/' & stmt~left(1) = '/' then do
         token~Value = '//'
         stmt = stmt~substr(2)
         end
      else if token~Value = '=' & stmt~left(1) = '=' then do
         token~Value = '=='
         stmt = stmt~substr(2)
         end
      else nop
      end
   when alpha~pos(token~Value) > 0 then do
      /* get keywords */
      token~Type = 'KEYWORD'
      do while ext_alphanum~pos(stmt~left(1)) > 0
         token~Value = token~value || stmt~left(1)
         stmt = stmt~substr(2)
         end
      token~Value = token~Value~translate() /* upper-case all keywords */
      end
   when numeric~pos(token~Value) > 0 then do
      /* get numbers */
      /* note: does not parse numbers in scientific notation format! */
      token~Type = 'NUMBER'
      do while decnum~pos(stmt~left(1)) > 0
         token~Value = token~Value || stmt~left(1)
         stmt = stmt~substr(2)
         end
      end
   when token~Value = '"' then do
      /* get double-quoted {"} delimited strings */
      /* note that there is no interpretation of the quoted string */
      token~Type = 'STRING'
      token~Value = '' /* the beginning & end quotes are NOT included */
      do while stmt~left(1) <> '"'
         if stmt~length() = 0 then do
            token~ErrorMsg = 'Unexpected end of statement encountered during parsing.'
            token~Error = .true
            return token
            end
         token~Value = token~Value || stmt~left(1)
         stmt = stmt~substr(2)
         end
      stmt = stmt~substr(2)
      end
   when token~Value = "'" then do
      /* get single-quoted {'} delimited strings */
      /* note that there is no interpretation of the quoted string */
      token~Type = 'STRING'
      token~Value = '' /* the beginning & end quotes are NOT included */
      do while stmt~left(1) <> "'"
         if stmt~length() = 0 then do
            token~ErrorMsg = 'Unexpected end of statement encountered during parsing.'
            token~Error = .true
            return token
            end
         token~Value = token~Value || stmt~left(1)
         stmt = stmt~substr(2)
         end
      stmt = stmt~substr(2)
      end
   otherwise do
      token~ErrorMsg = 'Invalid symbol "'token~Value || stmt'" encountered parsing statement.'
      token~Error = .true
      end
   end
token~Stmt = Stmt
return token


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class Token - the LexicalParser uses this class to return tokens           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Token public
::method Value              attribute  /* String value of the token           */
::method Type               attribute  /* Token type                          */
                                       /* 'OPERATOR' = operator               */
                                       /* 'KEYWORD' = potential keyword       */
                                       /* 'NUMBER' = number                   */
                                       /* 'STRING' = quoted string            */
                                       /* 'NULL' = no token found or error    */
::method Error              attribute  /* Parsing error?                      */
                                       /* .true = parsing error               */
                                       /* .false = no error                   */
::method ErrorMsg           attribute  /* Error message if Error = .true      */
::method Stmt               attribute  /* statement after token taken         */

/*----------------------------------------------------------------------------*/
/* Method init - initialize the object                                        */
/*----------------------------------------------------------------------------*/

::method init
self~Value = ''
self~Type = 'NULL'
self~Error = .false
self~ErrorMsg = ''
self~Stmt = ''
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class GlobalOpt - global options class                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class GlobalOpt public

::method Pause              attribute  /* Pause after each command?           */
                                       /* .true = pause after each command    */
                                       /* .false = do not pause               */
::method Changes            attribute  /* Number of changes to target file    */
::method OnError            attribute  /* Exit on error?                      */
                                       /* 'S' = stop if error code GT 4       */
                                       /* 'C' = otherwise continue            */
::method EnvName            attribute  /* REXX environment name               */
::method Debug              attribute  /* Debug mode?                         */
                                       /* .true = execute debug code          */
                                       /* .false = do not execute dubug code  */
::method Verbose            attribute  /* Verbose output?                     */
                                       /* .true = verbose output mode         */
                                       /* .false = terse output mode          */
::method TruncInLines       attribute  /* Truncate target file input lines?   */
                                       /* .true = truncate trailing spaces    */
                                       /* .false = do not truncate lines      */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
self~Pause = .false
self~Changes = 0
self~OnError = 'S'
parse source env .
/* Still using OS/2? Get a new life */
if env = 'OS/2' then self~EnvName = 'OS2ENVIRONMENT'
else self~EnvName = 'ENVIRONMENT'
self~Debug = .false
self~Verbose = .false
self~TruncInLines = .false
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class ProcStmt - parent class for all procedure statement classes          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class ProcStmt public

::method GlobalOpt          attribute  /* Global options class reference      */
::method LineNum            attribute  /* current procedure file line number  */
::method Stmt               attribute  /* current procedure file line         */
::method ErrorCode          attribute  /* Statement error code                */
                                       /* 0 = no error                        */
                                       /* 4 = warning                         */
                                       /* 8 = error                           */
                                       /* 12 = really bad error               */
::method ErrorMsg           attribute  /* error message                       */
::method Lines              attribute  /* Target file lines                   */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
use arg GlobalOpt, LineNum, Stmt
self~ErrorCode = 0
self~ErrorMsg = ''
return

/*----------------------------------------------------------------------------*/
/* Method SetError - set the error code and message                           */
/*----------------------------------------------------------------------------*/
::method SetError
expose ErrorCode ErrorMsg LineNum GlobalOpt
use arg ErrorCode, errMsg
/* ErrorCode = error code to be set */
/* ErrMsg    = error message        */
if ErrorCode = 0 then return
else if ErrorCode = 4 then ,
 ErrorMsg = 'Warning: Line' LineNum':' errMsg
else ErrorMsg = 'Error: Line' LineNum':' errMsg
say ErrorMsg
/* if OnError option is CONTINUE then reset the ErrorCode */
if GlobalOpt~OnError = 'C' then do
   if ErrorCode > 4 then ErrorCode = 4
   end
return

/*----------------------------------------------------------------------------*/
/* Method EnvSubstitute - do environment variable substitution                */
/*----------------------------------------------------------------------------*/
::method EnvSubstitute
expose GlobalOpt
use arg repstr, dlm
/* repstr = target string                              */
/* dlm    = delimiter to be used for environment names */
if dlm = '' then return repstr
parse var repstr front (dlm) name (dlm) back
do while name <> ''
   repstr = front || value(name, , GlobalOpt~EnvName) || back
   parse var repstr front (dlm) name (dlm) back
   end
return repstr

/*----------------------------------------------------------------------------*/
/* Method FindMatches - find all lines matching the specified string          */
/*----------------------------------------------------------------------------*/
::method FindMatches
use arg MatchStr, Lines, MatchCase, AnyPos
/* MatchStr    = string to be searched for                       */
/* TargetLines = list of lines to be searched                    */
/* MatchCase   = .true if a case-sensitive search should be used */
/* AnyPos      = .true if the MatchStr can be in any position    */
MatchLines = .array~new()
if Lines~items = 0 then return MatchLines
if MatchCase then MatchString = MatchStr
else MatchString = MatchStr~translate()
x = Lines~first()
do while x <> .nil
   if MatchCase then TestLine = Lines[x]
   else TestLine = Lines[x]~translate()
   if AnyPos then do
      if TestLine~pos(MatchString) > 0 then MatchLines[MatchLines~items + 1] = x
      end
   else do
      if TestLine~left(MatchString~length) = MatchString then MatchLines[MatchLines~items + 1] = x
      end
   x = Lines~next(x)
   end
return MatchLines

/*----------------------------------------------------------------------------*/
/* Method LogChange - log the change to the target file                       */
/*----------------------------------------------------------------------------*/
::method LogChange
expose GlobalOpt
use arg action, old, new
/* action = info action message  */
/* old    = old target file line */
/* new    = new target file line */
if old <> new & GlobalOpt~Verbose then do
   say action
   say '  old: "'old'"'
   say '  new: "'new'"'
   end
if old <> new then do
   self~GlobalOpt~Changes = self~GlobalOpt~Changes + 1
   end
return

/*----------------------------------------------------------------------------*/
/* Method GetToken - get and check the next token from the procedure statement*/
/*----------------------------------------------------------------------------*/
::method GetToken
use arg Stmt, expected_token_type
/* Stmt               = procedure file statement to be parsed */
/* expected_toke_type = expected type for the next token      */
token = LexicalParser(Stmt)
self~Stmt = token~stmt
if token~Error then do
   self~SetError(8, token~ErrorMsg)
   end
if token~Type = 'NULL' then return token
if token~Type <> expected_token_type then do
   self~SetError(8, 'Unexpected token encountered.')
   token~Error = .true
   if self~GlobalOpt~Debug then say stmt
   end
return token


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class ApplyOnError - class for the ONERROR statement                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class ApplyOnError subclass ProcStmt public

::method NewOnError         attribute  /* new OnError setting (GlobalOpt)     */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---ONERROR---+---CONTINUE---+----------------------------------------->< */
/*                +---STOP-------+                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
/* get the new onerror option */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
select
   when token~Value = 'STOP' then self~NewOnError = 'S'
   when token~Value = 'CONTINUE' then self~NewOnError = 'C'
   otherwise do
      self~SetError(8, 'Invalid option on ONERROR statement.')
      return
      end
   end
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
self~GlobalOpt~OnError = self~NewOnError
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class AddLine - class for the ADDLINE statement                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class AddLine subclass ProcStmt public

::method NewLine            attribute  /* new line to be added                */
::method AfterBefore        attribute  /* add line after or before srchline   */
                                       /* '' - unconditionally add the line   */
                                       /* 'A' - add after srchline            */
                                       /* 'B' - add before srchline           */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method AddPos             attribute  /* where to add line if no match found */
                                       /* 'T' - add at the top of the file    */
                                       /* 'B' - add at the bottom of the file */
                                       /* 'D' - do not add the new line       */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~NewLine = ''
self~AfterBefore = ''
self~MatchStr = ''
self~AddPos = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---ADDLINE---+---'newline'-----------------------------------------> */
/*      +---AL--------+                                                       */
/*                                                                            */
/* >----+-----------------------------+----+-------------------+------------> */
/*      +---AFTER----+---'matchstr'---+    +---CASESENSITIVE---+              */
/*      +---BEFORE---+                                                        */
/*                                                                            */
/* >----+---------------+----+------------+----+----------------------+---->< */
/*      +---ADDTOP------+    +---ANYPOS---+    +---RSDLM---'dlmstr'---+       */
/*      +---ADDBOTTOM---+                                                     */
/*      +---DONTADD-----+                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid option on ADDLINE statement.'
/* get the new line to add */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~NewLine = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'AFTER' | token~value = 'BEFORE' then do
         if self~AfterBefore <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~AfterBefore = token~value~left(1)
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         self~MatchStr = token~value
         end
      when token~value = 'ADDTOP' | token~value = 'ADDBOTTOM' |,
       token~Value = 'DONTADD' then do
         if self~AddPos <> '' then do
            self~SetError(8, msg1)
            return
            end
         if token~value = 'ADDTOP' then self~AddPos = 'T'
         else if token~value = 'ADDBOTTOM' then self~AddPos = 'B'
         else self~AddPos = 'D'
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg1)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg1)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AddPos = '' then self~AddPos = 'T'
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines NewLine AfterBefore MatchStr AddPos AnyPos RsDlm MatchCase
/* do any string substutution necessary */
if RsDlm <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   NewLine = self~EnvSubstitute(NewLine, RsDlm)
   end
/* search for string matches in the target file */
MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
/* perform the function */
if MatchLines~items = 0 then do
   if AddPos = 'T' then do
      Lines~insert(NewLine, .nil)
      self~LogChange('Added new line:', '', NewLine)
      end
   else if AddPos = 'B' then do
      Lines~insert(NewLine)
      self~LogChange('Added new line:', '', NewLine)
      end
   else nop /* dont add */
   end
else do
   if AfterBefore = 'A' then do
      Lines~insert(NewLine, MatchLines[1])
      self~LogChange('Added new line:', '', NewLine)
      end
   else if AfterBefore = 'B' then do
      if MatchLines[1] = Lines~first then Lines~insert(NewLine, .nil)
      else Lines~insert(NewLine, Lines~previous(MatchLines[1]))
      self~LogChange('Added new line:', '', NewLine)
      end
   else do /* unconditionally add the line */
      if AddPos = 'T' then do
         Lines~insert(NewLine, .nil)
         self~LogChange('Added new line:', '', NewLine)
         end
      else if AddPos = 'B' then do
         Lines~insert(NewLine)
         self~LogChange('Added new line:', '', NewLine)
         end
      else nop /* dont add */
      end
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class DelLine - class for the DELLINE statement                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class DelLine subclass ProcStmt public

::method MatchStr           attribute  /* string to be matched in srchline    */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method DelLines           attribute  /* lines to be deleted                 */
                                       /* 'F' - first line found              */
                                       /* 'L' - last line found               */
                                       /* 'A' - all lines matching MatchStr   */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~MatchStr = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
self~DelLines = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---DELLINE---+---'matchstr'---+-------------------+----------------> */
/*      +---DL--------+                +---CASESENSITIVE---+                  */
/*                                                                            */
/* >----+-----------+----+----------------------+----+------------+-------->< */
/*      +---FIRST---+    +---RSDLM---'dlmstr'---+    +---ANYPOS---+           */
/*      +---LAST----+                                                         */
/*      +---ALL-----+                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid option on DELLINE statement.'
/* get the match string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~MatchStr = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'FIRST' | token~value = 'LAST' |,
       token~value = 'ALL' then do
         if self~DelLines <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~DelLines = token~value~left(1)
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg1)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg1)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg1)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~DelLines = '' then self~DelLines = 'F'
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines MatchStr AddPos AnyPos RsDlm MatchCase DelLines
/* do any string substutution necessary */
if RsDlm <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   end
/* search for string matches in the target file */
MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
/* perform the function */
if MatchLines~items = 0 then return
select
   when DelLines = 'F' then do
      x = MatchLines~first
      self~LogChange('Removed line:', Lines[MatchLines[x]], '')
      Lines~remove(MatchLines[x])
      end
   when DelLines = 'L' then do
      x = MatchLines~last
      self~LogChange('Removed line:', Lines[MatchLines[x]], '')
      Lines~remove(MatchLines[x])
      end
   otherwise do x over MatchLines /* DelLines = 'A' */
      self~LogChange('Removed line:', Lines[x], '')
      Lines~remove(x)
      end
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class RepLine - class for the REPLINE statement                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class RepLine subclass ProcStmt public

::method NewLine            attribute  /* new line to be added                */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method RepLines           attribute  /* lines to be replaced                */
                                       /* 'F' - first line found              */
                                       /* 'L' - last line found               */
                                       /* 'A' - all line matching MatchStr    */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~NewLine = ''
self~MatchStr = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
self~RepLines = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---REPLINE---+---'matchstr'---WITH---'newline'---------------------> */
/*      +---RL--------+                                                       */
/*                                                                            */
/* >----+-------------------+----+------------+---+----------------------+--> */
/*      +---CASESENSITIVE---+    +---ANYPOS---+   +---RSDLM---'dlmstr'---+    */
/*                                                                            */
/* >----+-----------+------------------------------------------------------>< */
/*      +---FIRST---+                                                         */
/*      +---LAST----+                                                         */
/*      +---ALL-----+                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid keyword on REPLINE statement.'
msg2 = 'Invalid option on REPLINE statement.'
/* get the match string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~MatchStr = token~value
/* get the WITH keyword */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
if token~value <> 'WITH' then do
   self~SetError(8, msg1)
   return
   end
/* get the new line */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~NewLine = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'FIRST' | token~value = 'LAST' |,
       token~value = 'ALL' then do
         if self~RepLines <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~RepLines = token~value~left(1)
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg2)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg2)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
if self~RepLines = '' then self~RepLines = 'F'
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines NewLine MatchStr AnyPos RsDlm MatchCase RepLines
/* do any string substutution necessary */
if RsDlm <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   NewLine = self~EnvSubstitute(NewLine, RsDlm)
   end
/* search for string matches in the target file */
MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
/* perform the function */
if MatchLines~items = 0 then return
if RepLines = 'F' then do
   x = MatchLines[MatchLines~first]
   self~LogChange('Replaced line:', Lines[x], NewLine)
   Lines[x] = NewLine
   end
else if RepLines = 'L' then do
   x = MatchLines[MatchLines~last]
   self~LogChange('Replaced line:', Lines[x], NewLine)
   Lines[x] = NewLine
   end
else do x over MatchLines /* RepLines = 'A' */
   self~LogChange('Replaced line:', Lines[x], NewLine)
   Lines[x] = NewLine
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class CmtLine - class for the COMMENTLINE statement                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class CmtLine subclass ProcStmt public

::method CmtStr             attribute  /* comment character string            */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method CmtLines           attribute  /* lines to be commented               */
                                       /* 'F' - first line found              */
                                       /* 'L' - last line found               */
                                       /* 'A' - all line matching MatchStr    */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~CmtStr = ''
self~MatchStr = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
self~CmtLines = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---COMMENTLINE---+---'matchstr'---WITH---'cmtstring'---------------> */
/*      +---CL------------+                                                   */
/*                                                                            */
/* >----+-------------------+----+------------+---+----------------------+--> */
/*      +---CASESENSITIVE---+    +---ANYPOS---+   +---RSDLM---'dlmstr'---+    */
/*                                                                            */
/* >----+-----------+------------------------------------------------------>< */
/*      +---FIRST---+                                                         */
/*      +---LAST----+                                                         */
/*      +---ALL-----+                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid keyword on COMMENTLINE statement.'
msg2 = 'Invalid option on COMMENTLINE statement.'
/* get the match string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~MatchStr = token~value
/* get the WITH keyword */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
if token~value <> 'WITH' then do
   self~SetError(8, msg1)
   return
   end
/* get the comment string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~CmtStr = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'FIRST' | token~value = 'LAST' |,
       token~value = 'ALL' then do
         if self~CmtLines <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~CmtLines = token~value~left(1)
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg2)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg2)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
if self~CmtLines = '' then self~CmtLines = 'F'
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines MatchStr AnyPos RsDlm MatchCase CmtLines CmtStr
/* do any string substutution necessary */
if RsDlm <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   end
/* search for string matches in the target file */
MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
/* perform the function */
if MatchLines~items = 0 then return
if CmtLines = 'F' then do
   x = MatchLines[MatchLines~first]
   self~LogChange('Commented line:', Lines[x], CmtStr Lines[x])
   Lines[x] = CmtStr Lines[x]
   end
else if CmtLines = 'L' then do
   x = MatchLines[MatchLines~last]
   self~LogChange('Commented line:', Lines[x], CmtStr Lines[x])
   Lines[x] = CmtStr Lines[x]
   end
else do x over MatchLines /* CmtLines = 'A' */
   self~LogChange('Commented line:', Lines[x], CmtStr Lines[x])
   Lines[x] = CmtStr Lines[x]
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class AddString - class for the ADDSTRING string                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class AddString subclass ProcStmt public

::method NewString          attribute  /* new string to be added              */
::method AfterBefore        attribute  /* add string after or before SrchStr  */
                                       /* '' - unconditionally add the string */
                                       /* 'A' - add after SrchStr             */
                                       /* 'B' - add before SrchStr            */
::method SrchString         attribute  /* string to be matched in srchline    */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method AddPos             attribute  /* where to add line if no match found */
                                       /* 'T' - add at the top of the file    */
                                       /* 'B' - add at the bottom of the file */
                                       /* 'D' - do not add the new line       */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~NewString = ''
self~SrchString = ''
self~AfterBefore = ''
self~MatchStr = ''
self~AddPos = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---ADDSTRING---+---'newstring'---IN---'matchstr'-------------------> */
/*      +---AS----------+                                                     */
/*                                                                            */
/* >----+----------------------------+----+-------------------+-------------> */
/*      +---AFTER----+---'srchstr'---+    +---CASESENSITIVE---+               */
/*      +---BEFORE---+                                                        */
/*                                                                            */
/* >----+---------------+----+------------+----+----------------------+---->< */
/*      +---ADDTOP------+    +---ANYPOS---+    +---RSDLM---'dlmstr'---+       */
/*      +---ADDBOTTOM---+                                                     */
/*      +---DONTADD-----+                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid keyword on ADDSTRING statement.'
msg2 = 'Invalid option on ADDSTRING statement.'
/* get the new line to add */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~NewString = token~value
/* get the IN keyword */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
if token~value <> 'IN' then do
   self~SetError(8, msg1)
   return
   end
/* get the matchstr string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~MatchStr = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'AFTER' | token~value = 'BEFORE' then do
         if self~AfterBefore <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AfterBefore = token~value~left(1)
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         self~SrchString = token~value
         end
      when token~value = 'ADDTOP' | token~value = 'ADDBOTTOM' |,
       token~Value = 'DONTADD' then do
         if self~AddPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         if token~value = 'ADDTOP' then self~AddPos = 'T'
         else if token~value = 'ADDBOTTOM' then self~AddPos = 'B'
         else self~AddPos = 'D'
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg2)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg2)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AddPos = '' then self~AddPos = 'T'
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
if self~SrchString = '' then self~SrchString = MatchString
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines NewString SrchString AfterBefore MatchStr AddPos AnyPos RsDlm,
 MatchCase
/* do any string substutution necessary */
if RsDlm <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   NewString = self~EnvSubstitute(NewString, RsDlm)
   SrchString = self~EnvSubstitute(SrchString, RsDlm)
   end
/* search for string matches in the target file */
MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
/* perform the function */
if MatchLines~items = 0 then do
   if AddPos = 'T' then do
      Lines~insert(MatchStr || NewString, .nil)
      self~LogChange('Added new line:', '', MatchStr || NewString)
      end
   else if AddPos = 'B' then do
      Lines~insert(MatchStr || NewString)
      self~LogChange('Added new line:', '', MatchStr || NewString)
      end
   else nop /* dont add */
   end
else do x over MatchLines
   if AfterBefore = 'A' then do
      OldLine = Lines[x]
      y = Lines[x]~pos(SrchString)
      if y > 0 then do
         Lines[x] = Lines[x]~substr(1, y + SrchString~length() - 1) ||,
          NewString || Lines[x]~substr(y + SrchString~length())
         end
      else do
         Lines[x] = Lines[x] || NewString
         end
      self~LogChange('Added string to line:', OldLine, Lines[x])
      end
   else if AfterBefore = 'B' then do
      OldLine = Lines[x]
      y = Lines[x]~pos(SrchString)
      if y > 0 then do
         Lines[x] = Lines[x]~substr(1, y - 1) ||,
          NewString || Lines[x]~substr(y)
         end
      else do
         y = Lines[x]~pos(MatchStr)
         Lines[x] = Lines[x]~substr(1, y + MatchStr~length() - 1) ||,
          NewString || Lines[x]~substr(y + MatchStr~length())
         end
      self~LogChange('Added string to line:', OldLine, Lines[x])
      end
   else do /* unconditionally add a new line */
      NewLine = MatchStr NewString
      if AddPos = 'T' then do
         Lines~insert(NewLine, .nil)
         self~LogChange('Added new line:', '', NewLine)
         end
      else if AddPos = 'B' then do
         Lines~insert(NewLine)
         self~LogChange('Added new line:', '', NewLine)
         end
      else nop /* dont add */
      end
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class RepString - class for the REPSTRING string                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class RepString subclass ProcStmt public

::method NewString          attribute  /* new string to be added              */
::method SrchString         attribute  /* string to be matched in srchline    */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~NewString = ''
self~SrchString = ''
self~MatchStr = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---REPSTRING---+---'srchstr'---WITH---'newstring'------------------> */
/*      +---RS----------+                                                     */
/*                                                                            */
/* >----+---------------------+----+-------------------+--------------------> */
/*      +---IN---'matchstr'---+    +---CASESENSITIVE---+                      */
/*                                                                            */
/* >----+------------+----+----------------------+------------------------->< */
/*      +---ANYPOS---+    +---RSDLM---'dlmstr'---+                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid keyword on REPSTRING statement.'
msg2 = 'Invalid option on REPSTRING statement.'
/* get the search string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~SrchString = token~value
/* get the WITH keyword */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
if token~value <> 'WITH' then do
   self~SetError(8, msg1)
   return
   end
/* get the replacement string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~NewString = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'IN' then do
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if self~MatchStr <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchStr = token~value
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg2)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if token~value <> '' then self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg2)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines NewString SrchString MatchStr AnyPos RsDlm MatchCase
/* do any string substutution necessary */
if RsDlm <> '' & MatchStr <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   NewString = self~EnvSubstitute(NewString, RsDlm)
   SrchString = self~EnvSubstitute(SrchString, RsDlm)
   end
/* search for string matches in the target file */
if MatchStr <> '' then do
   MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
   end
else do
   MatchLines = self~FindMatches(SrchString, Lines, MatchCase, .true)
   end
/* perform the function */
if MatchLines~items = 0 then return
do x over MatchLines
   OldLine = Lines[x]
   y = Lines[x]~pos(SrchString)
   if y > 0 then do
      Lines[x] = Lines[x]~substr(1, y - 1) || NewString ||,
       Lines[x]~substr(y + SrchString~length())
      self~LogChange('Replaced string in line:', OldLine, Lines[x])
      end
   end
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class DelString - class for the DELSTRING string                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class DelString subclass ProcStmt public

::method SrchString         attribute  /* string to be matched in srchline    */
::method MatchStr           attribute  /* string to be matched in srchline    */
::method AnyPos             attribute  /* where to search for matchstr        */
                                       /* .true - any position                */
                                       /* .false - beginning of the line      */
::method MatchCase          attribute  /* is search case sensitive?           */
                                       /* .true - match case                  */
                                       /* .false - ignore case                */
::method RsDlm              attribute  /* replacement string delimiter        */

/*----------------------------------------------------------------------------*/
/* Method Init - instance initialization                                      */
/*----------------------------------------------------------------------------*/
::method Init
expose GlobalOpt LineNum Stmt Lines
use arg GlobalOpt, LineNum, Stmt, Lines
/* GlobalOpt = global options class instance */
/* LineNum   = procedure file line number    */
/* Stmt      = procedure file statement      */
/* Lines     = target file .list of lines    */
self~init:super(GlobalOpt, LineNum, Stmt)
self~SrchString = ''
self~MatchStr = ''
self~AnyPos = ''
self~RsDlm = ''
self~MatchCase = ''
/* parse the procedure statement */
self~Parse
return

/*----------------------------------------------------------------------------*/
/* Method Parse - parse the statement and set all the instance variables      */
/*                                                                            */
/* Syntax diagram:                                                            */
/*                                                                            */
/* >>---+---DELSTRING---+---'srchstr'---------------------------------------> */
/*      +---DS----------+                                                     */
/*                                                                            */
/* >----+---------------------+----+-------------------+--------------------> */
/*      +---IN---'matchstr'---+    +---CASESENSITIVE---+                      */
/*                                                                            */
/* >----+------------+----+----------------------+------------------------->< */
/*      +---ANYPOS---+    +---RSDLM---'dlmstr'---+                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
::method Parse
msg1 = 'Invalid keyword on DELSTRING statement.'
msg2 = 'Invalid option on DELSTRING statement.'
/* get the search string */
token = self~GetToken(self~Stmt, 'STRING')
if self~ErrorCode > 4 then return
self~SrchString = token~value
/* process all optional options (all these can be in any order) */
token = self~GetToken(self~Stmt, 'KEYWORD')
if self~ErrorCode > 4 then return
do while token~value <> ''
   select
      when token~value = 'IN' then do
         token = self~GetToken(self~Stmt, 'STRING')
         if self~ErrorCode > 4 then return
         if self~MatchStr <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchStr = token~value
         end
      when token~value = 'ANYPOS' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~AnyPos = .true
         end
      when token~value = 'CASESENSITIVE' then do
         if self~AnyPos <> '' then do
            self~SetError(8, msg2)
            return
            end
         self~MatchCase = .true
         end
      when token~value = 'RSDLM' then do
         if self~RsDlm <> '' then do
            self~SetError(8, msg2)
            return
            end
         token = self~GetToken(self~Stmt, 'STRING')
         if token~value <> '' then self~RsDlm = token~value~left(1)
         self~RsDlm = token~value~left(1)
         end
      otherwise do
         self~SetError(8, msg2)
         return
         end
      end
   token = self~GetToken(self~Stmt, 'KEYWORD')
   if self~ErrorCode > 4 then return
   end
/* set defaults */
if self~AnyPos = '' then self~AnyPos = .false
if self~MatchCase = '' then self~MatchCase = .false
/* now execute the statement */
self~Execute
return

/*----------------------------------------------------------------------------*/
/* Method Execute - execute the instance variables against the list of lines  */
/*----------------------------------------------------------------------------*/
::method Execute
expose Lines SrchString MatchStr AnyPos RsDlm MatchCase
/* do any string substutution necessary */
if RsDlm <> '' & MatchStr <> '' then do
   MatchStr = self~EnvSubstitute(MatchStr, RsDlm)
   SrchString = self~EnvSubstitute(SrchString, RsDlm)
   end
/* search for string matches in the target file */
if MatchStr <> '' then do
   MatchLines = self~FindMatches(MatchStr, Lines, MatchCase, AnyPos)
   end
else do
   MatchLines = self~FindMatches(SrchString, Lines, MatchCase, .true)
   end
/* perform the function */
if MatchLines~items = 0 then return
do x over MatchLines
   OldLine = Lines[x]
   y = Lines[x]~pos(SrchString)
   if y > 0 then do
      Lines[x] = Lines[x]~delstr(y, SrchString~length())
      self~LogChange('Removed string in line:', OldLine, Lines[x])
      end
   end
return


