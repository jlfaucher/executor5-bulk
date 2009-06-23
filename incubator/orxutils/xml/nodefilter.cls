/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeFilter                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeIterator" mixinclass Object
-- constants returned by acceptNode
::constant FILTER_ACCEPT 1
::constant FILTER_REJECT 2
::constant FILTER_SKIP   3

-- whatToShow values
::constant SHOW_ALL                     -1
::constant SHOW_ELEMENT                  1
::constant SHOW_ATTRIBUTE                2
::constant SHOW_TEXT                     4
::constant SHOW_CDATA_SECTION            8
::constant SHOW_ENTITY_REFERENCE        16
::constant SHOW_ENTITY                  32
::constant SHOW_PROCESSING_INSTRUCTION  64
::constant SHOW_COMMENT                128
::constant SHOW_DOCUMENT               256
::constant SHOW_DOCUMENT_TYPE          512
::constant SHOW_DOCUMENT_FRAGMENT     1024
::constant SHOW_NOTATION              2048

-- use to filter the individual nodes
::method acceptNode
  use strict arg node
  -- default implementation accepts everything
  return self~FILTER_ACCEPT



