/*
	program: createOleInfo.rex
	type:    Object Rexx for Windows
	purpose: Query the OLE/ActiveX automation interface, create HTML-renderings,
            optionally display HTML-renderings with Internet Explorer

	version: 1.02
	date:    2003-01-03, ---rgf, released
   changed: 2002-07-14, ---rgf, returns always .true, so if called as function, a return value is supplied
            2002-08-01, ---rgf, added ability to pass an OleObject as first argument
            2002-12-29, ---rgf, changed interface, removed a bug if called as a function
            2002-12-31, ---rgf, changed syntax for invoking as function
            2004-01-22, ---rgf, if called as function, honor 4th argument (display rendering in MSIE?)
            2022-04-28, ---rgf, renamed to "createOleInfo.rex", changed file name, if compact
                                listing then use "_reference" in the file name, corrected/updated
                                university domain; uses the default Windows browser to display
                                generated HTML file; added meaningful error message if .oleobject
                                could not get created
            2022-05-05, ---rgf, added sanitize() to remove illegal chars from filename,
                                added qu() to quote and escape parentheses in commands with ^

	author:  Rony G. Flatscher,
				Wirtschaftsuniversitaet/Vienna
				http://www.wu.ac.at
				Rony.Flatscher@wu.ac.at

	needs:	oleinfo2html.frm

   usage:   can be used as a function or as a command

    - can be called from the command line as well:

       rexx createOleInfo.rex app_name [mode [display]]

         where:
       app_name   ... OLE/ActiveX: a string denominating either PROGID or CLSID
       mode       ... 0|1 full=default (0) or compact (1) listing
       display    ... 0|1 no display (0) or display=default (1) with Internet Explorer

            example (command-line): rexx createOleInfo InternetExplorer.Application 1 1

    - called from a function:

       res=createOleInfo(app_name | {ole_object [, [header]} [, [mode] [, [display]]]] )
       ... invocation as a function, e.g. from *any* running Object Rexx program,
                returns .true, if OLEObject could get created|interrogated, .false else
         where:
            app_name   ... OLE/ActiveX: a string denominating either PROGID or CLSID
         or ole_object ... OLE/ActiveX-object to interrogate

            header     ... HTML-heading, if 1st argument is an OLE-object
            mode       ... 0|1 full (0) or compact=default (1) listing
            display    ... 0|1 no display (0) or display=default (1) with Internet Explorer

            example:   ole=.OleObject~new("InternetExplorer.Application")
                       success=createOleInfo(ole, "Interfaces for the Internet Explorer")
*/

   if arg()=0 then -- show usage
   do
      do i=1 to 999 until pos("usage:", sourceline(i)) > 0; nop; end
      do i=i to 999 while pos("*/", sourceline(i))=0;say strip(sourceline(i));end
      exit
   end

	parse source . calltype .		-- get type of invocation

   bCommand=(calltype="COMMAND")    -- called as function ?
	if bCommand then
               do
                  parse arg app bCompact bRunIE		-- called from the commandline
                  fn=sanitize(app)
               end
		   		else  -- called from another Rexx program
               do
                  use arg app, header=("Name of OLE-Object Not Supplied by Programmer!<br>created on:" date("S") time()), -
                          bCompact=.false, bRunIE=.true -- called as a function
                  fn=sanitize(header)
               end

   if bCompact = "" then bCompact=.false  -- default to .false
                    else bCompact = (bCompact=.true | translate(bCompact)="TRUE")

   if bRunIE = "" then bRunIE=.true      -- default to .true
	               else bRunIE = \(bRunIE=.false | translate(bRunIE)="FALSE")	-- default to .true

   if app~class=.string then  -- first argument is a String (PROGID or ClSID), not an OLEObject !
   do
   	resArr=oleinfo2html(app, .nil, bCompact)	   -- create OLE object and render interrogation results
   end
   else
   do    -- 'app' is assumed to be an OleObject
      resArr=oleinfo2html(header, app, bCompact)	-- pass OLE object and render interrogation results
   end

	if resArr~items=0 then
   do
      .error~say('Problem: could not create an OLEObject for supplied ProgID/CLSID "'app'"')
      return .false	-- "app": not an OLEobject, return .false
   end

   -- create html-file, add extension ".html" to given name
   hint=""
   if bCompact then hint="reference_"

	-- fn= app || "_" || hint || date("S") || "_" || changestr(" ", translate(time(),"",":"), "") || ".html"
	fn= fn || "_" || hint || date("S") || "_" || changestr(" ", translate(time(),"",":"), "") || ".html"


		-- write received data into file, close it
	str=.stream~new(fn)
	str~~open("write replace")~~lineout("<html>")~~arrayout(resArr)~~lineout("</html>")~close

   if \bRunIE then return .true -- do not run IE, hence exit

/* --- although works as of 2022-04-28, Microsoft actively deprecates it in favor of Chromium
		--	create an instance of Internet Explorer
	myIE=.OLEObject~new("InternetExplorer.Application")
	myIE~visible=.true				-- make it visible
	myIE~navigate2(str~qualify)	-- load freshly created html file, needs full path with filename
   return .true
-- */

   address system qu(fn)         -- let Windows open file in default browser
   return .true                  -- indicate success

::requires "oleinfo2html.frm"



/* Make sure we create a valid name for Windows, cf. <https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file>
   (2022-05-05), illegal chars:
    < (less than)
    > (greater than)
    : (colon)
    " (double quote)
    / (forward slash)
    \ (backslash)
    | (vertical bar or pipe)
    ? (question mark)
    * (asterisk)
*/

::routine sanitize
  parse arg fn
  return translate(fn, "", "<>:""/\|?*", " ")~space(1)~changestr(" ","_")

/* Put quotes around filename and make sure parentheses get escaped with ^. */
::routine qu
  return '"'arg(1)~changeStr('(','^(')~changeStr(')','^)')'"'

