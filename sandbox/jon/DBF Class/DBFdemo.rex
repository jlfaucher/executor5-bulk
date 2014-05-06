/* demonstrate dbf.cls       ---          Please run me!                     */

/* ---------------------------------------------------------
   Here we create a brand new dbf file using a create string
*/ ---------------------------------------------------------
   dbfFilename = 'c:\temp\test.dbf'
   definition = "CODE,C,8,0;DESC,C,30,0;SELL,N,8,2;GROSS,L,1,0;TAXCODE,C,8,0;"
   .dbf~Create(dbfFilename,definition)
   dbf = .dbf~new(dbfFilename)

   disp = .dbfdisplay~new(.line, 2, 0, dbf, dbf)
/* ---------------------------------------------------
   Here we create a directory and append it to the dbf
*/ ---------------------------------------------------
   d = .directory~new
   d~CODE    = 'ABCD123'
   d~DESC    = 'First Row of data'
   d~SELL    = 1.23
   d~GROSS   = 'T'
   d~TAXCODE = 'UK1'
   dbf~append(d)

   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf)
/* ----------------------------------------------
   Here we create a stem and append it to the dbf
*/ ----------------------------------------------
   drop s.
   s.CODE    = 'ABCD124'
   s.DESC    = 'Second row of data'
   s.SELL    = 1.24
   s.GROSS   = .true
   s.TAXCODE = 'UK1'
   dbf~append(s.)

   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf)
/* -----------------------------------------------
   Here we create a Table and append it to the dbf
*/ -----------------------------------------------
   t = .table~new
   t['CODE']    = 'ABCD125'
   t~put('Third row of data','DESC')
   t['SELL']    = 1.25
   t['GROSS']   = 1
   t['TAXCODE'] = 'UK2'
   dbf~append(t)

   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf)
/* ---------------------------------------------------------
   Now we create an index on the dbf based on the field CODE
*/ ---------------------------------------------------------
   ind = dbf~createIndex('CODE')

-- NB: The index order is the same as the natural order at this point

   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf, ind)
call sysCls
say ' '
trace r

   a = ind~find('ABCD124') -- 'seeking ABCD124 in code index'
   say 'we found' a['CODE'] '-' a['DESC']

trace n
   call sysgetKey 'NoEcho'  -- wait for keypress

/* -----------------------------
   now we delete the current row
*/ -----------------------------
   dbf~delete(dbf~rowNumber)

-- the row doesn't disappear - it is just marked as deleted
-- we can enquire after the deleted status with dbf~curRowDeleted
-- The index doesn't know to remove the row
   disp = .dbfdisplay~new(.line, disp, 9, dbf, dbf, ind)

call sysCls
say ' '
trace r

   a = dbf~fetch(2)
   if dbf~curRowdeleted then say 'row' dbf~rowNumber 'Deleted'

   a = dbf~previous
   if dbf~curRowdeleted then say 'row' dbf~rowNumber 'Deleted'

trace n
   call sysgetKey 'NoEcho'  -- wait for keypress


/* ----------------------------------------------
   a standard index does not include deleted rows
*/ ----------------------------------------------
   ind = dbf~createIndex('CODE')

   disp = .dbfdisplay~new(.line, disp, 14, dbf, dbf, ind)

call sysCls
say ' '
trace r

   a = ind~find('ABCD124')
   if a = .nil then say 'deleted row not found'

trace n
   call sysgetKey 'NoEcho'  -- wait for keypress

/* --------------------------------------------------------
   We can create an index including deleted rows if we want
*/ --------------------------------------------------------
   ind = dbf~createIndex('CODE','DEL')

   disp = .dbfdisplay~new(.line, disp, 10, dbf, dbf, ind)

call sysCls
say ' '
say ' ------------------------------------------------'
say ' By default data is returned in a stem variable'
say ' If we want we can ask for it back in a directory'
say ' ------------------------------------------------'
trace r

   dbf~returnType = 'D'
   a = dbf~first
   say 'Requested a directory, got ['a~class'] containing' a~CODE
trace n
say ' ------------------------------------------------'
say ' or a table ...'
say ' ------------------------------------------------'
say
trace r
   dbf~returnType = 'T'
   a = dbf~next
   say 'Requested a table, got ['a~class'] containing' a['CODE']

trace n
   call sysgetKey 'NoEcho'  -- wait for keypress

/* ----------------------------------------------
   To permenantly remove deleted rows we use Pack
*/ ----------------------------------------------
   dbf~pack
   dbf~open  -- pack closes the database

   disp = .dbfdisplay~new(.line, disp, 24, dbf, dbf)
/* -----------------------------------------------------------------
   We can append a row just supplying the fields in definition order
*/ -----------------------------------------------------------------
   dbf~append(dbf~dataset('ABCD124','Second row of data',1.24,'T','UK1'))

-- As you can see they are now out of order.
   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf)

/* -----------------------------------------------
   We can update the data in rows within the table
*/ -----------------------------------------------
   a = dbf~fetch(2)
   a['SELL'] = a['SELL'] + 1
   dbf~update(2,a)
-- See ... the price of ABCD125 went up!

   disp = .dbfdisplay~new(.line, disp, 0, dbf, dbf)
call sysCls

say '--------------------------'
say 'Error checking is included'
say '--------------------------'
trace r
   a['SELL'] = 'DONT'
   dbf~update(2,a)
   if dbf~error then say 'Error:' dbf~errortext
trace n
   call sysgetKey 'NoEcho'  -- wait for keypress

/* -------------------------------------------------------------------------
   If we append rows using the indexes method, the new rows are in the index
*/ -------------------------------------------------------------------------
   ind = dbf~createIndex('CODE')
   ind~append(dbf~dataset('ABCD122','The prequel',1.26,'T','UK0'))

   disp = .dbfdisplay~new(.line, disp, 11, dbf, dbf, ind)
