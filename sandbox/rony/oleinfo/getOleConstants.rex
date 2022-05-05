/*
   name:    getOleConstants.rex
   version: 1.10
   date:    2003-01-03, --- released
   changed: 2004-07-29, ---rgf, make sure that numbers being longer than digits() are quoted
            2022-04-27, ---rgf, renamed from "rgf_oleconstants.rex" to "getOleConstants.rex",
                                updated university domain
            2022-05-03, ---rgf, use a stringTable instead of a directory, use a resource
                                directive for usage instructions, use .DateTime to protocol
                                run, inhibit runtime error in case OLEObject cannot be
                                created
   author:  Rony G. Flatscher
   purpose: create an Object Rexx file which saves the constants of the given
            OLE/ActiveX appId/clsId in a directory called "ole.const" in the
            environment .local (and hence can be retrieved by the environment
            symbol ".ole.const")
   usage:   getOleConstants appid/clsid [outfile]

            appid/clsid ... OLE/ActiveX application ID or CLSID
            outfile ....... optional name of the outputfile, otherwise STDOUT is used

                            use the above outfile in a "::requires" directive
                            which then allows you to use the defined constants,
                            e.g. ".ole.const~SWC_3RDPARTY"

            example: rexx getOleConstants InternetExplorer.Application msie_const.rex
                     ... then, whenever you need to access MSIE-constants, then merely use
                         "::requires msie_const.rex" and refer to them e.g. like
                         ".ole.const~csc_navigateBack"

   license: CPL 1.0, Apache License 2.0
*/

   if arg()=0 then -- show usage
   do
      say .resources~usage
      exit
   end


parse arg appId outFile

-- ole=.oleobject~new(appId)  -- get OLE proxy
ole=createOleObject(appId)    -- try to create OLE proxy
if ole=.nil then
do
   .error~say(appId": could not create the OLE proxy, aborting ...")
   exit -1
end

oleconst="ole.const"  -- define directory name to store OLE constants, change if needed

   -- get OLE constants, create sortable stem
constants.=ole~getConstant
stem. = .stem~new

stem.0=0
max=0
do counter i idx over constants.
   idx=substr(idx,2)  -- remove leading "!" character
   stem.i=idx || "09"x || ole~getConstant(idx)
   stem.0=i
   max=max(max, length(idx))  -- save longest constant name
end

   -- if no constants, abort
if stem.0=0 then
do
   .error~say("No constants found for:" pp(appId)", aborting ..." )
   exit -1
end

call SysStemSort "stem.", "A", "I"  -- sort stem

   -- dump infos
parse source . . thisPgm

   -- open file-stream
if outfile="" then of=.output -- appId"_constants.rex"
              else of=.stream~new(strip(outFile))~~open("write replace")

of~say("/*" pp(filespec("name", thisPgm)) "run on:" pp(.dateTime~new) "*/" )
of~say
tmpStr1="are"
tmpStr2="constants"
if stem.0=1 then  -- singular
do
   tmpStr1="is"
   tmpStr2="constant"
end

of~say("-- OLE/ActiveX-application/clsid:" pp(appId) "- there" tmpStr1 pp(stem.0) tmpStr2 )
of~say
of~say("-- create stringTable '"OLECONST"', if necessary; maybe shared with OLE constant definitions of other programs")
of~say("if .local~hasentry('"oleconst"')=.false then .local~"oleconst"=.stringTable~new -- create stringTable '"OLEconst"' in .local")
of~say

do i=1 to stem.0
  parse var stem.i const "09"x value
  of~say( '.'oleconst'~'left(const, max) '=' encode(value) )
end

of~close

::routine pp
  return "[" || arg(1)~string || "]"

::routine encode  -- if a number, leave unchanged, else escape quotes Rexx-style, enclose string into quotes

  if datatype(arg(1), "Number") & length(arg(1))<=digits() then return arg(1)  -- if a number, return value unchanged

  quote='"'
  tmp=changestr(quote, arg(1), quote||quote)
  return quote || tmp || quote

::resource usage  -- usage instructions
usage:   getOleConstants appid/clsid [outfile]

   appid/clsid ... OLE/ActiveX application ID or CLSID
   outfile ....... optional name of the outputfile, otherwise STDOUT is used

                   use the above outputfile in a "::requires" directive
                   which then allows you to use the defined constants,
                   e.g. ".ole.const~SWC_3RDPARTY"

   example: rexx getOleConstants InternetExplorer.Application msie_const.rex

            ... creates an ooRexx program named "msie_const.rex" which adds
                all found constants to the .ole.const stringTable (ooRexx 5.0,
                like a directory)

            ... then, whenever you need to access MSIE-constants, merely use

                   ::requires "msie_const.rex"

                in your program. This makes the InternetExplorer constants
                available via the environment symbol ".ole.const" such that
                you can refer to the constants by name in your programs like:

                   val=.ole.const~csc_navigateBack -- get the constant

                Note: you can require as many different ole constant files as
                      necessary in a single program; all constants will get
                      saved in ".ole.const"

::END

/** Makes sure that program does not get stopped if OLEObject cannot be created (e.g. SYNTAX 92.911),
    rather return .nil in that case. */
::routine createOleObject
   use strict arg appId
   signal on syntax
   return .oleobject~new(appId)  -- get OLE proxy
syntax:
   return .nil
