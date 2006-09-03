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
/****************************************************************************/
/* Name: REXXTRY.CMD                                                        */
/* Type: Object REXX Script                                                 */
/*                                                                          */
/*  IBM Object REXX                                                         */
/*                                                                          */
/* Description: SAA-portable REXXTRY procedure   07/10/2002 Version 1.08    */
/*                                                                          */
/*  Owned by IBM REXX Development.                                          */
/*  Loosely derived from an ancient formulation of Mike Cowlishaw.          */
/*                                                                          */
/*  This procedure lets you interactively try REXX statements.              */
/*  If you run it with no parameter, or with a question mark                */
/*  as a parameter, it will briefly describe itself.                        */
/*  You may also enter a REXX statement directly on the command line        */
/*  for immediate execution and exit.  Example:  rexxtry call show          */
/*                                                                          */
/*  Enter 'call show' to see user variables provided by REXXTRY.            */
/*  Enter '=' to repeat your previous statement.                            */
/*  Enter '?' to invoke system-provided online help for REXX.               */
/*  The subroutine named 'sub' can be CALLed or invoked as 'sub()'.         */
/*  REXXTRY can be run recursively with CALL.                               */
/*                                                                          */
/*  Except for the signal instructions after a syntax error, this           */
/*  procedure is an example of structured programming.                      */
/*  The 'clear' routine illustrates system-specific SAA-portable coding.    */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/* DISCLAIMER OF WARRANTIES.  The following [enclosed]                      */
/* code is sample code created by IBM Corporation. This                     */
/* sample code is not part of any standard or IBM                           */
/* product and is provided to you solely for the                            */
/* purpose of assisting you in the development of your                      */
/* applications.  The code is provided "AS IS", without                     */
/* warranty of any kind.  IBM shall not be liable for                       */
/* any damages arising out of your use of the sample                        */
/* code,  even if they have been advised of the                             */
/* possibility of such damages.                                             */
/*                                                                          */
/****************************************************************************/

parse arg argrx                                /* Get user's arg string.    */
call house                                     /* Go do some housekeeping.  */

select                                         /* 3 modes of operation...   */
  when argrx = '?' then                        /*   1.  Tell user how.      */
    call tell
  when argrx = ''  then do                     /*   2.  Interactive mode.   */
    call intro
    call main
  end
  otherwise                                    /*   3.  One-liner and exit. */
    push argrx
    call main
end

done:
  exit                                         /* The only exit.            */

