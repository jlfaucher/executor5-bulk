/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2024-2026 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

json   = .json~new
tests  = 0
pass   = 0
fail   = 0

say copies("=", 64)
say "json.cls test suite"
say copies("=", 64)
say

/*========================================================================*/
say "--- 1. Decoding: basic types ---"

call check json~fromJSON('"hello"'), "hello", "decode string"
call check json~fromJSON('""'), "", "decode empty string"
call check json~fromJSON('42'), 42, "decode integer"
call check json~fromJSON('-17'), -17, "decode negative integer"
call check json~fromJSON('3.14'), 3.14, "decode float"
call check json~fromJSON('-2.718'), -2.718, "decode negative float"
call check json~fromJSON('6.022e23'), "6.022e23", "decode scientific"
call check json~fromJSON('1E10'), "1E10", "decode scientific upper E"
call check json~fromJSON('1e+5'), "1e+5", "decode scientific positive exp"
call check json~fromJSON('-1.602e-19'), "-1.602e-19", "decode scientific negative"
call check json~fromJSON('0'), 0, "decode zero"
call check json~fromJSON('0.0'), 0.0, "decode zero point"
call check json~fromJSON('true'), .JsonBoolean~true, "decode true"
call check json~fromJSON('false'), .JsonBoolean~false, "decode false"
call checkNil json~fromJSON('null'), "decode null"
say

/*========================================================================*/
say "--- 2. Decoding: strings with escape sequences ---"

call check json~fromJSON('"hello world"'), "hello world", "decode plain string"
call check json~fromJSON('"tab\there"'), "tab" || '09'x || "here", "decode \\t"
call check json~fromJSON('"line\nbreak"'), "line" || '0a'x || "break", "decode \\n"
call check json~fromJSON('"cr\rhere"'), "cr" || '0d'x || "here", "decode \\r"
call check json~fromJSON('"back\bspace"'), "back" || '08'x || "space", "decode \\b"
call check json~fromJSON('"form\ffeed"'), "form" || '0c'x || "feed", "decode \\f"
call check json~fromJSON('"a\"b"'), 'a"b', 'decode escaped quote'
call check json~fromJSON('"a\\b"'), 'a\b', 'decode escaped backslash'
call check json~fromJSON('"a\/b"'), 'a/b', 'decode escaped slash'
say

/*========================================================================*/
say "--- 3. Decoding: \\uXXXX escape sequences ---"

-- \u00XX sequences are decoded to single-byte characters
call check json~fromJSON('"\u0041"'), "A", "decode \\u0041 -> A"
call check json~fromJSON('"\u005A"'), "Z", "decode \\u005A -> Z"
call check json~fromJSON('"\u0061"'), "a", "decode \\u0061 -> a"
call check json~fromJSON('"\u007A"'), "z", "decode \\u007A -> z"
call check json~fromJSON('"\u0020"'), " ", "decode \\u0020 -> space"
call check json~fromJSON('"\u0009"'), '09'x, "decode \\u0009 -> tab"
call check json~fromJSON('"\u000A"'), '0a'x, "decode \\u000A -> LF"

-- non-\u00XX sequences are kept as-is (no Unicode support in ooRexx)
call check json~fromJSON('"\u4E16"'), "\u4E16", "decode \\u4E16 kept as-is"
call check json~fromJSON('"\u754C"'), "\u754C", "decode \\u754C kept as-is"
call check json~fromJSON('"Hello \u4E16\u754C!"'), "Hello \u4E16\u754C!", "decode mixed unicode"
say

/*========================================================================*/
say "--- 4. Decoding: arrays ---"

arr = json~fromJSON('[]')
call check arr~items, 0, "decode empty array > items"
call checkIsA arr, .array, "decode empty array > isA"

arr = json~fromJSON('[1]')
call check arr~items, 1, "decode single-item array > items"
call check arr[1], 1, "decode single-item array > value"

arr = json~fromJSON('[1, 2, 3]')
call check arr~items, 3, "decode int array > items"
call check arr[1], 1, "decode int array > [1]"
call check arr[2], 2, "decode int array > [2]"
call check arr[3], 3, "decode int array > [3]"

arr = json~fromJSON('["alpha", "beta", "gamma"]')
call check arr~items, 3, "decode string array > items"
call check arr[1], "alpha", "decode string array > [1]"

arr = json~fromJSON('[1, "two", true, false, null, 3.14]')
call check arr~items, 6, "decode mixed array > items"
call check arr[1], 1, "decode mixed array > number"
call check arr[2], "two", "decode mixed array > string"
call check arr[3], .JsonBoolean~true, "decode mixed array > true"
call check arr[4], .JsonBoolean~false, "decode mixed array > false"
call checkNil arr[5], "decode mixed array > null"
call check arr[6], 3.14, "decode mixed array > float"

arr = json~fromJSON('[[1, 2], [3, 4]]')
call check arr~items, 2, "decode nested array > items"
call check arr[1][1], 1, "decode nested array > [1][1]"
call check arr[2][2], 4, "decode nested array > [2][2]"

arr = json~fromJSON('[[[["deep"]]]]')
call check arr[1][1][1][1], "deep", "decode deeply nested array"
say

/*========================================================================*/
say "--- 5. Decoding: objects ---"

