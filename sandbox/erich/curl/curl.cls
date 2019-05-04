-- an object-oriented interface to Rexx/CURL, http://rexxcurl.sourceforge.net/
-- requires Rexx/CURL from https://sourceforge.net/projects/rexxcurl/files/rexxcurl/
-- see also Rexx/CURL docs http://rexxcurl.sourceforge.net/doc/index.html

/*
usage examples:

  say .cUrl~version~rexxcurl           -- rexxcurl 2.0 25 Apr 2012 libcurl/7.23.1 ..

  -- HTTP example
  osm = .cUrl~new("http://www.openstreetmap.org/api/0.6/way/163840840")
  say osm~get
  say osm~getInsecure                  -- short for url~get("SSLVERIFYPEER 0")
  say osm~code                         -- 200

  -- FTP example
  gb = .curl~new('ftp://gutenberg.pglaf.org/mirrors/gutenberg/')
  say gb~perform('DIRLISTONLY')

  ::requires curl.cls"
*/

::class cUrl public

------------------------------------------------------------------------
-- class methods

-- initalize cURL interface
::method activate class
  expose handle supports protocols version host

  call rxfuncadd "CurlLoadFuncs", "rexxcurl"
  call CurlLoadFuncs
  handle = CurlInit()
  if handle = "" then
    raise syntax 93.900 array("cURL failed to initialize") RETURN ("")

  -- setup protocols, support and other constants
  supports = .Directory~new
  protocols = .Array~new(20)
  do tail over !REXXCURL.~allIndexes
    select
      when tail~abbrev("!SUPPORTS_") then do
        parse var tail "!SUPPORTS_" feature
        supports[feature] = !REXXCURL.[tail]
      end
      when tail~abbrev("!PROTOCOLS.") then do
        parse var tail "!PROTOCOLS." index
        if index > 0 then              -- ignore !PROTOCOLS.0
          protocols[index] = !REXXCURL.[tail]
      end
      otherwise nop
    end
  end
  host = !REXXCURL.!HOST
  version = .Directory~new
  version["ARES"] = !REXXCURL.!ARES
  version["LIBCURL"] = !REXXCURL.!VERSION
  version["ICONV"] = !REXXCURL.!ICONV_VER_NUM
  version["LIBIDN"] = !REXXCURL.!LIBIDN
  version["LIBZ"] = !REXXCURL.!LIBZ_VERSION
  version["SSH"] = !REXXCURL.!LIBSSH_VERSION
  version["SSL"] = !REXXCURL.!SSL_VERSION
  version["REXXCURL"] = CurlVariable("VERSION")

-- Stop cURL interface
::method uninit class
  expose handle

  call CurlCleanup handle
  call CurlDropFuncs "UNLOAD"

-- class attributes
::attribute supports class get    -- e. g. .cUrl~supports~ssl
::attribute protocols class get   -- e. g. .cUrl~protocols~hasItem("https")
::attribute version class get     -- e. g. .cUrl~version~libcurl
::attribute host class get        -- e. g. .cUrl~host


-- returns string converted to encoded URL
-- all chars except [-._~a-zA-Z0-9] are converted to %nn
::method escape class
  expose handle

  use strict arg url
  string = CurlEscape(handle, url)
  call testForFailure "Escape", curlerror.
  return string


-- returns encoded URL converted to plain URL
-- all %nn - encoded chars are converted to their binary representation
::method unEscape class
  expose handle

  use strict arg url
  string = CurlUnescape(handle, url)
  call testForFailure "Unescape", curlerror.
  return string


------------------------------------------------------------------------
-- instance methods

-- initialize a new Url instance
::method init
  expose handle url

  use strict arg url = ""

  -- each Url instance gets its own cUrl handle
  -- "CurlLoadFuncs" function has been made available at class init time
  call CurlLoadFuncs
  handle = CurlInit()
  if handle = "" then
    raise syntax 93.900 array("cURL failed to initialize") RETURN ("")

::method uninit 
  expose handle

  -- remove cUrl handle
  call CurlCleanup handle


::attribute url


