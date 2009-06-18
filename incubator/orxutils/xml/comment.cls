/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment -- a comment node                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Comment" subclass CharacterData PUBLIC
::attribute nodeType GET
  use strict arg
  return .Node~COMMENT_NODE

::attribute nodeName get
  use strict arg
  return "#comment"