obj = json~fromJSON('{}')
call check obj~items, 0, "decode empty object > items"
call checkIsA obj, .directory, "decode empty object > isA"

obj = json~fromJSON('{"name": "Alice", "age": 30}')
call check obj["name"], "Alice", "decode object > name"
call check obj["age"], 30, "decode object > age"

obj = json~fromJSON('{"level1": {"level2": {"level3": "deep"}}}')
call check obj["level1"]["level2"]["level3"], "deep", "decode nested object"

obj = json~fromJSON('{"arr": [1, 2], "obj": {"x": true}}')
call check obj["arr"][1], 1, "decode mixed > array value"
call check obj["obj"]["x"], .JsonBoolean~true, "decode mixed > nested object value"
say

/*========================================================================*/
say "--- 6. Decoding: duplicate keys (last wins) ---"

obj = json~fromJSON('{"key": "first", "key": "second"}')
call check obj["key"], "second", "decode duplicate keys > last wins"
call check obj~items, 1, "decode duplicate keys > single entry"
say

/*========================================================================*/
say "--- 7. Decoding: whitespace handling ---"

-- extra whitespace around structural characters
obj = json~fromJSON('  {  "a"  :  1  ,  "b"  :  2  }  ')
call check obj["a"], 1, "decode whitespace > a"
call check obj["b"], 2, "decode whitespace > b"

-- tabs and newlines as whitespace
obj = json~fromJSON('09'x || '0a'x || '{"x":1}' || '0d'x || '0a'x)
call check obj["x"], 1, "decode tabs/newlines as whitespace"

-- whitespace inside arrays
arr = json~fromJSON('[  1  ,  2  ,  3  ]')
call check arr~items, 3, "decode whitespace in array"
say

/*========================================================================*/
say "--- 8. Decoding: top-level values (RFC 8259) ---"

call check json~fromJSON('"just a string"'), "just a string", "decode top-level string"
call check json~fromJSON('42'), 42, "decode top-level number"
call check json~fromJSON('true'), .JsonBoolean~true, "decode top-level true"
call check json~fromJSON('false'), .JsonBoolean~false, "decode top-level false"
call checkNil json~fromJSON('null'), "decode top-level null"
say

/*========================================================================*/
say "--- 9. Decoding: decoded types ---"

-- strings are .JsonString instances
val = json~fromJSON('"hello"')
call checkIsA val, .JsonString, "decoded string > isA JsonString"
call checkIsA val, .String, "decoded string > isA String (parent)"

-- booleans are .JsonBoolean instances
val = json~fromJSON('true')
call checkIsA val, .JsonBoolean, "decoded true > isA JsonBoolean"

val = json~fromJSON('false')
call checkIsA val, .JsonBoolean, "decoded false > isA JsonBoolean"

-- numbers are plain .String
val = json~fromJSON('42')
call checkIsA val, .String, "decoded number > isA String"

-- objects are .Directory
val = json~fromJSON('{"a": 1}')
call checkIsA val, .Directory, "decoded object > isA Directory"

-- arrays are .Array
val = json~fromJSON('[1]')
call checkIsA val, .Array, "decoded array > isA Array"
say

/*========================================================================*/
say "--- 10. Encoding: basic types ---"

call check json~toJSON("hello"), '"hello"', "encode string"
call check json~toJSON(""), '""', "encode empty string"
call check json~toJSON(42), '42', "encode integer"
call check json~toJSON(-17), '-17', "encode negative integer"
call check json~toJSON(3.14), '3.14', "encode float"
call check json~toJSON(0), '0', "encode zero"
call check json~toJSON(.nil), 'null', "encode nil"
call check json~toJSON(.JsonBoolean~true), 'true', "encode JsonBoolean true"
call check json~toJSON(.JsonBoolean~false), 'false', "encode JsonBoolean false"
say

/*========================================================================*/
say "--- 11. Encoding: strings with special characters ---"

call check json~toJSON('tab' || '09'x || 'here'), '"tab\there"', "encode tab"
call check json~toJSON('line' || '0a'x || 'break'), '"line\nbreak"', "encode newline"
call check json~toJSON('cr' || '0d'x || 'here'), '"cr\rhere"', "encode carriage return"
call check json~toJSON('back' || '08'x || 'space'), '"back\bspace"', "encode backspace"
call check json~toJSON('form' || '0c'x || 'feed'), '"form\ffeed"', "encode formfeed"
call check json~toJSON('a"b'), '"a\"b"', "encode double quote"
call check json~toJSON('a\b'), '"a\\b"', "encode backslash"
call check json~toJSON('a/b'), '"a\/b"', "encode slash"

-- control characters (< 0x20) encoded as \u00XX
call check json~toJSON('01'x), '"\u0001"', "encode control char 01"
call check json~toJSON('1f'x), '"\u001F"', "encode control char 1F"
say

/*========================================================================*/
say "--- 12. Encoding: \uXXXX pass-through ---"

-- strings containing literal \uXXXX are passed through
call check json~toJSON('\u4E16'), '"\u4E16"', "encode \\uXXXX pass-through"
call check json~toJSON('Hello \u4E16\u754C!'), '"Hello \u4E16\u754C!"', "encode mixed \\uXXXX pass-through"
say

/*========================================================================*/
say "--- 13. Encoding: JsonString (force quoting) ---"

