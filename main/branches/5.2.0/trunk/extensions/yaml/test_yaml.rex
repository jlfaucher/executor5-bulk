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

parser = .Yaml~new
tests  = 0
pass   = 0
fail   = 0

say copies("=", 64)
say "yaml.cls test suite"
say copies("=", 64)
say

/*========================================================================*/
say "--- 1. Basic mapping ---"

yaml = "title: Hello World" || "0A"x || -
       "author: Jane Doe"  || "0A"x || -
       "year: 2025"

doc = parser~parseString(yaml)
call check doc["title"], "Hello World", "title"
call check doc["author"], "Jane Doe", "author"
call check doc["year"], 2025, "year"
say

/*========================================================================*/
say "--- 2. Nested mapping ---"

yaml = "database:"          || "0A"x || -
       "  host: localhost"  || "0A"x || -
       "  port: 5432"       || "0A"x || -
       "  name: mydb"

doc = parser~parseString(yaml)
db = doc["database"]
call check db["host"], "localhost", "nested host"
call check db["port"], 5432, "nested port"
say

/*========================================================================*/
say "--- 3. Block sequence ---"

yaml = "fruits:"     || "0A"x || -
       "  - apple"   || "0A"x || -
       "  - banana"  || "0A"x || -
       "  - cherry"

doc = parser~parseString(yaml)
fruits = doc["fruits"]
call check fruits~items, 3, "seq count"
call check fruits[1], "apple", "seq item 1"
call check fruits[3], "cherry", "seq item 3"
say

/*========================================================================*/
say "--- 4. Flow collections ---"

yaml = "colors: [red, green, blue]" || "0A"x || -
       "point: {x: 10, y: 20}"

doc = parser~parseString(yaml)
colors = doc["colors"]
point  = doc["point"]
call check colors[2], "green", "flow seq"
call check point["x"], 10, "flow map x"
call check point["y"], 20, "flow map y"
say

/*========================================================================*/
say "--- 5. Quoted strings ---"

yaml = "single: 'It''s a test'"            || "0A"x || -
       'double: "line1\nline2"'            || "0A"x || -
       'escaped: "tab\there"'              || "0A"x || -
       'preserve: "hello world"'

doc = parser~parseString(yaml)
call check doc["single"], "It's a test", "single quote"
call check (doc["double"]~pos("0A"x) > 0), 1, "dquote newline"
call check (doc["escaped"]~pos("09"x) > 0), 1, "dquote tab"
say

/*========================================================================*/
say "--- 6. Type resolution ---"

yaml = "null1: null"   || "0A"x || -
       "null2: ~"      || "0A"x || -
       "btrue: true"   || "0A"x || -
       "bfalse: no"    || "0A"x || -
       "int: 42"       || "0A"x || -
       "neg: -17"      || "0A"x || -
       "hex: 0xFF"     || "0A"x || -
       "oct: 0o77"     || "0A"x || -
       "float: 3.14"   || "0A"x || -
       "sci: 1.5e+3"   || "0A"x || -
       "inf: .inf"     || "0A"x || -
       "nan: .nan"     || "0A"x || -
       'strnull: "null"'

doc = parser~parseString(yaml)
call check (doc["null1"] == .nil), 1, "null"
call check (doc["null2"] == .nil), 1, "tilde"
call check doc["btrue"], 1, "true"
call check doc["bfalse"], 0, "false/no"
call check doc["int"], 42, "int"
call check doc["neg"], -17, "negative"
call check doc["hex"], 255, "hex"
call check doc["oct"], 63, "octal"
call check doc["float"], 3.14, "float"
call check doc["inf"], ".inf", "inf"
call check doc["nan"], ".nan", "nan"
call check doc["strnull"], "null", "str null"
say

/*========================================================================*/
say "--- 7. Block scalars ---"

yaml = "literal: |"    || "0A"x || -
       "  line one"    || "0A"x || -
       "  line two"    || "0A"x || -
       "folded: >"     || "0A"x || -
       "  this is"     || "0A"x || -
       "  one line"    || "0A"x || -
       "stripped: |-"  || "0A"x || -
       "  no newline"

doc = parser~parseString(yaml)
call check (doc["literal"]~countStr("0A"x) >= 1), 1, "literal has newline"
call check (doc["folded"]~pos("this is one line") > 0), 1, "folded joins"
call check (doc["stripped"]~right(1) \== "0A"x), 1, "strip chomp"
say

/*========================================================================*/
say "--- 8. Anchors & aliases ---"

yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"

doc = parser~parseString(yaml)
dev = doc["development"]
call check dev["adapter"], "postgres", "merge adapter"
call check dev["host"], "localhost", "merge host"
call check dev["database"], "myapp_dev", "own key"
say

/*========================================================================*/
say "--- 9. Sequence of mappings ---"

yaml = "people:"            || "0A"x || -
       "  - name: Alice"    || "0A"x || -
       "    age: 30"        || "0A"x || -
       "  - name: Bob"      || "0A"x || -
       "    age: 25"

doc = parser~parseString(yaml)
people = doc["people"]
call check people~items, 2, "person count"
call check people[1]["name"], "Alice", "person 1 name"
call check people[2]["age"], 25, "person 2 age"
say

/*========================================================================*/
say "--- 10. Multi-document ---"

yaml = "---"            || "0A"x || -
       "first: doc1"    || "0A"x || -
       "---"            || "0A"x || -
       "second: doc2"   || "0A"x || -
       "..."

docs = parser~parseAll(yaml)
call check docs~items, 2, "doc count"
call check docs[1]["first"], "doc1", "doc 1"
call check docs[2]["second"], "doc2", "doc 2"
say

/*========================================================================*/
say "--- 11. Front matter ---"

md = "---"                      || "0A"x || -
     "title: My Post"           || "0A"x || -
     "date: 2025-01-15"         || "0A"x || -
     "tags: [yaml, rexx]"       || "0A"x || -
     "---"                      || "0A"x || -
     ""                         || "0A"x || -
     "# This is the body"       || "0A"x || -
     "Not parsed as YAML."

fm = parser~parseFrontMatter(md)
call check fm["title"], "My Post", "fm title"
call check fm["date"], "2025-01-15", "fm date"
tags = fm["tags"]
call check tags~items, 2, "fm tag count"
call check tags[1], "yaml", "fm tag 1"
say

/*========================================================================*/
say "--- 12. Comments ---"

yaml = "# Full line comment"     || "0A"x || -
       "key: value  # inline"    || "0A"x || -
       "other: stuff"

doc = parser~parseString(yaml)
call check doc["key"], "value", "inline comment stripped"
say

/*========================================================================*/
say "--- 13. Complex nesting ---"

yaml = "servers:"                 || "0A"x || -
       "  - name: web1"           || "0A"x || -
       "    roles:"               || "0A"x || -
       "      - frontend"         || "0A"x || -
       "      - api"              || "0A"x || -
       "    config:"              || "0A"x || -
       "      port: 8080"         || "0A"x || -
       "      ssl: true"          || "0A"x || -
       "  - name: db1"            || "0A"x || -
       "    roles:"               || "0A"x || -
       "      - database"

doc = parser~parseString(yaml)
servers = doc["servers"]
call check servers~items, 2, "server count"
call check servers[1]["name"], "web1", "server1 name"
roles = servers[1]["roles"]
call check roles~items, 2, "server1 roles"
call check roles[1], "frontend", "server1 role 1"
cfg = servers[1]["config"]
call check cfg["port"], 8080, "server1 port"
call check cfg["ssl"], 1, "server1 ssl"
call check servers[2]["roles"]~items, 1, "server2 role cnt"
say

/*========================================================================*/
say "--- 14. Nested flow collections ---"

yaml = "matrix: [[1, 2], [3, 4]]"  || "0A"x || -
       "mixed: [{a: 1}, {b: 2}]"

doc = parser~parseString(yaml)
matrix = doc["matrix"]
call check matrix[1][2], 2, "nested flow [1][2]"
call check matrix[2][1], 3, "nested flow [2][1]"
call check doc["mixed"][1]["a"], 1, "mixed flow"
say

/*========================================================================*/
say "--- 15. Serialisation round-trip ---"

yaml = "name: Test"             || "0A"x || -
       "items:"                  || "0A"x || -
       "  - alpha"               || "0A"x || -
       "  - beta"                || "0A"x || -
       "nested:"                 || "0A"x || -
       "  key: value"

doc  = parser~parseString(yaml)
out  = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check doc2["name"], "Test", "roundtrip name"
call check doc2["items"]~items, 2, "roundtrip items"
call check doc2["nested"]["key"], "value", "roundtrip nested"
say

/*========================================================================*/
say "--- 16. Pandoc-style front matter ---"

md = "---"                                || "0A"x || -
     "title: 'Advanced ooRexx'"            || "0A"x || -
     "author:"                             || "0A"x || -
     "  - name: John Smith"                || "0A"x || -
     "    affiliation: ACME Corp"          || "0A"x || -
     "  - name: Jane Doe"                  || "0A"x || -
     "    affiliation: Widgets Inc"        || "0A"x || -
     "abstract: |"                         || "0A"x || -
     "  This paper presents a new"         || "0A"x || -
     "  approach to YAML parsing."         || "0A"x || -
     "keywords: [ooRexx, YAML, parser]"    || "0A"x || -
     "lang: en-GB"                         || "0A"x || -
     "bibliography: refs.bib"              || "0A"x || -
     "---"                                 || "0A"x || -
     ""                                    || "0A"x || -
     "# Introduction"                      || "0A"x || -
     "This is the body."

fm = parser~parseFrontMatter(md)
call check fm["title"], "Advanced ooRexx", "pandoc title"
authors = fm["author"]
call check authors~items, 2, "pandoc authors"
call check authors[1]["name"], "John Smith", "pandoc author1"
call check authors[2]["affiliation"], "Widgets Inc", "pandoc affil2"
call check (fm["abstract"]~pos("approach") > 0), 1, "pandoc abstract"
kw = fm["keywords"]
call check kw~items, 3, "pandoc keywords"
call check fm["lang"], "en-GB", "pandoc lang"
call check fm["bibliography"], "refs.bib", "pandoc bib"
say

/*========================================================================*/
say "--- 17. Comprehensive serialisation round-trip ---"

/* Build a complex document programmatically and verify it survives
   dump → parse → dump without data loss.                                */
big = .table~new
big["plain"] = "hello world"
big["integer"] = 42
big["negative"] = -17
big["float"] = 3.14
big["null_value"] = .nil
big["empty_string"] = ""
big["bool_true"] = "true"       -- stored as quoted string by emitter
big["bool_false"] = "false"

nested = .table~new
nested["level2"] = .table~new
nested["level2"]["deep"] = "value"
big["nested_map"] = nested

big["simple_list"] = .array~of("alpha", "beta", "gamma")

lom = .array~new
m1 = .table~new; m1["name"] = "Alice"; m1["age"] = 30
m2 = .table~new; m2["name"] = "Bob";   m2["age"] = 25
lom~append(m1); lom~append(m2)
big["list_of_maps"] = lom

big["empty_map"] = .table~new
big["empty_list"] = .array~new

multiline = "Line one" || "0A"x || "Line two" || "0A"x || "Line three"
big["multiline"] = multiline

big["special_chars"] = "tabs"||"09"x||"here and\backslash"
big["comment-like"] = "value"
big["key: colon"] = "value"

/* Round-trip: dump → parse → dump. Compare the two dump outputs. */
yaml1 = .Yaml~toYaml(big)
doc2  = parser~parseString(yaml1)
call check doc2~isA(.table), .true, "big roundtrip type"
call check doc2["plain"], "hello world", "big roundtrip plain"
call check doc2["integer"], 42, "big roundtrip integer"
call check doc2["negative"], -17, "big roundtrip negative"
call check doc2["float"], 3.14, "big roundtrip float"
call check doc2["null_value"], .nil, "big roundtrip null"
call check doc2["empty_string"], "", "big roundtrip empty str"
call check doc2["bool_true"], "true", "big roundtrip bool"
call check doc2["nested_map"]["level2"]["deep"], "value", "big roundtrip nested"
call check doc2["simple_list"]~items, 3, "big roundtrip list"
call check doc2["simple_list"][2], "beta", "big roundtrip list[2]"
call check doc2["list_of_maps"][1]["name"], "Alice", "big roundtrip lom"
call check doc2["empty_map"]~isA(.table), .true, "big roundtrip empty map"
call check doc2["empty_map"]~items, 0, "big roundtrip empty map size"
call check doc2["empty_list"]~isA(.array), .true, "big roundtrip empty list"
call check doc2["empty_list"]~items, 0, "big roundtrip empty list size"
call check (doc2["multiline"]~pos("Line two") > 0), 1, "big roundtrip multiline"
call check (doc2["special_chars"]~pos("09"x) > 0), 1, "big roundtrip special"
call check doc2["comment-like"], "value", "big roundtrip comment key"
call check doc2["key: colon"], "value", "big roundtrip colon key"

/* Also verify that dumping the re-parsed doc yields identical YAML */
yaml2 = .Yaml~toYaml(doc2)
call check yaml1, yaml2, "big roundtrip dump stable"
say

/*========================================================================*/
say "--- 18. File round-trip (toYamlFile / parseFile) ---"

/* Note: Table does not preserve insertion order, so we cannot
   compare files byte-for-byte after a round-trip. Instead we verify
   semantic equivalence by parsing both files and comparing objects. */

inFile  = "test_all_constructs.yaml"
outFile = "test_all_constructs_roundtrip.yaml"

doc1 = parser~parseFile(inFile)
.Yaml~toYamlFile(doc1, outFile)
doc3 = parser~parseFile(outFile)

/* Deep structural comparison */
call check YAML.deepEqual(doc1, doc3), .true, "file roundtrip equal"

/* Spot-check specific values */
call check doc3["plain"], "hello world", "file roundtrip plain"
call check doc3["integer"], 42, "file roundtrip integer"
call check doc3["nested_map"]["level1"]["level2"]["level3"], "deep", "file roundtrip nested"
call check doc3["simple_list"][2], "beta", "file roundtrip list"
call check doc3["list_of_maps"][1]["name"], "Alice", "file roundtrip lom"
call check doc3["empty_map"]~items, 0, "file roundtrip empty m"
call check doc3["empty_list"]~items, 0, "file roundtrip empty l"
call check (doc3["multiline"]~pos("multi-line") > 0), 1, "file roundtrip multi"
call check doc3["mixed_nesting"]["users"][1]["name"], "admin", "file roundtrip users"
call check doc3["mixed_nesting"]["users"][1]["roles"][2], "write", "file roundtrip roles"

/* The re-dumped file should also be semantically equal */
.Yaml~toYamlFile(doc3, outFile)
doc4 = parser~parseFile(outFile)
call check YAML.deepEqual(doc3, doc4), .true, "file roundtrip stable"
say

/*========================================================================*/
say "--- 19. Array round-trip (parseArray / toYamlArray) ---"

arrIn = .stream~new(inFile)~~open("READ")~arrayIn

doc5  = parser~parseArray(arrIn)
arrOut = .Yaml~toYamlArray(doc5)
doc6  = parser~parseArray(arrOut)

call check YAML.deepEqual(doc5, doc6), .true, "array roundtrip equal"

/* Spot-checks */
call check doc6["plain"], "hello world", "array roundtrip plain"
call check doc6["integer"], 42, "array roundtrip integer"
call check doc6["simple_list"]~items, 3, "array roundtrip list"
call check doc6["nested_map"]["level1"]["level2"]["level3"], "deep", "array roundtrip nested"
call check doc6["list_of_maps"][2]["name"], "Bob", "array roundtrip lom name"

/* Stability: toYamlArray on re-parsed data should match */
arrOut2 = .Yaml~toYamlArray(doc6)
call check arrOut~items, arrOut2~items, "array roundtrip stable count"
doc7 = parser~parseArray(arrOut2)
call check YAML.deepEqual(doc6, doc7), .true, "array roundtrip stable"
say

/*========================================================================*/
say "--- 20. Front-matter file round-trip (toYamlFMFile / parseFrontMatterFile) ---"

fmData = .table~new
fmData["title"] = "Advanced ooRexx"
fmData["lang"] = "en-GB"
authors = .array~new
a1 = .table~new; a1["name"] = "John Smith"; a1["affiliation"] = "ACME Corp"
a2 = .table~new; a2["name"] = "Jane Doe";   a2["affiliation"] = "Widgets Inc"
authors~append(a1); authors~append(a2)
fmData["author"] = authors
fmData["keywords"] = .array~of("ooRexx", "YAML", "parser")

fmFile = "test_frontmatter.yaml"
.Yaml~toYamlFMFile(fmData, fmFile)

