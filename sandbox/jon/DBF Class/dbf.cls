/* DBF.Cls ================================================================= */
/* A suite of utility classes for dealing with dbf files                     */
/* Author: Sahananda windhorse:evolution                            Nov 2008 */
/* ========================================================================= */
/* dbf.class a class to read dbf files without a database manager            */
/* ========================================================================= */
/* Dependancies:                                                             */
/*    rgf_Util2.rex from http://wi.wu-wien.ac.at:8002/rgf/rexx/orx20         */
/*                                                                           */
/* ========================================================================= */
/* Amendments                                                                */
/*                                                                           */
/* 3.00b  SN 18Sep09 Prevent error 13 on reopening previously closed files   *//*{3.00b}*/
/* 3.01g  SN 01May11 IndexByField method added                               *//*{3.01g}*/
/* 3.01h  SN 24Jun11 Bugfix: simpleappend was overwriting endofheader mark   *//*{3.01h}*/
/*                           when appending to a previously empty table      *//*{3.01h}*/
/* 3.01l  SN 03Jan12 Bugfix: indexed append failed on empty index            *//*{3.01l}*/
/* 3.01p  SN 06Dec12 Optimised createIndex (40x improvement)                 *//*{3.01p}*/
/*           11Feb13 Bugfix: zap remembered no of items if instance reopened *//*{3.01p}*/
/*                                                                           */
/* 3.02   SN   Jan14 Major rewrite of dbfIndex class                         */
/*                   +  Complex keys allowed                                 */
/*                   +  New methods [Last, RefreshIndex, MakeKey]            */
/*                   Create made class method of .dbf                        */
/* 5.0.0 SN  11Dec18 Create a bug due to changes in ooRexx 5.0.0             *//*{5.0.0}*/

/* ------------------------------------------------------------------------- */
/* NB: creating a dbf file with DBFUtils:                                    */
/*                                                                           */
/* DBFUtils created files are different.  In the field headers the           */
/* fieldoffset is set to 0 so needs to be inserted with a hex editor         */
/* (4 little endian bytes immediately after the field type).                 */
/* ------------------------------------------------------------------------- */

---<<<>>>--- diagnostic
call sysfiledelete 'c:\dbfdiag.txt'
---<<<>>>--- diagnostic


/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class dbf public
/* ========================================================================= */
/*                                                                           */
/* A class for DBF files.                                                    */
/*                                                                           */
/* ========================================================================= */
::method create CLASS    -- 3.02 moved from deprecated class dbfCreator        /*{3.02}*/
/* ------------------------------------------------------------------------- */
use strict arg filename, createstring

  fieldHeads = .array~new
  headlength = 0
  offset = 0
  do while createString \= ''
     parse var createString fieldString';'createString
     parse var fieldString fldname','type','length','dps
     fieldHead = fldname~strip~left(11,'00'x),          -- field name
                 ||type~left(1),                        -- field type
                 ||reverse(d2c(offset+1,4)),            -- field offset
                 ||d2c(length,1),                       -- field length
                 ||d2c(dps,1),                          -- decimal places
                 ||d2c(0,1),                            -- field flags
                 ||d2c(0,4),                            -- Next Value
                 ||d2c(0,1),                            -- step value
                 ||d2c(0,8)                             -- reserved
     fieldHeads~append(fieldHead)
     offset += length
  end /* DO */

  parse value date('s') with yyyy 5 mm 7 dd
  yy = yyyy-1900
  header = '03'x,                         --  'FoxBASE+/Dbase III plus, no memo'
           ||d2c(yy,1)||d2c(mm)||d2c(dd), -- last updated today
           ||reverse(d2c(0,4)),           --  number of records in file
           ||reverse(d2c(32 * (fieldHeads~items + 1) + 1,2)), -- data offset
           ||reverse(d2c(offset + 1,2)),  --  record length
           ||d2c(0,15),                   --  reserved
           ||d2c(0,1),                    --  header flags
           ||d2c(0,2),                    --  codepage
           ||d2c(0,2)                     --  reserved

  dbf = .stream~new(filename)
  dbf~open('write replace')
  dbf~charout(header||fieldHeads~makeString('c')||'0d'x)
  dbf~close

return
/* ========================================================================= */
/*                          Instance Methods                                 */
/* ========================================================================= */
::attribute fieldArr                        -- array accesible by order
::attribute fields                          -- directory accesible by name
::attribute curRowDeleted                   -- bool last record fetched
                                            -- was marked for deletion
::attribute stream                          -- stream supplier for dbf file
::attribute rownumber                       -- row number of current row or -1 /*{3.01h}*/
::attribute header                          -- raw header data from file
::attribute error                           -- bool to indicate stream error
::attribute errortext                       -- text relating to error
::attribute rawRowData                      -- row data read by next
::attribute rawfieldHeaders                 -- field header data as read
::attribute various                         -- directory for subclass use
::attribute returnType                      -- what data is returned in        /*{3.01p}*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* The following file attribute objects themselves have two attributes:      */
/*                                                                           */
/*     Value       -- the value of the attribute in the DBF header           */
/*     Text        -- the attribute converted into text format               */
/*                                                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
::attribute name
::attribute type
::attribute lastUpdate
::attribute items
::attribute dataOffset
::attribute recordLength
::attribute tableFlags
::attribute codePage
::attribute reserved
::attribute blankBadData
::attribute index                     -- hook that user can hang an index on
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose log

use strict arg file, log = .nil

   parse value filespec('n',file) with table'.'.
   self~Name          = .dbfAttribute~new(file,table)

   self~error         = .false
   self~errorText     = ''
                                                           -- for subclass use
   self~various       = .directory~new~~setMethod('UNKNOWN',"Return ''")
   self~fieldArr      = .array~new
   self~fields        = .directory~new
   self~stream        = .stream~new(file)
   self~rawRowData    = .nil     -- indicates that rows have not yet been read
   self~blankBadData  = .false   -- true - replace with '', false raise error
   self~rownumber     = -1                                                     /*{3.01h}*/
   self~index         = .nil     -- hook for user to hang index on             /*{3.01h}*/
   self~returnType    = 'STEM'

   state = self~open
   if state \= 'READY:'
   then do
      self~error = .true
      self~errorText = 'Opening' file 'returned ['||state||']'
      return
   end /* DO */

   self~curRowDeleted = .nil                            -- no record read yet

   /* first 32 bytes of the file are the file header details */
   /* it is followed by 32 bit records for each field header */
   self~header = self~stream~CharIn(,32)
   header = self~header

   if header~length \= 32
   then do
      self~error = .true
      self~errortext = 'File does not exist or is not a dbf'
      return
   end /* DO */

   parse var header headFileType    2,  --                               1  1
                    headLastUpdate  5,  -- yy (years after 1900) mm dd   2  3
                    headRecsInFile  9,  -- little endian                 5  4
                    headDataOffset 11,  -- little endian                 9  2
                    headRecLength  13,  -- includes delete flag         11  2
                    headReserved   29,  --                              13 16
                    headTableFlags 30,  --                              29  1
                    headCodePage   31 . --                              30  1

   select
      when headFileType = '03'x then hft = 'FoxBASE+/Dbase III plus, no memo'
      when headFileType = '02'x then hft = 'FoxBASE'
      when headFileType = '30'x then hft = 'Visual FoxPro'
      when headFileType = '31'x then hft = 'Visual FoxPro, autoincrement enabled'
      when headFileType = '43'x then hft = 'dBASE IV SQL table files, no memo'
      when headFileType = '63'x then hft = 'dBASE IV SQL system files, no memo'
      when headFileType = '83'x then hft = 'FoxBASE+/dBASE III PLUS, with memo'
      when headFileType = '8B'x then hft = 'dBASE IV with memo'
      when headFileType = 'CB'x then hft = 'dBASE IV SQL table files, with memo'
      when headFileType = 'F5'x then hft = 'FoxPro 2.x (or earlier) with memo'
      when headFileType = 'FB'x then hft = 'FoxBASE'
      otherwise
         self~error = .true
         self~errortext = 'Unrecognised filetype' c2x(headerFileType)
         return
   end /* select */
   self~type = .dbfAttribute~new(headFileType,hft)

   parse var headLastUpdate yy 2 mm 3 dd
   self~lastUpdate  =.dbfAttribute~new(headLastUpdate,,
                                       self~DBFDate2ISO(headLastUpdate))
   self~items       =.dbfAttribute~new(headRecsInFile,c2d(headrecsinfile~reverse))

   self~dataOffset  =.dbfAttribute~new(headDataOffset,c2d(headDataOffset~reverse))

   self~recordLength=.dbfAttribute~new(headRecLength, c2d(headRecLength~reverse))
   self~reserved    =.dbfAttribute~new(headReserved,'Reserved')

   txtArray = .array~new
   if headTableFlags~bitAnd('1'x)='1'x then txtArray~append('file has a structural .cdx')
   if headTableFlags~bitAnd('2'x)='2'x then txtArray~append('file has a Memo field')
   if headTableFlags~bitAnd('4'x)='4'x then txtArray~append('file is a database (.dbc)')
   self~tableFlags = .dbfAttribute~new(headTableFlags, txtArray~MakeString('l',', '))

   self~codePage   = .dbfAttribute~new(headCodePage, c2d(headCodePage))

   /* now get the field headers */
   fieldHeaders = self~stream~charin(,(self~DataOffset~text) - 33)
   self~rawFieldHeaders = fieldHeaders
   noOfFields           = 0
   calcOffSet           = 1          -- some dbfs do not have offset in header

   do while fieldHeaders \= ''
      parse var fieldHeaders fieldHead 33 fieldHeaders

      field                = .dbfField~new(fieldHead)                          /*{3.02}*//* dbfFileField renamed bdfField */

                                     -- some dbfs do not have offset in header
      if field~offset~text = 0 then field~offset~text = calcOffSet
      calcOffset           = calcOffset + field~length~text

      self~fieldArr~append(field)
      self~fields[field~name~text~upper]     = field

      noOfFields       = noOfFields + 1
   end /* DO */

   x = self~stream~charin(,1)        -- move past header terminator