js = .JsonString~new("42")
call check json~toJSON(js), '"42"', "encode JsonString numeric as quoted"

js = .JsonString~new("hello")
call check json~toJSON(js), '"hello"', "encode JsonString non-numeric"

js = .JsonString~new("")
call check json~toJSON(js), '""', "encode JsonString empty"

js = .JsonString~new("3.14")
call check json~toJSON(js), '"3.14"', "encode JsonString float as quoted"
say

/*========================================================================*/
say "--- 14. Encoding: arrays ---"

call check json~toJSON(.array~new), '[]', "encode empty array"
call check json~toJSON(.array~of(1)), '[1]', "encode single-item array"
call check json~toJSON(.array~of(1, 2, 3)), '[1,2,3]', "encode int array"
call check json~toJSON(.array~of("a", "b")), '["a","b"]', "encode string array"

-- mixed types in array
arr = .array~new
arr~append(1)
arr~append("two")
arr~append(.JsonBoolean~true)
arr~append(.nil)
call check json~toJSON(arr), '[1,"two",true,null]', "encode mixed array"

-- nested arrays
call check json~toJSON(.array~of(.array~of(1, 2), .array~of(3, 4))), '[[1,2],[3,4]]', "encode nested arrays"
say

/*========================================================================*/
say "--- 15. Encoding: objects ---"

call check json~toJSON(.directory~new), '{}', "encode empty object"

dir = .directory~new
dir["name"] = "Alice"
call check json~toJSON(dir), '{"name":"Alice"}', "encode single-key object"

-- keys are sorted alphabetically
dir = .directory~new
dir["b"] = 2
dir["a"] = 1
dir["c"] = 3
call check json~toJSON(dir), '{"a":1,"b":2,"c":3}', "encode object sorted keys"

-- numeric key values are quoted when used as keys
dir = .directory~new
dir["42"] = "value"
call check json~toJSON(dir), '{"42":"value"}', "encode numeric key quoted"
say

/*========================================================================*/
say "--- 16. Encoding: legible output ---"

crlf = .rexxInfo~endOfLine

-- legible object
dir = .directory~new
dir["a"] = 1
dir["b"] = 2
expected = '{' || crlf || '   "a": 1,' || crlf || '   "b": 2' || crlf || '}'
call check json~toJSON(dir, .true), expected, "encode legible object"

-- legible array
arr = .array~of(1, 2, 3)
expected = '[' || crlf || '   1,' || crlf || '   2,' || crlf || '   3' || crlf || ']'
call check json~toJSON(arr, .true), expected, "encode legible array"

-- legible empty collections
call check json~toJSON(.directory~new, .true), '{}', "encode legible empty object"
call check json~toJSON(.array~new, .true), '[]', "encode legible empty array"
say

/*========================================================================*/
say "--- 17. Encoding: nested legible output ---"

dir = .directory~new
inner = .directory~new
inner["x"] = 1
dir["obj"] = inner
expected = '{' || crlf || -
           '   "obj": {' || crlf || -
           '      "x": 1' || crlf || -
           '   }' || crlf || -
           '}'
call check json~toJSON(dir, .true), expected, "encode legible nested object"
say

/*========================================================================*/
say "--- 18. Encoding: special objects (makeJSON, makeArray, makeString) ---"

-- JsonBoolean via makeJSON
call check json~toJSON(.JsonBoolean~true), 'true', "encode via makeJSON true"
call check json~toJSON(.JsonBoolean~false), 'false', "encode via makeJSON false"
say

/*========================================================================*/
say "--- 19. Class methods ---"

-- toJSON class method
call check .json~toJSON(42), '42', "class toJSON number"
call check .json~toJSON("hello"), '"hello"', "class toJSON string"

-- fromJSON class method
call check .json~fromJSON('42'), 42, "class fromJSON number"
call check .json~fromJSON('"hello"'), "hello", "class fromJSON string"
say

/*========================================================================*/
say "--- 20. File I/O ---"

-- write and read back
dir = .directory~new
dir["test"] = "file io"
dir["number"] = 42

.json~toJsonFile("/tmp/test_json_io.json", dir)
readBack = .json~fromJsonFile("/tmp/test_json_io.json")
call check readBack["test"], "file io", "file I/O > string value"
call check readBack["number"], 42, "file I/O > number value"

-- write legible
.json~toJsonFile("/tmp/test_json_io_legible.json", dir, .true)
readBack2 = .json~fromJsonFile("/tmp/test_json_io_legible.json")
call check readBack2["test"], "file io", "file I/O legible > string value"
call check readBack2["number"], 42, "file I/O legible > number value"
say

/*========================================================================*/
say "--- 21. JsonBoolean behavior ---"

jt = .JsonBoolean~true
jf = .JsonBoolean~false

-- makeString returns logical values
call check jt~makeString, 1, "JsonBoolean true makeString"
call check jf~makeString, 0, "JsonBoolean false makeString"

-- makeJSON returns JSON literals
call check jt~makeJSON, "true", "JsonBoolean true makeJSON"
call check jf~makeJSON, "false", "JsonBoolean false makeJSON"

-- equality
call check jt = .true, .true, "JsonBoolean true = .true"
call check jf = .false, .true, "JsonBoolean false = .false"
call check jt = jt, .true, "JsonBoolean true = true"
call check jf = jf, .true, "JsonBoolean false = false"
call check (jt = jf), .false, "JsonBoolean true \= false"

