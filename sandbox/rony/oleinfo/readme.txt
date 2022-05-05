"oleinfo"
=========

ooRexx set of Window programs for analyzing and documenting registered Windows
OLE/COM components.


- listProgIds.rex   ... program lists all registered Windows OLE/COM components

        - optional argument 1: supplying the digit 1 adds CLSID to the output
        - optional argument "needle": lists only ProgIDs containing the needle

        - examples:

                rexx listProgIds.rex
                rexx listProgIds.rex policy
                rexx listProgIds.rex 1
                rexx listProgIds.rex 1 policy

- createOleInfo.rex ... program to create the documentation on the fly, if a
                        typelib got installed with the OLE/COM component

        - argument 1:          ProgID or CLSID
        - optional argument 2: 0 (default) long, 1 short (reference) documentation
        - optional argument 3: 0 no display, 1 (default) display documentation

        - examples:

                rexx createOleInfo.rex InternetExplorer.Application
                rexx createOleInfo.rex InternetExplorer.Application 1
                rexx createOleInfo.rex InternetExplorer.Application 1 0
                rexx createOleInfo.rex {0002DF01-0000-0000-C000-000000000046}
                rexx createOleInfo.rex {0002DF01-0000-0000-C000-000000000046} 1 0
                rexx createOleInfo.rex {0002DF01-0000-0000-C000-000000000046} 0 1



- getOleConstants.rex ... program creates a Rexx program that puts all constant values
                          into .local~ole.const; requiring or calling that created
                          Rexx program makes all constants of that OLE/COM component
                          available to the programmer. To retrieve a specific constant
                          send the constant name to ".ole.const" which will return its
                          value.

        - argument 1:          ProgID or CLSID
        - optional argument 2: name of the file the generated generated Rexx code
                               should be saved to; if omitted output goes to stdout (the screen)
                               ... if file gets generated incorporate it either with the
                                   "::requires file.rex" or "call file.rex" statement; therafter
                                   fetch any constant sending its name .ole.const

        - examples:

                rexx getOleConstants.rex Excel.Application excel_constants.rex
                ... inspect "excel_constants.rex", use in Rexx program:
                        ...
                        say .ole.const~xlColumnHeader   -- access Excel constant "xlColumnHeader"
                        ...
                        ::requires excel_constants.rex  -- incorporate all Excel constants

                rexx getOleConstants.rex Word.Application word_constants.rex
                ... inspect "word_constants.rex", use in Rexx program: call word_constants.rex
                        ...
                        call word_constants.rex         -- incorporate all Word constants
                        ...
                        say .ole.const~wdReadingView    -- access Word constant "wdReadingView"
                        ...

--- supporting programs

        - oleinfo.css   ... the cascading stylesheet for the generated documentation

        - oleinfo.cls   ... support package for these utilities representing each OLE/COM component
                            with the most important information about it

        - oleinfo.properties ... a property (text) file defining the CSS file name and whether its
                            content should be incorporated into the head elelment of the generated
                            HTML documentation

        - oleinfo2html.frm ... program to create the long and short/reference HTML documentation

        - reg_classids4ole.cls ... support package to analyze the registry for OLE/COM components

--- NOTE: you can invoke createOleInfo as a subroutine, function from an ooRexx program with the
          following arguments:

        - argument 1:          an OleObject or a string with the ProgID or CLSID; this way
                               it becomes possible to have the documentation created of an OLE/COM
                               object returned by a windows method or property

        - optional argument 2: title to use for the HTML rendering and filename

        - optional argument 3: 0 (default) long, 1 short (reference) documentation

        - optional argument 4: 0 no display, 1 (default) display documentation

        - example:

                ... cut ...
                oleobject = winapp~getSomething(...) -- returns an OLE object
                        -- document OLE object as HTML with a supplied title
                call createOleInfo oleobject, "From winapp~getSomething(..)"
                ... cut ...



Rony G. Flatscher, http://www.wu.ac.at, Vienna, 2022-05-03