call sysCls
say ' '
say ' ------------------------------------------------'
say ' And of course we can use the index to find rows'
say ' An exact match (returns .nil if not found)'
say ' ------------------------------------------------'
trace r
   a = ind~Find('ABCD124')
   if a = .nil
   then say '<= ABCD124: Not found'
   else say '<= ABCD124:' a['CODE'] a['DESC']
trace n
say '----------------------------------'
say '... or the closest match above ...'
say '----------------------------------'
trace r
   a = ind~FindGE('ABCD126')
   if a = .nil
   then say '>= ABCD126: Not found'
   else say '>= ABCD126:' a['CODE'] a['DESC']
trace n
say '----------------------------------'
say '... or the closest match below    '
say '----------------------------------'
trace r
   a = ind~FindLE('ABCD126')
   if a = .nil
   then say '<= ABCD126: Not found'
   else say '<= ABCD126:' a['CODE']
trace n
   call sysgetKey 'NoEcho'  -- wait for keypress

/* ------------------------------------------------------------
   The index~refresh method allows us to apply an array of rows
*/ ------------------------------------------------------------
   a = .array~of(dbf~dataset('ABCD124','Updated in Place',7.24,'T','UK1'),
                ,dbf~dataset('ABCD127','New Row appended',6.27,'T','UK1'))
   ind~refresh(a)

   disp = .dbfdisplay~new(.line, disp, 31, dbf, ind)



/* ------------------------------------------------------------
   We can use a complex index (Taxcode descending then pennies)
*/ ------------------------------------------------------------

   ind = dbf~createIndex('TAXCODE,,,D;SELL,6,2')

   disp = .dbfdisplay~new(.line, disp, 1, dbf, ind)





   dbf~close
call sysCls
say ; say ; say
Say "        That's all there is to it!"

Return
/* ========================================================================= */
::requires 'dbf.cls'
/* ========================================================================= */

/* ========================================================================= */
::class dbfDisplay
/* ========================================================================= */
::attribute lineNo           -- line we were called from
::attribute display          -- array of lines shown on screen
/* ------------------------------------------------------------------------- */
/* show contents of dbf as they change                                       */
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose lineNo display
use arg lineNo, lastInstance, skip = 0, dbf, s1, s2 = .nil

   del = ' '
   divider = '-'~copies(79)
   if lastInstance~isA(.dbfDisplay)
   then do
      lastLine = lastInstance~lineNo
      lastDisplay = lastInstance~display
   end /* DO */
   else do
      lastLine = lastInstance
      lastDisplay = .array~of(' dbf.cls      Table =' dbf~name~text '    Order = None              Items =' dbf~items)
   end /* DO */
   drop lastInstance

   call sysCls
   scrLine = 1

   do line over lastDisplay
      say line ; scrLine += 1
   end /* DO */

   say divider ; scrLine += 1

   do scrLine = scrline to 8
      say ' '
   end /* DO */

   do lineNo = (lastLine + 1 + skip) to (lineNo - 1)
      say '|' sourceline(lineNo)~substr(3) ; scrLine += 1
   end /* DO */

   do scrLine = scrline to 22
      say ' '
   end /* DO */

   display = .array~new
   if s1~hasMethod('DEFINITION')
   then order = s1~Definition
   else order = 'Natural'
   display~append(' dbf.cls      Table =' dbf~name~text '    Order =' order~left(20) '   Items =' dbf~items)
   display~append(divider)

   rowstart = dbf~items~text~length + 1
   rowNoLen = rowStart - 1

   fieldpos = .table~new
   header = ' '~right(rowNoLen,'_')||'  '
   do field over dbf~fieldArr
      fieldPos~put(header~length+1,field~name~text)
      if field~type~value = 'N'
      then header = header||field~name~text~right(field~length~text,'_')||' '
      else header = header||field~name~text~left(field~length~text,'_')||' '
   end /* DO */

   display~append(header)
   a = s1~first
   do while a \= .nil
      line = .mutableBuffer~new(' '~copies(header~length))
      line~overlay(dbf~rowNumber~right(rowNoLen),1)
      do field over dbf~fieldArr
         if field~type~value = 'N'
         then line~overlay(right(a[field~name~text],field~length~text),fieldPos~at(field~name~text),field~length~text)
         else line~overlay(a[field~name~text],fieldPos~at(field~name~text),field~length~text)
      end /* DO */
      if dbf~curRowDeleted then line = line~overlay('*',rowNoLen + 2)
      display~append(line~string)
      a = s1~next
   end /* DO */

   if s2 \= .nil
   then do
      display~append(divider)
      if s2~HasMethod('DEFINITION')
      then order = s2~Definition
      else order = 'Natural'
      display~append(' dbf.cls      Table =' dbf~name~text '    Order =' order~left(20) '   Items =' dbf~items)
      display~append(divider)

      display~append(header)
      a = s2~first
      do while a \= .nil
         line = .mutableBuffer~new(' '~copies(header~length))
         line~overlay(dbf~rowNumber~right(rowNoLen),1)
         do field over dbf~fieldArr
            if field~type~value = 'N'
            then line~overlay(right(a[field~name~text],field~length~text),fieldPos~at(field~name~text),field~length~text)
            else line~overlay(a[field~name~text],fieldPos~at(field~name~text),field~length~text)
         end /* DO */
         if dbf~curRowDeleted then line = line~overlay('*',rowNoLen + 2)
         display~append(line~string)
         a = s2~next
      end /* DO */
   end /* DO */

   say divider~overlay(' After ',2)
   do line over display
      say line
   end /* DO */

   call sysgetKey 'NoEcho'  -- wait for keypress

/* ========================================================================= */
