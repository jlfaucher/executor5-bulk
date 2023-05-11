Parse Arg in out _rexx_build _rexx_target
Call Stream out, 'C', 'OPEN WRITE REPLACE'
Do While Lines( in ) > 0
   line = Linein( in )
   line = Changestr( '%REXX_BUILD%', line, _rexx_build )
   line = Changestr( '%REXX_TARGET%', line, _rexx_target )
   Call Lineout out, line
End
Call Stream out, 'C', 'CLOSE'
