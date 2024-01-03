#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2023-2024, Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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

/* As of 2023-02-04 Gunther Rademacher's RailroadGenerator will create all svg
   definitions from an ebnf definition and export it as part of a single xhtml file.
   The ooRexx documentation needs single svg files which this utility will extract
   and create.

   The formatting definitions will be replaced by the ones found in
   "rexxextensions\en-US\images\json_init.svg" as of 2023-02-04.

   Cf: "Railroad-Diagram-Generator": <https://www.bottlecaps.de/rr/ui> (2023-02-04)

   Usage:
      - create EBNF and download the svg as part of xhtml from <https://www.bottlecaps.de/rr/ui>
      - run this program, supply it the name of the downloaded xhtml file: all included
        svg definitions will be saved in separate svg files in the current directory
*/

parse arg xhtml_fn
if \sysFileExists(xhtml_fn) then
do
   say pp(xhtml_fn) "does not exist, aborting ..."
   exit -1
end
   -- read content
s=.stream~new(xhtml_fn)~~open("read")
content=s~charin(1,s~chars)
s~close

arrSvgs=parse_svgs(content)
   -- create svg files
say "extracted" pp(arrSvgs~items) "svg definitions from" pp(xhtml_fn)
do counter c a over arrSvgs
   fn=a[1]".svg"
   say "creating svg file #" c":" pp(fn)
   .stream~new(fn)~~open("replace")~~charout(a[2])~~close
end

::routine parse_svgs
  use arg content

  xhtmlOTag='<xhtml:a name="'
  svgOTag="<svg "
  svgETag="</svg>"

  arrSvgs=.array~new
  nl="0d0a"x
  str_svg_style_defs=nl || .resources~svg_style_defs~makeString || nl

  do counter c while content<>""
      parse var content (xhtmlOTag) svnName '"' (svgOTag) <0 svgDef (svgETag) content
      if svgDef="" | svnName="Railroad-Diagram-Generator" then iterate
      parse var svgDef "<" <0 svgOpen '>' svgDef2
      res=svgOpen || '>' || str_svg_style_defs || svgDef2 || nl || svgETag
      arrSvgs~append( (svnName, res) )
      say "extracted #" c":" pp(svnName)
  end
  return arrSvgs


::routine pp
  return "["arg(1)"]"

/*
   Extracted the <defs> definitions from "rexxextensions\en-US\images\json_init.svg" as
   of 2023-02-04.
*/
::resource svg_style_defs
         <defs>
            <style type="text/css">
               @namespace "http://www.w3.org/2000/svg";
               .line                 {fill: none; stroke: #1F1F1F;}
               .bold-line            {stroke: #0A0A0A; shape-rendering: crispEdges; stroke-width:
               2; }
               .thin-line            {stroke: #0F0F0F; shape-rendering: crispEdges}
               .filled               {fill: #1F1F1F; stroke: none;}
               text.terminal         {font-family: Verdana, Sans-serif;
               font-size: 12px;
               fill: #0A0A0A;
               font-weight: bold;
               }
               text.nonterminal      {font-family: Verdana, Sans-serif; font-style: italic;
               font-size: 12px;
               fill: #0D0D0D;
               }
               text.regexp           {font-family: Verdana, Sans-serif;
               font-size: 12px;
               fill: #0F0F0F;
               }
               rect, circle, polygon {fill: #1F1F1F; stroke: #1F1F1F;}
               rect.terminal         {fill: #CCCCCC; stroke: #1F1F1F;}
               rect.nonterminal      {fill: #E3E3E3; stroke: #1F1F1F;}
               rect.text             {fill: none; stroke: none;}
               polygon.regexp        {fill: #EFEFEF; stroke: #1F1F1F;}

            </style>
         </defs>
::END