/* ------------------------------------------------------------------------- */
::method open
/* ------------------------------------------------------------------------- */
expose log

use strict arg mode = 'both'

-- we sometimes get error 13 from os when reopening a file that has previously /*{3.00b}*/
-- been opened
   if mode \= 'read' then self~stream~flush       -- Ven Ilagan suggested this /*{3.00b}*/

   do attempt = 1 to 3
      ret = self~stream~open(mode)
      if ret = 'READY:' then leave
      parse var ret 'ERROR:'errCode
      if errCode = 2    then leave               -- file does not (yet) exist  /*{3.00b}*/
      if errCode = 13   then call sysSleep 1            -- increased from 0.1  /*{3.00b}*/
   end /* DO */

return ret

/* ------------------------------------------------------------------------- */
::method close                    -- closes the dbf stream
/* ------------------------------------------------------------------------- */
expose log

   self~rowNumber = -1                                                         /*{3.01h}*/
   ret = self~stream~close

return ret
/* ------------------------------------------------------------------------- */
::method first
/* ------------------------------------------------------------------------- */
/* first returns the read pointer to the first record                        */

   self~stream~seek('='||(self~dataoffset~text+1) 'read')
   self~rowNumber = 0                                                          /*{3.01h}*/

return self~next

/* ------------------------------------------------------------------------- */
::method previous
/* ------------------------------------------------------------------------- */

   if self~rownumber < 2 then return .nil

return self~fetch(self~rownumber - 1)

/* ------------------------------------------------------------------------- */
::method fetch                   -- fetch a row by number
/* ------------------------------------------------------------------------- */
use strict arg RowNum

   if .nil = rowNum then return .nil       -- index can request fetch of .nil
   if \rownum~datatype('w')   ,
    | rownum < 1              ,
    | rownum > self~Items~text
   then return .nil

   recOffSet = (rowNum - 1) * self~recordLength~text
   self~stream~seek('='||(self~dataoffset~text + 1 + recOffset) 'read')
   self~rowNumber = (rowNum - 1)                  -- next will increment this

return self~next

/* ------------------------------------------------------------------------- */
::method next                     -- returns a stem with the original values
/* ------------------------------------------------------------------------- */
expose returnType

if self~stream~chars < self~recordLength~text then return .nil

   row = self~stream~charin(,self~recordLength~text)
   self~rawRowData = row
   self~curRowDeleted = (row~substr(1,1) \= '20'x)

-- 3.02 make returned directory caseless wrt to field names
   select
      when 'Directory'~caselessAbbrev(returntype)
         then do                                                               /*{3.02}*/
            mTxt = "if self~hasIndex(upper(arg(1)))  ",                        /*{3.02}*/
                   "then return self[upper(arg(1))]; ",                        /*{3.02}*/
                   "else return .nil                 "                         /*{3.02}*/
            dataset = .directory~new~~setmethod('UNKNOWN',mTxt)                /*{3.02}*/
         end /* DO */                                                          /*{3.02}*/
      when 'Table'~caselessAbbrev(returntype)
         then dataset = .table~new
      otherwise                                        dataset = .stem~new
   end /* select */

   do field over self~fieldArr
      fieldType = field~type~value
      data      = row~substr(field~offset~text + 1, field~length~text)

      select
         when data      = ''  then nop -- ignore blank fields
         when fieldtype = 'C' then data = data~strip('t')
         when fieldType = 'N' then data = data~strip
         when fieldtype = 'D' then if valid_date(data)
                                   then data = date('s',data,'s','-')
                                   else if self~blankBadData
                                        then data = ''
                                        else raise syntax 40.900 array('Data in field of type date is not valid - found' data)
         otherwise nop
      end /* select */
      dataset[field~name~text] = data
   end /* DO */

   self~rowNumber += 1                                                         /*{3.01h}*/

--return a.
  return dataset

/* ------------------------------------------------------------------------- */
::method pack
/* ------------------------------------------------------------------------- */
/* removes physically from a file rows marked for deletion                   */
/* PACK CLOSES THE FILE ON COMPLETION                                        */
/*                                                                           */
/* return codes:                                                             */
/*    -1 - an error has occured                                              */
/*     0 - nothing to delete                                                 */
/*     n - number of rows deleted                                            */
/*                                                                           */
/* ------------------------------------------------------------------------- */
tempfile = self~createTempFile
if tempfile = .nil then return -1

   rows = 0 ; deletions = 0

   a. = self~first
   do while a. \= .nil
      if self~curRowdeleted
      then deletions = deletions + 1
      else do
         tempfile~charout(self~rawRowData)
         rows = rows + 1
      end /* DO */
      a. = self~next
   end /* DO */

   -- build new header string to overwrite lastupdate||items  YYMMDDrrrrrrrr
   headerString = self~ISODate2DBF(date('s',,,'-'))||reverse(d2c(rows,4))

   tempfile~charout('1A'x)                              -- end of file marker
   tempFile~charout(headerString,2)
   tempfile~close

   self~items~text = rows
   self~close

   if deletions > 0 then call sysFileCopy tempfile~qualify, self~name~value
   call sysFiledelete tempfile~qualify