fmDoc = parser~parseFrontMatterFile(fmFile)
call check fmDoc~isA(.table), .true, "fm roundtrip type"
call check YAML.deepEqual(fmData, fmDoc), .true, "fm roundtrip equal"
call check fmDoc["title"], "Advanced ooRexx", "fm roundtrip title"
call check fmDoc["lang"], "en-GB", "fm roundtrip lang"
call check fmDoc["author"]~items, 2, "fm roundtrip authors"
call check fmDoc["author"][1]["name"], "John Smith", "fm roundtrip author1"
call check fmDoc["keywords"]~items, 3, "fm roundtrip kw"

/* Stability: dump again and re-parse */
.Yaml~toYamlFMFile(fmDoc, fmFile)
fmDoc2 = parser~parseFrontMatterFile(fmFile)
call check YAML.deepEqual(fmDoc, fmDoc2), .true, "fm roundtrip stable"
say

/*========================================================================*/
say "--- 21. XML round-trip via XSD (yamlToXml / parseXml) ---"

inFile  = "test_all_constructs.yaml"
xmlFile = "test_all_constructs_xsd.xml"
outFile = "test_all_constructs_with_xsd.yaml"

doc1 = parser~parseFile(inFile)

/* Generate XSD-flavoured XML */
xml1 = .Yaml~yamlToXml(doc1, "xsd")
call check (xml1~pos('<?xml') > 0), 1, "xsd xml has declaration"
call check (xml1~pos('xmlns=') > 0), 1, "xsd xml has namespace"
call check (xml1~pos('<yaml') > 0), 1, "xsd xml has yaml element"

/* Parse XML back to objects */
doc2 = parser~parseXml(xml1)
call check doc2~isA(.table), .true, "xsd roundtrip type"
call check YAML.deepEqual(doc1, doc2), .true, "xsd roundtrip equal"

/* Spot-checks on specific values */
call check doc2["plain"], "hello world", "xsd roundtrip plain"
call check doc2["integer"], 42, "xsd roundtrip integer"
call check doc2["float"], 3.14, "xsd roundtrip float"
call check doc2["null_value"], .nil, "xsd roundtrip null"
call check doc2["empty_string"], "", "xsd roundtrip empty s"
call check doc2["nested_map"]["level1"]["level2"]["level3"], "deep", "xsd roundtrip nested"
call check doc2["simple_list"]~items, 3, "xsd roundtrip list"
call check doc2["simple_list"][2], "beta", "xsd roundtrip list[2]"
call check doc2["list_of_maps"][1]["name"], "Alice", "xsd roundtrip lom"
call check doc2["empty_map"]~items, 0, "xsd roundtrip empty m"
call check doc2["empty_list"]~items, 0, "xsd roundtrip empty l"
call check (doc2["multiline"]~pos("multi-line") > 0), 1, "xsd roundtrip multi"
call check (doc2["special_chars"]~pos("09"x) > 0), 1, "xsd roundtrip special"
call check doc2["mixed_nesting"]["users"][1]["name"], "admin", "xsd roundtrip users"
call check doc2["mixed_nesting"]["users"][1]["roles"][2], "write", "xsd roundtrip roles"

/* Stability: XML → parse → XML should produce identical XML */
xml2 = .Yaml~yamlToXml(doc2, "xsd")
doc3 = parser~parseXml(xml2)
call check YAML.deepEqual(doc2, doc3), .true, "xsd roundtrip stable"

/* Also dump back to YAML and verify */
.Yaml~toYamlFile(doc2, outFile)
doc4 = parser~parseFile(outFile)
call check YAML.deepEqual(doc1, doc4), .true, "xsd yaml output equal"
say

/*========================================================================*/
say "--- 22. XML round-trip via DTD (yamlToXml / parseXml) ---"

xmlDtdFile = "test_all_constructs_dtd.xml"
outDtdFile = "test_all_constructs_with_dtd.yaml"

/* Generate DTD-flavoured XML */
xml3 = .Yaml~yamlToXml(doc1, "dtd")
call check (xml3~pos('<!DOCTYPE') > 0), 1, "dtd xml has DOCTYPE"
call check (xml3~pos('xmlns=') == 0), 1, "dtd xml no namespace"

/* Parse DTD XML back to objects */
doc5 = parser~parseXml(xml3)
call check doc5~isA(.table), .true, "dtd roundtrip type"
call check YAML.deepEqual(doc1, doc5), .true, "dtd roundtrip equal"

/* Spot-checks */
call check doc5["plain"], "hello world", "dtd roundtrip plain"
call check doc5["integer"], 42, "dtd roundtrip integer"
call check doc5["nested_map"]["level1"]["level2"]["level3"], "deep", "dtd roundtrip nested"
call check doc5["simple_list"][2], "beta", "dtd roundtrip list"
call check doc5["list_of_maps"][1]["name"], "Alice", "dtd roundtrip lom"
call check doc5["empty_map"]~items, 0, "dtd roundtrip empty m"
call check doc5["empty_list"]~items, 0, "dtd roundtrip empty l"
call check doc5["mixed_nesting"]["users"][1]["name"], "admin", "dtd roundtrip users"

/* Stability */
xml4 = .Yaml~yamlToXml(doc5, "dtd")
doc6 = parser~parseXml(xml4)
call check YAML.deepEqual(doc5, doc6), .true, "dtd roundtrip stable"

/* Dump back to YAML */
.Yaml~toYamlFile(doc5, outDtdFile)
doc7 = parser~parseFile(outDtdFile)
call check YAML.deepEqual(doc1, doc7), .true, "dtd yaml output equal"
say

/*========================================================================*/
say "--- 23. XML file round-trip (yamlToXmlFile / parseXmlFile) ---"

/* XSD file round-trip */
.Yaml~yamlToXmlFile(doc1, xmlFile, "xsd")
doc8 = parser~parseXmlFile(xmlFile)
call check YAML.deepEqual(doc1, doc8), .true, "xsd file roundtrip equal"

/* DTD file round-trip */
.Yaml~yamlToXmlFile(doc1, xmlDtdFile, "dtd")
doc9 = parser~parseXmlFile(xmlDtdFile)
call check YAML.deepEqual(doc1, doc9), .true, "dtd file roundtrip equal"

/* Stability: re-export and re-parse */
.Yaml~yamlToXmlFile(doc8, xmlFile, "xsd")
doc10 = parser~parseXmlFile(xmlFile)
call check YAML.deepEqual(doc8, doc10), .true, "xsd file roundtrip stable"

.Yaml~yamlToXmlFile(doc9, xmlDtdFile, "dtd")
doc11 = parser~parseXmlFile(xmlDtdFile)
call check YAML.deepEqual(doc9, doc11), .true, "dtd file roundtrip stable"
say

/*========================================================================*/
say "--- 24. YamlBoolean type preservation ---"

yaml = "btrue: true"     || "0A"x || -
       "bfalse: false"   || "0A"x || -
       "byes: yes"       || "0A"x || -
       "bno: no"         || "0A"x || -
       "bon: on"         || "0A"x || -
       "boff: off"       || "0A"x || -
       "int1: 1"         || "0A"x || -
       "int0: 0"

doc = parser~parseString(yaml)

/* YamlBoolean instances for boolean words */
call check doc["btrue"]~isA(.YamlBoolean), .true, "true isA YamlBoolean"
call check doc["bfalse"]~isA(.YamlBoolean), .true, "false isA YamlBoolean"
call check doc["byes"]~isA(.YamlBoolean), .true, "yes isA YamlBoolean"
call check doc["bno"]~isA(.YamlBoolean), .true, "no isA YamlBoolean"
call check doc["bon"]~isA(.YamlBoolean), .true, "on isA YamlBoolean"
call check doc["boff"]~isA(.YamlBoolean), .true, "off isA YamlBoolean"

/* Plain integers are NOT YamlBoolean */
call check doc["int1"]~isA(.YamlBoolean), .false, "1 not YamlBoolean"
call check doc["int0"]~isA(.YamlBoolean), .false, "0 not YamlBoolean"

/* Transparent comparison with 1/0 */
call check doc["btrue"], 1, "true == 1"
call check doc["bfalse"], 0, "false == 0"
call check doc["byes"], 1, "yes == 1"
call check doc["bno"], 0, "no == 0"

/* makeYAML produces canonical form */
call check doc["btrue"]~makeYAML, "true", "true makeYAML"
call check doc["bfalse"]~makeYAML, "false", "false makeYAML"

/* Yaml class accessors */
call check .Yaml~true~isA(.YamlBoolean), .true, ".Yaml~true"
call check .Yaml~false~isA(.YamlBoolean), .true, ".Yaml~false"
call check .Yaml~true, 1, ".Yaml~true value"
call check .Yaml~false, 0, ".Yaml~false value"

/* YAML serialisation round-trip preserves boolean */
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["btrue"]~isA(.YamlBoolean), .true, "yaml roundtrip btrue"
call check doc2["bfalse"]~isA(.YamlBoolean), .true, "yaml roundtrip bfalse"
call check doc2["int1"]~isA(.YamlBoolean), .false, "yaml roundtrip int1"
call check YAML.deepEqual(doc, doc2), .true, "yaml roundtrip equal"

/* XML round-trip preserves boolean type */
xml = .Yaml~yamlToXml(doc, "xsd")
call check (xml~pos('type="bool"') > 0), 1, "xml has bool type"
doc3 = parser~parseXml(xml)
call check doc3["btrue"]~isA(.YamlBoolean), .true, "xml roundtrip btrue isA"
call check doc3["bfalse"]~isA(.YamlBoolean), .true, "xml roundtrip bfalse isA"
call check doc3["int1"]~isA(.YamlBoolean), .false, "xml roundtrip int1 not"
call check YAML.deepEqual(doc, doc3), .true, "xml roundtrip equal"
say

/*========================================================================*/
say "--- 25. Anchor/alias round-trip ---"

/* Parse YAML with anchors */
yamlAnch = "defaults: &defs"     || "0A"x || -
           "  adapter: postgres"  || "0A"x || -
           "  host: localhost"    || "0A"x || -
           "other: &other"        || "0A"x || -
           "  port: 5432"         || "0A"x || -
           "development:"         || "0A"x || -
           "  <<: *defs"          || "0A"x || -
           "  database: myapp_dev"

docA = parser~parseString(yamlAnch)
am   = parser~anchorMap

/* anchorMap captured anchor metadata */
call check (am~items > 0), .true, "anchorMap has entries"
call check docA["development"]["adapter"], "postgres", "merge adapter"
call check docA["development"]["database"], "myapp_dev", "merge database"

/* Non-merge shared objects */
yamlShared = "base: &shared"        || "0A"x || -
             "  x: 10"              || "0A"x || -
             "  y: 20"              || "0A"x || -
             "ref1: *shared"        || "0A"x || -
             "ref2: *shared"

docS = parser~parseString(yamlShared)
amS  = parser~anchorMap

/* Same object identity (alias returns same instance) */
call check (docS["ref1"] == docS["base"]) | (docS["ref1"] == docS["ref2"]), .true, "ref1 identity"
call check docS["ref1"]["x"], 10, "ref1 x"
call check docS["ref2"]["y"], 20, "ref2 y"

/* YAML round-trip with anchorMap preserves aliases */
yamlOut = .Yaml~toYaml(docS, 2, amS)
call check (yamlOut~pos("&shared") > 0), .true, "yaml has anchor"
call check (yamlOut~pos("*shared") > 0), .true, "yaml has alias"

/* Parse the anchor-aware YAML back */
docS2 = parser~parseString(yamlOut)
call check YAML.deepEqual(docS, docS2), .true, "yaml rt equal"

/* YAML dump without anchorMap expands everything (no aliases) */
yamlExpanded = .Yaml~toYaml(docS)
call check (yamlExpanded~pos("*shared") = 0), .true, "expanded no alias"

/* XML round-trip with anchorMap (XSD) */
xmlAnch = .Yaml~yamlToXml(docS, "xsd", amS)
call check (xmlAnch~pos('anchor="shared"') > 0), .true, "xml has anchor attr"
call check (xmlAnch~pos('<alias') > 0), .true, "xml has alias elem"

docS3 = parser~parseXml(xmlAnch)
call check YAML.deepEqual(docS, docS3), .true, "xml rt equal"
/* XML parser restores shared identity */
call check (docS3["ref1"] == docS3["ref2"]) | -
           (docS3["ref1"] == docS3["base"]) | -
           (docS3["ref2"] == docS3["base"]), .true, "xml identity"

/* XML round-trip with anchorMap (DTD) */
xmlDtd = .Yaml~yamlToXml(docS, "dtd", amS)
call check (xmlDtd~pos('anchor="shared"') > 0), .true, "dtd has anchor"
docS4 = parser~parseXml(xmlDtd)
call check YAML.deepEqual(docS, docS4), .true, "dtd rt equal"

/* Sequence with anchored items */
yamlSeq = "items:"             || "0A"x || -
          "  - &item1"         || "0A"x || -
          "    name: first"    || "0A"x || -
          "    val: 100"       || "0A"x || -
          "  - *item1"

docSeq = parser~parseString(yamlSeq)
amSeq  = parser~anchorMap
call check docSeq["items"][2]["name"], "first", "seq alias name"
call check (docSeq["items"][1] == docSeq["items"][2]), .true, "seq identity"

/* XML round-trip for sequence aliases */
xmlSeq = .Yaml~yamlToXml(docSeq, "xsd", amSeq)
docSeq2 = parser~parseXml(xmlSeq)
call check YAML.deepEqual(docSeq, docSeq2), .true, "seq xml rt equal"
call check (docSeq2["items"][1] == docSeq2["items"][2]), .true, "seq xml identity"
say

/*========================================================================*/
say "--- 26. Comprehensive file round-trip with all constructs ---"

/* Parse the expanded test_all_constructs.yaml (now includes anchors,
   booleans, flow collections, folded scalars, numeric types, etc.)
   and round-trip through YAML dump+parse (expanded, no anchorMap)
   and XML round-trips. */

inFile  = "test_all_constructs.yaml"
doc1 = parser~parseFile(inFile)
am1  = parser~anchorMap

/* Verify anchors were captured during parsing */
call check (am1~items > 0), .true, "file anchor captured"

/* YAML round-trip (expanded — no anchorMap, which already works) */
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc1, doc2), .true, "file yaml rt equal"

/* Verify booleans survived */
call check doc2["bool_true"]~isA(.YamlBoolean), .true, "file bool true isA"
call check doc2["bool_false"]~isA(.YamlBoolean), .true, "file bool false isA"
call check doc2["bool_yes"]~isA(.YamlBoolean), .true, "file bool yes isA"
call check doc2["bool_no"]~isA(.YamlBoolean), .true, "file bool no isA"
call check doc2["bool_on"]~isA(.YamlBoolean), .true, "file bool on isA"
call check doc2["bool_off"]~isA(.YamlBoolean), .true, "file bool off isA"

/* Verify numeric types */
call check doc2["hex_value"], 255, "file hex value"
call check doc2["octal_value"], 63, "file octal value"
call check doc2["infinity"], ".inf", "file infinity"
call check doc2["neg_infinity"], "-.inf", "file neg infinity"
call check doc2["not_a_number"], ".nan", "file nan"
call check doc2["scientific"], 6.022E23, "file scientific"

/* Verify flow collections */
call check doc2["flow_sequence"]~items, 3, "file flow seq"
call check doc2["flow_sequence"][2], "two", "file flow seq 2"
call check doc2["flow_mapping"]["x"], 10, "file flow map x"
call check doc2["nested_flow"][1][2], 2, "file nested flow"
call check doc2["mixed_flow"][1]["a"], 1, "file mixed flow"

/* Verify block scalars */
call check (doc2["folded"]~pos("folded block scalar") > 0), 1, "file folded"
call check (doc2["strip_chomp"]~right(1) \== "0A"x), 1, "file strip chomp"
call check (doc2["multiline"]~pos("multi-line") > 0), 1, "file multiline"

/* Verify quoted strings */
call check doc2["single_quoted"], "It's a test", "file single quoted"
call check (doc2["double_quoted"]~pos("09"x) > 0), 1, "file double quoted"

/* Verify edge cases */
call check doc2["deeply_nested_list"][1][1][1], "deep", "file deeply nested"
call check doc2["numeric_keys"]["0"], "zero", "file numeric key 0"
call check doc2["numeric_keys"]["3.14"], "pi", "file numeric key pi"
call check doc2["leading_space_value"], "  two spaces", "file leading spaces"
call check doc2["trailing_space_value"], "value  ", "file trailing spaces"
call check doc2["colon_in_value"], "http://example.com", "file colon in value"
call check doc2["hash_in_value"], "color #FF0000", "file hash in value"
call check doc2["single_item_map"]["only_key"], "only_value", "file single item map"
call check doc2["single_item_list"][1], "only", "file single item list"

/* Verify null variants */
call check doc2["null_value"] == .nil, .true, "file null value"
call check doc2["null_tilde"] == .nil, .true, "file null tilde"

/* Verify reserved-word string keys */
call check doc2["true"], "true", "file str true key"
call check doc2["false"], "false", "file str false key"
call check doc2["null"], "null", "file str null key"
call check doc2["NULL"], "NULL", "file str NULL key"

