Class file is in oorexx-3.1\json.cls

This class is pretty straight forward. It uses ooRexx arrays and directories.
Originally created it to work with ooRexx 3.1.

Because I call the makearray method if you do not pass an array or
directory, you could use it for other objects as well (lists, queue, etc).

The methods you should really care about are the "fromJSON" and "toJSON" methods.

If you are using this to write JSON files that will be processed by ooRexx
everthing should work fine. If you are trying to use this with another language,
you might want to know about a couple of interoperability issues.

- No Unicode support
    When ooRexx gets Unicode support, this can be addressed.

- .True and .False are not really types in ooRexx
    Because of how booleans are treated in ooRexx, this can cause a weird issue
    where a boolean gets turned into an integer.
