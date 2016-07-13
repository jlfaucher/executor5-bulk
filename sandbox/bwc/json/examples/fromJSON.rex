/* fromJSON.rex */

/* read file in */
json_file = '.\fromJSON.json'
mystream = .stream~new(json_file)
data = mystream~charin(, mystream~chars())
mystream~close()

/* run the parser */
parser = .json~new()
myobj = parser~fromJSON(data)

/* show that we have the object loaded */
say 'Item 1 Name:'          myobj[1]['name']
say 'Item 2 Location:'      myobj[2]['location']
say 'Item 3 Terminated:'    myobj[3]['terminated']

say

/* show everything */
fields = .array~of('Name', 'Location', 'Terminated', 'Number', 'Description')
do i = 1 to myobj~items()
    do field over fields
        say 'Item 'i' 'field':' myobj[i][field~lower]
    end
end

::requires 'json.cls'
