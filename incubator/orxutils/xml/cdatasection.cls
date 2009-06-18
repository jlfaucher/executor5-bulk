/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CDATASection" subclass Text PUBLIC
::attribute nodeType GET
  use strict arg
  return .Node~CDATA_SECTION_NODE

::attribute nodeName GET
  use strict arg
  return "#cdata-section"
