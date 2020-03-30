docs\trunk\tools (as of: 2020-02-10)
------------------------------------

This directory is supposed to collect all utilities that can be used to
create/change documentation parts.

- rexxpg/createClassHierarchy.rex:

        - creates the class hierarchy for the "rexxpg" book, chapter "provide.xml"

        - usage:

                createClassHierarchy > classHierarchy.xml

- postProcessSVG.rex

        - if SVG images get created by exporting an ODG image using OpenOffice, then
          remove the string " embedded" in the font-family name that AOO adds




