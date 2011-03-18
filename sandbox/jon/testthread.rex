
t = .task~new
say t~method1
say t~method2
say t~method1
say t~method2

.task~server~stopwork




/* ========================================================================= */
::class threadServer public -- receives messages and despatches them on
                            -- the thread it was created on
/* ========================================================================= */
::attribute stop        -- if set to .true, then "doTheWork" will stop working
::attribute requests    -- queue of messages to despatch on this thread
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
  expose requests stop
  requests = .queue~new
  stop     = .false

/* ------------------------------------------------------------------------- */
::method startWork unguarded          -- worker method, runs on its own thread
/* ------------------------------------------------------------------------- */
  expose requests stop
  REPLY                           -- return to caller, start a new thread --->|

  self~setup                      -- e.g. create a database connection
  do forever
  -- wait for requests or stop 
     guard on when requests~items > 0 | stop = .true

        if stop = .true then leave

        do while requests~items>0            -- process all pending requests
            message = requests~pull          -- pull workItem object
            message~send                     -- do the work on this thread
        end

     guard off
  end
  self~shutdown                        -- e.g. release a database connection

/* ------------------------------------------------------------------------- */
::method stopWork  -- cause the worker method to stop
/* ------------------------------------------------------------------------- */
  expose stop
  stop = .true

/* ------------------------------------------------------------------------- */
::method setup       -- setup whatever the worker needs
/* ------------------------------------------------------------------------- */
/*
  if rxfuncquery('MYLoadFuncs') then call rxfuncadd 'MYLoadFuncs', 'REXXMY', 'MYLoadFuncs'
  if rxfuncquery('MYconnect')   then call MYloadfuncs
*/
/* ------------------------------------------------------------------------- */
::method shutdown    -- shutdown whatever the worker has initiated
/* ------------------------------------------------------------------------- */
/*
call myDropFuncs
*/
/* ========================================================================= */



/* ========================================================================= */
::class BaseTask 
/* basetask represents the origional class that would only work if it was    */
/* invoked on the thread it was 'required' on.  As there are thousands of    */
/* invokations of this class in my code, I have renamed it and given its     */
/* name to the subclass task                                                 */
/* ========================================================================= */
::method method1 -- a method that must run on the thread startup procedures were run on
/* ------------------------------------------------------------------------- */
-- code to process method1
return 'method1 result'
/* ------------------------------------------------------------------------- */
::method method2 -- a method that is thread agnostic
/* ------------------------------------------------------------------------- */
-- code to process method2
return 'method2 result'


/* ========================================================================= */
::class task subclass baseTask
/* the purpose of this class is to make sure that for any methods overriden  */
/* below, the coresponding method in the superclass is invoked on the thread */
/* that the class was origionally 'required' on.                             */
/* ========================================================================= */
::attribute server             CLASS
/* ------------------------------------------------------------------------- */
::method init                  CLASS
/* ------------------------------------------------------------------------- */
self~server = .threadServer~new
self~server~startWork
/* ------------------------------------------------------------------------- */
::method uninit                CLASS
/* ------------------------------------------------------------------------- */
self~class~server~stopWork

/* ------------------------------------------------------------------------- */
::method method1                      unguarded
/* ------------------------------------------------------------------------- */
if arg(1)~isA(.forwardToSuperMarker)
then forward class (super) arguments (arg(1,'Array')~section(2))
else do
   methodName = 'METHOD1'  -- << Replace with context object method when available
   argArray   = .array~of(.forwardToSuperMarker~new)~union(arg(1,'Array'))
   msg        = .message~new(self,methodname,'Array',argArray)
   self~class~server~requests~queue(msg)                     -- post to server
   return msg~result
end
/* ------------------------------------------------------------------------- */




/* ========================================================================= */
::class forwardtoSuperMarker
/* this class has no methods.  Its only purpose is to identify messages that */
/* have been sent to the server and are coming back from it                  */
/* ========================================================================= */