/* Verify merge key expanded correctly */
call check doc2["development"]["adapter"], "postgres", "file merge adapter"
call check doc2["development"]["host"], "localhost", "file merge host"
call check doc2["development"]["database"], "myapp_dev", "file merge database"

/* Verify keep chomp (|+) */
call check (doc2["keep_chomp"]~right(1) == "0A"x), 1, "file keep chomp"

/* Verify folded strip (>-) and folded keep (>+) */
call check (doc2["folded_strip"]~right(1) \== "0A"x), 1, "file folded strip"
call check (doc2["folded_strip"]~pos("folded without trailing newline") > 0), 1, "file folded strip content"
call check (doc2["folded_keep"]~right(1) == "0A"x), 1, "file folded keep"
call check (doc2["folded_keep"]~pos("folded with trailing newlines kept") > 0), 1, "file folded keep content"

/* Verify Unicode escapes (unescaped to UTF-8 by default) */
call check doc2["unicode_escape_2byte"]~c2x, "C3A9", "file unicode 2byte"
call check doc2["unicode_escape_3byte"]~length, 3, "file unicode 3byte len"
call check doc2["unicode_escape_8digit"]~length, 4, "file unicode 8digit len"

/* Verify tags — values resolved, tags stripped */
call check doc2["tagged_map"]["x"], 1, "file tagged map x"
call check doc2["tagged_map"]["y"], 2, "file tagged map y"
call check doc2["tagged_flow"][1], "42", "file tagged flow str"
call check doc2["tagged_flow"][2], 7, "file tagged flow int"
call check doc2["custom_tag"], "value123", "file custom tag"
call check doc2["verbatim_tag"], "hello", "file verbatim tag"

/* Verify complex mapping keys */
call check doc2["scalar_key"], "scalar_complex_value", "file scalar complex key"
complexFound = 0
sup = doc2~supplier
Do While sup~available
  If sup~index~isA(.array) Then Do
    If sup~index~items = 2, sup~index[1] = "a", sup~index[2] = "b" Then
      complexFound += 1
  End
  If sup~index~isA(.table) Then Do
    If sup~index["x"] = 1 Then
      complexFound += 1
  End
  sup~next
End
call check complexFound, 2, "file complex keys found"

/* XML round-trip via XSD (expanded, no anchorMap) */
xml1 = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml1)
call check YAML.deepEqual(doc1, doc3), .true, "file xml xsd rt equal"

/* XML round-trip via DTD (expanded, no anchorMap) */
xml2 = .Yaml~yamlToXml(doc1, "dtd")
doc4 = parser~parseXml(xml2)
call check YAML.deepEqual(doc1, doc4), .true, "file xml dtd rt equal"

/* Full chain: YAML file → parse → XML (XSD) → parse → YAML dump → parse */
doc5 = parser~parseString(.Yaml~toYaml(doc3))
call check YAML.deepEqual(doc1, doc5), .true, "file chain rt equal"
say

/*========================================================================*/
say "--- 27. Multi-document with anchorMap ---"

/* anchorMap is per-parse, so multi-doc preserves anchors within each parse */
yamlMulti = "---"                     || "0A"x || -
            "name: first"             || "0A"x || -
            "---"                     || "0A"x || -
            "name: second"

docs = parser~parseAll(yamlMulti)
call check docs~items, 2, "multi doc count"
call check docs[1]["name"], "first", "multi doc 1"
call check docs[2]["name"], "second", "multi doc 2"

/* toYamlAll round-trip */
multiOut = .Yaml~toYamlAll(docs)
call check (multiOut~countStr("---") >= 2), .true, "multi has ---"
docs2 = parser~parseAll(multiOut)
call check docs2~items, 2, "multi rt count"
call check docs2[1]["name"], "first", "multi rt doc 1"
call check docs2[2]["name"], "second", "multi rt doc 2"
call check YAML.deepEqual(docs[1], docs2[1]), .true, "multi rt equal 1"
call check YAML.deepEqual(docs[2], docs2[2]), .true, "multi rt equal 2"
say

/*========================================================================*/
say "--- 28. Escape character round-trips ---"

/* Test all YAML double-quoted escape sequences that escapeStr handles.
   For each escape: build a YAML string with the escape, parse it,
   verify the in-memory value, dump it, re-parse, and compare.
   
   Parser escMap supports: \0 \a \b \t \n \v \f \r \e \" \\ \/ \<space>
   Emitter escapeStr supports: \0 \a \b \t \n \v \f \r \e \" \\
   
   Note: \n (newline) in a non-multiline context is tested separately
   because the emitter converts strings with embedded newlines to block
   scalars. */

/* --- Individual escape characters (no newline) --- */

/* \0 NUL */
yaml = 'esc_nul: "before\0after"'
doc = parser~parseString(yaml)
call check doc["esc_nul"], "before" || "00"x || "after", 'esc \0 parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_nul"], doc["esc_nul"], 'esc \0 roundtrip'

/* \a BEL */
yaml = 'esc_bel: "before\aafter"'
doc = parser~parseString(yaml)
call check doc["esc_bel"], "before" || "07"x || "after", 'esc \a parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_bel"], doc["esc_bel"], 'esc \a roundtrip'

/* \b BS */
yaml = 'esc_bs: "before\bafter"'
doc = parser~parseString(yaml)
call check doc["esc_bs"], "before" || "08"x || "after", 'esc \b parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_bs"], doc["esc_bs"], 'esc \b roundtrip'

/* \t TAB */
yaml = 'esc_tab: "before\tafter"'
doc = parser~parseString(yaml)
call check doc["esc_tab"], "before" || "09"x || "after", 'esc \t parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_tab"], doc["esc_tab"], 'esc \t roundtrip'

/* \v VT */
yaml = 'esc_vt: "before\vafter"'
doc = parser~parseString(yaml)
call check doc["esc_vt"], "before" || "0B"x || "after", 'esc \v parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_vt"], doc["esc_vt"], 'esc \v roundtrip'

/* \f FF */
yaml = 'esc_ff: "before\fafter"'
doc = parser~parseString(yaml)
call check doc["esc_ff"], "before" || "0C"x || "after", 'esc \f parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_ff"], doc["esc_ff"], 'esc \f roundtrip'

/* \r CR */
yaml = 'esc_cr: "before\rafter"'
doc = parser~parseString(yaml)
call check doc["esc_cr"], "before" || "0D"x || "after", 'esc \r parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_cr"], doc["esc_cr"], 'esc \r roundtrip'

/* \e ESC */
yaml = 'esc_esc: "before\eafter"'
doc = parser~parseString(yaml)
call check doc["esc_esc"], "before" || "1B"x || "after", 'esc \e parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_esc"], doc["esc_esc"], 'esc \e roundtrip'

/* \" double quote */
yaml = 'esc_dq: "before\"after"'
doc = parser~parseString(yaml)
call check doc["esc_dq"], 'before"after', 'esc \" parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_dq"], doc["esc_dq"], 'esc \" roundtrip'

/* \\ backslash */
yaml = 'esc_bs2: "before\\\\after"'
doc = parser~parseString(yaml)
call check doc["esc_bs2"], "before\\after", 'esc \\ parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_bs2"], doc["esc_bs2"], 'esc \\ roundtrip'

/* \/ slash (parsed but emitted literally) */
yaml = 'esc_slash: "before\/after"'
doc = parser~parseString(yaml)
call check doc["esc_slash"], "before/after", 'esc \/ parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_slash"], doc["esc_slash"], 'esc \/ roundtrip'

/* \<space> (parsed but emitted literally) */
yaml = 'esc_space: "before\ after"'
doc = parser~parseString(yaml)
call check doc["esc_space"], "before after", 'esc \  parse'
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["esc_space"], doc["esc_space"], 'esc \  roundtrip'

/* --- Combined: multiple escapes in one string --- */
yaml = 'combined: "tab\there\abell\0nul"'
doc = parser~parseString(yaml)
expected = "tab" || "09"x || "here" || "07"x || "bell" || "00"x || "nul"
call check doc["combined"], expected, "esc combined parse"
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["combined"], doc["combined"], "esc combined roundtrip"

/* --- XML round-trip for escaped strings --- */
yaml = 'esc_tab: "a\tb"' || "0A"x || -
       'esc_bel: "a\ab"' || "0A"x || -
       'esc_bs: "a\bb"'
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["esc_tab"], doc["esc_tab"], "esc xml rt tab"
call check doc3["esc_bel"], doc["esc_bel"], "esc xml rt bel"
call check doc3["esc_bs"], doc["esc_bs"], "esc xml rt bs"

/* --- DTD XML round-trip for escaped strings --- */
xml2 = .Yaml~yamlToXml(doc, "dtd")
doc4 = parser~parseXml(xml2)
call check doc4["esc_tab"], doc["esc_tab"], "esc dtd rt tab"
call check doc4["esc_bel"], doc["esc_bel"], "esc dtd rt bel"
call check doc4["esc_bs"], doc["esc_bs"], "esc dtd rt bs"
say

/*========================================================================*/
say "--- 29. Chomp preservation & embedded-newline round-trip ---"

/* 29a. Clip chomp (|) — string ends with exactly one newline */
yaml = "clip: |" || "0A"x || "  line1" || "0A"x || "  line2"
doc = parser~parseString(yaml)
clipVal = doc["clip"]
call check clipVal, "line1" || "0A"x || "line2" || "0A"x, "clip parse"
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("|") > 0), 1, "clip emits |"
call check (yamlOut~pos("|+") = 0), 1, "clip not |+"
doc2 = parser~parseString(yamlOut)
call check doc2["clip"], clipVal, "clip roundtrip"

/* 29b. Strip chomp (|-) — string ends with no trailing newline.
   Now emitted as double-quoted with \n escapes for exact round-trip. */
yaml = "strip: |-" || "0A"x || "  line1" || "0A"x || "  line2"
doc = parser~parseString(yaml)
stripVal = doc["strip"]
call check stripVal, "line1" || "0A"x || "line2", "strip parse"
call check (stripVal~right(1) \== "0A"x), 1, "strip no trailing NL"
yamlOut = .Yaml~toYaml(doc)
/* Should be emitted as double-quoted (not block scalar) */
call check (yamlOut~pos('"') > 0), 1, 'strip emits dquoted'
doc2 = parser~parseString(yamlOut)
call check doc2["strip"], stripVal, "strip roundtrip"

/* 29c. Keep chomp (|+) — string ends with multiple trailing newlines */
yaml = "keep: |+" || "0A"x || "  line1" || "0A"x || "  line2" || "0A"x || "0A"x || "0A"x
doc = parser~parseString(yaml)
keepVal = doc["keep"]
/* |+ preserves trailing newlines: content + \n + \n + \n */
call check (keepVal~right(2) == "0A0A"x), 1, "keep trailing NLs"
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("|+") > 0), 1, "keep emits |+"
doc2 = parser~parseString(yamlOut)
call check doc2["keep"], keepVal, "keep roundtrip"

/* 29d. Embedded newline in double-quoted string (no trailing NL) */
yaml = 'embedded: "line1\nline2"'
doc = parser~parseString(yaml)
embVal = doc["embedded"]
call check embVal, "line1" || "0A"x || "line2", "embedded parse"
call check (embVal~right(1) \== "0A"x), 1, "embedded no trailing NL"
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos('"') > 0), 1, "embedded emits dquoted"
doc2 = parser~parseString(yamlOut)
call check doc2["embedded"], embVal, "embedded roundtrip"

/* 29e. Embedded newline with trailing NL — should use | (clip) */
yaml = 'mixed: "line1\nline2\n"'
doc = parser~parseString(yaml)
mixVal = doc["mixed"]
call check mixVal, "line1" || "0A"x || "line2" || "0A"x, "mixed parse"
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("|") > 0), 1, "mixed emits |"
call check (yamlOut~pos("|+") = 0), 1, "mixed not |+"
doc2 = parser~parseString(yamlOut)
call check doc2["mixed"], mixVal, "mixed roundtrip"

/* 29f. Full round-trip: parse → dump → parse for all chomp variants */
yaml = "clip: |"  || "0A"x || "  A" || "0A"x || "  B" || "0A"x || -
       "strip: |-" || "0A"x || "  C" || "0A"x || "  D" || "0A"x || -
       "keep: |+" || "0A"x || "  E" || "0A"x || "  F" || "0A"x || "0A"x || "0A"x || -
       'embed: "G\nH"'
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "all chomp deepEqual"

/* 29g. XML round-trips for chomp variants */
xml = .Yaml~yamlToXml(doc, "xsd")
doc3 = parser~parseXml(xml)
call check YAML.deepEqual(doc, doc3), .true, "chomp xml xsd rt"

xml2 = .Yaml~yamlToXml(doc, "dtd")
doc4 = parser~parseXml(xml2)
call check YAML.deepEqual(doc, doc4), .true, "chomp xml dtd rt"

/* 29h. Folded (>) round-trip — folded content ends with one NL */
yaml = "folded: >" || "0A"x || "  this is" || "0A"x || "  one line"
doc = parser~parseString(yaml)
foldVal = doc["folded"]
call check (foldVal~right(1) == "0A"x), 1, "folded trailing NL"
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["folded"], foldVal, "folded roundtrip"
say

/*========================================================================*/
say "--- 30. Flow style emission ---"

/* 30a. Simple sequence emitted as flow when nested */
yaml = "colors:"         || "0A"x || -
       "  - red"         || "0A"x || -
       "  - green"       || "0A"x || -
       "  - blue"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("[") > 0), 1, "flow seq emitted"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "flow seq roundtrip"

/* 30b. Sequence with 4 items (max for flow) */
yaml = "nums:"       || "0A"x || -
       "  - 1"       || "0A"x || -
       "  - 2"       || "0A"x || -
       "  - 3"       || "0A"x || -
       "  - 4"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("[") > 0), 1, "flow seq 4 items"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "flow seq 4 roundtrip"

/* 30c. Sequence with 5 items stays block */
yaml = "nums:"       || "0A"x || -
       "  - 1"       || "0A"x || -
       "  - 2"       || "0A"x || -
       "  - 3"       || "0A"x || -
       "  - 4"       || "0A"x || -
       "  - 5"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
/* The list itself should be block (has "- ") */
call check (yamlOut~pos("- 1") > 0), 1, "block seq 5 items"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "block seq 5 roundtrip"

/* 30d. Sequence with nested collection stays block */
yaml = "data:"              || "0A"x || -
       "  - name: Alice"    || "0A"x || -
       "  - name: Bob"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
/* Items are mappings, so sequence stays block (no [...]) */
call check (yamlOut~pos("[") = 0), 1, "block seq nested maps"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "block seq nested roundtrip"

/* 30e. Flow sequence with null item stays block */
yaml = "vals:"     || "0A"x || -
       "  - hello" || "0A"x || -
       "  - null"  || "0A"x || -
       "  - world"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "seq with null roundtrip"

/* 30f. Empty collections stay as {} and [] */
yaml = "em: {}" || "0A"x || "el: []"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("{}") > 0), 1, "empty map stays {}"
call check (yamlOut~pos("[]") > 0), 1, "empty list stays []"

/* 30g. Flow sequence survives XML round-trip */
yaml = "colors:"  || "0A"x || -
       "  - red"  || "0A"x || -
       "  - blue"
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc3 = parser~parseXml(xml)
call check YAML.deepEqual(doc, doc3), .true, "flow seq xml xsd rt"
xml2 = .Yaml~yamlToXml(doc, "dtd")
doc4 = parser~parseXml(xml2)
call check YAML.deepEqual(doc, doc4), .true, "flow seq xml dtd rt"

/* 30h. Top-level sequence stays block (not flow) */
yaml = "- alpha" || "0A"x || "- beta"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("[") = 0), 1, "top seq stays block"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "top seq roundtrip"
say

/*========================================================================*/
say "--- 31. Anchor ordering in toYaml ---"

/* Anchor definition should always appear before aliases regardless of
   Table hash iteration order. */
yamlAnch = "base: &shared"        || "0A"x || -
           "  x: 10"              || "0A"x || -
           "  y: 20"              || "0A"x || -
           "ref1: *shared"        || "0A"x || -
           "ref2: *shared"
doc = parser~parseString(yamlAnch)
am  = parser~anchorMap

/* Dump with anchorMap — anchor must come before aliases */
yamlOut = .Yaml~toYaml(doc, 2, am)
anchorPos = yamlOut~pos("&shared")
aliasPos  = yamlOut~pos("*shared")
call check (anchorPos > 0), 1, "anchor present"
call check (aliasPos > 0), 1, "alias present"
call check (anchorPos < aliasPos), 1, "anchor before alias"

/* Re-parse succeeds (no unknown alias error) */
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "anchor order rt"

