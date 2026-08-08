<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/stream/stream_istty.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/stream/stream_istty.html">HTML</a>
</p>

# Stream class

## query("isTTY")

[RFE #867][rfe_867] Determine whether a stream is associated with a terminal

I use `isTTY` instead of `isaTerminal`, following these recommendations:

- [https://claude.ai/share/ad054966-a3fa-4426-91e6-a57faee3504a][claude_naming_recommendation]
- [https://chatgpt.com/share/699b057b-b7dc-8007-8d04-0a1b546cef56][chatgpt_naming_recommendation]

Added support for

- `stream~query("isTTY")`
- `stream~command("query isTTY")`
- `stream("aStream", "Command", "Query isTTY")`
returning `"YES"` or `"NO"` or `"UNKNOWN"`

### Examples

```rexx
say .stdout~query("isTTY")                        -- "YES"
say .stdout~command("Query isTTY")                -- "YES"
say stream("STDOUT", "Command", "Query isTTY")    -- "YES"
rexx -e "say .stdin~query('isTTY')"               -- "YES"
echo "test" | rexx -e "say .stdin~query('isTTY')" -- "NO"
```

### Validation

[`scripts/stream/stream_isatty_test`][stream_isatty_test]  
[`scripts/stream/stream_isatty_test.bat`][stream_isatty_test_bat]

```
Output on macOS:                                                            Output on Windows:
cd scripts/stream/                                                          cd scripts\stream\
./stream_isatty_test                                                        stream_isatty_test.bat

    output=stream_istty_test-out.txt                                            set output=stream_istty_test-out.txt

    rexx stream_istty.rex "$output"                                             rexx stream_istty.rex %output%
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

    rexx stream_istty.rex "$output" 1> /dev/stdout                              rexx stream_istty.rex %output% 1> CON
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

    rexx stream_istty.rex "$output" 2> /dev/stdout                              rexx stream_istty.rex %output% 2> CON
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

                                                              *****************
                                                              * Difference #1 *
                                                              *****************
    rexx stream_istty.rex "$output" dont_stdout 1> /dev/null                    rexx stream_istty.rex %output% dont_stdout 1> NUL
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: NO                                     stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

                                                              *****************
                                                              * Difference #2 *
                                                              *****************
    rexx stream_istty.rex "$output" 2> /dev/null                                rexx stream_istty.rex %output% 2> NUL
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: NO                                     stderr streamType: TRANSIENT  isTTY: YES

    echo test | rexx stream_istty.rex "$output"                                 echo test | rexx stream_istty.rex %output%
         stdin  streamType: TRANSIENT  isTTY: NO                                     stdin  streamType: TRANSIENT  isTTY: NO
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

                                                                                :: Executed manually from CMD because must press ENTER
    echo test | cat /dev/stdin | rexx stream_istty.rex "$output"                echo test | type CON | rexx stream_istty.rex %output%
         stdin  streamType: TRANSIENT  isTTY: NO                                     stdin  streamType: TRANSIENT  isTTY: NO
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES

    rexx stream_istty.rex "$output" < /dev/stdin                                rexx stream_istty.rex %output% < CON
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

    echo test > in.tmp.txt && rexx stream_istty.rex "$output" < in.tmp.txt      echo test > in.tmp.txt && rexx stream_istty.rex %output% < in.tmp.txt
         stdin  streamType: PERSISTENT isTTY: NO                                     stdin  streamType: PERSISTENT isTTY: NO
         stdout streamType: TRANSIENT  isTTY: YES                                    stdout streamType: TRANSIENT  isTTY: YES
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

    rexx stream_istty.rex "$output" > out.tmp.txt && cat out.tmp.txt            rexx stream_istty.rex %output% > out.tmp.txt && type out.tmp.txt
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: PERSISTENT isTTY: NO                                     stdout streamType: PERSISTENT isTTY: NO
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

    rexx stream_istty.rex "$output" | cat                                       rexx stream_istty.rex %output% | findstr "^"
         stdin  streamType: TRANSIENT  isTTY: YES                                    stdin  streamType: TRANSIENT  isTTY: YES
         stdout streamType: TRANSIENT  isTTY: NO                                     stdout streamType: TRANSIENT  isTTY: NO
         stderr streamType: TRANSIENT  isTTY: YES                                    stderr streamType: TRANSIENT  isTTY: YES

                                                              *****************
                                                              * Difference #3 *
                                                              *****************
    rexx -e "say .stream~new('/dev/null')~~open('read')~query('isTTY')"         rexx -e "say .stream~new('NUL')~~open('read')~query('isTTY')"
         UNKNOWN                                                                     YES
```

#### Difference #1 and #2

Windows.
`_isatty` returns non-zero for `NUL` ==> `isTTY == YES`, should be `NO`.  
This is a known problem:

- [https://stackoverflow.com/questions/3648711/detect-nul-file-descriptor-isatty-is-bogus](https://stackoverflow.com/questions/3648711/detect-nul-file-descriptor-isatty-is-bogus)
- [https://github.com/pallets/click/pull/1135](https://github.com/pallets/click/pull/1135)


#### Difference #3

Bug `ooRexx` on macOS and Linux.  
Error when opening `/dev/null` read-only ==> `isTTY == UNKNOWN`  
No error when opening `/dev/null` write-only or read-write ==> `isTTY == NO`.

More details in [`_notes.txt`][_notes_txt].


### Alternative implementations

#### Windows

An alternative implementation based on Windows-specific services is used in [`rexxtry.rex`][rexxtry_alternative_implementation]
to avoid introducing a dependency on a new feature.

```rexx
isaTerminal:                                     /* For Windows only          */
  use arg streamName
  if streamName <> "stdin" then return 0         /* Works only for stdin      */
  "timeout 0 1>nul 2>nul"
  terminal = (RC == 0)
  return terminal
```

#### macOS, Linux

This alternative implementation is currently not needed in `rexxtry`.

```rexx
isaTerminal:
  use arg streamName
  select
      when streamName == "stdin"  then "[ -t 0 ]"
      when streamName == "stdout" then "[ -t 1 ]"
      when streamName == "stderr" then "[ -t 2 ]"
      otherwise return 0 -- not supported
  end
  terminal = (RC == 0)
  return terminal
```


[_notes_txt]: https://github.com/jlfaucher/executor5-bulk/blob/927eef74d67854562d6b422910c8ef0687c4081e/_notes.txt#L2 "_notes.txt"
[chatgpt_naming_recommendation]: https://chatgpt.com/share/699b057b-b7dc-8007-8d04-0a1b546cef56 "ChatGPT naming recommendation"
[claude_naming_recommendation]: https://claude.ai/share/ad054966-a3fa-4426-91e6-a57faee3504a "Claude naming recommendation"
[rexxtry_alternative_implementation]: https://github.com/jlfaucher/executor5-bulk/blob/927eef74d67854562d6b422910c8ef0687c4081e/main/trunk/samples/rexxtry.rex#L244-L254 "rexxtry workaround"
[rfe_867]: https://sourceforge.net/p/oorexx/feature-requests/867/ "RFE 867" 
[stream_isatty_test]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/stream/stream_istty_test "stream_isatty_test"
[stream_isatty_test_bat]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/stream/stream_istty_test.bat "stream_isatty_test.bat"