return deletions

/* ------------------------------------------------------------------------- */
::method delete
/* ------------------------------------------------------------------------- */
expose rowNumber

use arg rows, marker = '*'
if \rows~isA(.set) then rows =.set~of(rows)

   saved_read_pointer = self~stream~query('seek read char') --save read pointer

      data_start = self~dataoffset~text + 1
      do target over rows
         disp = data_start + ((target - 1) * self~recordLength~text)
         self~stream~charout(marker,disp)               -- mark row as deleted
         if target = rowNumber then self~curRowDeleted = .true
      end /* DO */

   -- build new header string to overwrite lastupdate  YYMMDD
      headerString = self~ISODate2DBF(date('s',,,'-'))
      self~stream~charout(headerString,2)

   self~stream~seek('='||saved_read_pointer 'read')     --restore read pointer

return

/* ------------------------------------------------------------------------- */
::method undelete
/* ------------------------------------------------------------------------- */
use arg rows

   self~delete(rows,' ')            -- pass undelete marker to delete method

return

/* ------------------------------------------------------------------------- */
::method update
/* ------------------------------------------------------------------------- */
use strict arg rows, dataCollection
if \rows~isA(.set) then rows =.set~of(rows)

saved_read_pointer = self~stream~query('seek read char') -- save read pointer

   self~errortext = ''
   do row over rows
      if \row~datatype('w')
      then do
         self~error = .true
         self~errortext = "Update method expected a rownumber - found" row
         return -1
      end /* DO */

      do fieldname over dataCollection
         fieldObj = self~fields[fieldName~upper]
         if fieldObj = .nil
         then do
            self~error = .true
            self~errortext = 'could not find field ['fieldname']' ,
                             'to update on row' row ,
                             'of' self~name~text    ,
                             'found ['self~fields~allIndexes~makestring('l',', ')']'
            return -1
         end /* DO */
         else do
            displacement =   self~dataoffset~text ,
                           + ((row - 1) * self~recordLength~text) ,
                           + fieldObj~offset~text ,
                           + 1
            datum = datacollection[fieldname]
            fieldType = fieldObj~type~value
            select
               when fieldtype = 'C'
                  then datum = datum~left(fieldObj~length~text)
               when fieldType = 'D'            /* convert possible sql style dates */
                  then datum = datum~changeStr('-','')~left(8)
               when fieldtype = 'L'
                  then select
                          when datum = 1 then datum = 'T'
                          when datum = 0 then datum = 'F'
                          otherwise
                             datum = datum~strip~left(1)
                       end /* select */
               when fieldType = 'N'
                  then do
                     if \datum~datatype('n')
                     then do
                        self~error = .true
                        self~errortext = 'Bad data updating' self~name~text 'row' row||'. ',
                                         'Field' fieldname 'should be type' fieldtype||'. ',
                                         'Found' datum
                        return -1
                     end /* DO */
                     /* there may be a silent truncate here - report it? */
                     datum = datum~format(fieldObj~length~text,fieldObj~dps~text),
                                  ~right(fieldobj~length~text)
                  end /* DO */
               otherwise
                  nop               /* we only deal with the dbaseIV types here */
            end /* select */
         self~stream~charout(datum,displacement)
         end /* DO */
      end /* DO */
   end /* DO */

   -- build new header string to overwrite lastupdate  YYMMDD
   headerString = self~ISODate2DBF(date('s',,,'-'))
   self~stream~charout(headerString,2)

self~stream~seek('='||saved_read_pointer 'read')       -- restore read pointer

return 0

/* ------------------------------------------------------------------------- */
::method append
/* ------------------------------------------------------------------------- */
use arg datacollection

saved_read_pointer = self~stream~query('seek read char') -- save read pointer
self~errortext = ''

-- do the append
-- build new row string template to append
   rowLen = self~recordLength~text + 1                 -- +1 for delete marker
   row = .mutableBuffer~new(copies(' ',rowlen),rowLen)

/* insert the fields into the template */
   do fieldName over datacollection
      field     = self~fields[fieldname~upper]
      if .nil = field
      then do
         error = .true
         errortext = 'Unrecognised field' fieldname
         iterate
      end /* DO */
      fieldType = field~type~value                 -- ~type~value = DBF type char
      select
         when fieldType = 'C'
            then fieldData = datacollection[fieldName]~strip('T')
         when fieldType = 'N'
            then if datacollection[fieldName]~dataType('N')
                 then fieldData = datacollection[fieldName]~format(,field~dps~text),
                                                ~right(field~length~text)
                 else fieldData = ''
         when fieldType = 'D'                    -- accept YYYY-MM-DD or YYYYMMDD
            then fieldData = space(datacollection[fieldName],'0','-')
         when fieldType = 'L'
            then if (datacollection[fieldName]~verify('tT'||.true) = 0)
                 then fieldData = 'T'
                 else fieldData = 'F'
         otherwise
            fieldData = datacollection[fieldName]~strip('t') -- default for unknown fieldtype
      end /* select */
      row~overlay(fieldData,field~offset~text + 1) -- 1st char is delete marker
   end /* DO */

-- write record at calculated end of file
   displacement = self~dataoffset~text + ((self~items~text) * self~recordLength~text) + 1
   self~stream~charout(row~string,displacement)

   self~items~text = self~items~text + 1

-- build new header string to overwrite lastupdate||items  YYMMDDrrrrrrrr
   headerString = self~ISODate2DBF(date('s',,,'-'))||,
                  reverse(d2c(self~items~text,4))
   self~stream~charout(headerString,2)

-- if this is the first record then replace the end of header marker           /*{3.01h}*/
-- which is sometimes an endoffile in empty tables                             /*{3.01h}*/
   if self~items~text = 1 then self~stream~charout('0d'x,self~dataOffset~text) /*{3.01h}*/

-- pop a new end of file marker on end of file

   self~stream~seek('<0 write')
   self~stream~charout('1A'x)

self~stream~seek('='||saved_read_pointer 'read')       -- restore read pointer


if self~errortext = ''
then do
   self~rowNumber = self~items~text
   return self~rowNumber
end
else return -1

/* ------------------------------------------------------------------------- */
::method ZAP
/* ------------------------------------------------------------------------- */
use strict arg quiet = ' '

   if \quiet~upper~abbrev('QUIET')
   then if RxMessageBox('This will DELETE ALL records!','Warning',"OKCANCEL","EXCLAMATION") \= 1
        then RETURN -1                                                     -->|

   tempfile = self~createTempFile
   if tempfile = .nil
   then RETURN -2                                                          -->|

   -- build new header string to overwrite lastupdate||items  YYMMDDrrrrrrrr
   headerString = self~ISODate2DBF(date('s',,,'-'))||reverse(d2c(0,4))
   self~items~text  = 0                                                        /*{3.01p}*/

   tempfile~charout('1A'x)                               -- end of file marker
   tempFile~charout(headerString,2)
   tempfile~close

   self~close

   call sysFileCopy tempfile~qualify, self~name~value
   call sysFiledelete tempfile~qualify