/* Multiple anchors: both must appear before their aliases */
yamlMulti = "a1: &first"          || "0A"x || -
            "  p: 1"              || "0A"x || -
            "a2: &second"         || "0A"x || -
            "  q: 2"              || "0A"x || -
            "r1: *first"          || "0A"x || -
            "r2: *second"
doc = parser~parseString(yamlMulti)
am  = parser~anchorMap
yamlOut = .Yaml~toYaml(doc, 2, am)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "multi anchor order rt"

/* Anchor ordering in XML round-trip (should still work) */
xml = .Yaml~yamlToXml(doc, "xsd", am)
doc3 = parser~parseXml(xml)
call check YAML.deepEqual(doc, doc3), .true, "anchor order xml rt"
say

/*========================================================================*/
say "--- 32. Merge key reconstruction in toYaml ---"

/* 32.1 Simple merge: <<: *alias appears in dump */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
call check (yamlOut~pos("<<: *defs") > 0), 1, "merge key reconstructed"

/* 32.2 Own keys not duplicated — merged keys excluded from normal emission */
call check (yamlOut~countStr("adapter:") = 1), 1, "adapter only in source"
call check (yamlOut~countStr("host:") = 1), 1, "host only in source"
call check (yamlOut~pos("database:") > 0), 1, "own key present"

/* 32.3 Override: key overridden by target emitted as own key */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  adapter: mysql"     || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
/* adapter appears twice: once in defaults, once as own key in development */
call check (yamlOut~countStr("adapter:") = 2), 1, "overridden key emitted as own"
/* host is not overridden, so only in defaults */
call check (yamlOut~countStr("host:") = 1), 1, "non-overridden stays merged"
call check (yamlOut~pos("<<: *defs") > 0), 1, "merge present with override"

/* 32.4 Multiple merges: <<: [*a, *b] */
yaml = "base1: &b1"          || "0A"x || -
       "  x: 1"              || "0A"x || -
       "base2: &b2"          || "0A"x || -
       "  y: 2"              || "0A"x || -
       "combined:"            || "0A"x || -
       "  <<: [*b1, *b2]"    || "0A"x || -
       "  z: 3"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
call check (yamlOut~pos("<<: [*b1, *b2]") > 0), 1, "multi merge reconstructed"
call check (yamlOut~countStr("x:") = 1), 1, "multi merge x only in source"
call check (yamlOut~countStr("y:") = 1), 1, "multi merge y only in source"
call check (yamlOut~pos("z:") > 0), 1, "multi merge own key z present"

/* 32.5 Round-trip: parse → dump → parse produces same data */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "merge simple roundtrip"

/* 32.6 Round-trip with override */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  adapter: mysql"     || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "merge override roundtrip"

/* 32.7 Round-trip with multiple merges */
yaml = "base1: &b1"          || "0A"x || -
       "  x: 1"              || "0A"x || -
       "base2: &b2"          || "0A"x || -
       "  y: 2"              || "0A"x || -
       "combined:"            || "0A"x || -
       "  <<: [*b1, *b2]"    || "0A"x || -
       "  z: 3"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
yamlOut = .Yaml~toYaml(doc, 2, am, msm)
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "multi merge roundtrip"

/* 32.8 XML round-trip with merge reconstruction (XSD) */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
xml = .Yaml~yamlToXml(doc, "xsd", am)
doc3 = parser~parseXml(xml)
call check YAML.deepEqual(doc, doc3), .true, "merge xml xsd roundtrip"

/* 32.9 XML round-trip with merge reconstruction (DTD) */
xml = .Yaml~yamlToXml(doc, "dtd", am)
doc4 = parser~parseXml(xml)
call check YAML.deepEqual(doc, doc4), .true, "merge xml dtd roundtrip"

/* 32.10 Dump without mergeSourceMap still works (backward compat) */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
yamlOut = .Yaml~toYaml(doc, 2, am)
/* Without mergeSourceMap, merged keys are emitted individually (old behavior) */
call check (yamlOut~pos("<<:") = 0), 1, "no merge without mergeSourceMap"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "roundtrip without mergeSourceMap"

/* 32.11 P2: XML <merge> element emitted with mergeSourceMap (XSD) */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
call check (xml~pos('<merge anchor="defs"/>') > 0), 1, "merge element in XSD XML"

/* 32.12 P2: XML <merge> element emitted with mergeSourceMap (DTD) */
xml = .Yaml~yamlToXml(doc, "dtd", am, , msm)
call check (xml~pos('<merge anchor="defs"/>') > 0), 1, "merge element in DTD XML"

/* 32.13 P2: merged keys excluded from XML entries */
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
/* The "development" mapping should have only 1 <entry> (database), not 3 */
/* Count entries inside the development mapping (after the merge element) */
mergePos = xml~pos('<merge anchor="defs"/>')
xmlAfterMerge = xml~substr(mergePos)
closeMappingPos = xmlAfterMerge~pos('</mapping>')
xmlMergeSection = xmlAfterMerge~left(closeMappingPos)
call check (xmlMergeSection~countStr("<entry>") = 1), 1, "only own entries in merge XML"

/* 32.14 P2: XML without mergeSourceMap has no <merge> (backward compat) */
xml = .Yaml~yamlToXml(doc, "xsd", am)
call check (xml~pos("<merge") = 0), 1, "no merge element without mergeSourceMap"

/* 32.15 P2: XML round-trip with <merge> preserves data (XSD) */
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
parser2 = .Yaml~new
doc3 = parser2~parseXml(xml)
call check YAML.deepEqual(doc, doc3), .true, "merge XML xsd data roundtrip"

/* 32.16 P2: XML round-trip with <merge> preserves data (DTD) */
xml = .Yaml~yamlToXml(doc, "dtd", am, , msm)
parser2 = .Yaml~new
doc4 = parser2~parseXml(xml)
call check YAML.deepEqual(doc, doc4), .true, "merge XML dtd data roundtrip"

/* 32.17 P2: XML round-trip reconstructs mergeSourceMap */
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
parser2 = .Yaml~new
doc3 = parser2~parseXml(xml)
am3  = parser2~anchorMap
msm3 = parser2~mergeSourceMap
yamlOut = .Yaml~toYaml(doc3, 2, am3, msm3)
call check (yamlOut~pos("<<: *defs") > 0), 1, "merge XML roundtrip reconstructs merge key"

/* 32.18 P2: XML round-trip with merge override */
yaml = "defaults: &defs"     || "0A"x || -
       "  adapter: postgres"  || "0A"x || -
       "  host: localhost"    || "0A"x || -
       "development:"         || "0A"x || -
       "  <<: *defs"          || "0A"x || -
       "  adapter: mysql"     || "0A"x || -
       "  database: myapp_dev"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
parser2 = .Yaml~new
doc5 = parser2~parseXml(xml)
call check YAML.deepEqual(doc, doc5), .true, "merge XML override data roundtrip"
am5  = parser2~anchorMap
msm5 = parser2~mergeSourceMap
yamlOut = .Yaml~toYaml(doc5, 2, am5, msm5)
call check (yamlOut~pos("<<: *defs") > 0), 1, "merge XML override reconstructs merge"
call check (yamlOut~countStr("adapter:") = 2), 1, "merge XML override own key emitted"

/* 32.19 P2: XML round-trip with multiple merges */
yaml = "base1: &b1"          || "0A"x || -
       "  x: 1"              || "0A"x || -
       "base2: &b2"          || "0A"x || -
       "  y: 2"              || "0A"x || -
       "combined:"            || "0A"x || -
       "  <<: [*b1, *b2]"    || "0A"x || -
       "  z: 3"
doc = parser~parseString(yaml)
am  = parser~anchorMap
msm = parser~mergeSourceMap
xml = .Yaml~yamlToXml(doc, "xsd", am, , msm)
call check (xml~countStr("<merge") = 2), 1, "two merge elements in XML"
parser2 = .Yaml~new
doc6 = parser2~parseXml(xml)
call check YAML.deepEqual(doc, doc6), .true, "multi merge XML data roundtrip"
am6  = parser2~anchorMap
msm6 = parser2~mergeSourceMap
yamlOut = .Yaml~toYaml(doc6, 2, am6, msm6)
call check (yamlOut~pos("<<: [*b1, *b2]") > 0), 1, "multi merge XML roundtrip reconstructs"

/* 32.20 P2: XML round-trip with multiple merges (DTD) */
xml = .Yaml~yamlToXml(doc, "dtd", am, , msm)
parser2 = .Yaml~new
doc7 = parser2~parseXml(xml)
call check YAML.deepEqual(doc, doc7), .true, "multi merge XML dtd data roundtrip"
say
/*========================================================================*/
say "--- 33. Single-quoted strings in emitter ---"
/*========================================================================*/

parser = .Yaml~new

/* 33.1 Reserved words should now be single-quoted */
doc = .table~new
doc["a"] = "true"
doc["b"] = "false"
doc["c"] = "null"
doc["d"] = "yes"
doc["e"] = "no"
doc["f"] = "on"
doc["g"] = "off"
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("'true'") > 0), 1, "single-quote true"
call check (yamlOut~pos("'false'") > 0), 1, "single-quote false"
call check (yamlOut~pos("'null'") > 0), 1, "single-quote null"
call check (yamlOut~pos("'yes'") > 0), 1, "single-quote yes"

/* 33.2 Strings needing quoting but no escapes use single quotes */
doc2 = .table~new
doc2["k"] = "key: colon"
doc2["h"] = "color #FF0000"
yamlOut2 = .Yaml~toYaml(doc2)
call check (yamlOut2~pos("'key: colon'") > 0), 1, "single-quote colon in value"
call check (yamlOut2~pos("'color #FF0000'") > 0), 1, "single-quote hash in value"

/* 33.3 Strings with backslash still use double quotes */
doc3 = .table~new
doc3["p"] = "C:\path\to"
yamlOut3 = .Yaml~toYaml(doc3)
call check (yamlOut3~pos('"C:\\path\\to"') > 0), 1, "double-quote backslash"

/* 33.4 Strings with control chars still use double quotes */
doc4 = .table~new
doc4["t"] = "tab" || "09"x || "here"
yamlOut4 = .Yaml~toYaml(doc4)
call check (yamlOut4~pos('"tab\there"') > 0), 1, "double-quote control char"

/* 33.5 Round-trip: single-quoted values parse back correctly */
doc5 = .table~new
doc5["w1"] = "true"
doc5["w2"] = "null"
doc5["w3"] = "key: colon"
doc5["w4"] = "has 'apostrophe' here"
yamlOut5 = .Yaml~toYaml(doc5)
doc5b = parser~parseString(yamlOut5)
call check doc5b["w1"], "true", "single-quote roundtrip true"
call check doc5b["w2"], "null", "single-quote roundtrip null"
call check doc5b["w3"], "key: colon", "single-quote roundtrip colon"
call check doc5b["w4"], "has 'apostrophe' here", "single-quote roundtrip apostrophe"

/* 33.6 Keys: reserved words as keys use single quotes */
doc6 = .table~new
doc6["true"] = 1
doc6["null"] = 2
yamlOut6 = .Yaml~toYaml(doc6)
call check (yamlOut6~pos("'true':") > 0), 1, "single-quote key true"
call check (yamlOut6~pos("'null':") > 0), 1, "single-quote key null"
say

/*========================================================================*/
say "--- 34. Flow mappings in emitter ---"
/*========================================================================*/

parser = .Yaml~new

/* 34.1 Small nested mapping emitted in flow style */
doc = .table~new
inner = .table~new
inner["x"] = 10; inner["y"] = 20
doc["point"] = inner
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("{") > 0), 1, "flow map emitted for small nested"

/* 34.2 Flow map round-trip */
doc2 = parser~parseString(yamlOut)
call check doc2["point"]["x"], 10, "flow map roundtrip x"
call check doc2["point"]["y"], 20, "flow map roundtrip y"

/* 34.3 Large mappings stay block */
doc3 = .table~new
big = .table~new
Do i = 1 To 6
  big["key" || i] = "value" || i
End
doc3["big"] = big
yamlOut3 = .Yaml~toYaml(doc3)
/* Flow maps only for <=4 items, so big should be block */
call check (yamlOut3~pos("{key") = 0), 1, "big map stays block"

/* 34.4 Top-level mapping stays block even if small */
doc4 = .table~new
doc4["a"] = 1; doc4["b"] = 2
yamlOut4 = .Yaml~toYaml(doc4)
call check (yamlOut4~left(1) \== "{"), 1, "top-level map stays block"

/* 34.5 Mapping with nested collections stays block */
doc5 = .table~new
inner2 = .table~new
inner2["a"] = .array~of(1, 2)
doc5["nested"] = inner2
yamlOut5 = .Yaml~toYaml(doc5)
/* inner2 has a nested array so canFlowStyle returns false */
call check (yamlOut5~pos("{a:") = 0), 1, "nested collection stays block"

/* 34.6 Sequence of small maps — flow emitted */
doc6 = .table~new
arr = .array~new
m1 = .table~new; m1["name"] = "Alice"; m1["age"] = 30
m2 = .table~new; m2["name"] = "Bob"; m2["age"] = 25
arr~append(m1); arr~append(m2)
doc6["people"] = arr
yamlOut6 = .Yaml~toYaml(doc6)
doc6b = parser~parseString(yamlOut6)
call check doc6b["people"][1]["name"], "Alice", "flow map in seq roundtrip name"
call check doc6b["people"][2]["age"], 25, "flow map in seq roundtrip age"
say

/*========================================================================*/
say "--- 35. Directives (%YAML, %TAG) ---"
/*========================================================================*/

parser = .Yaml~new

/* 35.1 %YAML directive is parsed */
yaml = "%YAML 1.2" || "0A"x || -
       "---"       || "0A"x || -
       "key: value"
doc = parser~parseString(yaml)
call check doc["key"], "value", "YAML directive parsed"

/* 35.2 %TAG directive is parsed */
yaml = "%TAG ! tag:example.com,2000:" || "0A"x || -
       "---"                          || "0A"x || -
       "name: test"
doc = parser~parseString(yaml)
call check doc["name"], "test", "TAG directive parsed"

/* 35.3 Multiple directives */
yaml = "%YAML 1.2"                     || "0A"x || -
       "%TAG !! tag:yaml.org,2002:"    || "0A"x || -
       "%TAG !e! tag:example.com,2000:" || "0A"x || -
       "---"                            || "0A"x || -
       "color: red"
doc = parser~parseString(yaml)
call check doc["color"], "red", "multiple directives parsed"

/* 35.4 Directives with multi-document */
yaml = "%YAML 1.2"   || "0A"x || -
       "---"          || "0A"x || -
       "first: 1"     || "0A"x || -
       "..."          || "0A"x || -
       "%YAML 1.2"    || "0A"x || -
       "---"          || "0A"x || -
       "second: 2"
docs = parser~parseAll(yaml)
call check docs~items, 2, "directives multi-doc count"
call check docs[1]["first"], 1, "directives multi-doc first"
call check docs[2]["second"], 2, "directives multi-doc second"

/* 35.5 Without directivesMap, directives do not appear in output */
yaml = "%YAML 1.2" || "0A"x || "---" || "0A"x || "a: 1"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
call check (yamlOut~pos("%YAML") = 0), 1, "no directives without map"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "directives roundtrip no map"

/* 35.6 directivesMap captures %YAML version */
parser6 = .Yaml~new
yaml = "%YAML 1.2" || "0A"x || "---" || "0A"x || "x: 1"
doc = parser6~parseString(yaml)
dirMap = parser6~directivesMap
dir = dirMap~at(doc)
call check (dir \== .nil), 1, "directivesMap has entry"
call check dir["yamlVersion"], "1.2", "yamlVersion captured"

/* 35.7 directivesMap captures %TAG handles */
parser7 = .Yaml~new
yaml = "%TAG !! tag:yaml.org,2002:"    || "0A"x || -
       "%TAG !e! tag:example.com,2000:" || "0A"x || -
       "---"                            || "0A"x || -
       "a: 1"
doc = parser7~parseString(yaml)
dir = parser7~directivesMap~at(doc)
call check (dir \== .nil), 1, "TAG directivesMap has entry"
th = dir["tagHandles"]
call check (th \== .nil), 1, "tagHandles captured"
call check th["!!"], "tag:yaml.org,2002:", "!! handle captured"
call check th["!e!"], "tag:example.com,2000:", "!e! handle captured"

/* 35.8 Round-trip with directivesMap preserves directives */
parser8 = .Yaml~new
yaml = "%YAML 1.2"                     || "0A"x || -
       "%TAG !! tag:yaml.org,2002:"    || "0A"x || -
       "---"                            || "0A"x || -
       "color: red"
doc = parser8~parseString(yaml)
dirMap = parser8~directivesMap
yamlOut = .Yaml~toYaml(doc, 2, .nil, .nil, dirMap)
call check (yamlOut~pos("%YAML 1.2") > 0), 1, "YAML directive in output"
call check (yamlOut~pos("%TAG !!") > 0), 1, "TAG directive in output"
call check (yamlOut~pos("---") > 0), 1, "doc-start marker in output"
doc2 = parser~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "directives full roundtrip"