-- compareTo
call check jt~compareTo(.true), 0, "JsonBoolean true compareTo .true"
call check jf~compareTo(.false), 0, "JsonBoolean false compareTo .false"
call check jt~compareTo(jf), 1, "JsonBoolean true compareTo false"
call check jf~compareTo(jt), -1, "JsonBoolean false compareTo true"

-- inequality operators
call check (jt \= jf), .true, "JsonBoolean \\= operator"
call check (jt <> jf), .true, "JsonBoolean <> operator"
call check (jt >< jf), .true, "JsonBoolean >< operator"

-- singletons: both references are the same object
call check (.JsonBoolean~true = .JSON~true), .true, "JsonBoolean singleton true"
call check (.JsonBoolean~false = .JSON~false), .true, "JsonBoolean singleton false"
say

/*========================================================================*/
say "--- 22. JsonString behavior ---"

js = .JsonString~new("42")
call checkIsA js, .JsonString, "JsonString isA JsonString"
call checkIsA js, .String, "JsonString isA String"
call check js~makeJSON, '"42"', "JsonString makeJSON"
call check js, 42, "JsonString value equals number"

js2 = .JsonString~new("hello")
call check js2~makeJSON, '"hello"', "JsonString non-numeric makeJSON"

js3 = .JsonString~new("")
call check js3~makeJSON, '""', "JsonString empty makeJSON"
say

/*========================================================================*/
say "--- 23. Round-trip: encode then decode ---"

-- string round-trip
original = "hello world"
call check json~fromJSON(json~toJSON(original)), original, "round-trip string"

-- number round-trip
original = 42
call check json~fromJSON(json~toJSON(original)), original, "round-trip number"

-- boolean round-trip
call check json~fromJSON(json~toJSON(.JsonBoolean~true)), .JsonBoolean~true, "round-trip true"
call check json~fromJSON(json~toJSON(.JsonBoolean~false)), .JsonBoolean~false, "round-trip false"

-- null round-trip
call checkNil json~fromJSON(json~toJSON(.nil)), "round-trip null"

-- object round-trip
dir = .directory~new
dir["name"] = "Alice"
dir["age"] = 30
dir["active"] = .JsonBoolean~true
encoded = json~toJSON(dir)
decoded = json~fromJSON(encoded)
call check decoded["name"], "Alice", "round-trip object > name"
call check decoded["age"], 30, "round-trip object > age"
call check decoded["active"], .JsonBoolean~true, "round-trip object > active"

-- array round-trip
arr = .array~of(1, "two", .JsonBoolean~true, .nil)
encoded = json~toJSON(arr)
decoded = json~fromJSON(encoded)
call check decoded[1], 1, "round-trip array > [1]"
call check decoded[2], "two", "round-trip array > [2]"
call check decoded[3], .JsonBoolean~true, "round-trip array > [3]"
call checkNil decoded[4], "round-trip array > [4] null"

-- nested round-trip
dir = .directory~new
dir["list"] = .array~of(1, 2, 3)
inner = .directory~new
inner["deep"] = .JsonBoolean~false
dir["nested"] = inner
encoded = json~toJSON(dir)
decoded = json~fromJSON(encoded)
call check decoded["list"][2], 2, "round-trip nested > list[2]"
call check decoded["nested"]["deep"], .JsonBoolean~false, "round-trip nested > deep"

-- escape characters round-trip
original = 'tab:' || '09'x || ' newline:' || '0a'x || ' quote:" backslash:\'
encoded = json~toJSON(original)
decoded = json~fromJSON(encoded)
call check decoded, original, "round-trip escape chars"
say

/*========================================================================*/
say "--- 24. Round-trip: decode then encode ---"

jsonText = '{"a":1,"b":"hello","c":true,"d":false,"e":null,"f":[1,2,3]}'
decoded = json~fromJSON(jsonText)
reencoded = json~toJSON(decoded)
call check reencoded, jsonText, "decode-encode round-trip"

-- with nested structures (keys sorted alphabetically in output)
jsonText = '{"arr":[1,[2,3]],"obj":{"nested":true}}'
decoded = json~fromJSON(jsonText)
reencoded = json~toJSON(decoded)
call check reencoded, jsonText, "decode-encode nested round-trip"
say

/*========================================================================*/
say "--- 25. Round-trip: test_all_constructs.json ---"

say "  Loading test_all_constructs.json..."
obj1 = .json~fromJsonFile("test_all_constructs.json")

say "  Encoding to JSON..."
encoded = json~toJSON(obj1)

say "  Decoding the dynamically created JSON..."
obj2 = json~fromJSON(encoded)

say "  Comparing both decodings for semantic equivalence..."
call checkDeepEqual obj1, obj2, "test_all_constructs round-trip"
say

/*========================================================================*/
say "--- 26. Edge cases: empty and minimal inputs ---"

-- empty object and array
call check json~toJSON(json~fromJSON('{}')), '{}', "edge: empty object round-trip"
call check json~toJSON(json~fromJSON('[]')), '[]', "edge: empty array round-trip"

-- empty string
call check json~toJSON(json~fromJSON('""')), '""', "edge: empty string round-trip"