return 0

/* ------------------------------------------------------------------------- */
::method createIndex                            -- returns a dbfindex object
/* ------------------------------------------------------------------------- */
/* 3.02 - return a new complex index.  Fast indexes dropped                  *//*{3.02}*/
/*        previously arg(1) limited to fieldname - now indexDefinitionString *//*{3.02}*/
use arg definition, options, fast? = .false
return .dbfIndex~new(self, definition, options)

/* ------------------------------------------------------------------------- */
::method dbfData                                                               /*{3.02}*/
/* ------------------------------------------------------------------------- */
/* returns all the row data as a single string                               */

   saved_read_pointer = self~stream~query('seek read char') --save read pointer

      data = self~stream~charIn(self~dataOffset~text + 1                      -
                               ,self~items~text * self~recordLength~text)

   self~stream~seek('='||saved_read_pointer 'read')      --restore read pointer

return data
/* ------------------------------------------------------------------------- */
::method FieldExists
/* ------------------------------------------------------------------------- */
use arg fieldName

   do field over self~fieldArr
      if field~name~text~caselessequals(fieldname) then RETURN .true      -->|
   end /* DO */

RETURN .false                                                              -->|

/* ------------------------------------------------------------------------- */
::method FieldInsert
/* ------------------------------------------------------------------------- */
use arg name, type, length = 1, dps = 0, colNo = ''

   if \colNo~datatype('w') then colNo = -1

   if self~fieldExists(name) then RETURN -1                                -->|

   createStrArr = .Array~new
   newFieldStr  = .array~of(name,type,length,dps)~makeString('l',',')

   do i = 0 to max(self~fieldArr~items,colNo)
      if i = colNo then createStrArr~append(newFieldStr)
      if i > 0 ,
      ,  self~fieldArr[i]~isA(.dbfField)
      then createstrArr~append(.array~of(self~fieldArr[i]~name~text,
                                        ,self~fieldArr[i]~type~value,
                                        ,self~fieldArr[i]~length~text,
                                        ,self~fieldArr[i]~dps~text)~makeString('l',','))
   end /* DO */
   if colNo = -1 then createStrArr~append(newFieldStr)    -- append to extant fields

   createStr = createStrArr~makeString('l',';')||';'

   tfn = self~getTempFileName

   .dbf~Create(tfn,createStr)
   dbf = .dbf~new(tfn)
   deletes = .set~new
   a. = self~first
   do while a. \= .nil
      row = dbf~append(a.)
      if self~curRowDeleted then deletes~put(row)
      a. = self~next
   end /* DO */
   if deletes~items > 0 then dbf~delete(deletes)
   dbf~close

   if self~error then RETURN -2                                            -->|
   if dbf~error  then RETURN -3 dbf~errortext                              -->|

   -- put the new file in place of this one (closed) ----------------------
   self~close
   call sysfiledelete self~name~value
   call sysFileMove tfn, self~name~value

return rc

/* ------------------------------------------------------------------------- */
::method dataset
/* ------------------------------------------------------------------------- */
/* returns a directory with the arguments indexed by field names             */
/* arguments must be in the order of the table definition                    */
arr = arg(1,'a')

   d = .directory~new
   do i = 1 to arr~items
      d[self~fieldArr[i]~name~text] = arr[i]
   end /* DO */

return d

/* ------------------------------------------------------------------------- */
::method DBFDate2ISO private
/* ------------------------------------------------------------------------- */
use arg DBFDate
   parse var DBFDate yy 2 mm 3 dd
return right(c2d(YY)+1900,4,0)||right(c2d(mm),2,'0')   ||right(c2d(dd),2,'0')

/* ------------------------------------------------------------------------- */
::method ISODate2DBF private
/* ------------------------------------------------------------------------- */
use arg ISODate
   parse var ISOdate yyyy'-'mm'-'dd
return d2c(yyyy-1900,1)||d2c(mm,1)||d2c(dd,1)

/* ------------------------------------------------------------------------- */
::method createTempFile private
/* ------------------------------------------------------------------------- */

   tempfn = self~getTempFileName
   tempfile = .stream~new(tempfn)
   RC = tempfile~open('write replace')
   if RC \= 'READY:'
   then do
      self~errortext='Could not open temporary file' tempfile '(RC='||RC||')'
      self~error = .true
      return .nil
   end /* DO */

   tempfile~charout(self~header||self~RawFieldHeaders||'0d'x)

return tempfile

/* ------------------------------------------------------------------------- */
::method getTempFileName private
/* ------------------------------------------------------------------------- */

   parse upper source os .
   select
      when os~left(7) = 'WINDOWS' then tempdir = value('TEMP',,'environment')
      when os         = 'LINUX'   then tempdir = value('TMPDIR',,'environment') -- not tested
      otherwise                        tempdir = ''
   end /* select */
   if tempdir = '' then tempdir = 'C:\'