/* 35.9 Multi-doc round-trip with directives */
parser9 = .Yaml~new
yaml = "%YAML 1.2"   || "0A"x || -
       "---"          || "0A"x || -
       "first: 1"     || "0A"x || -
       "..."          || "0A"x || -
       "%YAML 1.1"    || "0A"x || -
       "---"          || "0A"x || -
       "second: 2"
docs = parser9~parseAll(yaml)
dirMap = parser9~directivesMap
dir1 = dirMap~at(docs[1])
dir2 = dirMap~at(docs[2])
call check dir1["yamlVersion"], "1.2", "multi-doc dir1 version"
call check dir2["yamlVersion"], "1.1", "multi-doc dir2 version"

/* 35.10 No directives: directivesMap entry is absent */
parser10 = .Yaml~new
doc = parser10~parseString("a: 1")
dir = parser10~directivesMap~at(doc)
call check (dir == .nil), 1, "no directives no entry"

/* 35.11 XML round-trip with directives (XSD) */
parser11 = .Yaml~new
yaml = "%YAML 1.2"                     || "0A"x || -
       "%TAG !e! tag:example.com,2000:" || "0A"x || -
       "---"                            || "0A"x || -
       "name: test"
doc = parser11~parseString(yaml)
dirMap = parser11~directivesMap
xml = .Yaml~yamlToXml(doc, "xsd", .nil, dirMap)
call check (xml~pos('yaml-version="1.2"') > 0), 1, "XML has yaml-version"
call check (xml~pos('tag-directive') > 0), 1, "XML has tag-directive"
call check (xml~pos('handle="!e!"') > 0), 1, "XML has handle attr"
call check (xml~pos('prefix="tag:example.com,2000:"') > 0), 1, "XML has prefix attr"
/* Parse XML back */
parser11b = .Yaml~new
doc2 = parser11b~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML directive roundtrip data"
dir2 = parser11b~directivesMap~at(doc2)
call check (dir2 \== .nil), 1, "XML directive roundtrip has directives"
call check dir2["yamlVersion"], "1.2", "XML directive roundtrip version"
th2 = dir2["tagHandles"]
call check th2["!e!"], "tag:example.com,2000:", "XML directive roundtrip handle"

/* 35.12 XML round-trip with directives (DTD) */
parser12 = .Yaml~new
yaml = "%YAML 1.2"                     || "0A"x || -
       "%TAG !! tag:yaml.org,2002:"    || "0A"x || -
       "---"                            || "0A"x || -
       "val: 42"
doc = parser12~parseString(yaml)
dirMap = parser12~directivesMap
xml = .Yaml~yamlToXml(doc, "dtd", .nil, dirMap)
call check (xml~pos('yaml-version="1.2"') > 0), 1, "DTD XML has yaml-version"
call check (xml~pos('tag-directive') > 0), 1, "DTD XML has tag-directive"
parser12b = .Yaml~new
doc2 = parser12b~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "DTD XML directive roundtrip data"

/* 35.13 P9: tags stored as shorthand with preserveTags */
parser13 = .Yaml~new(.true, .true)
yaml = "%TAG !e! tag:example.com,2000:" || "0A"x || -
       "---"                            || "0A"x || -
       "name: !e!person Alice"
doc = parser13~parseString(yaml)
val = doc["name"]
call check val~isA(.YamlTagged), .true, "P9 tag is YamlTagged"
call check val~tag, "!e!person", "P9 !e! stored as shorthand"
call check val~value, "Alice", "P9 tag value preserved"

/* 35.14 P9: !! stored as shorthand */
parser14 = .Yaml~new(.true, .true)
yaml = "%TAG !! tag:yaml.org,2002:" || "0A"x || -
       "---"                        || "0A"x || -
       "count: !!int 42"
doc = parser14~parseString(yaml)
val = doc["count"]
call check val~isA(.YamlTagged), .true, "P9 !!int is YamlTagged"
call check val~tag, "!!int", "P9 !! stored as shorthand"

/* 35.15 P9: primary handle ! stored as shorthand */
parser15 = .Yaml~new(.true, .true)
yaml = "%TAG ! tag:custom.org/" || "0A"x || -
       "---"                    || "0A"x || -
       "item: !widget Sprocket"
doc = parser15~parseString(yaml)
val = doc["item"]
call check val~isA(.YamlTagged), .true, "P9 !widget is YamlTagged"
call check val~tag, "!widget", "P9 ! stored as shorthand"
call check val~value, "Sprocket", "P9 ! handle value"

/* 35.16 P9: no %TAG means no resolution */
parser16 = .Yaml~new(.true, .true)
yaml = "name: !custom Alice"
doc = parser16~parseString(yaml)
val = doc["name"]
call check val~tag, "!custom", "P9 no TAG no resolution"

/* 35.17 P9: lazy resolution via resolveTagHandle */
parser17 = .Yaml~new(.true, .true)
yaml = "%TAG !e! tag:example.com,2000:" || "0A"x || -
       "%TAG !! tag:yaml.org,2002:"    || "0A"x || -
       "---"                            || "0A"x || -
       "name: !e!person Alice"          || "0A"x || -
       "count: !!int 42"
doc = parser17~parseString(yaml)
dir = parser17~directivesMap~at(doc)
th = dir["tagHandles"]
/* Resolve !e!person lazily */
resolved = parser17~resolveTagHandle("!e!person", th)
call check resolved, "!<tag:example.com,2000:person>", "P9 lazy !e! resolved"
/* Resolve !!int lazily */
resolved = parser17~resolveTagHandle("!!int", th)
call check resolved, "!<tag:yaml.org,2002:int>", "P9 lazy !! resolved"
/* Resolve with no matching handle */
resolved = parser17~resolveTagHandle("!custom", th)
call check resolved, "!custom", "P9 lazy no match unchanged"
say

/*========================================================================*/
say "--- 36. Tags (!!str, !!int, !custom) ---"
/*========================================================================*/

parser = .Yaml~new

/* 36.1 !!str tag on a value */
yaml = "name: !!str Alice"
doc = parser~parseString(yaml)
call check doc["name"], "Alice", "tag !!str value"

/* 36.2 !!int tag on a value */
yaml = "port: !!int 8080"
doc = parser~parseString(yaml)
call check doc["port"], 8080, "tag !!int value"

/* 36.3 !!null tag */
yaml = "empty: !!null ~"
doc = parser~parseString(yaml)
call check (doc["empty"] == .nil), 1, "tag !!null value"

/* 36.4 !!bool tag */
yaml = "flag: !!bool true"
doc = parser~parseString(yaml)
call check doc["flag"], .YamlBoolean~true, "tag !!bool value"

/* 36.5 Tag on sequence item */
yaml = "items:" || "0A"x || -
       "  - !!str 42"  || "0A"x || -
       "  - !!int 42"
doc = parser~parseString(yaml)
call check doc["items"][1], "42", "tag !!str seq item"
call check doc["items"][2], 42, "tag !!int seq item"

/* 36.6 Tag on mapping key */
yaml = "!!str name: Alice"
doc = parser~parseString(yaml)
call check doc["name"], "Alice", "tag on key"

/* 36.7 Tag on block node */
yaml = "data: !!map" || "0A"x || -
       "  x: 1"      || "0A"x || -
       "  y: 2"
doc = parser~parseString(yaml)
call check doc["data"]["x"], 1, "tag !!map block node x"
call check doc["data"]["y"], 2, "tag !!map block node y"

/* 36.8 Tag on flow value */
yaml = "items: [!!str 42, !!int 7]"
doc = parser~parseString(yaml)
call check doc["items"][1], "42", "tag !!str flow value"
call check doc["items"][2], 7, "tag !!int flow value"

/* 36.9 Custom tag stripped */
yaml = "data: !custom value123"
doc = parser~parseString(yaml)
call check doc["data"], "value123", "custom tag stripped"

/* 36.10 Verbatim tag stripped */
yaml = 'data: !<tag:yaml.org,2002:str> hello'
doc = parser~parseString(yaml)
call check doc["data"], "hello", "verbatim tag stripped"

/* 36.11 Tag round-trip (tags are stripped — core schema only) */
yaml = "a: !!str test"   || "0A"x || -
       "b: !!int 42"     || "0A"x || -
       "c: !!bool false"
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["a"], "test", "tag roundtrip str"
call check doc2["b"], 42, "tag roundtrip int"
call check doc2["c"], .YamlBoolean~false, "tag roundtrip bool"
say

/*========================================================================*/
say "Test group 37: XSLT round-trip (xsltproc / runXSLT.rxj)"
/*========================================================================*/

/* Parse the reference YAML file */
parser = .Yaml~new
original = parser~parseFile("test_all_constructs.yaml")
am = parser~anchorMap

/* Generate XSD and DTD XML files */
xsdFile = "test33_xsd.xml"
dtdFile = "test33_dtd.xml"
.Yaml~yamlToXmlFile(original, xsdFile, "xsd", am)
.Yaml~yamlToXmlFile(original, dtdFile, "dtd", am)

/* Try xsltproc first, then runXSLT.rxj */
xsltAvailable = .false
xslFile = "xmlToYaml.xsl"

/* Attempt xsltproc */
Signal On Syntax Name TryRunXSLT
outXsd = .array~new
errXsd = .array~new
address system "xsltproc" xslFile xsdFile with output using (outXsd) error using (errXsd)
If outXsd~items > 0 Then Do
  xsltAvailable = .true
  xsltTool = "xsltproc"
End
Signal XsltCheckDone

TryRunXSLT:
/* Attempt runXSLT.rxj (requires BSF4ooRexx) */
Signal On Syntax Name NoXsltTool
outXsd = .array~new
errXsd = .array~new
address system "rexx runXSLT.rxj" xslFile xsdFile with output using (outXsd) error using (errXsd)
If outXsd~items > 0 Then Do
  xsltAvailable = .true
  xsltTool = "runXSLT.rxj"
End
Signal XsltCheckDone

NoXsltTool:
Signal Off Syntax

XsltCheckDone:
Signal Off Syntax

If \xsltAvailable Then Do
  say "  SKIP: no XSLT processor available (xsltproc or runXSLT.rxj)"
End
Else Do
  /* XSD round-trip: parse the XSLT output and compare */
  xsdYaml = outXsd~makeString('L', "0A"x)
  parser2 = .Yaml~new
  xsdDoc = parser2~parseString(xsdYaml)
  call check YAML.deepEqual(original, xsdDoc), .true, "XSLT xsd round-trip (" || xsltTool || ")"

  /* DTD round-trip */
  outDtd = .array~new
  errDtd = .array~new
  If xsltTool == "xsltproc" Then
    address system "xsltproc" xslFile dtdFile with output using (outDtd) error using (errDtd)
  Else
    address system "rexx runXSLT.rxj" xslFile dtdFile with output using (outDtd) error using (errDtd)
  dtdYaml = outDtd~makeString('L', "0A"x)
  parser3 = .Yaml~new
  dtdDoc = parser3~parseString(dtdYaml)
  call check YAML.deepEqual(original, dtdDoc), .true, "XSLT dtd round-trip (" || xsltTool || ")"
End

/* Clean up temporary XML files */
Call SysFileDelete xsdFile
Call SysFileDelete dtdFile
say

/*========================================================================*/
say "Test group 38: Complex mapping keys (? indicator)"
/*========================================================================*/

parser = .Yaml~new

/* 38.1 Flow sequence as key */
yaml = "? [a, b]" || "0A"x || ": value1" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 1, "flow seq key: one entry"
sup = doc~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "flow seq key: key is array"
  call check sup~index~items, 2, "flow seq key: key has 2 items"
  call check sup~index[1], "a", "flow seq key: key[1]"
  call check sup~index[2], "b", "flow seq key: key[2]"
  call check sup~item, "value1", "flow seq key: value"
  sup~next
End

/* 38.2 Flow mapping as key */
yaml = "? {x: 1}" || "0A"x || ": value2" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 1, "flow map key: one entry"
sup = doc~supplier
Do While sup~available
  call check sup~index~isA(.table), .true, "flow map key: key is table"
  call check sup~index["x"], 1, "flow map key: key[x]"
  call check sup~item, "value2", "flow map key: value"
  sup~next
End

/* 38.3 Block sequence as key */
yaml = "?" || "0A"x || "  - item1" || "0A"x || "  - item2" || "0A"x || ": value3" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 1, "block seq key: one entry"
sup = doc~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "block seq key: key is array"
  call check sup~index[1], "item1", "block seq key: key[1]"
  call check sup~index[2], "item2", "block seq key: key[2]"
  call check sup~item, "value3", "block seq key: value"
  sup~next
End

/* 38.4 Scalar complex key */
yaml = "? scalar_key" || "0A"x || ": value4" || "0A"x
doc = parser~parseString(yaml)
call check doc["scalar_key"], "value4", "scalar complex key"

/* 38.5 Complex key with null value (no : line) */
yaml = "? [a, b]" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 1, "complex key null value: one entry"
sup = doc~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "complex key null value: key is array"
  call check sup~item == .nil, .true, "complex key null value: value is nil"
  sup~next
End

/* 38.6 Mixed simple and complex keys */
yaml = "simple: val1" || "0A"x || "? [x, y]" || "0A"x || ": val2" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 2, "mixed keys: two entries"
call check doc["simple"], "val1", "mixed keys: simple key"

/* 38.7 Multiple complex keys */
yaml = "? [a, b]" || "0A"x || ": val1" || "0A"x || -
       "? [c, d]" || "0A"x || ": val2" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 2, "multiple complex keys: two entries"

/* 38.8 Round-trip: flow sequence key */
yaml = "? [a, b]" || "0A"x || ": value1" || "0A"x
doc = parser~parseString(yaml)
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip flow seq key"

/* 38.9 Round-trip: flow mapping key */
yaml = "? {x: 1}" || "0A"x || ": value2" || "0A"x
doc = parser~parseString(yaml)
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip flow map key"

/* 38.10 Round-trip: block sequence key */
yaml = "?" || "0A"x || "  - item1" || "0A"x || "  - item2" || "0A"x || ": value3" || "0A"x
doc = parser~parseString(yaml)
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip block seq key"

/* 38.11 Round-trip: mixed simple + complex keys */
yaml = "simple: val1" || "0A"x || "? [x, y]" || "0A"x || ": val2" || "0A"x
doc = parser~parseString(yaml)
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip mixed keys"

/* 38.12 Round-trip: programmatic creation with array key */
doc = .table~new
doc[.array~of("a", "b")] = "value1"
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip programmatic array key"

/* 38.13 Round-trip: programmatic creation with table key */
doc = .table~new
tKey = .table~new; tKey["x"] = 1
doc[tKey] = "value2"
out = .Yaml~toYaml(doc)
doc2 = parser~parseString(out)
call check YAML.deepEqual(doc, doc2), .true, "round-trip programmatic table key"

/* 38.14 Emitter uses ? syntax for non-string keys */
doc = .table~new
doc[.array~of("a", "b")] = "value1"
out = .Yaml~toYaml(doc)
call check out~pos("? ") > 0, .true, "emitter uses ? for array key"
call check out~pos(": value1") > 0, .true, "emitter emits value after :"

/* 38.15 YAML.deepEqual with non-string keys */
a = .table~new; a[.array~of(1, 2)] = "v1"
b = .table~new; b[.array~of(1, 2)] = "v1"
call check YAML.deepEqual(a, b), .true, "deepEqual non-string keys equal"

/* 38.16 YAML.deepEqual with non-string keys, different values */
a = .table~new; a[.array~of(1, 2)] = "v1"
b = .table~new; b[.array~of(1, 2)] = "v2"
call check YAML.deepEqual(a, b), .false, "deepEqual non-string keys diff values"

/* 38.17 YAML.deepEqual with non-string keys, different keys */
a = .table~new; a[.array~of(1, 2)] = "v1"
b = .table~new; b[.array~of(3, 4)] = "v1"
call check YAML.deepEqual(a, b), .false, "deepEqual non-string keys diff keys"

/* 38.18 Complex key with block value */
yaml = "? [a, b]" || "0A"x || ":" || "0A"x || "  nested: val" || "0A"x
doc = parser~parseString(yaml)
call check doc~items, 1, "complex key block value: one entry"
sup = doc~supplier
Do While sup~available
  call check sup~item~isA(.table), .true, "complex key block value: value is table"
  call check sup~item["nested"], "val", "complex key block value: nested"
  sup~next
End
say

/*========================================================================*/
say "Test group 39: Unicode escape handling (\u / \U)"
/*========================================================================*/

/* --- 39.1 \u unescape: 2-byte UTF-8 (é = U+00E9 → C3 A9) --- */
parser = .Yaml~new            /* default: unescapeUnicode = .true */
yaml = 'val: "\u00E9"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "C3A9", '\u 2-byte UTF-8 (é)'