house:                                         /* Housekeeping.             */
  parse version version                        /* Fill-in 2 user variables. */
  parse source source
  parse source sysrx . procrx .                /* Get system & proc names.  */
  remindrx = "Enter 'exit' to end."            /* How to escape rexxtry.    */
  helprx = ''                                  /* Null if not CMS or OS/2.  */
  if sysrx = 'CMS' | sysrx = 'OS/2'            /*   Extra reminder for CMS  */
    then helprx = '   ',                       /*     or OS/2.              */
    "  Or '?' for online REXX help."           /*   Not used in intro.      */
  promptrx = ''                                /* Null if not one-liner.    */
  if argrx<>'' then
    promptrx = procrx' '                       /*   Name part of user line. */
  select                                       /* System-specific...        */
    when sysrx = 'OS/2' | abbrev(sysrx,  'Windows') then
      posrx = lastpos('\', procrx)             /*   Find name separator.    */
    when sysrx = 'LINUX' | sysrx = 'AIX' | sysrx = 'SUNOS' then
      posrx = lastpos('/', procrx)             /*   Find name separator.    */
    otherwise
      posrx = 0                                /*   No name separator.      */
  end
  procrx = substr(procrx, posrx+1)             /* Pick up the proc name.    */
  temprx = ' 'procrx' on 'sysrx                /* Make border...            */
    posrx = 69-length(temprx)                  /*   where to overlay name,  */
    bordrx = copies('.', 68)                   /*   background of periods,  */
    bordrx =,                                  /*   name right-adjusted.    */
      overlay(temprx, bordrx, posrx)
  save = ''                                    /* Don't save user input.    */
  trace = 'Off'                                /* Init user trace variable. */
  return result                                /* Preserve result contents. */

tell:
  call clear
  do irx = 1 to 20                             /* Tell about rexxtry by     */
    say sourceline(irx)                        /*   displaying the prolog.  */
  end
  return result                                /* Preserve result contents. */

clear:
  select                                       /* SAA-portable code.        */
    when sysrx = 'CMS' then
      'VMFCLEAR'                               /* system to clear screen    */
    when sysrx = 'OS/2' | abbrev(sysrx,  'Windows') then
      'CLS'                                    /* system to clear screen    */
    when sysrx = 'LINUX' | sysrx = 'AIX' | sysrx = 'SUNOS' then
      'clear'                                  /* system to clear screen    */
    otherwise nop                              /* No such command available */
  end; say
  return result                                /* Preserve result contents. */

intro:                                         /* Display brief             */
  say version                                  /*   introductory            */
  say '  'procrx' lets you',                   /*   about rexxtry and       */
    'interactively try REXX',                  /*   remarks for             */
    'statements.'                              /*   interactive mode.       */
  say '    Each string is executed when you hit Enter.'
  say "    Enter 'call tell' for",             /* How to see description.   */
    "a description of the features."
  say '  Go on - try a few...            'remindrx
  return result                                /* Preserve result contents. */

sub:
  say '  ...test subroutine',                  /* User can CALL this        */
  "'sub'  ...returning 1234..."                /*   subroutine or           */
  return 1234                                  /*   invoke with 'sub()'.    */

main:
  signal on syntax                             /* Enable syntax trap.       */
  do forever                                   /* Loop forever.             */
    prev = inputrx                             /* User can repeat previous. */
    parse pull inputrx                         /* Input keyboard or queue.  */
    current = inputrx                          /* Current line for 'show'.  */
    if save <> '' then call save               /* Save before interpreting. */
    if inputrx = '=' then inputrx=prev         /* '=' means repeat previous */
    select
      when inputrx = '' then say ' ',          /* If null line, remind      */
        procrx':  'remindrx helprx             /*   user how to escape.     */
      when inputrx='?' then call help          /* Request for online help.  */
      otherwise
        rc = 'X'                               /* Make rc change visible.   */
        call set2; trace (trace)               /* Need these on same line.  */
        interpret inputrx                      /* Try the user's input.     */
        trace 'Off'                            /* Don't trace rexxtry.      */
        call border                            /* Go write the border.      */
    end
    if argrx <> '' & queued() = 0              /* For one-liner, loop until */
      then leave                               /*   queue is empty.         */
  end
  return result                                /* Preserve result contents. */

set1:  siglrx1 = sigl                          /* Save pointer to lineout.  */
  return result                                /* Preserve result contents. */

set2:  siglrx2 = sigl                          /* Save pointer to trace.    */
  return result                                /* Preserve result contents. */

save:                                          /* Save before interpreting. */
  call set1;rcrx=lineout(save,inputrx)         /* Need on same line.        */
  if rcrx <> 0 then                            /* Catch non-syntax error    */
    say "  Error on save="save                 /*   from lineout.           */
  return result                                /* Preserve result contents. */

help:                                          /* Request for online help.  */
  select
    when sysrx = 'CMS' then                    /* Invoke CMS help menu for  */
      address CMS 'HELP REXX MENU'             /*   for REXX.               */
    when sysrx = 'OS/2' then do                /* Invoke REXX Reference     */
      'view rexx.inf'                          /* ... for OS/2              */
      say '  Online Help started...'
    end
    when abbrev(sysrx, 'Windows') then do      /* ... for Windows           */
      'start winhlp32 rexxref.hlp'
      rc = '  Online Help started...'
    end                                        /* ... for Unix              */
    when sysrx = AIX | sysrx = LINUX | sysrx = 'SUNOS' then do
      say '  Online Help started using Acroread .../rexxref.pdf'
      if sysrx = AIX then
        'acroread /usr/lpp/orexx.info.en_US/rexxref.pdf&'
      else
        'acroread /opt/orexx/doc/rexxref.pdf&'
    end
    otherwise say '  'sysrx' has no online help for REXX.'
      rc = 'Sorry!'                            /* No help available         */
  end
  call border
  return result                                /* Preserve result contents. */

border:
  if rc = 'X' then                             /* Display border.           */
    say '  'bordrx
  else say ' ',                                /* Show return code if it    */
    overlay('rc = 'rc' ', bordrx)              /*   has changed.            */
  return result                                /* Preserve result contents. */

syntax:
  trace 'Off'                                  /* Stop any tracing.         */
  select
    when sigl = siglrx1 then do                /* User's 'save' value bad.  */
      say "  Invalid 'save' value '"save"', resetting to ''."
      save = ''
    end
    when sigl = siglrx2 then do                /* User's 'trace' value bad. */
      say "  Invalid 'trace' value '"trace"', resetting to 'Off'."
      trace = 'Off'
    end
    otherwise                                  /* Some other syntax error.  */
                                               /* Show the error msg text.  */
         say '  Oooops ! ... try again.     'errortext(rc)

      parse version . rxlevel .
      if rxlevel > 4.00 then do                /* get the secondary message */
        secondary = condition('o')~message
        if .nil <> secondary then              /* get a real one?           */
                                               /* display it also           */
          say '                              'secondary
      end

      if sysrx='CMS' then                      /* More syntax stuff for CMS */
        call cmstax
  end
  call border                                  /* Go write the border.      */
  if argrx <> '' & queued() = 0 then           /* One-liner not finished    */
    signal done                                /*   until queue is empty.   */
  signal main                                  /* Resume main loop.         */

cmstax:
  rcrx = rc                                    /* More syntax stuff for CMS */
  if exist('PROMPT MODULE') then               /* User's input to cmd line. */
    'PROMPT' promptrx||'13'x||inputrx          /*   '13'x places cursor.    */
  if exist('REXX EXEC') & argrx=''             /* Not for one-liners.       */
  then do;
    more='EXEC REXX' rcrx+20000                /* CMS rc is 20,000 higher.  */
    say "  Enter 'more' to see",               /* Prepare 'more' to access  */
      'information about this',                /*  REXX IOSLIB information. */
      'syntax error.'                          /* Tell user what to do.     */
  end
  rc = rcrx
  return result                                /* Preserve result contents. */

exist:
  arg inrx ; outrx = 0                         /* Assume file is missing.   */
  address command 'ESTATE' inrx                /* Does the file exist ?     */
  if rc = 0 then outrx = 1                     /* estate says it exists.    */
  return outrx                                 /* 1 makes condition true.   */

show:
  trace 'Off'; call clear                      /* Display user variables    */
  say '  'procrx' provides',                   /*   provided by rexxtry.    */
    'these user variables.'
  say '  The current values are...'            /* Show current values.      */
  say
  say "    'version'   = '"version"'"          /* What level of REXX.       */
  say "    'source'    = '"source"'"           /* What oper system etc.     */
  say "    'result'    = '"result"'"           /* REXX special variable.    */
  say
  say '     Previous line entered by user.  Initial value=INPUTRX.'
  say "    'prev'      = '"prev"'"             /* Previous user statement.  */
  say "    'current'   = '"current"'"          /* Compare curr with prev.   */
  say
  say "     Save your input with save=filespec. Stop saving with save=''."
  say "    'save'      = '"save"'"             /* Filespec for input keep.  */
  say
  say '     Enter trace=i, trace=o etc. to control tracing.'
  say "    'trace'     = '"trace"'"            /* Trace user statements.    */
  return result                                /* Preserve result contents. */