return sysTempFileName(tempdir||'\'self~name~text~left(6)~strip||'??.dbf')

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */


/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class dbfField     private  -- renamed from dbfFileField (3.02)              /*{3.02}*/
/* ========================================================================= */
::attribute name
/* - - - - - - - - - - - - from the dbf file header  - - - - - - - - - - - - */
::attribute type
::attribute offset
::attribute length
::attribute dps
::attribute flags
::attribute next                          -- for autoIncrement
::attribute step                          -- for autoIncrement
::attribute reserved                      -- reserved
::attribute header
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
::attribute various
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
use arg header
self~header  = header
self~various = .directory~new~~setMethod('UNKNOWN',"Return ''")

/* parse out the header fields */
parse var Header fldName   12 ,
                 fldType   13 ,
                 fldOffSet 17 ,      -- little endian
                 fldLength 18 ,
                 fldDPs    19 ,
                 fldFlags  20 ,
                 fldNext   24 ,
                 fldStep   25 ,
                 fldResv

self~name   = .dbfAttribute~new(fldName,fldName~strip('t','00'x))
select
   when fldType = 'C' then self~type = .dbfAttribute~new(fldType,'Character')
   when fldType = 'N' then self~type = .dbfAttribute~new(fldType,'Numeric'  )
   when fldType = 'F' then self~type = .dbfAttribute~new(fldType,'Float'    )
   when fldType = 'D' then self~type = .dbfAttribute~new(fldType,'Date'     )
   when fldType = 'L' then self~type = .dbfAttribute~new(fldType,'Logical'  )
   when fldType = 'M' then self~type = .dbfAttribute~new(fldType,'Memo'     )
   when fldType = 'G' then self~type = .dbfAttribute~new(fldType,'General'  )
   when fldType = 'B' then self~type = .dbfAttribute~new(fldType,'Double'   )
   when fldType = 'I' then self~type = .dbfAttribute~new(fldType,'Integer'  )
   when fldType = 'Y' then self~type = .dbfAttribute~new(fldType,'Currency' )
   when fldType = 'T' then self~type = .dbfAttribute~new(fldType,'DateTime' )
   when fldType = 'P' then self~type = .dbfAttribute~new(fldType,'Picture'  )
   otherwise               self~type = .dbfAttribute~new(fldType,'Unknown'  )
end /* select */
self~offSet       = .dbfAttribute~new(fldOffSet, c2d(fldOffSet~reverse))

self~length       = .dbfAttribute~new(fldLength, c2d(fldLength))
self~dps          = .dbfAttribute~new(fldDPs,    c2d(fldDPs))

flagArray = .array~new
if flagset(fldFlags,'01'x) then flagArray~append('System Column (not visible to user)')
if flagSet(fldFlags,'02'x) then flagArray~append('Column can store null values')
if flagSet(fldFlags,'04'x) then flagArray~append('Binary column (for CHAR and MEMO only)')
if flagSet(fldFlags,'0C'x) then flagArray~append('Column is autoincrementing')
self~flags = .dbfAttribute~new(fldFlags, flagArray~makeString('l',', '))

self~Next         = .dbfAttribute~new(fldNext, c2d(fldNext~reverse))
self~step         = .dbfAttribute~new(fldStep, c2d(fldStep))
self~reserved     = .dbfAttribute~new(fldResv, 'Reserved')

return

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
flagset: procedure
return (bitAnd(arg(1),arg(2)) == arg(2))

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */


/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class dbfAttribute
/* ========================================================================= */
::attribute value                       -- value as it appears in the header
::attribute text                        -- readable value
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose value text
use arg value, text

/* ------------------------------------------------------------------------- */
::method string
/* ------------------------------------------------------------------------- */
expose text
return text

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */


/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class dbfIndex
/* ========================================================================= */
/* major rewrite 3.02 */                                                       /*{3.02}*/
::attribute comparator              -- used to compare rows in index-vector    /*{3.02}*/
::attribute vector                  -- array: key||row ordered by comparator   /*{3.02}*/
::attribute orderedRows             -- array: row     (index matched to vector)/*{3.02}*/
::attribute relKey2Row              -- relation: key -> row (was called rel)   /*{3.02}*/
::attribute elements                -- rules to construct the key & comparator /*{3.02}*/
::attribute dbf                     -- the dbf object this index applies to
/* ------------------------------------------------------------------------- */
::method init                       -- major rewrite                           /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf elements comparator keypos includeDeletes? currentIndex
use arg dbf, definition, options     -- nb: prev index had 4th parm dbfdata

   includeDeletes? = (options~caselesspos('del') > 0)

   defs           = definition~makearray(';')
   keypos         = 1
   elements       = .array~new    -- elements of the index
   comparatorArgs = .array~new    -- arg string to construct comparator with
   currentIndex   = .nil          -- used by skip to remember orderedRows index

   defNo = 0
   do keydef over defs

      parse upper var keydef fieldname','start','length','direction','case
      defNo += 1
      if fieldname = '' then iterate          -- allow trailing ';'        --^|

      fieldHeader = dbf~fields[fieldname~upper]
      if fieldHeader = .nil
      then do
         errMsg = 'dbfIndex definition "'definition'" contains field'         -
                  defno "("fieldname") not from table" dbf~NAME~text"."       -
                  "Expected any of ("dbf~fields~makearray~sort~makestring('l',', ')")."
                  raise syntax 93.900 array (errMsg)
      end /* DO */

      if \start~datatype('w'),start \= ''
      then do
         errMsg = 'dbfIndex definition "'definition'" field' defNo            -
                  "("fieldname") has start value ("start"). "                 -
                  "Expected integer, blank or ommitted."
                  raise syntax 93.900 array (errMsg)
      end /* DO */

      if \length~datatype('w'),length \= ''
      then do
         errMsg = 'dbfIndex definition "'definition'" field' defNo            -
                  "("fieldname") has length value ("length"). "               -
                  "Expected integer, blank or ommitted."
                  raise syntax 93.900 array (errMsg)
      end /* DO */

      if 'AD '~caselesspos(direction~left(1))= 0
      then do
         errMsg = 'dbfIndex definition "'definition'" field' defNo            -
                  "("fieldname") has direction value ("direction"). "         -
                  "Expected A, D or ommitted."
                  raise syntax 93.900 array (errMsg)
      end /* DO */

      if 'CIN '~caselesspos(case~left(1))= 0
      then do
         errMsg = 'dbfIndex definition "'definition'" field' defNo            -
                  "("fieldname") has case value ("case"). "                   -
                  "Expected C, I, N or ommitted."
                  raise syntax 93.900 array (errMsg)
      end /* DO */

      fieldType   = fieldheader~type~value
      fieldLength = fieldHeader~length~text
      rawStart    = fieldHeader~offSet~text + 1

      element = .dbfIndexElement~new(fieldName, fieldType, keypos             -
                                    ,fieldLength, rawStart, direction, case)

      select
         when 'CDN'~caselesspos(fieldtype) > 0
            then do
               if start~datatype('w')  then element~start  = start
               if length~datatype('w') then element~length = length
               if element~start  \= 1 ,
               |  element~length \= fieldlength
               then element~subString? = .true
            end /* DO */
         otherwise nop      -- no action for field type 'L'
      end /* select */

      comparatorArgs = comparatorArgs~union(element~comparatorArgArray)

      keypos += element~length
      elements~append(element)
   end /* DO */

   if \.StringColumnComparator~hasMethod('NEW') then call 'rgf_util2.rex'
   comparator = .StringColumnComparator~new(comparatorArgs)

   self~refreshIndex

/* ------------------------------------------------------------------------- */
::method refreshIndex                                                          /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf vector orderedrows relKey2Row comparator keypos includeDeletes? itemslength

   dbfData = dbf~dbfData   -- get all rows as raw text

-- create the index artefacts
   vector      = .array~new(dbf~items~text)  -- ordered keys with row suffix
   orderedRows = .array~new(dbf~items~text)  -- row numbers
   relKey2Row  = .relation~new

   /* index creation optomised (used to use dbf~methods)                     */
   records      = dbf~items~text
   itemsLength  = records~length
   recordLength = dbf~recordLength~text

   recordStart = 1
   do i = 1 to records
      delmark    = dbfData~substr(recordStart,1)
      recordData = dbfData~subStr(recordStart,recordLength)

      if includeDeletes? | delMark \= '*'
      then do
         key = self~makeKey(recordData)
         vector~append(key||i~right(itemsLength))
         relKey2Row~put(i,key~strip('t'))
      end                      -- nb orderedRows populated after vector sorted
      recordStart += recordLength
   end /* DO */

   vector~stableSortWith(comparator)   -- arrange the array in order of key values

   do i = 1 to vector~items    -- orderedRows allows one to traverse the index
      parse value vector[i] with . =(keypos) row .
      orderedRows~append(row)
   end /* DO */

/* ------------------------------------------------------------------------- */
::method makeKey                                                               /*{3.02}*/
/* ------------------------------------------------------------------------- */
/* data can be entire database or rowdataset                                 */
expose elements
use arg data
  dataIsDir? = data~isA(.mapCollection)         -- was ~hasMethod('[]') before /*{5.0.0}*/
  key = .array~new
  do element over elements
     if dataIsDir?
     then datum = data[element~fieldname]
     else datum = data~substr(element~rawstart, element~rawlength)
      select
         when element~substring?
            then datum = datum~substr(element~start,element~length)
         otherwise
                 datum = datum~left(element~length)
      end /* select */
      key~append(datum)
  end /* DO */

return key~makeString('c')

/* ------------------------------------------------------------------------- */
::method first                                                                 /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf orderedRows
return dbf~fetch(orderedRows[orderedRows~first])

/* ------------------------------------------------------------------------- */
::method last                                                                  /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf orderedRows
return dbf~fetch(orderedRows[orderedRows~last])

/* ------------------------------------------------------------------------- */
::method next                                                                  /*{3.02}*/
/* ------------------------------------------------------------------------- */
use arg skip = 1
return self~skip(skip)

/* ------------------------------------------------------------------------- */
::method previous                                                              /*{3.02}*/
/* ------------------------------------------------------------------------- */
use arg skip = 1
return self~skip(-1 * skip)

/* ------------------------------------------------------------------------- */
::method skip           private                                                /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf orderedRows currentIndex
use strict arg skip

   if currentIndex \= .nil,
   ,  orderedRows[currentIndex] = dbf~RowNumber
   then nop
   else currentIndex = orderedRows~index(dbf~RowNumber)

   if currentIndex = .nil
   then return .nil
   else do
      currentIndex += skip
      return dbf~fetch(orderedRows[currentIndex])
   end /* DO */

/* ------------------------------------------------------------------------- */
::method find -- finds exact match for key                                     /*{3.02}*/
/* ------------------------------------------------------------------------- */
/* mode - which index to return on multiple equality                         */
expose dbf relKey2Row
use arg key, mode

  if \'Last'~caselessAbbrev(mode) then mode = 'First'

  matches = relKey2Row~allAt(key~strip('t'))
  if matches~items > 0
  then if mode = 'First'
       then return dbf~fetch(matches[matches~first])  -- first match
       else return dbf~fetch(matches[matches~last])   -- last match
  else      return .nil                               -- no match found

/* ------------------------------------------------------------------------- */
::method findGE                                                                /*{3.02}*/
/* ------------------------------------------------------------------------- */
use arg key
return self~seek(key,'ge')

/* ------------------------------------------------------------------------- */
::method findLE                                                                /*{3.02}*/
/* ------------------------------------------------------------------------- */
use arg key
return self~seek(key,'le')

/* ------------------------------------------------------------------------- */
::method seek private                                                          /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf vector comparator keypos
use arg key, mode = 'ge'

-- search is ge or le
   keys = vector~items
   if keys = 0                                               then return .nil
   if mode = 'le', comparator~compare(vector[1],key)    =  1 then return .nil -- >> impossible search
   if mode = 'ge', comparator~compare(vector[keys],key) = -1 then return .nil -- << impossible search

-- binary chop to find nearest match position
   lo = 1 ; hi = keys
   do until abs(hi-lo) < 2
      mid = (hi + lo) % 2
      comp = comparator~compare(vector[mid],key)
      select
         when comp =  1 then hi = mid                                    -- >>
         when comp = -1 then lo = mid                                    -- <<
         otherwise           leave                                       -- ==
      end /* select */
   end /* DO */

-- binary chop may find record with a non-unique key
-- so we have to traverse to the first match (ge) or last match (le)
   select
      when mode = 'ge'
         then do                     -- find first candidate
            do mid = mid to 1 by -1
               if comparator~compare(vector[mid],key)  = -1,            -- <<
               |  mid = 1
               then leave
            end /* DO */
            do mid = mid to keys
               if comparator~compare(vector[mid],key) \= -1,            -- >>=
               |  mid = keys
               then leave
            end /* DO */
         end /* DO */
      when mode = 'le'
         then do                     -- find least candidate
            do mid = mid to keys
               if comparator~compare(vector[mid],key)  = 1,              -- >>
               |  mid = keys
               then leave
            end /* DO */
            do mid = mid to 1 by -1
               if comparator~compare(vector[mid],key) \= 1,              -- <<=
               | mid = 1
               then leave
            end /* DO */
         end /* DO */
      otherwise return .nil                            -- unknown search mode
   end /* select */

-- the row number is suffixed to the key in vector
   row = vector[mid]~substr(keypos)~strip

return dbf~fetch(row)

/* ------------------------------------------------------------------------- */
::method append                                                                /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf vector orderedrows relKey2Row comparator keypos itemslength
use arg data

   row  = dbf~append(data)      -- append the row
   if row = -1 then return row  -- append failed                           -->|
   if row~length > itemslength
   then do                      -- we have outgrown index
      self~refreshIndex         -- make a bigger one
      RETURN row                                                           -->|
   end /* DO */

   key  = self~makeKey(data)
   item = key||row~right(itemsLength)
   relKey2Row~put(row,key~strip('t'))                                                            /*{3.02d}*//*{3.02f}*/

-- find where to put it in vector
   lo = 1 ; hi = vector~items
   select
      when lo > hi
         then do                                          -- this is only row
            vector      = .array~of(item)
            orderedRows = .array~of(row)
         end /* DO */
      when comparator~compare(key,vector[lo]) = -1                       -- <<
         then do                                          -- new first row
            vector      = .array~of(item)~union(vector)
            orderedRows = .array~of(row)~union(orderedRows)
         end /* DO */
      when comparator~compare(key,vector[hi]) =  1                       -- >>
         then do                                          -- new last row
            vector~append(item)
            orderedRows~append(row)
         end /* DO */
      otherwise -- somewhere in the middle of the array
         do until abs(hi-lo) < 2
            mid = (hi + lo) % 2
            hit = vector[mid]
            comp = comparator~compare(hit,key)
            select
               when comp =  1 then hi = mid                              -- >>
               when comp = -1 then lo = mid                              -- <<
               otherwise leave                                           -- ==
            end /* select */
         end /* DO */
         vector      = insertIntoArray(vector     , item, hi)
         orderedRows = insertIntoArray(orderedRows, row , hi)
   end /* select */

return row

/* ------------------------------------------------------------------------- */
::method refresh                                                               /*{3.02}*/
/* ------------------------------------------------------------------------- */
expose dbf
use arg datacollection

   if \datacollection~isA(.array) then dataCollection = .array~of(dataCollection)

   ok = .true
   do dataset over datacollection
      key = self~makeKey(dataset)
      row = self~find(key)

      if row = .nil
      then ret = self~append(dataset)
      else ret = dbf~update(dbf~rowNumber,dataset)
      ok = ok & (ret \= -1)
   end /* DO */

   if ok
   then return datacollection~items
   else return -1

/* ========================================================================= */
/*                                     =                                     */



/*                                     =                                     */
/* ========================================================================= */
/* ========================================================================= */
::class dbfIndexElement    -- part of dbfIndex framework element               /*{3.02}*/
/* ========================================================================= */
::attribute fieldname
::attribute fieldType
::attribute keypos
::attribute substring?
::attribute format?
::attribute start
::attribute length
::attribute rawStart
::attribute rawLength
::attribute integers
::attribute dps
::attribute format
::attribute direction
::attribute case
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose fieldname fieldtype keypos length rawstart rawlength subString? start -- format?
use arg fieldname, fieldtype, keypos, length, rawstart, direction, case

   rawlength  = length
   subString? = .false
-- format?    = .false
   start      = 1

   if direction~left(1)~upper = 'D'
   then self~direction = 'D'
   else self~direction = 'A' -- default to ascending

   if 'CINO'~caselessPos(case) > 0
   then self~case = case
   else self~case = 'C'

/* ------------------------------------------------------------------------- */
::method comparatorArgArray     -- return comparator argument array
/* ------------------------------------------------------------------------- */
expose keypos length direction case
return .array~of(keypos,length,direction,case)

/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */






/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class dbf4SQL     subclass dbf public
/* ========================================================================= */
::attribute RowParseTemplate  Private  -- SQL safe row parse template
::attribute fields                     -- overwrite superclass field directory
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
use strict arg file,log = .nil

self~init:super(file,log)

myMy             = .MYSql~new          -- MySQL helper provides safe names
fieldNames       = .set~new            -- set to prevent name duplication
fields           = .directory~new
self~name~text   = myMy~safeName(self~name~text)


                  /* go through fields replacing names with MYSQL safe names */
                  /* and build a new parse template using those names        */
do field over self~fieldArr
   fName            = field~name~text
   field~Name       = .dbfAttribute~new(fName,myMy~safeName(fName,fieldNames))
   self~fields[field~name~text~upper] = field -- put field obj where we can find it
end /* DO */

/* ------------------------------------------------------------------------- */
::method next             -- returns a stem with safe names and trimmed values
                          -- to get records with dbf names use ~next:super
/* ------------------------------------------------------------------------- */
use arg option
if self~stream~chars < self~recordLength~text then return .nil  -- end of file

skips = self~various~SKIP~upper

row = self~stream~charin(,self~recordLength~text)       -- get row data
self~curRowDeleted = (rowDeleted!? \= '20'x)            -- set deleted flag

   select
      when 'Directory'~caselessAbbrev(self~returntype) then dataset = .directory~new
      when 'Table'~caselessAbbrev(self~returntype)     then dataset = .table~new
      otherwise                                             dataset = .stem~new
   end /* select */


do field over self~fieldArr
   if skips~wordpos(field~name~text~upper) then iterate

   data      = row~substr(field~offset~text + 1, field~length~text)
   fieldType = field~type~value  /* ~type~value = DBF type char */
   select
      when fieldType = 'C'
         then data = "'"||data~strip('t')~changeStr("''",'"'),
                                         ~changeStr("'","''"),
                                         ~changeStr('\','\\')"'"
      when fieldType = 'N'
         then data = data~strip
      when fieldType = 'D'
         then if data = ''
              then data = ''
              else data = convdate(data)
      when fieldType = 'L'
         then data = "'"||(data = 'T'| data = 1)||"'"
      otherwise
           nop
   end /* select */
   dataset[field~name~text] = data
end /* DO */


if option = 'SQL'
then do
   cols = '(' ; vals = ' VALUES('
   do col over dataset
      if dataset[col]~strip('b','"')~strip('b',"'") \= ''
      then do
         cols = cols||col||','
         vals = vals||dataset[col]||','
      end
   end /* DO */
   cols = cols~strip('t',',')||')'
   vals = vals~strip('t',',')||')'
   return 'insert into' self~name cols vals
end /* DO */

RETURN dataset

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
convdate:
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
procedure
arg SDate .

signal on syntax name baddate
retDate = "'"||date('s',sDate,'s','-')||"'"
signal off syntax

RETURN retDate
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
baddate:
signal off syntax
RETURN ''

/* ------------------------------------------------------------------------- */
::Method SqlToCreate
/* ------------------------------------------------------------------------- */

Qry='create table' self~name '(id int auto_increment, primary key (id), '

do field over self~fieldArr
   Qry = Qry||field~Name~text~lower space(self~sqlType(field) field~various~CREATE)||', '
end /* DO */

Qry = qry~strip~strip('t',',')||self~various~EXTRAFIELDS||');'

return Qry

/* ------------------------------------------------------------------------- */
::METHOD SQLType private
/* ------------------------------------------------------------------------- */
use arg field

select
   when field~type~value = 'C' then return 'VARCHAR('||field~length~text||')'
   when field~type~value~matchChar(1,'NIY')
      then if field~dps~text = 0
           then select
                   when field~length~text < 3  then return 'TINYINT'
                   when field~length~text < 5  then return 'SMALLINT'
                   when field~length~text < 7  then return 'MEDIUMINT'
                   when field~length~text < 10 then return 'INTEGER'
                   otherwise                        return 'BIGINT'
                end /* select */
           else return 'DOUBLE('||field~length~text||','||field~dps||')'
   when field~type~value = 'F'
      then return 'FLOAT('||(field~length~text - field~dps~text)||','||,
                                                 field~dps~text||')'
   when field~type~value = 'D' then return 'DATE'
   when field~type~value = 'L' then return 'BOOLEAN'
   when field~type~value = 'M' then return 'TEXT'  -- ????? Is this correct?
   when field~type~value = 'B' then return 'DOUBLE('||(field~length - field~dps)||','||field~dps||')'
   when field~type~value = 'T' then return 'DATETIME'
   when field~type~value = 'P' then return 'BLOB'
   otherwise                        return 'VARCHAR('||field~length~text||')'
end /* select */

/* ------------------------------------------------------------------------- */
::method translate
/* given a dbf create string returns an sql create string                    */
/* ------------------------------------------------------------------------- */
use arg dbfstring

myMy             = .MYSql~new          -- MySQL helper provides safe names

sqlstring = ''
fill = '00'x

do while dbfstring \= ''
   parse var dbfstring dbffield';'dbfstring
   parse var dbffield fieldname','fieldtype','fieldlength','decimals
   if \fieldlength~datatype('w') then fieldlength = 0
   if \decimals~datatype('w')    then decimals    = 0

   header    = fieldname~left(11,fill)||,
               fieldtype~translate~strip~left(1)||,
               copies(fill,4)||,
               d2c(fieldlength,1)||,
               d2c(decimals,1)||,
               copies(fill,14)

   field     = .dbffield~new(header)
   sqlstring = sqlstring myMy~safeName(fieldname~strip) self~sqltype(field)||","
end /* DO */

return sqlstring~strip~strip('t',',')~strip

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */



/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class MySQL public
/* ========================================================================= */
::attribute reservedWords
::attribute MYSqlVersion
::attribute QuotedTypes
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
self~MYSQLVersion = '5.1'
res = 'ACCESSIBLE ADD ALL ALTER ANALYZE AND AS ASC ASENSITIVE BEFORE     ',
      'BETWEEN BIGINT BINARY BLOB BOTH BY CALL CASCADE CASE CHANGE CHAR  ',
      'CHARACTER CHECK COLLATE COLUMN CONDITION CONSTRAINT CONTINUE      ',
      'CONVERT CREATE CROSS CURRENT_DATE CURRENT_TIME CURRENT_TIMESTAMP  ',
      'CURRENT_USER CURSOR DATABASE DATABASES DAY_HOUR DAY_MICROSECOND   ',
      'DAY_MINUTE DAY_SECOND DATE DEC DECIMAL DECLARE DEFAULT DELAYED    ',
      'DELETE DESC DESCRIBE DETERMINISTIC DISTINCT DISTINCTROW DIV       ',
      'DOUBLE DROP DUAL EACH ELSE ELSEIF ENCLOSED ESCAPED EXISTS EXIT    ',
      'EXPLAIN FALSE FETCH FLOAT FLOAT4 FLOAT8 FOR FORCE FOREIGN FROM    ',
      'FULLTEXT GRANT GROUP HAVING HIGH_PRIORITY HOUR_MICROSECOND        ',
      'HOUR_MINUTE HOUR_SECOND IF IGNORE IN INDEX INFILE INNER INOUT     ',
      'INSENSITIVE INSERT INT INT1 INT2 INT3 INT4 INT8 INTEGER INTERVAL  ',
      'INTO IS ITERATE JOIN KEY KEYS KILL LEADING LEAVE LEFT LIKE LIMIT  ',
      'LINEAR LINES LOAD LOCALTIME LOCALTIMESTAMP LOCK LONG LONGBLOB     ',
      'LONGTEXT LOOP LOW_PRIORITY MASTER_SSL_VERIFY_SERVER_CERT MATCH    ',
      'MEDIUMBLOB MEDIUMINT MEDIUMTEXT MIDDLEINT MINUTE_MICROSECOND      ',
      'MINUTE_SECOND MOD MODIFIES NATURAL NOT NO_WRITE_TO_BINLOG NULL    ',
      'NUMERIC ON OPTIMIZE OPTION OPTIONALLY OR ORDER OUT OUTER OUTFILE  ',
      'PRECISION PRIMARY PROCEDURE PURGE RANGE READ READS READ_ONLY      ',
      'READ_WRITE REAL REFERENCES REGEXP RELEASE RENAME REPEAT REPLACE   ',
      'REQUIRE RESTRICT RETURN REVOKE RIGHT RLIKE SCHEMA SCHEMAS         ',
      'SECOND_MICROSECOND SELECT SENSITIVE SEPARATOR SET SHOW SMALLINT   ',
      'SONAME SPATIAL SPECIFIC SQL SQLEXCEPTION SQLSTATE SQLWARNING      ',
      'SQL_BIG_RESULT SQL_CALC_FOUND_ROWS SQL_SMALL_RESULT SSL STARTING  ',
      'STRAIGHT_JOIN TABLE TERMINATED THEN TINYBLOB TINYINT TINYTEXT TO  ',
      'TRAILING TRIGGER TRUE UNDO UNION UNIQUE UNLOCK UNSIGNED UPDATE    ',
      'USAGE USE USING UTC_DATE UTC_TIME UTC_TIMESTAMP VALUES VARBINARY  ',
      'VARCHAR VARCHARACTER VARYING WHEN WHERE WHILE WITH WRITE XOR      ',
      'YEAR_MONTH ZEROFILL'

self~reservedWords = .set~new~union(res~makeArray(' '))
self~reservedWords~put('ID')          -- we use ID for our primary key column

self~quotedTypes = .set~of('DATE','DATETIME','TIMESTAMP','TIME','CHAR',,
                           'VARCHAR','TEXT','TINYTEXT','MEDIUMTEXT','LONGTEXT')

/* ------------------------------------------------------------------------- */
::method safeName       -- return a column name that is not reserved
                        -- optional second arg column names already assigned
/* ------------------------------------------------------------------------- */
use arg name, names

if \names~hasmethod('HASINDEX') | \names~hasMethod('PUT') then names = .set~new

name = name~translate('_',' ')                   -- replace spaces by _
do while self~reservedWords~hasindex(name~upper) | names~hasIndex(name~upper)
   name = name||'_'                              -- suffix with _ till unique
end /* DO */

names~put(name~upper)

return name

/* ------------------------------------------------------------------------- */
/* passed an REXXSQL describe stem returns a set of columns that require     */
/* being passed within quotes to an update or insert statment                */
::method LiteralSet
/* ------------------------------------------------------------------------- */
use arg stem.

lits = .set~new
if \stem.column.name.0~datatype('w') then return lits

do i = 1 to stem.column.name.0
   if self~quotedTypes~hasIndex(stem.column.type.i)
   then lits~put(stem.column.name.i~upper)
end /* DO */

return lits

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */



/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class DBFChange public
/* a queueable placeholder for defered changes to a dbf file                 */
/* ========================================================================= */
::attribute type                -- Append, Update, Delete
::attribute dbf_id              -- Row no in dbf file (for update & delete)
::attribute stem                -- stem holding values (for update & insert)
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose type dbf_id stem
use arg type, dbf_id, stem
type = type~strip~upper~left(1)
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */




/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class DBFAppender subclass dbf public             -- DEPRECATED
-- deprecated in favour of dbf append method
/* ========================================================================= */
::attribute fileChanged?
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
use arg DBFFile, data.       -- optional second argument for one shot append

   self~init:super(DBFFile)
   self~fileChanged? = .false

   if data.~items > 0 & self~error = .false             -- this is a one shot
   then do
      self~simpleAppend(data.)
      self~close
      return self~items~text
   end /* DO */

/* ------------------------------------------------------------------------- */
::method SimpleAppend  -- append to file (ignoring indexes)
                       -- accepts a stem with field values
/* ------------------------------------------------------------------------- */
use arg data.

-- build new row string to append
   rowLen = self~recordLength~text + 1                 -- +1 for delete marker
   row = .mutableBuffer~new(copies(' ',rowlen),rowLen)

   /* insert the fields into the template */
   do fieldName over data.
      field     = self~fields[fieldname]
      fieldType = field~type~value              -- ~type~value = DBF type char

      select
         when fieldType = 'C' then fieldData = data.fieldName~strip('T')
         when fieldType = 'N'
            then if data.fieldName~dataType('N')
                 then fieldData = data.fieldName~format(,field~dps~text),
                                                ~right(field~length~text)
                 else fieldData = ''
         when fieldType = 'D'                    -- accept YYYY-MM-DD or YYYYMMDD
            then fieldData = space(data.fieldName,'0','-')
         when fieldType = 'L'
            then if (data.fieldName~verify('tT'||.true) = 0)
                 then fieldData = 'T'
                 else fieldData = 'F'
         otherwise
            fieldData = data.fieldName~strip('t') -- default for unknown fieldtype
      end /* select */
      row~overlay(fieldData,field~offset~text + 1) -- 1st char is delete marker
   end /* DO */

-- if this is the first record then replace the end of header marker           /*{3.01h}*/
-- which is sometimes an endoffile in empty tables                             /*{3.01h}*/
   if self~items~text = 0 then self~stream~charout('0d'x,self~dataOffset~text) /*{3.01h}*/

-- write record at calculated end of file
   displacement = self~dataoffset~text + ((self~items~text) * self~recordLength~text) + 1
   self~stream~charout(row~string,displacement)

   self~fileChanged? = .true
   self~items~text = self~items~text + 1

return self~items~text

/* ------------------------------------------------------------------------- */
::method close
/* ------------------------------------------------------------------------- */

   if self~fileChanged? = .true
   then do
   -- build new header string to overwrite lastupdate||items  YYMMDDrrrrrrrr
      headerString = self~ISODate2DBF(date('s',,,'-')),
                  || reverse(d2c(self~items~text,4))

      self~stream~charout(headerString,2)
      displacement = self~dataoffset~text + 1,
                   + ((self~items~text) * self~recordLength~text)
   --   self~stream~seek('<0 write')
      self~stream~charout('1A'x,displacement)
   end

return self~close:super

/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */




/*                                     =                                     */
/*                                    ===                                    */
/* ========================================================================= */
::class DBFCreator public                           -- DEPRECATED
/* this class is DEPRECATED in favour of dbf create CLASS method             */
/* ========================================================================= */
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
use strict arg filename, createstring
  .dbf~create(filename,createString)
/* ========================================================================= */
/*                                    ===                                    */
/*                                     =                                     */




/* ========================================================================= */
::routine Valid_date
/* ========================================================================= */
use strict arg data,format = 's'

signal on syntax name badDate
if format~upper = 'S'
then dummy = date('b',data,'s')
else dummy = date('s',data,format)
signal off syntax
return .true

baddate:
signal off syntax
return .false
/* ========================================================================= */



/* ========================================================================= */
::routine insertIntoArray -- inserts key into arr[pos]
/* ========================================================================= */
use arg arr, key, pos
return arr~section(1,pos-1)~union(.array~of(key))~union(arr~section(pos))
/* ========================================================================= */

