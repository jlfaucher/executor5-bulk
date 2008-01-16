-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

::CLASS ClientEvent PUBLIC SUBCLASS SuperEvent
::METHOD Unknown UNGUARDED
    use arg methodname, methodargs
    methodname = methodname~translate
    call debug "[ClientEvent: Unknown method called:" methodname, 1
    if .LOCAL[methodname] \= .nil then do i = 1 to .local[methodname]~items
	name = .local[methodname][i]~translate
	trace
	call debug "[ClientEvent: Unknown: Searching" name,8
	if self~EventhasMethod(name) > 0 then do
	    call debug "[ClientEvent: Unknown: Found" name,8
	    do j = 1 to self~EventhasMethod(name)
    		-- this is a hack, should be replaced by something better
    		-- in order to allow methods to leave out a return value
    		-- the error of assigning an non existant value to a variable is
		-- is catched. The signal will only return 0, wich is needed because
		-- of recursion
		signal on syntax name generate_result
		method = self~EventGetMethod(name,j)
		if method~class = .method then
        	    ret = self~run(method,"A",methodargs)
		else
		    ret = .message~new(method,name,"A",methodargs)~send
		signal off syntax
		-- in case the method returned the number 1 the loop continues with
		-- executing the super method
		if ret \= 1 then leave j
	    end
	end
    end
--    call debug "[ClientEvent: Unknown: No method found",8

    generate_result:
    return 0
-- The message handler class is very similar to the ClientEvent
-- class. The only difference is that the MessageHandler returns as soon
-- as a method was executed. It does not continue searching for methods
-- if something special has been returned.
::CLASS MessageHandler PUBLIC SUBCLASS SuperEvent
::METHOD Init
    -- the init method needs to be called, otherwise the call to EventSetMode
    -- will not work.
    forward class (super) continue
    self~EventSetMode("Flat")
    call Debug "[MessageHandler: Init: Created new MessageHandler instance",1
    
::METHOD Unknown UNGUARDED
    use arg methodname, methodargs
    methodname = methodname~translate
    call debug "[MessageHandler: Unknown method called:" methodname, 1
    if .LOCAL[methodname] \= .nil then do i = 1 to .local[methodname]~items
	call debug "[MessageHandler: Unknown: Searching" .local[methodname][i],8
	if self~EventhasMethod(.local[methodname][i]~translate) then do
	    call debug "[MessageHandler: Unknown: Found" .local[methodname][i],8
	    method = self~EventGetMethod(.local[methodname][i]~translate)
	    if method~class = .method then
        	message = self~run(method,"A",methodargs)
	    else
		ret = .message~new(method,.local[methodname][i],"A",methodargs)~send
	    return message
	end
    end
    -- leave out the mapping from name to array of string via the .local directory
    else do
	if self~EventhasMethod(methodname) then do
	    call debug "[MessageHandler: Unknown: Found" methodname,8
	    method = self~EventGetMethod(methodname)
	    if method~class = .method then
        	message = self~run(method,"A",methodargs)
	    else
		if method~class = .message then
		    message = method~send
	    return message
	end
    end
    call debug "[MessageHandler: Unknown: No method found",8
    if methodargs~items > 0 then
        return methodargs[1]
    else return 0



::CLASS SuperEvent
::METHOD Init
    expose methods
    -- the directory to hold the methods and their names
    -- the directory will later hold stacks, either with more than one element
    -- or just one
    methods = .directory~new()
    -- mode can either be stack or flat
    -- in stack mode it will remember old methods defined and will invoke them
    -- if the current method for a name has been removed.
    -- in flat mode only the current method is stored, after removing it
    -- ~hasmethod() will return .false
    self~EventSetMode("Stack")
    call debug "[SuperClientEvent: Init: Instance initialized",1
    
::METHOD EventSetMode
    expose mode methods
    parse upper arg NewMode
    -- save only the first non whithspace character
    NewMode = NewMode~strip~left(1)
    call debug "[SuperClientEvent: EventSetMode: Mode set to" NewMode,1
    -- Don't do anything if the mode remains unchanged
    if Mode = NewMode then return
    -- Look for mode S and F, raise an error in case it's something else
    SELECT
	-- Mode was changed to stack mode
	When NewMode = "S" then do
	    Mode = "S"
	end
	-- Mode was changed to flat
	When NewMode = "F" then
	    do method over methods
		-- get the method stack for the current method name
		methodstack = methods[method]
		-- recreate it only transferring the currently active method
		methods[method] = .queue~new~~push(methodstack~pull)
		-- explicitly drop the old stack
		drop methodstack
	    end
	Otherwise
	    nop
	    -- RAISE NEEDS TO BE INCORRPORTAED HERE ---
    End

::METHOD EventGetMode
    expose mode;return mode

::METHOD EventSetMethod
    expose Methods
    use arg methodname, source
    call debug "[SuperClientEvent: EventSetMethod: defining method" methodname,1
    methodname = methodname~translate
    call debug "[SuperClientEvent: EventsetMethod: source is of type" source~class,8
    if source = .nil then return
/*
This part may be added in future, it has to be discussed whether it makes sense
or not...
    -- The following names can not be set, they are reserved
    reserved_names = "= == \= \== <> ><  ",
	" CLASS COPY DEFAULTNAME EREQUEST HASMETHOD ",
	" INIT OBJECTNAME OBJECTNAME=  RUN ",
	" SETMETHOD START STARTAT STRING  UNSETMETHOD"
    if reserved_names~wordpos(methodname) > 0 then do
	call debug "[SuperClientEvent: EventSetMethod: Trying to define reserved method; aborting",16
	return
    end
*/
    SELECT
	when source~class = .string then do
    	    method = .method~new(methodname, source)
	end
	OTHERWISE method = source
    END
    if self~EventGetMode = "S" then do
        if Methods~HasIndex(methodname) then
	    Methods[methodname]~push(method)
	else
	    Methods[methodname] = .queue~new~~push(method)
    end
    else Methods[methodname] = .queue~new~~push(method)

::METHOD EventSetMethods
    use arg methods
    Select
        when methods~class = .directory then
	    do name over methods
    		self~EventSetMethod(name,methods[name])
	    end
	when methods~class = .supplier then do
	    do while methods~Available
		self~EventSetMethod(methods~Index,methods~Item)
		methods~Next
	    end
	end
	Otherwise raise syntax 93.914 array (1,"directory or supplier",methods~class)
    end


::METHOD EventGetMethod
    expose methods
    if arg() = 1 then do
        if methods~HasIndex(arg(1)) then return methods[arg(1)]~peek
	else return .nil
    end
    use arg name, index
    return methods[name][index]

::METHOD EventRemoveMethod
    expose methods
    parse upper arg methodname
    call debug "[SuperClientEvent: EventRemoveMethod: trying to remove" methodname,3
    if methods~HasIndex(methodname) then do
	ret =  methods[methodname]~pull
	if methods[methodname]~items = 0 then methods~remove(methodname)
	call debug "[SuperClientEvent: EventRemoveMethod: remove succesful!",4
    end
    else ret = .nil
    return ret
    
::METHOD EventHasMethod
    expose methods
    parse arg methodname
    if methods~hasIndex(methodname) then return methods[methodname]~items
    else return 0