/* --- 39.2 \u unescape: 1-byte ASCII (A = U+0041 → 41) --- */
yaml = 'val: "\u0041"'
doc = parser~parseString(yaml)
call check doc["val"], "A", '\u 1-byte ASCII (A)'

/* --- 39.3 \u unescape: 3-byte UTF-8 (世 = U+4E16 → E4 B8 96) --- */
yaml = 'val: "\u4E16"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "E4B896", '\u 3-byte UTF-8 (世)'

/* --- 39.4 \U unescape: 4-byte UTF-8 (U+1F600 → F0 9F 98 80) --- */
yaml = 'val: "\U0001F600"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "F09F9880", '\U 4-byte UTF-8 (U+1F600)'

/* --- 39.5 \u unescape: null char (U+0000 → 00) --- */
yaml = 'val: "\u0000"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "00", '\u null char (U+0000)'

/* --- 39.6 \u unescape: 2-byte boundary (U+0080 → C2 80) --- */
yaml = 'val: "\u0080"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "C280", '\u 2-byte boundary (U+0080)'

/* --- 39.7 \u unescape: 3-byte boundary (U+0800 → E0 A0 80) --- */
yaml = 'val: "\u0800"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "E0A080", '\u 3-byte boundary (U+0800)'

/* --- 39.8 \U unescape: 4-byte boundary (U+10000 → F0 90 80 80) --- */
yaml = 'val: "\U00010000"'
doc = parser~parseString(yaml)
call check doc["val"]~c2x, "F0908080", '\U 4-byte boundary (U+10000)'

/* --- 39.9 Mixed \u with other escapes --- */
yaml = 'val: "caf\u00E9\tbr\u00FBl\u00E9e"'
doc = parser~parseString(yaml)
expected = "caf" || "C3A9"x || "09"x || "br" || "C3BB"x || "l" || "C3A9"x || "e"
call check doc["val"]~c2x, expected~c2x, '\u mixed with \t'

/* --- 39.10 Multiple \u in one string --- */
yaml = 'val: "\u00C0\u00C1\u00C2"'
doc = parser~parseString(yaml)
expected = "C380"x || "C381"x || "C382"x
call check doc["val"]~c2x, expected~c2x, 'multiple \u in string'

/* --- 39.11 Unescaped \u round-trip (default parser) --- */
yaml = 'val: "\u00E9"'
doc = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = parser~parseString(yamlOut)
call check doc2["val"]~c2x, "C3A9", '\u round-trip (unescaped)'

/* --- 39.12 Legacy mode: \u stored literally --- */
parserLegacy = .Yaml~new(.false)
yaml = 'val: "\u00E9"'
doc = parserLegacy~parseString(yaml)
call check doc["val"], "\u00E9", '\u legacy mode literal'

/* --- 39.13 Legacy mode: \U stored literally --- */
yaml = 'val: "\U0001F600"'
doc = parserLegacy~parseString(yaml)
call check doc["val"], "\U0001F600", '\U legacy mode literal'

/* --- 39.14 Legacy mode round-trip --- */
yaml = 'val: "\u00E9"'
doc = parserLegacy~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = parserLegacy~parseString(yamlOut)
call check doc2["val"], "\u00E9", '\u legacy round-trip'

/* --- 39.15 \u in mapping key (unescaped) --- */
yaml = '"\u00E9": value'
doc = parser~parseString(yaml)
call check doc["C3A9"x], "value", '\u in mapping key'

/* --- 39.16 \u in sequence item (unescaped) --- */
yaml = '- "\u00E9"' || "0A"x || '- "\u4E16"'
doc = parser~parseString(yaml)
call check doc[1]~c2x, "C3A9", '\u in seq item 1'
call check doc[2]~c2x, "E4B896", '\u in seq item 2'

/* --- 39.17 \u XML round-trip (unescaped) --- */
yaml = 'val: "\u00E9"'
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = parser~parseXml(xml)
call check doc2["val"]~c2x, "C3A9", '\u XML round-trip'

/* --- 39.18 \U in flow collection --- */
yaml = 'items: ["\U0001F600", "\u00E9"]'
doc = parser~parseString(yaml)
call check doc["items"][1]~c2x, "F09F9880", '\U in flow seq'
call check doc["items"][2]~c2x, "C3A9", '\u in flow seq'
say

/*========================================================================*/
say "Test group 40: Complex keys in XML round-trip"
/*========================================================================*/

parser = .Yaml~new

/* 40.1 XSD round-trip: sequence as key */
yaml = "? [a, b]" || "0A"x || ": value1" || "0A"x
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
call check (xml~pos("<sequence>") > 0), 1, "xml seq key: has sequence element"
doc2 = parser~parseXml(xml)
call check doc2~items, 1, "xml seq key: one entry"
sup = doc2~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "xml seq key: key is array"
  call check sup~index[1], "a", "xml seq key: key[1]"
  call check sup~index[2], "b", "xml seq key: key[2]"
  call check sup~item, "value1", "xml seq key: value"
  sup~next
End

/* 40.2 DTD round-trip: sequence as key */
xml = .Yaml~yamlToXml(doc, "dtd")
doc3 = parser~parseXml(xml)
sup = doc3~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "dtd seq key: key is array"
  call check sup~index[1], "a", "dtd seq key: key[1]"
  call check sup~item, "value1", "dtd seq key: value"
  sup~next
End

/* 40.3 XSD round-trip: mapping as key */
yaml = "? {x: 1}" || "0A"x || ": value2" || "0A"x
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = parser~parseXml(xml)
call check doc2~items, 1, "xml map key: one entry"
sup = doc2~supplier
Do While sup~available
  call check sup~index~isA(.table), .true, "xml map key: key is table"
  call check sup~index["x"], 1, "xml map key: key[x]"
  call check sup~item, "value2", "xml map key: value"
  sup~next
End

/* 40.4 XSD round-trip: complex key with null value */
yaml = "? [x, y]" || "0A"x
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = parser~parseXml(xml)
sup = doc2~supplier
Do While sup~available
  call check sup~index~isA(.array), .true, "xml null val: key is array"
  call check (sup~item == .nil), 1, "xml null val: value is nil"
  sup~next
End

/* 40.5 XSD round-trip: mixed simple and complex keys */
yaml = "simple: val1" || "0A"x || -
       "? [a, b]"      || "0A"x || -
       ": val2"         || "0A"x
doc = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = parser~parseXml(xml)
call check doc2["simple"], "val1", "xml mixed keys: simple"
/* Find the complex key */
found = .false
sup = doc2~supplier
Do While sup~available
  If sup~index~isA(.array) Then Do
    call check sup~index[1], "a", "xml mixed keys: complex[1]"
    call check sup~item, "val2", "xml mixed keys: complex val"
    found = .true
  End
  sup~next
End
call check found, .true, "xml mixed keys: found complex"

/* 40.6 XML stability: complex key XML → parse → XML → parse */
yaml = "? [1, 2]" || "0A"x || ": result" || "0A"x
doc = parser~parseString(yaml)
xml1 = .Yaml~yamlToXml(doc, "xsd")
doc2 = parser~parseXml(xml1)
xml2 = .Yaml~yamlToXml(doc2, "xsd")
doc3 = parser~parseXml(xml2)
call check YAML.deepEqual(doc2, doc3), .true, "xml complex key stable"
say

/*========================================================================*/
say "--- 41. Edge-case round-trips ---"

/* 41.1 Special floats: YAML round-trip */
yaml = "inf: .inf"   || "0A"x || -
       "ninf: -.inf"  || "0A"x || -
       "nan: .nan"    || "0A"x
doc1 = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check doc2["inf"], ".inf", "special float inf yaml rt"
call check doc2["ninf"], "-.inf", "special float -inf yaml rt"
call check doc2["nan"], ".nan", "special float nan yaml rt"

/* 41.2 Special floats: XML round-trip (XSD) */
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["inf"], ".inf", "special float inf xml rt"
call check doc3["ninf"], "-.inf", "special float -inf xml rt"
call check doc3["nan"], ".nan", "special float nan xml rt"

/* 41.3 Empty string values: YAML round-trip */
yaml = "empty_sq: ''"   || "0A"x || -
       "empty_dq: " || '"' || '"' || "0A"x || -
       "empty_plain: "   || "0A"x
doc1 = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check doc2["empty_sq"], "", "empty sq yaml rt"
call check doc2["empty_dq"], "", "empty dq yaml rt"
call check doc2["empty_plain"] == .nil, .true, "empty plain is nil"

/* 41.4 Empty string values: XML round-trip */
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["empty_sq"], "", "empty sq xml rt"
call check doc3["empty_dq"], "", "empty dq xml rt"

/* 41.5 Empty collections: YAML round-trip */
yaml = "empty_seq: []"   || "0A"x || -
       "empty_map: {}"   || "0A"x
doc1 = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check doc2["empty_seq"]~isA(.array), .true, "empty seq yaml rt type"
call check doc2["empty_seq"]~items, 0, "empty seq yaml rt items"
call check doc2["empty_map"]~isA(.table), .true, "empty map yaml rt type"
call check doc2["empty_map"]~items, 0, "empty map yaml rt items"

/* 41.6 Empty collections: XML round-trip */
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["empty_seq"]~isA(.array), .true, "empty seq xml rt type"
call check doc3["empty_seq"]~items, 0, "empty seq xml rt items"
call check doc3["empty_map"]~isA(.table), .true, "empty map xml rt type"
call check doc3["empty_map"]~items, 0, "empty map xml rt items"

/* 41.7 Multiline double-quoted string: YAML round-trip */
yaml = 'multi: "line one\nline two\nline three"' || "0A"x
doc1 = parser~parseString(yaml)
call check (doc1["multi"]~pos("0A"x) > 0), 1, "multiline dq has newlines"
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check doc2["multi"], doc1["multi"], "multiline dq yaml rt"

/* 41.8 Multiline double-quoted: XML round-trip */
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["multi"], doc1["multi"], "multiline dq xml rt"

/* 41.9 Directives: round-trip preserves content (directives stripped) */
yaml = "%YAML 1.2"     || "0A"x || -
       "---"            || "0A"x || -
       "key: value"     || "0A"x
doc1 = parser~parseString(yaml)
yamlOut = .Yaml~toYaml(doc1)
doc2 = parser~parseString(yamlOut)
call check doc2["key"], "value", "directives yaml rt"
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["key"], "value", "directives xml rt"

/* 41.10 Single-quoted strings: XML round-trip */
yaml = "sq: 'It''s a test'"  || "0A"x || -
       "plain_sq: 'hello'"   || "0A"x
doc1 = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["sq"], "It's a test", "single-quoted xml rt"
call check doc3["plain_sq"], "hello", "single-quoted plain xml rt"

/* 41.11 Flow mappings: XML round-trip */
yaml = "flow: {a: 1, b: 2, c: 3}"  || "0A"x
doc1 = parser~parseString(yaml)
xml = .Yaml~yamlToXml(doc1, "xsd")
doc3 = parser~parseXml(xml)
call check doc3["flow"]["a"], 1, "flow map xml rt a"
call check doc3["flow"]["b"], 2, "flow map xml rt b"
call check doc3["flow"]["c"], 3, "flow map xml rt c"

/* 41.12 Nested anchors/aliases: XML round-trip via DTD */
yaml = "defaults: &defs"    || "0A"x || -
       "  color: red"       || "0A"x || -
       "  size: 10"         || "0A"x || -
       "item1:"             || "0A"x || -
       "  <<: *defs"        || "0A"x || -
       "  size: 20"         || "0A"x
doc1 = parser~parseString(yaml)
am = parser~anchorMap
msm = parser~mergeSourceMap
xml = .Yaml~yamlToXml(doc1, "dtd", am)
doc3 = parser~parseXml(xml)
call check doc3["item1"]["color"], "red", "anchor merge dtd xml rt color"
call check doc3["item1"]["size"], 20, "anchor merge dtd xml rt size"

/* 41.13 Multi-document: XML round-trip */
yaml = "---"            || "0A"x || -
       "first: doc1"    || "0A"x || -
       "---"            || "0A"x || -
       "second: doc2"   || "0A"x
docs = parser~parseAll(yaml)
call check docs~items, 2, "multi-doc count"
xml1 = .Yaml~yamlToXml(docs[1], "xsd")
doc3 = parser~parseXml(xml1)
call check doc3["first"], "doc1", "multi-doc xml rt doc1"
xml2 = .Yaml~yamlToXml(docs[2], "xsd")
doc4 = parser~parseXml(xml2)
call check doc4["second"], "doc2", "multi-doc xml rt doc2"
say

/*========================================================================*/
/* Group 42 — Tag preservation round-trips                               */
/*========================================================================*/
say "--- 42. Tag preservation round-trips ---"

/* Use a tag-preserving parser for all group 42 tests */
tp = .Yaml~new(, .true)

/* 42.1 !!str scalar: parse produces YamlTagged */
doc = tp~parseString('!!str 42')
call check doc~isA(.YamlTagged), .true, "!!str produces YamlTagged"
call check doc~tag, "!!str", "!!str tag preserved"
call check doc~value, "42", "!!str value is string 42"

/* 42.2 !!int scalar */
doc = tp~parseString('!!int 42')
call check doc~tag, "!!int", "!!int tag preserved"
call check doc~value, 42, "!!int value is integer"

/* 42.3 !custom scalar */
doc = tp~parseString('!custom hello')
call check doc~tag, "!custom", "!custom tag preserved"
call check doc~value, "hello", "!custom value"

/* 42.4 Verbatim tag */
doc = tp~parseString('!<tag:yaml.org,2002:str> hello')
call check doc~tag, "!<tag:yaml.org,2002:str>", "verbatim tag preserved"
call check doc~value, "hello", "verbatim tag value"

/* 42.5 !!map on block mapping */
yaml = "!!map" || "0A"x || "a: 1" || "0A"x
doc = tp~parseString(yaml)
call check doc~isA(.YamlTagged), .true, "!!map produces YamlTagged"
call check doc~tag, "!!map", "!!map tag"
call check doc~value~isA(.table), .true, "!!map value is table"
call check doc~value["a"], 1, "!!map inner value"

/* 42.6 !!seq on block sequence */
yaml = "!!seq" || "0A"x || "- one" || "0A"x || "- two" || "0A"x
doc = tp~parseString(yaml)
call check doc~tag, "!!seq", "!!seq tag"
call check doc~value~isA(.array), .true, "!!seq value is array"
call check doc~value[1], "one", "!!seq item 1"

/* 42.7 Tags in flow collections */
yaml = "[!!str 42, !!int 7]"
doc = tp~parseString(yaml)
call check doc[1]~tag, "!!str", "flow seq !!str tag"
call check doc[2]~tag, "!!int", "flow seq !!int tag"

/* 42.8 Tag on mapping key */
yaml = "plain: x" || "0A"x || "!!str tagged: y" || "0A"x
doc = tp~parseString(yaml)
found = .false
sup = doc~supplier
do while sup~available
  k = sup~index
  if k~isA(.YamlTagged) then do
    call check k~tag, "!!str", "tag on key"
    call check k~value, "tagged", "tagged key value"
    found = .true
  end
  sup~next
end
call check found, .true, "tagged key found"

/* 42.9 YAML round-trip with tags */
doc = tp~parseString('!!str 42')
yamlOut = .Yaml~toYaml(doc)
doc2 = tp~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "YAML rt !!str"

/* 42.10 YAML round-trip: !!map */
yaml = "!!map" || "0A"x || "x: 1" || "0A"x
doc = tp~parseString(yaml)
yamlOut = .Yaml~toYaml(doc)
doc2 = tp~parseString(yamlOut)
call check YAML.deepEqual(doc, doc2), .true, "YAML rt !!map"

/* 42.11 XML round-trip XSD: !!str scalar */
doc = tp~parseString('!!str 42')
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt !!str"

