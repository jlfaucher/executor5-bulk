docs\trunk\tools (as of: 2020-02-10)
------------------------------------

This directory is supposed to collect all utilities that can be used to
create/change documentation parts.

- rexxpg/createClassHierarchy.rex (requiring 'rgf_util2.rex'):

        - creates the class hierarchy for the "rexxpg" book, chapter "provide.xml"

        - usage:

                createClassHierarchy > classHierarchy.xml

        - version as of: 2020-03-10

- rexxpg/rgf_util2.rex:

        utility package that defines among other things a MessageComparator class that is being used
        by "createClassHierarchy.rex" to simplify sorting the class objects by their id (class name,
        a string) attribute

        - usage (note quotes for case-dependent file systems):
                ::requires "rgf_util2.rex"

        - version as of: 2018-09-30

        - gets also distributed with BSF4ooRexx (documentation in "information" folder or
          <http://wi.wu.ac.at/rgf/rexx/orx20/>, first introduction at the 2009 Rexx symposium
          <https://www.rexxla.org/events/2009/abstracts.html#util>)



