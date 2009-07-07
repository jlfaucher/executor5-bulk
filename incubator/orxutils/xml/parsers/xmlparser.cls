/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLParser                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "XMLParser"
::constant ENTITY_RESOLVER "http://apache.org/xml/properties/internal/entity-resolver"
::constant ERROR_HANDLER "http://apache.org/xml/properties/internal/error-handler"

::method init
  expose configuration

  use strict arg configuration
  configuration~addRecognizedProperties(.array~of(self~ENTITY_RESOLVER, self~ERROR_HANDLER))

::method parse
  expose configuration
  use strict arg inputSource

  self~reset
  configuration~parse(inputSource)

::method reset
  -- nop for this the base parser version