-- single values
call check json~toJSON(json~fromJSON('0')), '0', "edge: zero round-trip"
call check json~toJSON(json~fromJSON('""')), '""', "edge: empty string value round-trip"
say

/*========================================================================*/
say "--- 27. Edge cases: deeply nested structures ---"

-- deeply nested arrays
jsonText = '[[[[["deep"]]]]]'
call check json~fromJSON(jsonText)[1][1][1][1][1], "deep", "edge: 5-level nested array"

-- deeply nested objects
jsonText = '{"a":{"b":{"c":{"d":{"e":"deep"}}}}}'
call check json~fromJSON(jsonText)["a"]["b"]["c"]["d"]["e"], "deep", "edge: 5-level nested object"
say

/*========================================================================*/
say "--- 28. Edge cases: strings ---"

-- string that looks like a number
call check json~fromJSON('"42"'), "42", "edge: string that looks like number"
val = json~fromJSON('"42"')
call checkIsA val, .JsonString, "edge: string '42' is JsonString"

-- string that looks like boolean
call check json~fromJSON('"true"'), "true", "edge: string 'true'"
call check json~fromJSON('"false"'), "false", "edge: string 'false'"
call check json~fromJSON('"null"'), "null", "edge: string 'null'"

-- string with only whitespace
call check json~fromJSON('"   "'), "   ", "edge: whitespace-only string"

-- single character string
call check json~fromJSON('"x"'), "x", "edge: single char string"

-- string with all escape sequences
all = json~fromJSON('"\"\\\b\f\n\r\t\/"')
expected = '"' || '\' || '08'x || '0c'x || '0a'x || '0d'x || '09'x || '/'
call check all, expected, "edge: all escape sequences combined"
say

/*========================================================================*/
say "--- 29. Error handling ---"

-- invalid JSON should raise syntax errors
call checkError '{ invalid }', "error: invalid object"
call checkError '{"key": }', "error: missing value"
call checkError '{"key" "value"}', "error: missing colon"
call checkError '[1, 2,]', "error: trailing comma in array"
call checkError '{"a": 1,}', "error: trailing comma in object"
call checkError 'undefined', "error: invalid keyword"
call checkError '', "error: empty input"
call checkError '"\u00GG"', "error: invalid \\u00XX hex"
call checkError '"\uGGGG"', "error: invalid \\uXXXX hex"
call checkError '"\z"', "error: invalid escape char"
call checkError '"unterminated', "error: unterminated string"
call checkError '{', "error: unterminated object"
call checkError '[', "error: unterminated array"
call checkError '{"a": 1} extra', "error: extra chars after value"
say

/*========================================================================*/
say "--- 30. Decoding: JSON number validation (RFC 8259) ---"

-- Valid JSON numbers that should decode fine
call check json~fromJSON('0'), 0, "num valid: 0"
call check json~fromJSON('-0'), "-0", "num valid: -0"
call check json~fromJSON('42'), 42, "num valid: 42"
call check json~fromJSON('-42'), -42, "num valid: -42"
call check json~fromJSON('3.14'), 3.14, "num valid: 3.14"
call check json~fromJSON('-3.14'), -3.14, "num valid: -3.14"
call check json~fromJSON('0.5'), 0.5, "num valid: 0.5"
call check json~fromJSON('1e2'), "1e2", "num valid: 1e2"
call check json~fromJSON('1E2'), "1E2", "num valid: 1E2"
call check json~fromJSON('1e+2'), "1e+2", "num valid: 1e+2"
call check json~fromJSON('1e-2'), "1e-2", "num valid: 1e-2"
call check json~fromJSON('1.5e3'), "1.5e3", "num valid: 1.5e3"
call check json~fromJSON('-1.5e-3'), "-1.5e-3", "num valid: -1.5e-3"
call check json~fromJSON('100'), 100, "num valid: 100"

-- Invalid JSON numbers that should be rejected
call checkError '+42', "num reject: leading plus"
call checkError '042', "num reject: leading zero"
call checkError '00', "num reject: double zero"
call checkError '.5', "num reject: leading dot"
call checkError '5.', "num reject: trailing dot"
call checkError '-.5', "num reject: minus leading dot"
call checkError '-042', "num reject: minus leading zero"
call checkError '01.5', "num reject: leading zero with frac"
say

/*========================================================================*/
say "--- 31. Encoding: number normalization ---"

-- Rexx numbers that are not valid JSON are normalized via +0
call check json~toJSON(42), '42', "enc norm: integer unchanged"
call check json~toJSON(-42), '-42', "enc norm: negative unchanged"
call check json~toJSON(3.14), '3.14', "enc norm: float unchanged"
call check json~toJSON(0), '0', "enc norm: zero"
call check json~toJSON(.5), '0.5', "enc norm: .5 -> 0.5"
call check json~toJSON(042), '42', "enc norm: 042 -> 42"
call check json~toJSON(00), '0', "enc norm: 00 -> 0"

-- Large numbers: round-trip must preserve value without truncation
number = 12345678901234567890
call check json~toJSON(json~fromJSON(number)), number, "enc norm: large integer round-trip"
number = 99999999999999999999999999999999
call check json~toJSON(json~fromJSON(number)), number, "enc norm: very large integer round-trip"
number = -12345678901234567890
call check json~toJSON(json~fromJSON(number)), number, "enc norm: large negative round-trip"
number = 12345678901234567890.123456789
call check json~toJSON(json~fromJSON(number)), number, "enc norm: large decimal round-trip"
say

