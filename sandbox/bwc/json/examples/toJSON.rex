/* toJSON.rex */

/* make up an array that contains directories */
example = 'The quick brown fox jumped over the lazy dog.'

myarray = .array~new()

mydir = .directory~new()
mydir['name']           = 'Adam'
mydir['location']       = 'NC'
mydir['terminated']     = .true
mydir['number']         = 2
mydir['description']    = example || .endofline || example
myarray~append(mydir)

mydir = .directory~new()
mydir['name']           = 'Bob'
mydir['location']       = 'SC'
mydir['terminated']     = .false
mydir['number']         = 10
mydir['description']    = ''
myarray~append(mydir)

mydir = .directory~new()
mydir['name']           = 'Carl'
mydir['location']       = 'VA'
mydir['terminated']     = .false
mydir['number']         = 15
mydir['description']    = .nil
myarray~append(mydir)

/* run the parser */
parser = .json~new()
data = parser~toJSON(myarray)

/* just print out the result */
say data

/* or output to a file */
json_file = '.\toJSON.json'
mystream = .stream~new(json_file)
mystream~open('write replace')
mystream~charout(data)
mystream~close()

::requires 'json.cls'