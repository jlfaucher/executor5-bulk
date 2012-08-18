/*---------------------------------------------------------------------
  GenericFile v00-01 - A generic file superclass
              v00-02 - 10Jly11 - added commas between arg args.

  interface {
    records init( in string filespec, in int columns )          records = records in file
                                                                records = -1 if no file; 0 if file empty.
    rc      find( in string recordId, inout table recordTable ) rc = 0 or 1.
    records list()

  Constraints: RecordId must be the first column.
  }
  -------------------------------------------------------------------*/

::CLASS GenericFile PUBLIC


  ::ATTRIBUTE fileArray GET PUBLIC			/* Note - does auto expose - */
  ::ATTRIBUTE fileArray SET PRIVATE


  -- ------------------------------------------------------------------


  ::METHOD init PUBLIC
    -- Given a filename and the number of columns in the file, returns a
    -- Row x Column array containing the file, or:
    -- 0 (.false) if file doesn't exist, or -1 if no records in file.
    expose columns records fileArray filename
    use arg filename, columns
    records = 0
    say "GenericFile-init-01: fileName =" fileName "columns =" columns
    file  = .stream~new(filename)
    say "GenericFile-init-01a: file~query(exists):" file~query("exists")
    if file~query("exists") = "" then do
      say "GenericFile-init-02: file" filename " does not exist."
      return .false
    end
    records = file~lines
    if records <2 then return -1
    fileArray = file~makearray -- gets each line as an array item
    --say "GenericFile-Init-02:" filearray~string
    file~close
    return records

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  -- ------------------------------------------------------------------
  ::METHOD list PUBLIC
    expose records fileArray filename
    if records = 0 then do
       return .false
    end
    say; say "records in File '"||filename||"'"
    do i=1 to records
      say fileArray[i]
    end
    return records
  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  -- ------------------------------------------------------------------
  ::METHOD find PUBLIC
    /*--------------------------------------------------------------------------
      Returns a directory containing the data for the record with the requested
      key (dataId).
      ------------------------------------------------------------------------*/
    expose records columns fileArray
    use arg dataId
    if dataId = .nil | dataId = "" then return .false
    say "GenericFile-find-01: dataId=" dataId
    if records = 0 then return .false    	-- File empty or does not exist

    found = .false
    do i = 2 to records     			-- skip header line
      record = fileArray[i]
      parse var record recordId .		-- assume recordId is first column
      say "GenericFile-find-02: recordId = '"||recordId"'"
      if recordId = dataId then do
        found = .true;  leave;
      end
    end
    if \found then return .false			-- Records exist, but specified record not found.

    dirData = .Directory~new
    -- Get Headers:
    headers = fileArray[1]
    data = fileArray[i]
    do j=1 to columns
      parse var headers index "|" headers
      index = index~strip
      parse var data item "|" data
      item = item~strip
      dirData[index] = item
    end
    say; say "GenericFile-find-03: Results:"
    do i over dirData
      say "'"||i||"' = '"||dirData[i]||"'"
    end
    return dirData
  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    queryAll - return an nx2 array, each field in a different cell.
    - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ::METHOD queryAll PUBLIC
    expose fileArray records columns
    say "GenericFile-queryAll-01."
    dataArr = .Array~new
    do i=2 to records
      record = fileArray[i]
      do j = 1 to columns
        parse var record field "|" record
        field = field~strip
        dataArr[i,j] = field
      end
    end
    return dataArr

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    update - given a record as a Directory, replaces the existing record with the
             new data and writes the whole file to disk.
      Returns .false (a) if a record with the provided key does not exist, or
                     (b) if values for all fields are not provided.
    - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ::METHOD update
    expose records columns fileArray fileName
    use arg recordKey, dirRecord		-- record key + new record

    -- Find the record:
    do i=2 to records
      record = fileArray[i]
      parse var record recordId .		-- assume recordId is first column
      if recordId = recordKey then do
        found = .true;  leave;
      end
    end
    if \found then return .false		-- Records exist, but specified record not found.

    -- found record is i'th. Now replace that array item with the new data:
    currentRecord = fileArray[i]
---- This method is only opartially completed.  !!!!!!!!!!!!
    newrecord = ""
    do j=1 to columns
      --fileArray[i]field =
    end



/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