/*========================================================================*/
say "--- 32. UTF-8 BOM handling ---"

-- BOM is stripped before parsing
bom = 'EFBBBF'x
call check json~fromJSON(bom || '{}')~items, 0, "BOM: empty object"
call check json~fromJSON(bom || '42'), 42, "BOM: number"
call check json~fromJSON(bom || '"hello"'), "hello", "BOM: string"
say

/*========================================================================*/
say "--- 33. Error reporting: line/column/context ---"

-- single-line error
call checkErrorMsg '{"a": bad}', "line 1", "errfmt: single-line has line 1"
call checkErrorMsg '{"a": bad}', "column 7", "errfmt: single-line has column"

-- multi-line error on line 3
multiLine = '{"a": 1,' || '0A'x || ' "b": 2,' || '0A'x || ' "c": bad}'
call checkErrorMsg multiLine, "line 3", "errfmt: multi-line correct line"
call checkErrorMsg multiLine, "column 7", "errfmt: multi-line correct column"

-- error on line 2 with CR+LF
crlfInput = '{"a": 1,' || '0D0A'x || ' "b": bad}'
call checkErrorMsg crlfInput, "line 2", "errfmt: CRLF line counting"

-- error message starts with JsonError:
call checkErrorMsg '042', "JsonError:", "errfmt: prefix is JsonError:"

-- context line is present in additional info
call checkErrorContext '{"x": bad}', '"x": bad}', "errfmt: context line present"
say

/*========================================================================*/
say "--- 34. Legible round-trip ---"

-- encode legible, decode, re-encode minimized, verify
dir = .directory~new
dir["name"] = "test"
dir["items"] = .array~of(1, 2, 3)
inner = .directory~new
inner["flag"] = .JsonBoolean~true
dir["config"] = inner

legibleJson = json~toJSON(dir, .true)
decoded = json~fromJSON(legibleJson)
minimized = json~toJSON(decoded)
decoded2 = json~fromJSON(minimized)
call checkDeepEqual decoded, decoded2, "legible vs minimized round-trip"
say


/*========================================================================*/
/* XML round-trip tests                                                   */
/*========================================================================*/
say
say "--- XML round-trip tests ---"
say

/* XSD round-trip with test_all_constructs.json */
obj1 = json~fromJSON(jsonText)
xml1 = .json~jsonToXml(obj1, "xsd")
call checkTrue xml1~pos('<?xml') > 0, "xsd xml has declaration"
call checkTrue xml1~pos('xmlns=') > 0, "xsd xml has namespace"
call checkTrue xml1~pos('<json') > 0, "xsd xml has json element"
doc2 = json~parseXml(xml1)
call checkDeepEqual obj1, doc2, "xsd roundtrip all_constructs"
/* Re-encode stability */
xml1b = .json~jsonToXml(doc2, "xsd")
doc2b = json~parseXml(xml1b)
call checkDeepEqual doc2, doc2b, "xsd roundtrip stable"

/* DTD round-trip with test_all_constructs.json */
xml3 = .json~jsonToXml(obj1, "dtd")
call checkTrue xml3~pos('<!DOCTYPE') > 0, "dtd xml has DOCTYPE"
call checkTrue xml3~pos('xmlns=') == 0, "dtd xml no namespace"
doc3 = json~parseXml(xml3)
call checkDeepEqual obj1, doc3, "dtd roundtrip all_constructs"
/* Re-encode stability */
xml3b = .json~jsonToXml(doc3, "dtd")
doc3b = json~parseXml(xml3b)
call checkDeepEqual doc3, doc3b, "dtd roundtrip stable"

/* File round-trip (XSD) */
xmlFile = "test_xml_xsd_tmp.xml"
.json~jsonToXmlFile(obj1, xmlFile, "xsd")
doc4 = json~parseXmlFile(xmlFile)
call checkDeepEqual obj1, doc4, "xsd file roundtrip"
call SysFileDelete xmlFile

/* File round-trip (DTD) */
xmlDtdFile = "test_xml_dtd_tmp.xml"
.json~jsonToXmlFile(obj1, xmlDtdFile, "dtd")
doc5 = json~parseXmlFile(xmlDtdFile)
call checkDeepEqual obj1, doc5, "dtd file roundtrip"
call SysFileDelete xmlDtdFile

/* Type preservation: JsonBoolean */
dir = .directory~new
dir["bt"] = .JsonBoolean~true
dir["bf"] = .JsonBoolean~false
dir["one"] = 1
dir["zero"] = 0
xmlBool = .json~jsonToXml(dir)
call checkTrue xmlBool~pos('<boolean>true</boolean>') > 0, "xml has bool true"
call checkTrue xmlBool~pos('<boolean>false</boolean>') > 0, "xml has bool false"
docBool = json~parseXml(xmlBool)
call checkIsA docBool["bt"], .JsonBoolean, "xml roundtrip bt isA"
call checkIsA docBool["bf"], .JsonBoolean, "xml roundtrip bf isA"
call checkTrue docBool["bt"] = .true, "xml roundtrip bt value"
call checkTrue docBool["bf"] = .false, "xml roundtrip bf value"
call checkFalse docBool["one"]~isA(.JsonBoolean), "xml roundtrip one not bool"