/* 42.12 XML round-trip DTD: !!str scalar */
doc = tp~parseString('!!str 42')
xml = .Yaml~yamlToXml(doc, "dtd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML DTD rt !!str"

/* 42.13 XML round-trip XSD: !custom tag */
doc = tp~parseString('!custom hello')
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt !custom"

/* 42.14 XML round-trip: verbatim tag */
doc = tp~parseString('!<tag:yaml.org,2002:str> hello')
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt verbatim tag"

/* 42.15 XML round-trip: !!map on mapping */
yaml = "!!map" || "0A"x || "a: 1" || "0A"x
doc = tp~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt !!map"

/* 42.16 XML round-trip DTD: !!map */
yaml = "!!map" || "0A"x || "a: 1" || "0A"x
doc = tp~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "dtd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML DTD rt !!map"

/* 42.17 XML round-trip: !!seq on sequence */
yaml = "!!seq" || "0A"x || "- one" || "0A"x || "- two" || "0A"x
doc = tp~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt !!seq"

/* 42.18 Full chain round-trip: YAML -> parse -> XML -> parse -> YAML */
doc = tp~parseString('!!str 42')
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
yamlOut = .Yaml~toYaml(doc2)
doc3 = tp~parseString(yamlOut)
call check YAML.deepEqual(doc, doc3), .true, "full chain rt !!str"

/* 42.19 preserveTags = .false (default) — tags stripped as before */
pDef = .Yaml~new
doc = pDef~parseString('!!str 42')
call check doc~isA(.YamlTagged), .false, "default no YamlTagged"
call check doc, "42", "default strips tag"

/* 42.20 Tags on sequence items in XML round-trip */
yaml = "- !!str foo" || "0A"x || "- !!int 7" || "0A"x
doc = tp~parseString(yaml)
xml = .Yaml~yamlToXml(doc, "xsd")
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML XSD rt tagged seq items"

/* 42.21 Tag + anchor combination */
yaml = "&myA !!str hello" || "0A"x
doc = tp~parseString(yaml)
am = tp~anchorMap
call check doc~isA(.YamlTagged), .true, "tag+anchor YamlTagged"
call check doc~tag, "!!str", "tag+anchor tag"
call check doc~value, "hello", "tag+anchor value"
xml = .Yaml~yamlToXml(doc, "xsd", am)
doc2 = tp~parseXml(xml)
call check YAML.deepEqual(doc, doc2), .true, "XML rt tag+anchor"

/* 42.22 YAML.deepEqual: both tagged same */
a = .YamlTagged~new("!!str", "42")
b = .YamlTagged~new("!!str", "42")
call check YAML.deepEqual(a, b), .true, "deepEqual same tagged"

/* 42.23 YAML.deepEqual: different tags */
a = .YamlTagged~new("!!str", "42")
b = .YamlTagged~new("!!int", 42)
call check YAML.deepEqual(a, b), .false, "deepEqual diff tags"

/* 42.24 YAML.deepEqual: tagged vs untagged */
a = .YamlTagged~new("!!str", "42")
call check YAML.deepEqual(a, "42"), .false, "deepEqual tagged vs plain"

/* 42.25 YAML.deepEqual: untagged vs tagged */
call check YAML.deepEqual("42", a), .false, "deepEqual plain vs tagged"
say

/*========================================================================*/
/* Group 43 — Unicode escape shortcuts \N, \_, \L, \P (P8)               */
/*========================================================================*/
say "Group 43: Unicode escape shortcuts"

yaml43 = '---'                                || "0A"x || -
         'next_line: "hello\Nworld"'          || "0A"x || -
         'nbsp: "non\_breaking"'             || "0A"x || -
         'line_sep: "line\Lsep"'              || "0A"x || -
         'para_sep: "para\Psep"'              || "0A"x

p43 = .Yaml~new
doc43 = p43~parseString(yaml43)

/* 43.1 \N → U+0085 (UTF-8: C2 85) */
call check doc43["next_line"], "hello" || "C285"x || "world", "\N escape (U+0085)"

/* 43.2 \_ → U+00A0 (UTF-8: C2 A0) */
call check doc43["nbsp"], "non" || "C2A0"x || "breaking", "\_ escape (U+00A0)"

/* 43.3 \L → U+2028 (UTF-8: E2 80 A8) */
call check doc43["line_sep"], "line" || "E280A8"x || "sep", "\L escape (U+2028)"

/* 43.4 \P → U+2029 (UTF-8: E2 80 A9) */
call check doc43["para_sep"], "para" || "E280A9"x || "sep", "\P escape (U+2029)"

/* 43.5 YAML round-trip preserves these characters */
yaml43rt = .Yaml~toYaml(doc43)
doc43rt = .Yaml~new~parseString(yaml43rt)
call check doc43rt["next_line"], doc43["next_line"], "\N YAML round-trip"
call check doc43rt["nbsp"], doc43["nbsp"], "\_ YAML round-trip"
call check doc43rt["line_sep"], doc43["line_sep"], "\L YAML round-trip"
call check doc43rt["para_sep"], doc43["para_sep"], "\P YAML round-trip"

/* 43.9 XML round-trip (XSD) */
xml43 = .Yaml~yamlToXml(doc43)
doc43x = p43~parseXml(xml43)
call check doc43x["next_line"], doc43["next_line"], "\N XML round-trip"
call check doc43x["nbsp"], doc43["nbsp"], "\_ XML round-trip"
call check doc43x["line_sep"], doc43["line_sep"], "\L XML round-trip"
call check doc43x["para_sep"], doc43["para_sep"], "\P XML round-trip"
say

/*========================================================================*/
/* Group 44 — Tab handling (P7)                                           */
/*========================================================================*/
say "Group 44: Tab handling"

/* 44.1 Tab at start of line in block mapping — should raise error */
yaml44a = "key:" || "0A"x || "09"x || "value: bad"
signal on syntax name tab44_1
p44 = .Yaml~new
doc44 = p44~parseString(yaml44a)
call check .false, .true, "tab at start raises error"
signal tab44_1done
tab44_1:
call check .true, .true, "tab at start raises error"
tab44_1done:

/* 44.2 Tab after spaces in indentation should raise error */
yaml44b = "key:" || "0A"x || "  " || "09"x || "value: bad"
signal on syntax name tab44_2
p44b = .Yaml~new
doc44b = p44b~parseString(yaml44b)
call check .false, .true, "tab after spaces raises error"
signal tab44_2done
tab44_2:
call check .true, .true, "tab after spaces raises error"
tab44_2done:

/* 44.3 Tab in content (not indentation) should be fine */
yaml44c = 'key: "value' || "09"x || 'with tab"'
p44c = .Yaml~new
doc44c = p44c~parseString(yaml44c)
call check doc44c["key"], "value" || "09"x || "with tab", "tab in content OK"
say

/*========================================================================*/
/* Group 45 — Explicit key ? with key on next line (P10)                 */
/*========================================================================*/
say "Group 45: Explicit key ? with key on next line"

/* 45.1 ? with scalar key on next line */
yaml45a = "?" || "0A"x || "  my_key" || "0A"x || ": value1"
p45 = .Yaml~new
doc45a = p45~parseString(yaml45a)
call check doc45a["my_key"], "value1", "? scalar key next line"

/* 45.2 ? with flow sequence key on next line */
yaml45b = "?" || "0A"x || "  [a, b]" || "0A"x || ": value2"
doc45b = .Yaml~new~parseString(yaml45b)
/* Key is an Array — we can't look up by Array identity, so iterate */
found45b = .false
sup45b = doc45b~supplier
do while sup45b~available
  if sup45b~index~isA(.array) then do
    call check sup45b~index~items, 2, "? seq key next line: items"
    call check sup45b~index[1], "a", "? seq key next line: [1]"
    call check sup45b~index[2], "b", "? seq key next line: [2]"
    call check sup45b~item, "value2", "? seq key next line: value"
    found45b = .true
  end
  sup45b~next
end
call check found45b, .true, "? seq key next line: found"

/* 45.3 ? with block mapping key on next lines */
yaml45c = "?" || "0A"x || "  x: 1" || "0A"x || "  y: 2" || "0A"x || ": complex_value"
doc45c = .Yaml~new~parseString(yaml45c)
found45c = .false
sup45c = doc45c~supplier
do while sup45c~available
  if sup45c~index~isA(.table) then do
    k45 = sup45c~index
    call check k45["x"], 1, "? map key next line: x"
    call check k45["y"], 2, "? map key next line: y"
    call check sup45c~item, "complex_value", "? map key next line: value"
    found45c = .true
  end
  sup45c~next
end
call check found45c, .true, "? map key next line: found"

/* 45.4 ? alone with no key at higher indent = null key */
yaml45d = "?" || "0A"x || ": null_key_value"
doc45d = .Yaml~new~parseString(yaml45d)
call check doc45d~at(.nil), "null_key_value", "? null key next line"

/* 45.5 YAML round-trip of ? with scalar key next line */
yaml45rt = .Yaml~toYaml(doc45a)
doc45art = .Yaml~new~parseString(yaml45rt)
call check doc45art["my_key"], "value1", "? scalar key next line YAML rt"

/* 45.6 XML round-trip of ? with scalar key next line */
xml45 = .Yaml~yamlToXml(doc45a)
doc45ax = .Yaml~new~parseXml(xml45)
call check doc45ax["my_key"], "value1", "? scalar key next line XML rt"
say

/*========================================================================*/
/* Group 46 — Multi-line quoted strings (P4)                             */
/*========================================================================*/
say "Group 46: Multi-line quoted strings"

/* 46.1  Double-quoted: fold across two lines */
yaml46a = '---'                                || "0A"x || -
          'key: "hello'                        || "0A"x || -
          '  world"'                           || "0A"x
p46 = .Yaml~new
doc46a = p46~parseString(yaml46a)
call check doc46a["key"], "hello world", "dq fold two lines"

/* 46.2  Single-quoted: fold across two lines */
yaml46b = '---'                                || "0A"x || -
          "key: 'hello"                        || "0A"x || -
          "  world'"                           || "0A"x
doc46b = p46~parseString(yaml46b)
call check doc46b["key"], "hello world", "sq fold two lines"

/* 46.3  Double-quoted: three continuation lines */
yaml46c = '---'                                || "0A"x || -
          'key: "line one'                     || "0A"x || -
          '  line two'                         || "0A"x || -
          '  line three"'                      || "0A"x
doc46c = p46~parseString(yaml46c)
call check doc46c["key"], "line one line two line three", "dq fold three lines"

/* 46.4  Double-quoted: blank line → literal newline */
yaml46d = '---'                                || "0A"x || -
          'key: "first'                        || "0A"x || -
          ''                                   || "0A"x || -
          '  second"'                          || "0A"x
doc46d = p46~parseString(yaml46d)
call check doc46d["key"], "first" || "0A"x || "second", "dq blank line → newline"

/* 46.5  Single-quoted: blank line → literal newline */
yaml46e = '---'                                || "0A"x || -
          "key: 'first"                        || "0A"x || -
          ''                                   || "0A"x || -
          "  second'"                          || "0A"x
doc46e = p46~parseString(yaml46e)
call check doc46e["key"], "first" || "0A"x || "second", "sq blank line → newline"

/* 46.6  Double-quoted: escape-newline (\ at end) joins without space */
yaml46f = '---'                                || "0A"x || -
          'key: "no\'                          || "0A"x || -
          '  space"'                           || "0A"x
doc46f = p46~parseString(yaml46f)
call check doc46f["key"], "nospace", "dq escape-newline joins"

/* 46.7  Double-quoted: escape in multi-line content */
yaml46g = '---'                                || "0A"x || -
          'key: "has \t tab'                   || "0A"x || -
          '  and more"'                        || "0A"x
doc46g = p46~parseString(yaml46g)
call check doc46g["key"], "has " || "09"x || " tab and more", "dq escape in multiline"

/* 46.8  Multi-line dq in sequence item */
yaml46h = '---'                                || "0A"x || -
          'items:'                             || "0A"x || -
          '  - "multi'                         || "0A"x || -
          '    line"'                          || "0A"x || -
          '  - plain'                          || "0A"x
doc46h = p46~parseString(yaml46h)
call check doc46h["items"][1], "multi line", "dq in sequence item"
call check doc46h["items"][2], "plain", "plain after multi-line dq"

/* 46.9  Multi-line sq in sequence item */
yaml46i = '---'                                || "0A"x || -
          'items:'                             || "0A"x || -
          "  - 'single"                        || "0A"x || -
          "    line'"                          || "0A"x || -
          '  - other'                          || "0A"x
doc46i = p46~parseString(yaml46i)
call check doc46i["items"][1], "single line", "sq in sequence item"
call check doc46i["items"][2], "other", "plain after multi-line sq"

/* 46.10  Multi-line dq as mapping value with next key */
yaml46j = '---'                                || "0A"x || -
          'k1: "multi'                         || "0A"x || -
          '  line"'                            || "0A"x || -
          'k2: normal'                         || "0A"x
doc46j = p46~parseString(yaml46j)
call check doc46j["k1"], "multi line", "dq value with next key"
call check doc46j["k2"], "normal", "next key after multi-line dq"

/* 46.11  Single-quoted with escaped single quote (doubled) */
yaml46k = '---'                                || "0A"x || -
          "key: 'it''s"                        || "0A"x || -
          "  multi'"                           || "0A"x
doc46k = p46~parseString(yaml46k)
call check doc46k["key"], "it's multi", "sq doubled quote in multiline"

/* 46.12  YAML round-trip for multi-line quoted */
yaml46rt = .Yaml~toYaml(doc46d)
doc46rt = p46~parseString(yaml46rt)
call check doc46rt["key"], doc46d["key"], "dq blank-line YAML round-trip"

/* 46.13  XML round-trip for multi-line quoted */
xml46 = .Yaml~yamlToXml(doc46d)
doc46x = p46~parseXml(xml46)
call check doc46x["key"], doc46d["key"], "dq blank-line XML round-trip"

/* 46.14  YAML round-trip for escape-newline */
yaml46frt = .Yaml~toYaml(doc46f)
doc46frt = p46~parseString(yaml46frt)
call check doc46frt["key"], "nospace", "escape-newline YAML round-trip"

/* 46.15  YAML round-trip for multi-line sequence */
yaml46hrt = .Yaml~toYaml(doc46h)
doc46hrt = p46~parseString(yaml46hrt)
call check doc46hrt["items"][1], "multi line", "dq seq YAML round-trip"

/* 46.16  Double-quoted: two consecutive blank lines → two newlines */
yaml46l = '---'                                || "0A"x || -
          'key: "first'                        || "0A"x || -
          ''                                   || "0A"x || -
          ''                                   || "0A"x || -
          '  second"'                          || "0A"x
doc46l = p46~parseString(yaml46l)
call check doc46l["key"], "first" || "0A0A"x || "second", "dq two blank lines"

/* 46.17  Single-quoted: two consecutive blank lines → two newlines */
yaml46m = '---'                                || "0A"x || -
          "key: 'first"                        || "0A"x || -
          ''                                   || "0A"x || -
          ''                                   || "0A"x || -
          "  second'"                          || "0A"x
doc46m = p46~parseString(yaml46m)
call check doc46m["key"], "first" || "0A0A"x || "second", "sq two blank lines"

/* 46.18  Multi-line dq as standalone block scalar (blockScalarOrPlain) */
yaml46n = '---'                                || "0A"x || -
          'parent:'                            || "0A"x || -
          '  "multi'                           || "0A"x || -
          '  line block"'                      || "0A"x
doc46n = p46~parseString(yaml46n)
call check doc46n["parent"], "multi line block", "dq as block scalar"

/* 46.19  Multi-line sq as standalone block scalar */
yaml46o = '---'                                || "0A"x || -
          'parent:'                            || "0A"x || -
          "  'multi"                           || "0A"x || -
          "  line block'"                      || "0A"x
doc46o = p46~parseString(yaml46o)
call check doc46o["parent"], "multi line block", "sq as block scalar"
say

/*========================================================================*/
/* Group 47 — Multi-line flow collections (P5)                           */
/*========================================================================*/
say "Group 47: Multi-line flow collections"

/* 47.1  Flow sequence spanning lines as mapping value */
yaml47a = '---'                                || "0A"x || -
          'key: [a,'                           || "0A"x || -
          '  b, c]'                            || "0A"x
p47 = .Yaml~new
doc47a = p47~parseString(yaml47a)
call check doc47a["key"]~items, 3, "flow seq ml: items"
call check doc47a["key"][1], "a", "flow seq ml: [1]"
call check doc47a["key"][3], "c", "flow seq ml: [3]"

/* 47.2  Flow mapping spanning lines as mapping value */
yaml47b = '---'                                || "0A"x || -
          'key: {a: 1,'                        || "0A"x || -
          '  b: 2}'                            || "0A"x
doc47b = p47~parseString(yaml47b)
call check doc47b["key"]["a"], 1, "flow map ml: a"
call check doc47b["key"]["b"], 2, "flow map ml: b"

/* 47.3  Flow seq in sequence item */
yaml47c = '---'                                || "0A"x || -
          'items:'                             || "0A"x || -
          '  - [x,'                            || "0A"x || -
          '    y]'                             || "0A"x || -
          '  - plain'                          || "0A"x
doc47c = p47~parseString(yaml47c)
call check doc47c["items"][1]~items, 2, "flow seq in seq: items"
call check doc47c["items"][1][1], "x", "flow seq in seq: [1]"
call check doc47c["items"][2], "plain", "flow seq in seq: next item"

/* 47.4  Flow map in sequence item */
yaml47d = '---'                                || "0A"x || -
          'items:'                             || "0A"x || -
          '  - {p: 1,'                         || "0A"x || -
          '    q: 2}'                          || "0A"x
doc47d = p47~parseString(yaml47d)
call check doc47d["items"][1]["p"], 1, "flow map in seq: p"
call check doc47d["items"][1]["q"], 2, "flow map in seq: q"

/* 47.5  Nested multi-line flow */
yaml47e = '---'                                || "0A"x || -
          'key: {outer: [1,'                   || "0A"x || -
          '  2, 3],'                           || "0A"x || -
          '  other: ok}'                       || "0A"x
doc47e = p47~parseString(yaml47e)
call check doc47e["key"]["outer"]~items, 3, "nested ml flow: items"
call check doc47e["key"]["other"], "ok", "nested ml flow: other"

/* 47.6  Flow collection in nested mapping value */
yaml47f = '---'                                || "0A"x || -
          'items:'                             || "0A"x || -
          '  - name: test'                     || "0A"x || -
          '    tags: [a,'                      || "0A"x || -
          '      b]'                           || "0A"x
doc47f = p47~parseString(yaml47f)
call check doc47f["items"][1]["tags"]~items, 2, "flow seq in nested map: items"
call check doc47f["items"][1]["tags"][1], "a", "flow seq in nested map: [1]"
call check doc47f["items"][1]["tags"][2], "b", "flow seq in nested map: [2]"

/* 47.7  YAML round-trip for multi-line flow seq */
yaml47art = .Yaml~toYaml(doc47a)
doc47art = p47~parseString(yaml47art)
call check doc47art["key"]~items, 3, "flow seq ml YAML rt: items"

/* 47.8  XML round-trip for multi-line flow map */
xml47b = .Yaml~yamlToXml(doc47b)
doc47bx = p47~parseXml(xml47b)
call check doc47bx["key"]["a"], 1, "flow map ml XML rt: a"
call check doc47bx["key"]["b"], 2, "flow map ml XML rt: b"
say

/*========================================================================*/
say "--- 48. Folded scalar (>) emission ---"

parser48 = .Yaml~new

/* 48.1  Single-line content emits as > (clip) */
val48a = "this is one line" || "0A"x
doc48a = .table~new; doc48a["text"] = val48a
yaml48a = .Yaml~toYaml(doc48a)
call check (yaml48a~pos(">") > 0), 1, "folded > emitted for single-line"
call check (yaml48a~pos("|") = 0), 1, "folded no literal for single-line"
doc48a2 = parser48~parseString(yaml48a)
call check doc48a2["text"], val48a, "folded > clip roundtrip"

/* 48.2  Single-line content with >+ (keep, 2 trailing NLs) */
val48b = "trailing newlines kept" || "0A"x || "0A"x
doc48b = .table~new; doc48b["keep"] = val48b
yaml48b = .Yaml~toYaml(doc48b)
call check (yaml48b~pos(">+") > 0), 1, "folded >+ emitted"
doc48b2 = parser48~parseString(yaml48b)
call check doc48b2["keep"], val48b, "folded >+ roundtrip"

/* 48.3  Multi-line content stays as | (literal) */
val48c = "line one" || "0A"x || "line two" || "0A"x
doc48c = .table~new; doc48c["code"] = val48c
yaml48c = .Yaml~toYaml(doc48c)
call check (yaml48c~pos("|") > 0), 1, "literal | for multi-line"
doc48c2 = parser48~parseString(yaml48c)
call check doc48c2["code"], val48c, "literal | multi-line roundtrip"

/* 48.4  Content starting with space stays as | */
val48d = " indented start" || "0A"x
doc48d = .table~new; doc48d["indent"] = val48d
yaml48d = .Yaml~toYaml(doc48d)
call check (yaml48d~pos("|") > 0), 1, "literal | for space-start content"
doc48d2 = parser48~parseString(yaml48d)
call check doc48d2["indent"], val48d, "literal | space-start roundtrip"

/* 48.5  Long single-line wraps to multiple folded lines */
val48e = copies("word ", 20)~strip || "0A"x
doc48e = .table~new; doc48e["long"] = val48e
yaml48e = .Yaml~toYaml(doc48e)
call check (yaml48e~pos(">") > 0), 1, "folded > for long line"
/* The folded body should have more than one indented line */
bodyLines = 0
Do line Over yaml48e~makeArray("0A"x)
  If line~left(2) == "  " Then bodyLines = bodyLines + 1
End
call check (bodyLines > 1), 1, "folded long line wrapped"
doc48e2 = parser48~parseString(yaml48e)
call check doc48e2["long"], val48e, "folded long line roundtrip"

/* 48.6  Folded in sequence context */
val48f = "sequence item text" || "0A"x
doc48f = .array~of(val48f, "plain")
yaml48f = .Yaml~toYaml(doc48f)
call check (yaml48f~pos(">") > 0), 1, "folded > in sequence"
doc48f2 = parser48~parseString(yaml48f)
call check doc48f2[1], val48f, "folded > in seq roundtrip"

/* 48.7  Folded in nested mapping */
inner48g = .table~new; inner48g["desc"] = "a description" || "0A"x
doc48g = .table~new; doc48g["outer"] = inner48g
yaml48g = .Yaml~toYaml(doc48g)
call check (yaml48g~pos(">") > 0), 1, "folded > nested mapping"
doc48g2 = parser48~parseString(yaml48g)
call check doc48g2["outer"]["desc"], inner48g["desc"], "folded > nested roundtrip"

/* 48.8  String not ending with NL uses double-quoted (unchanged) */
val48h = "no trailing" || "0A"x || "newline"
doc48h = .table~new; doc48h["dq"] = val48h
yaml48h = .Yaml~toYaml(doc48h)
call check (yaml48h~pos('"') > 0), 1, "double-quoted no trailing NL"
doc48h2 = parser48~parseString(yaml48h)
call check doc48h2["dq"], val48h, "double-quoted no trailing NL roundtrip"

/* 48.9  Folded > with |+ (keep, 3 trailing NLs) */
val48i = "three trailing" || "0A"x || "0A"x || "0A"x
doc48i = .table~new; doc48i["many"] = val48i
yaml48i = .Yaml~toYaml(doc48i)
call check (yaml48i~pos(">+") > 0), 1, "folded >+ three trailing"
doc48i2 = parser48~parseString(yaml48i)
call check doc48i2["many"], val48i, "folded >+ three trailing roundtrip"

/* 48.10 XML round-trip for folded content */
xml48a = .Yaml~yamlToXml(doc48a)
doc48ax = parser48~parseXml(xml48a)
call check doc48ax["text"], val48a, "folded > XML xsd roundtrip"

xml48b = .Yaml~yamlToXml(doc48b, "dtd")
doc48bx = parser48~parseXml(xml48b)
call check doc48bx["keep"], val48b, "folded >+ XML dtd roundtrip"

/* 48.11 Comprehensive round-trip with mixed block scalar types */
doc48k = .table~new
doc48k["folded_single"]  = "one line of text" || "0A"x
doc48k["folded_keep"]    = "kept" || "0A"x || "0A"x
doc48k["literal_multi"]  = "a" || "0A"x || "b" || "0A"x
doc48k["literal_space"]  = " starts with space" || "0A"x
doc48k["double_quoted"]  = "no" || "0A"x || "trailing"
yaml48k = .Yaml~toYaml(doc48k)
doc48k2 = parser48~parseString(yaml48k)
call check YAML.deepEqual(doc48k, doc48k2), .true, "mixed block scalars roundtrip"

/* 48.12 Tagged folded scalar round-trip */
parser48t = .Yaml~new("preserveTags", .true)
doc48l = .table~new
doc48l["desc"] = .YamlTagged~new("!custom", "a single line" || "0A"x)
yaml48l = .Yaml~toYaml(doc48l)
call check (yaml48l~pos("!custom >") > 0), 1, "tagged folded > on same line"
doc48l2 = parser48t~parseString(yaml48l)
call check doc48l2["desc"]~tag, "!custom", "tagged folded > tag preserved"
call check doc48l2["desc"]~value, doc48l["desc"]~value, "tagged folded > roundtrip"

/* 48.13 Tagged literal scalar round-trip */
doc48m = .table~new
doc48m["code"] = .YamlTagged~new("!code", "a" || "0A"x || "b" || "0A"x)
yaml48m = .Yaml~toYaml(doc48m)
call check (yaml48m~pos("!code |") > 0), 1, "tagged literal | on same line"
doc48m2 = parser48t~parseString(yaml48m)
call check doc48m2["code"]~value, doc48m["code"]~value, "tagged literal | roundtrip"

/* 48.14 Tagged folded >+ (keep) round-trip */
doc48n = .table~new
doc48n["k"] = .YamlTagged~new("!keep", "kept" || "0A"x || "0A"x)
yaml48n = .Yaml~toYaml(doc48n)
call check (yaml48n~pos("!keep >+") > 0), 1, "tagged folded >+ on same line"
doc48n2 = parser48t~parseString(yaml48n)
call check doc48n2["k"]~value, doc48n["k"]~value, "tagged folded >+ roundtrip"

/* 48.15 Content exactly at wrap boundary (76 chars) */
val48o = copies("x", 76) || "0A"x
doc48o = .table~new; doc48o["exact"] = val48o
yaml48o = .Yaml~toYaml(doc48o)
doc48o2 = parser48~parseString(yaml48o)
call check doc48o2["exact"], val48o, "folded > exact boundary roundtrip"

/* 48.16 Single very long word (no spaces, 200 chars) */
val48p = copies("abcdefghij", 20) || "0A"x
doc48p = .table~new; doc48p["long"] = val48p
yaml48p = .Yaml~toYaml(doc48p)
doc48p2 = parser48~parseString(yaml48p)
call check doc48p2["long"], val48p, "folded > long word no spaces roundtrip"
say

/*========================================================================*/
say "Group 49: Compact block sequences and document count"
/*========================================================================*/

parser49 = .yaml~new

/* 49.1 Compact block sequence as mapping value (same indent) */
yaml49a = "key:" || "0A"x || "- val1" || "0A"x || "- val2"
doc49a = parser49~parseString(yaml49a)
call check doc49a~isA(.table), 1, "compact seq > is table"
v49a = doc49a["key"]
call check v49a~isA(.array), 1, "compact seq > value is array"
call check v49a~items, 2, "compact seq > 2 items"
call check v49a[1], "val1", "compact seq > [1]"
call check v49a[2], "val2", "compact seq > [2]"

/* 49.2 Compact block sequence with multiple mapping keys */
yaml49b = "a:" || "0A"x || "- 1" || "0A"x || "- 2" || "0A"x || "b: 3"
doc49b = parser49~parseString(yaml49b)
call check doc49b~isA(.table), 1, "compact seq multi-key > is table"
call check doc49b~items, 2, "compact seq multi-key > 2 entries"
v49b = doc49b["a"]
call check v49b~isA(.array), 1, "compact seq multi-key > a is array"
call check v49b~items, 2, "compact seq multi-key > a has 2 items"
call check v49b[1], 1, "compact seq multi-key > a[1]"
call check v49b[2], 2, "compact seq multi-key > a[2]"
call check doc49b["b"], 3, "compact seq multi-key > b"

/* 49.3 Comment after colon with value on next lines */
yaml49c = "hr: # comment" || "0A"x || "  - Mark" || "0A"x || "  - Sammy"
doc49c = parser49~parseString(yaml49c)
call check doc49c~isA(.table), 1, "comment after colon > is table"
v49c = doc49c["hr"]
call check v49c~isA(.array), 1, "comment after colon > value is array"
call check v49c~items, 2, "comment after colon > 2 items"
call check v49c[1], "Mark", "comment after colon > [1]"
call check v49c[2], "Sammy", "comment after colon > [2]"

/* 49.4 Comment after colon with mapping value on next lines */
yaml49d = "outer: # this is a comment" || "0A"x || "  inner: value"
doc49d = parser49~parseString(yaml49d)
call check doc49d~isA(.table), 1, "comment after colon map > is table"
v49d = doc49d["outer"]
call check v49d~isA(.table), 1, "comment after colon map > value is table"
call check v49d["inner"], "value", "comment after colon map > inner"

/* 49.5 Multiple keys with comments, compact sequences */
yaml49e = "hr: # ranking" || "0A"x || "  - Mark" || "0A"x || "  - Sammy" || "0A"x || -
           "rbi: # ranking" || "0A"x || "  - Sammy" || "0A"x || "  - Ken"
doc49e = parser49~parseString(yaml49e)
call check doc49e~isA(.table), 1, "multi-key comment > is table"
call check doc49e~items, 2, "multi-key comment > 2 entries"
call check doc49e["hr"]~isA(.array), 1, "multi-key comment > hr is array"
call check doc49e["hr"]~items, 2, "multi-key comment > hr 2 items"
call check doc49e["rbi"]~isA(.array), 1, "multi-key comment > rbi is array"
call check doc49e["rbi"]~items, 2, "multi-key comment > rbi 2 items"

/* 49.6 parseAll document count — single doc without markers */
yaml49f = "a:" || "0A"x || "- 1" || "0A"x || "- 2" || "0A"x || "b: 3"
docs49f = parser49~parseAll(yaml49f)
call check docs49f~items, 1, "parseAll > single doc no markers"

/* 49.7 parseAll document count — explicit markers */
yaml49g = "---" || "0A"x || "doc1" || "0A"x || "---" || "0A"x || "doc2"
docs49g = parser49~parseAll(yaml49g)
call check docs49g~items, 2, "parseAll > two docs with ---"

/* 49.8 parseAll document count — doc-end then doc-start */
yaml49h = "doc1" || "0A"x || "..." || "0A"x || "---" || "0A"x || "doc2"
docs49h = parser49~parseAll(yaml49h)
call check docs49h~items, 2, "parseAll > two docs with ... and ---"

/* 49.9 Compact sequence at indent 0 — mapping then sequence value */
yaml49i = "foo:" || "0A"x || "  bar" || "0A"x || "list:" || "0A"x || "- a" || "0A"x || "- b"
doc49i = parser49~parseString(yaml49i)
call check doc49i["foo"], "bar", "compact seq indent 0 > foo"
v49i = doc49i["list"]
call check v49i~isA(.array), 1, "compact seq indent 0 > list is array"
call check v49i~items, 2, "compact seq indent 0 > list 2 items"

say

/*========================================================================*/
say "--- 50. toYaml with MapCollection / OrderedCollection (R5.3) ---"

/* StringTable as map input */
st = .StringTable~new
st["name"] = "Alice"
st["age"]  = "30"
yamlSt = .Yaml~toYaml(st)
docSt  = parser~parseString(yamlSt)
call check docSt["name"], "Alice", "StringTable toYaml > name"
call check docSt["age"], "30", "StringTable toYaml > age"

/* Directory as map input */
dir = .Directory~new
dir["host"] = "localhost"
dir["port"] = "8080"
yamlDir = .Yaml~toYaml(dir)
docDir  = parser~parseString(yamlDir)
call check docDir["host"], "localhost", "Directory toYaml > host"
call check docDir["port"], "8080", "Directory toYaml > port"

/* Queue as sequence input (via wrapper map) */
q = .Queue~new
q~queue("first")
q~queue("second")
q~queue("third")
wrapper = .Table~new
wrapper["items"] = q
yamlQ = .Yaml~toYaml(wrapper)
docQ  = parser~parseString(yamlQ)
call check docQ["items"]~items, 3, "Queue toYaml > count"
call check docQ["items"][1], "first", "Queue toYaml > item 1"
call check docQ["items"][3], "third", "Queue toYaml > item 3"

/* List as sequence input (via wrapper map) */
li = .List~new
li~insert("alpha")
li~insert("beta", li~last)
li~insert("gamma", li~last)
wrapper2 = .Table~new
wrapper2["data"] = li
yamlLi = .Yaml~toYaml(wrapper2)
docLi  = parser~parseString(yamlLi)
call check docLi["data"]~items, 3, "List toYaml > count"
call check docLi["data"][1], "alpha", "List toYaml > item 1"
call check docLi["data"][3], "gamma", "List toYaml > item 3"

/* yamlToXml with StringTable */
xmlSt = .Yaml~yamlToXml(st, "xsd")
call check xmlSt~pos("<mapping>") > 0, 1, "StringTable yamlToXml > has mapping"
call check xmlSt~pos("Alice") > 0, 1, "StringTable yamlToXml > has Alice"

/* yamlToXml with Queue (via wrapper) */
xmlQ = .Yaml~yamlToXml(wrapper, "xsd")
call check xmlQ~pos("<sequence>") > 0, 1, "Queue yamlToXml > has sequence"
call check xmlQ~pos("first") > 0, 1, "Queue yamlToXml > has first"

say

/*========================================================================*/
say copies("=", 64)
say "Results:" pass "/" tests "passed," fail "failed"
say copies("=", 64)

if fail > 0 then exit 1
exit 0

/* ---- internal subroutine (shares prolog variables) ---- */
check: procedure expose tests pass fail
  use arg actual, expected, label
  tests = tests + 1
  if actual = expected then do
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

/* YAML.deepEqual is now a public routine provided by yaml.cls */

::requires "yaml.cls"
