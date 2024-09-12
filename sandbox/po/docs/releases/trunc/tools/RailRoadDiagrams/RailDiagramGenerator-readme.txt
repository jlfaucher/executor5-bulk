The ooRexx project uses a free tool "RailDiagramGenerator" for creating its
syntax diagrams from ebnf.

OUTDATED (20240315) It is located at: <https://www.bottlecaps.de/rr/ui>.
It is located at (20240315): <https://rr.red-dove.com/ui>


Those books that use syntax diagrams store them in: ${book}/en-US/images

        - *.ebnf: these are text files that contain the syntax definitions
                  for functions and methods; their content serves as input
                  in the "Edit Grammar" tab; if there are changes made to
                  it, then replace the content of the .ebnf file with it

        - to create the syntax diagram do the following:

                - "Options" tab (important as of 2023-02-07: uncheck "Inline literals"

                - "View Diagram" tab: displays the generated diagrams

                  - at the upper right hand corner select "XHTML+SVG"
                    and click "Download" such that the generated xhtml
                    file can be saved to the respective ${book}/en-US/images
                    directory; this file will include all svg definitions for
                    the .ebnf defined rail road diagrams, which one can view
                    offline by loading the xhtml file into the web browser

                - change into ${book}/en-US/images and run the extract Rexx script
                  against the saved .xhtml file, e.g. if named xyz.html then:

                     docs/trunk/tools/RailRoadDiagrams/extract_svg_from_rr_xhtml.rex xyz.xhtml

                  this will extract all the svg diagram definitions from the .xhtml
                  file using the production names as file names and appending ".svg"
                  to each file; in the extraction process the formatting (CSS) rules
                  will be replaced by the ones currently used for the ooRexx
                  documentation

---

Stand-alone version: to download go to <https://www.bottlecaps.de/rr/ui>, chose the
"Welcome" tab, scroll down to the "download" section and download and unzip the latest
version of the tool. It is a Java application packed as a web archive that can be run
standalone from the command line.

The docs/trunk/tools/RailRoadDiagrams contains a subdirectory named after the current
downloaded version of the RailDiagram Generator, change into it and issue the following
command to learn about the available options:

        java -jaqr rr.jar

which yields e.g.:

        docs/trunk/tools/RailRoadDiagrams>java -jar rr.war
        RR - Railroad Diagram Generator

          version 1.67
          released Jan 18, 2023
          from https://bottlecaps.de/rr

        Usage: java -jar rr.war {-suppressebnf|-keeprecursion|-nofactoring|-noinline|-noepsilon|-color:COLOR|-offset:OFFSET|-png|-md|-out:FILE|width:PIXELS}... GRAMMAR
            or java -jar rr.war -gui [-port:PORT]

          -suppressebnf    do not show EBNF next to generated diagrams
          -keeprecursion   no direct recursion elimination
          -nofactoring     no left or right factoring
          -noinline        do not inline nonterminals that derive to single literals
          -noepsilon       remove nonterminal references that derive to epsilon only
          -color:COLOR     use COLOR as base color, pattern: #[0-9a-fA-F]{6}
          -offset:OFFSET   hue offset to secondary color in degrees
          -png             create HTML+PNG in a ZIP file, rather than XHTML+SVG output
          -out:FILE        create FILE, rather than writing result to standard output
          -width:PIXELS    try to break graphics into multiple lines, when width exceeds PIXELS (default 992)
          -enc:ENCODING    set grammar input encoding (default: autodetect UTF8/16 or use system encoding)

          GRAMMAR          path of grammar, in W3C style EBNF (use '-' for stdin)

          -gui             run GUI on http://localhost:8080/
          -port:PORT       use PORT rather than 8080

        rr.war is an executable war file. It can be run with "java -jar" as shown
        above, but it can also be deployed in servlet containers like Tomcat or Jetty.

To run the GUI standalone one would issue:

        java -jar rr.war -gui

which will give the output:

        Now listening on http://localhost:8080/

Entering the following URL in your browser will allow you to work with it: http://localhost:8080/

To stop, just close the window/terminal or press Ctl-c

As once can see the tool allows for generating diagrams stand-alone as well if using
the appropriate switches on the command line.


2023-02-07, rgf