/* Type preservation: JsonString */
dir2 = .directory~new
dir2["jstr"] = .JsonString~new("42")
dir2["num"] = 42
dir2["plain"] = "hello"
xmlStr = .json~jsonToXml(dir2)
docStr = json~parseXml(xmlStr)
call checkIsA docStr["jstr"], .JsonString, "xml roundtrip jstr isA"
call checkIsA docStr["plain"], .JsonString, "xml roundtrip plain isA"
call check docStr["jstr"], "42", "xml roundtrip jstr value"
call check docStr["num"] + 0, 42, "xml roundtrip num value"
call checkFalse docStr["num"]~isA(.JsonString), "xml roundtrip num not string"

/* Type preservation: null */
dir3 = .directory~new
dir3["x"] = .nil
xmlNull = .json~jsonToXml(dir3)
call checkTrue xmlNull~pos('<null/>') > 0, "xml has null element"
docNull = json~parseXml(xmlNull)
call checkNil docNull["x"], "xml roundtrip null"

/* Empty collections */
dir4 = .directory~new
dir4["eo"] = .directory~new
dir4["ea"] = .array~new
xmlEmpty = .json~jsonToXml(dir4)
call checkTrue xmlEmpty~pos('<object/>') > 0, "xml has empty object"
call checkTrue xmlEmpty~pos('<array/>') > 0, "xml has empty array"
docEmpty = json~parseXml(xmlEmpty)
call check docEmpty["eo"]~items, 0, "xml roundtrip empty object"
call check docEmpty["ea"]~items, 0, "xml roundtrip empty array"

/* Array at root */
arr = json~fromJSON('[1,"hello",true,null,{"k":"v"},[2,3]]')
xmlArr = .json~jsonToXml(arr)
docArr = json~parseXml(xmlArr)
call checkDeepEqual arr, docArr, "xml roundtrip array root"

/* Nested structure */
nested = json~fromJSON('{"a":{"b":{"c":[1,2,{"d":true}]}}}')
xmlNest = .json~jsonToXml(nested)
docNest = json~parseXml(xmlNest)
call checkDeepEqual nested, docNest, "xml roundtrip nested"

/* Special characters (XML entities) */
dir5 = .directory~new
dir5["amp"] = "a&b"
dir5["lt"] = "a<b"
dir5["gt"] = "a>b"
dir5["quot"] = 'a"b'
dir5["apos"] = "a'b"
xmlSpec = .json~jsonToXml(dir5)
docSpec = json~parseXml(xmlSpec)
call check docSpec["amp"], "a&b", "xml roundtrip ampersand"
call check docSpec["lt"], "a<b", "xml roundtrip less-than"
call check docSpec["gt"], "a>b", "xml roundtrip greater-than"
call check docSpec["quot"], 'a"b', "xml roundtrip double-quote"
call check docSpec["apos"], "a'b", "xml roundtrip apostrophe"

/* Special chars in key names */
dir6 = .directory~new
dir6["a&b"] = 1
dir6["c<d"] = 2
xmlKey = .json~jsonToXml(dir6)
docKey = json~parseXml(xmlKey)
call check docKey["a&b"], 1, "xml roundtrip key with ampersand"
call check docKey["c<d"], 2, "xml roundtrip key with less-than"

/* XSD vs DTD cross-roundtrip */
xmlXsd = .json~jsonToXml(obj1, "xsd")
docXsd = json~parseXml(xmlXsd)
xmlDtd = .json~jsonToXml(docXsd, "dtd")
docDtd = json~parseXml(xmlDtd)
call checkDeepEqual docXsd, docDtd, "xsd->dtd cross roundtrip"

/* XSLT round-trip: JSON -> XML -> xsltproc xmlToJson.xsl -> JSON */
say
say "--- XSLT round-trip tests ---"
say

xsltAvailable = .false

/* Generate temp XML files */
xsdTmpFile = "test_xslt_xsd_tmp.xml"
dtdTmpFile = "test_xslt_dtd_tmp.xml"
.json~jsonToXmlFile(obj1, xsdTmpFile, "xsd")
.json~jsonToXmlFile(obj1, dtdTmpFile, "dtd")

/* json.dtd must be findable relative to the DTD XML file */
needDtdCleanup = .false
If SysFileExists("docbook/json.dtd"), \SysFileExists("json.dtd") Then Do
  address system "cp docbook/json.dtd json.dtd"
  needDtdCleanup = .true
End

/* Attempt xsltproc */
Signal On Syntax Name NoXsltProc
xslFile = "docbook/xmlToJson.xsl"
outXsd = .array~new
errXsd = .array~new
address system "xsltproc" xslFile xsdTmpFile -
  with output using (outXsd) error using (errXsd)
If outXsd~items > 0 Then xsltAvailable = .true
Signal XsltCheckDone

NoXsltProc:
  Signal Off Syntax

XsltCheckDone:
  Signal Off Syntax

If \xsltAvailable Then Do
  say "  SKIP: xsltproc not available, skipping XSLT round-trip tests"
  Call SysFileDelete xsdTmpFile
  Call SysFileDelete dtdTmpFile
  If needDtdCleanup Then Call SysFileDelete "json.dtd"