-- set options through CurlSetopt()
::method setOpt
  expose handle
  use strict arg options 

  -- we accept a string or a collection of strings
  do option over options
    -- 'option' may be of the form "type [value]"
    -- if value isn't specified, it defaults to '1'
    -- we really don't yet support options which read from or write to a stem
    -- except for:
    --   HTTPHEADER, for which we allow a single string only
    --   HTTPPOSTFIELDS, supported by individual method setOptPostFields()
    first = option~word(1) 
    remainder = option~subWord(2) 
    select
      when first~caselessEquals("HTTPHEADER") then do
        httpHeader.0 = 1 
        httpHeader.1 = remainder 
        call CurlSetopt handle, first, "httpHeader."
      end
      otherwise
        if remainder = "" then
          call CurlSetopt handle, first, 1
        else
          call CurlSetopt handle, first, remainder
    end
    call testForFailure "Setopt", curlerror.
  end


-- set HTTPPOSTFIELDS option through CurlSetopt()
::method setOptPostFields
  expose handle
  use strict arg fields 

  -- we accept a string or a collection of strings
  nr = 0
  do field over fields
    -- 'option' must be of the form "type=value"
    nr += 1
    field.nr = field
  end
  field.0 = nr
  call CurlSetopt handle, "HTTPPOSTFIELDS", "field."
  call testForFailure "Setopt", curlerror.


-- perform cURL operation
::method perform
  expose handle url
  use strict arg options = .nil

  if .nil \== options then             -- set individual 'options', if any
    self~setOpt(options)

  -- if we don't know for sure whether returned data is Lf- or CrLf-terminated,
  -- we cannot reliably supply the appropriate OUTSTEM termination sequence
  -- so we instead use a very unlikely line termination sequence, to get data
  -- returned as a single line
  call CurlSetopt handle, "URL", url
  call CurlSetopt handle, "OUTSTEM", "lines.", '5f3759df'x
  call testForFailure "Setopt", curlerror.
  call CurlPerform handle
  call testForFailure "Perform", curlerror.
  select
    when lines.0 = 0 then -- valid if e. g. "connectonly" option specified
      return ""
    when lines.0 = 1 then
      return lines.1
    otherwise
      raise syntax 93.900 array("CurlPerform() expected to return a single line;" lines.0 "lines returned") RETURN ("")
  end


-- GET and return output as string
::method get
  expose handle url

  -- set required options HTTPGET and URL
  self~setOpt(.Array~of("HTTPGET", "URL" url))

  -- let perform() handle everything else, including individual options
  forward message "PERFORM"


-- insecure GET, a.k.a. --no-check-certificate
::method getInsecure

  self~setOpt("SSLVERIFYPEER 0")

  -- let get() handle everything else, including individual options
  forward message "GET"


-- POST and return output as string
::method post
  expose handle url
  use strict arg fields = .nil         -- a String or an Array of Strings

  -- set required options HTTPPOST, URL, and optional HTTPPOSTFIELDS
  self~setOpt(.Array~of("HTTPPOST", "URL" url))
  if .nil \== fields then
    self~setOptPostFields(fields)

  return self~perform


-- insecure POST, a.k.a. --no-check-certificate
::method postInsecure

  self~setOpt("SSLVERIFYPEER 0")

  -- let post() handle everything else, including individual options
  forward message "POST"


-- GET and return output as array
::method getArray
  forward message "GET" continue
  return result~makeArray


-- return information for latest CurlPerform
::method getInfo
  expose handle

  use strict arg name

  -- we don't yet support names which require a stem as the third arg
  value = CurlGetInfo(handle, name) 
  call testForFailure "GetInfo", curlerror.

  return value


-- returns HTTP code for latest GET; short for getInfo("RESPONSE_CODE") 
::method code
  use strict arg

  forward message "GETINFO" ARRAY ("RESPONSE_CODE")


-- returns result for an option requiring a stem, requested with setOpt()
::method not_implemented_getOpt
  use strict arg 


-- private routine to test Rexx/Curl calls for failure return
::routine testForFailure
  use strict arg name, curlerror.
  select
    when var("CURLERROR.INTCODE") = 0 then nop -- may be uninitialized, although docs don't say so
    when curlerror.intcode = 0 then nop
    when curlerror.intcode~abs = 1 then -- docs say intcode = 1, but we receive -1
      raise syntax 93.900 array("Curl"name "failed with cURL error" curlerror.curlcode"," curlerror.curlerrm) RETURN ("")
    otherwise
      raise syntax 93.900 array("Curl"name "failed with Rexx/Curl error" curlerror.intcode"," curlerror.interrm) RETURN ("")
  end




-- call interface

-- usage example:
--  html = Url("http://www.openstreetmap.org/api/0.6/way/163840840")

::routine Url public
  use arg url
  return .cUrl~new(url)~get


--::options novalue error
