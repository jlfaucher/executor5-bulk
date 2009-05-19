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
/* Description: Compile an *.rsp (Rexx Server Page) into a real Rexx pgm.     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global option variables:                                                   */
/*                                                                            */
/* errmsg   = 0  do not show error messages (default)                         */
/*            1  output error messages to stdout                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global state variable:                                                     */
/*                                                                            */
/* state    = 0  parsing HTML lines (starting state)                          */
/*            1  parsing Rexx statements delimited by '<?rexx' and '?>' tags  */
/*            2  parsing Rexx statements delimited by                         */
/*               '<script type="rexx">' and '</script>' tags                  */
/*                                or                                          */
/*               '<script language="rexx">' and '</script>' tags              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Return codes:                                                              */
/*                                                                            */
/* 0  successful execution                                                    */
/* 1  no output Rexx filename specified                                       */
/* 2  the input RSP file was not found                                        */
/* 3  cannot open the RSP input file                                          */
/* 4  cannot open the Rexx output file                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* version of this script */
script_version = 'v2.2.0'

/* set global variables and option defaults */
errmsg = 0
parse upper version rexx_ver

/* get the input filename arguments and options */
cmdline = arg(1)
if cmdline~substr(1, 1) = '"' then parse var cmdline '"' rspfilename '"' cmdline
else parse var cmdline rspfilename cmdline
cmdline = cmdline~strip()
if cmdline~substr(1, 1) = '"' then parse var cmdline '"' rexfilename '"' option1 .
else parse var cmdline rexfilename option1 .
if rexfilename = '' then do
   call console_msg 'Error: No output Rexx filename specified.'
   return 1
   end
if option1~translate() = 'ERRMSG' then errmsg = 1

/* try to open the rsp file */
if rspfilename = '' then do
   call console_msg 'Error: No input RSP File specified.'
   return 2
   end
rspstream = .stream(rspfilename)
retc = rpsstream~open('read')
if retc <> 'READY:' then do
   call console_msg 'Error: cannot open file' rspfilename
   return 3
   end

/* open the rex file */
rexstream = .stream(rexfilename)
retc = rexstream~open('write replace')
if retc <> 'READY:' then do
   call console_msg 'Error: cannot open file' rexfilename
   return 4
   end

/* read in the rsp file */
rsplines = rspstream~arrayin()
rspstream~close()

/* read in the rsp file and look for the the rsp tags */
state = 0 /* initial state is to output HTML lines */
call Rexx_pgm_header
do rspline over esplines
   call process_line rspline
   end
rexstream~close()

/* done */
return 0


/*----------------------------------------------------------------------------*/
/* process a line from the rsp file                                           */
/*----------------------------------------------------------------------------*/

process_line: procedure expose state rexfilename
use strict arg line
uline = line~strip()~upper()
select
   when state = 0 then do
      if uline = '<?REXX' then do
         /* change state to output Rexx pgm line(s) */
         state = 1
         end
      else if uline~substr(1, 8) = '<SCRIPT ' then do
         parse var uline '<SCRIPT' . 'TYPE=' lang . '>'
         if lang = '' then parse var uline '<SCRIPT' . 'LANGUAGE=' lang . '>'
         lang = strip(lang,, '"')
         lang = strip(lang,, "'")
         if lang = 'REXX' then do
            /* change state to output Rexx pgm line(s) */
            state = 2
            end
         else nop /* must be another language (like Javascript) */
         end
      if state = 0 then do
         /* state has not changed, output the HTML line */
         call splitline line
         /* see if we are at the end of the HTML document */
         if uline~pos('</HTML>') > 0 then do
            /* doing this separates the mainline code from any Rexx     */
            /* subroutines/functions added at the end of the RSP file,  */
            /* that way we do not just fall into them (a Rexx error).   */
            call Rexx_pgm_footer
            end
         end
      end
   when state = 1 then do
      if uline = '?>' then do
         /* change state to output HTML line(s) */
         state = 0
         end
      else do
         /* output the Rexx pgm line */
         rexstream~lineout(line)
         end
      end
   when state = 2 then do
      if uline = '</SCRIPT>' then do
         /* change state to output HTML line(s) */
         state = 0
         end
      else do
         /* output the Rexx pgm line */
         rexstream~lineout(line)
         end
      end
   otherwise nop
   end
return


/*----------------------------------------------------------------------------*/
/* split a single HTML line into multiple lines if necessary                  */
/* (so Rexx maximum program line length is not exceeded)                      */
/*----------------------------------------------------------------------------*/

splitline: procedure expose rexfilename
use strict arg line
do while line~length() > 90
   x = line~substr(1, 80)
   do i = 81 to line~length()
      if line~substr(i, 1) <> ' ' then x = x || line~substr(i, 1)
      else leave
      end
   rexstream~lineout("say '" || add_quotes("'", x) || "'")
   line = substr(line, i)
   end
rexstream~lineout( "say '" || add_quotes("'", line) || "'")
return


/*----------------------------------------------------------------------------*/
/* add quotes to a string (two for one)                                       */
/*----------------------------------------------------------------------------*/

add_quotes: procedure
use strict arg quote, line
return line~changestr(quote, quote || quote)


/*----------------------------------------------------------------------------*/
/* add header information to the Rexx program                                 */
/*----------------------------------------------------------------------------*/

Rexx_pgm_header:
use strict arg
rexstream~lineout('/* Start of Rexx RSP header information */')
rexstream~lineout('/*')
rexstream~lineout('')

rexstream~lineout('RSP Compiler Information:')
rexstream~lineout('   Compiler version: ' script_version)
rexstream~lineout('   Compile timestamp:' date() time())
rexstream~lineout('   Operating System: ' os)
rexstream~lineout('   Rexx Version:     ' rexx_ver)
rexstream~lineout('')

rexstream~lineout('Source RSP File Information:')
fsize = rexstream~command('query size')
ftimestamp = rexstream~command('query timestamp')
rexstream~lineout('   ' || ftimestamp || '  ' || format(fsize, 10) || ,
                  '  ' || stream(rspfilename, 'c', 'query exists'))
rexstream~lineout('')

rexstream~lineout('Output Rexx Program File Information:')
rexstream~lineout('   Filename:' stream(rexfilename, 'c', 'query exists'))
-rexstream~lineout('')

rexstream~lineout('*/')

rexstream~lineout('')
rexstream~lineout('/* note: the following call is NOT optional */')
rexstream~lineout('call WWWSendHTTPHeader arg(1), "text/html"')
rexstream~lineout('/* note: these calls are optional but here anyway */')
rexstream~lineout('call WWWGetArgs arg(1)')
rexstream~lineout('call WWWGetCookies arg(1)')
rexstream~lineout('')
rexstream~lineout('/* End of Rexx RSP header information */')

rexstream~lineout('')
return


/*----------------------------------------------------------------------------*/
/* add footer information to the Rexx program                                 */
/*----------------------------------------------------------------------------*/

Rexx_pgm_footer:
use strict arg
rexstream~lineout('/* Start of Rexx RSP footer information */')
rexstream~lineout('return 0')
rexstream~lineout('/* End of Rexx RSP footer information */')
return


/*----------------------------------------------------------------------------*/
/* send message to the console (stdout)                                       */
/*----------------------------------------------------------------------------*/

console_msg:
use strict arg consolemsg
if errmsg = 1 then say consolemsg
return

