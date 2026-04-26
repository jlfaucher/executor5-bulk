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

  Signal On Syntax Name SyntaxHandler

  Parse Arg dir doJson .
  dir = Strip(dir, "Both", '"')

  If dir == "" Then Do
    Say "Usage: rexx test_yaml-single.rex <test-dir> [json]"
    Exit 3
  End

  isError = SysFileExists(dir"/error")

  -- Read in.yaml
  inYaml = ReadFile(dir"/in.yaml")
  If inYaml == .nil Then Do
    Say "SKIP: cannot read in.yaml"
    Exit 3
  End

  -- Try to parse
  parseOk = .true
  parseResult = .nil
  Signal On Syntax Name ParseFailed
  yaml = .Yaml~new
  parseResult = yaml~parseAll(inYaml)
  Signal Off Syntax
  Signal AfterParse

ParseFailed:
  parseOk = .false
  Signal Off Syntax

AfterParse:

  If isError Then Do
    If \parseOk Then Do
      Say "PASS"
      Exit 0
    End
    Else Do
      Say "FAIL: expected error, parsed OK"
      Exit 1
    End
  End
  Else Do
    If parseOk Then Do
      -- JSON comparison?
      If doJson == "json" Then Do
        jsonFile = dir"/in.json"
        If SysFileExists(jsonFile) Then Do
          jsonContent = ReadFile(jsonFile)
          If jsonContent \== .nil Then Do
            Signal On Syntax Name JsonFailed
            jsonValues = SplitJsonValues(jsonContent)
            If jsonValues~items \== parseResult~items Then Do
              Say "JSON_FAIL: doc count mismatch YAML="parseResult~items "JSON="jsonValues~items
              Exit 2
            End
            Do i = 1 To parseResult~items
              expected = .JSON~fromJSON(jsonValues[i])
              If \DeepEqual(parseResult[i], expected) Then Do
                Say "JSON_FAIL: doc" i "value mismatch"
                Exit 2
              End
            End
            Signal Off Syntax
            Say "PASS+JSON"
            Exit 0
          End
        End
        -- No in.json available
        Say "PASS"
        Exit 0
      End
      Else Do
        Say "PASS"
        Exit 0
      End
    End
    Else Do
      Say "FAIL: unexpected parse error"
      Exit 1
    End
  End

  Exit 0

SyntaxHandler:
  co = Condition("O")
  Say "ERROR:" co~message
  Exit 3

JsonFailed:
  co = Condition("O")
  Say "JSON_FAIL:" co~message
  Exit 2

--------------------------------------------------------------------------------
-- ReadFile                                                                   --
--------------------------------------------------------------------------------

::routine ReadFile
  Use Strict Arg path
  If \SysFileExists(path) Then Return .nil
  s = .Stream~new(path)~~open("READ SHARED")
  If s~state \== "READY" Then Return .nil
  content = s~charIn(, s~chars)
  s~close
  Return content

--------------------------------------------------------------------------------
-- SplitJsonValues                                                            --
--------------------------------------------------------------------------------

::routine SplitJsonValues
  Use Strict Arg content

  values = .array~new
  lines = content~changeStr("0D0A"x, "0A"x)~changeStr("0D"x, "0A"x) -
    ~makeArray("0A"x)

  mb = .mutableBuffer~new
  depth = 0
  inString = .false

  Do i = 1 To lines~items
    line = lines[i]

    If line~strip == "" & depth == 0 & mb~string~strip == "" Then Iterate

    If mb~length > 0 Then mb~append("0A"x)
    mb~append(line)

    Do j = 1 To line~length
      ch = line[j]
      If inString Then Do
        If ch == "\" Then Do
          j += 1
          Iterate
        End
        If ch == '"' Then inString = .false
      End
      Else Do
        Select Case ch
          When '"' Then inString = .true
          When "{", "[" Then depth += 1
          When "}", "]" Then depth -= 1
          Otherwise Nop
        End
      End
    End

    If depth == 0 & mb~string~strip \== "" Then Do
      values~append(mb~string)
      mb = .mutableBuffer~new
    End
  End

  If mb~string~strip \== "" Then values~append(mb~string)

  Return values

--------------------------------------------------------------------------------
-- DeepEqual                                                                  --
--------------------------------------------------------------------------------

::routine DeepEqual
  Use Strict Arg yamlVal, jsonVal

  If yamlVal == .nil & jsonVal == .nil Then Return .true
  If yamlVal == .nil | jsonVal == .nil Then Return .false

  If yamlVal~isA(.YamlTagged) Then yamlVal = yamlVal~value

  yIsMap = yamlVal~isA(.Table) | yamlVal~isA(.StringTable) | yamlVal~isA(.Directory)
  jIsMap = jsonVal~isA(.Table) | jsonVal~isA(.StringTable) | jsonVal~isA(.Directory)
  If yIsMap & jIsMap Then Do
    If yamlVal~items \== jsonVal~items Then Return .false
    sup = yamlVal~supplier
    Do While sup~available
      key = sup~index
      If \jsonVal~hasIndex(key) Then Return .false
      If \DeepEqual(sup~item, jsonVal[key]) Then Return .false
      sup~next
    End
    Return .true
  End

  If yamlVal~isA(.Array) & jsonVal~isA(.Array) Then Do
    If yamlVal~items \== jsonVal~items Then Return .false
    Do i = 1 To yamlVal~items
      If \DeepEqual(yamlVal[i], jsonVal[i]) Then Return .false
    End
    Return .true
  End

  Return yamlVal = jsonVal

::requires "yaml.cls"
::requires "json.cls"