End
Else Do
  /* XSD XSLT round-trip */
  xsdJson = outXsd~makeString('L', "0A"x)
  xsdDoc = json~fromJSON(xsdJson)
  xsdOrigJson = json~toJSON(obj1)
  xsdRtJson   = json~toJSON(xsdDoc)
  call check xsdRtJson, xsdOrigJson, "XSLT xsd round-trip"

  /* DTD XSLT round-trip */
  outDtd = .array~new
  errDtd = .array~new
  address system "xsltproc" xslFile dtdTmpFile -
    with output using (outDtd) error using (errDtd)
  dtdJson = outDtd~makeString('L', "0A"x)
  dtdDoc = json~fromJSON(dtdJson)
  dtdRtJson = json~toJSON(dtdDoc)
  call check dtdRtJson, xsdOrigJson, "XSLT dtd round-trip"

  Call SysFileDelete xsdTmpFile
  Call SysFileDelete dtdTmpFile
  If needDtdCleanup Then Call SysFileDelete "json.dtd"
End

say


/*========================================================================*/
say copies("=", 64)
say "Results:" pass "/" tests "passed," fail "failed"
say copies("=", 64)

if fail > 0 then exit 1
exit 0


/* ---- internal subroutines ---- */

check: procedure expose tests pass fail
  use arg actual, expected, label
  tests = tests + 1
  if actual == expected then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected:" expected
    say "    actual  :" actual
    fail = fail + 1
  end
  return

checkNil: procedure expose tests pass fail
  use arg actual, label
  tests = tests + 1
  if actual~isNil then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected: .nil"
    say "    actual  :" actual
    fail = fail + 1
  end
  return

checkIsA: procedure expose tests pass fail
  use arg obj, cls, label
  tests = tests + 1
  if obj~isA(cls) then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected: isA" cls
    say "    actual  :" obj~class
    fail = fail + 1
  end
  return

checkTrue: procedure expose tests pass fail
  use arg condition, label
  tests = tests + 1
  if condition then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected: .true"
    fail = fail + 1
  end
  return

checkFalse: procedure expose tests pass fail
  use arg condition, label
  tests = tests + 1
  if \condition then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected: .false"
    fail = fail + 1
  end
  return

checkError: procedure expose tests pass fail
  use arg jsonText, label
  tests = tests + 1
  json = .json~new
  signal on syntax name caught
  json~fromJSON(jsonText)
  -- if we get here, no error was raised
  say "  FAIL:" label "(no error raised)"
  fail = fail + 1
  return
caught:
  say "  PASS:" label
  pass = pass + 1
  return

checkErrorMsg: procedure expose tests pass fail
  use arg jsonText, expected, label
  tests = tests + 1
  json = .json~new
  signal on syntax name caught
  json~fromJSON(jsonText)
  say "  FAIL:" label "(no error raised)"
  fail = fail + 1
  return
caught:
  co = condition("O")
  msg = co~message
  if msg~caselessPos(expected) > 0 then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected to contain:" expected
    say "    actual message     :" msg
    fail = fail + 1
  end
  return

checkErrorContext: procedure expose tests pass fail
  use arg jsonText, expected, label
  tests = tests + 1
  json = .json~new
  signal on syntax name caught
  json~fromJSON(jsonText)
  say "  FAIL:" label "(no error raised)"
  fail = fail + 1
  return
caught:
  co = condition("O")
  additional = co~additional
  if additional~caselessPos(expected) > 0 then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    expected to contain:" expected
    say "    additional         :" additional
    fail = fail + 1
  end
  return

/** Recursively compares two decoded JSON structures for semantic equivalence.
 *  Both objects should have been produced by json~fromJSON.
 */
checkDeepEqual: procedure expose tests pass fail
  use arg obj1, obj2, label
  tests = tests + 1
  if deepEqual(obj1, obj2) then do
    say "  PASS:" label
    pass = pass + 1
  end
  else do
    say "  FAIL:" label
    say "    objects are not semantically equivalent"
    fail = fail + 1
  end
  return

/** Recursive deep-equality comparison for JSON-decoded structures.
 *  Handles .Directory, .Array, .JsonBoolean, .JsonString, .String, .nil.
 */
deepEqual: procedure
  use arg a, b

  -- both nil?
  if a~isNil, b~isNil then return .true
  -- one nil, other not?
  if a~isNil | b~isNil then return .false

  -- both directories?
  if a~isA(.directory), b~isA(.directory) then do
    if a~items \== b~items then return .false
    do key over a~allIndexes
      if \b~hasIndex(key) then return .false
      if \deepEqual(a[key], b[key]) then return .false
    end
    return .true
  end

  -- both arrays?
  if a~isA(.array), b~isA(.array) then do
    if a~items \== b~items then return .false
    do i = 1 to a~items
      if \deepEqual(a[i], b[i]) then return .false
    end
    return .true
  end

  -- both JsonBoolean?
  if a~isA(.JsonBoolean), b~isA(.JsonBoolean) then
    return a~makeJSON == b~makeJSON

  -- string/number comparison (covers .JsonString and plain .String)
  -- Use numeric comparison for numbers to handle equivalent representations
  -- (e.g. "0.0" and "0", "1e+5" and "100000")
  if a~dataType('n'), b~dataType('n') then
    return a = b
  return a == b


::requires "json.cls"
