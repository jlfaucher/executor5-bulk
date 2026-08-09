# Experimental ooRexx 5

Forked from [https://sourceforge.net/p/oorexx/code-0/HEAD/tree][sourceforge].

Unlike [Executor][executor] and [Executor5][executor5], this repository includes everything from
[SourceForge][sourceforge]: trunk, branches, releases, docs, tests, incubator, and sandboxes.


## Documentation:

- [Diary][diary]
- [Instructions to build Executor5-bulk][build_executor5_bulk]
- [Instructions to build ICU4ooRexx][build_icu4oorexx] (optional)


## Extensions:

- [RxMessageBox][rxmessagebox]
- [String~translateInto][string_translateinto]
- [Unicode support][unicode_support]


## ooRexx 5 bugs fixed but not yet backported:

- [[bugs:#2066](https://sourceforge.net/p/oorexx/bugs/2066/)] ADDRESS with input, child process blocked on stdin when stdin is not ready.  
[Diary](https://github.com/jlfaucher/executor5-bulk/blob/5522b7d0a1e5d1cbbb25f096735334f86c39a056/_diary.txt#L1793-L1795).


## ooRexx 5 feature requests implemented but not yet backported:

- [[feature-requests:#868](https://sourceforge.net/p/oorexx/feature-requests/868/)] Better support of EOF in rexxtry.  
[Diary](https://github.com/jlfaucher/executor5-bulk/blob/5522b7d0a1e5d1cbbb25f096735334f86c39a056/_diary.txt#L2976-L2987).

- [[feature-requests:#867](https://sourceforge.net/p/oorexx/feature-requests/867/)] Determine whether a stream is associated with a terminal.  
[Documentation][stream_istty].  
[Diary](https://github.com/jlfaucher/executor5-bulk/blob/5522b7d0a1e5d1cbbb25f096735334f86c39a056/_diary.txt#L2993-L3105).


<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<div align="center">
<img src="images/Executor5-bulk_mascot_wbg.png" alt="Executor5-bulk mascot" width="60%">
</div>

[build_executor5_bulk]: https://github.com/jlfaucher/builder/blob/master/build-executor5-bulk.txt
[build_icu4oorexx]: https://github.com/jlfaucher/builder/blob/master/build-ICU4ooRexx.txt
[diary]: https://github.com/jlfaucher/executor5-bulk/blob/main/_diary.txt "Diary"
[executor]: https://github.com/jlfaucher/executor "Executor"
[executor5]: https://github.com/jlfaucher/executor5 "Executor5"
[rxmessagebox]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/rexxutil/rxmessagebox.html "RxMessageBox"
[sourceforge]: https://sourceforge.net/p/oorexx/code-0/HEAD/tree "SourceForge"
[stream_istty]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/stream/stream_istty.html "stream~query('isTTY')"
[string_translateinto]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/string/translateInto.html "String~translateInto"
[unicode_support]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/README.html
