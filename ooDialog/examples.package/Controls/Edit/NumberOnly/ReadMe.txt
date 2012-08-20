
                          NumberOnlyEditEx Class
                          =====================

  The NumberOnlyEditEx class extends the .Edit class to provide an edit
  control which restricts entry to decimal only, or signed decimal only,
  numbers.

  The NumberOnlyEditEx.cls file is meant to be included in any ooDialog
  program where this extension to the edit control is desired.  The
  programmer needs to take 4 steps to gain the functionality of a signed
  decimal number only edit control:

  1.) Require the NumberOnlyEditEx.cls file (this file.)

    ::requires 'NumberOnlyEditEx.cls'

  2.) Invoke the initDecimalOnly() method on each edit control object that
  is to be a decimal number only edit control.  If initDecimalOnly() is not
  invoked, the behaviour of the edit control unchanged.

    editObject = self~newEdit(IDC_EDIT)
    editObject~initDecimalOnly(decimalPlaces, allowSign)

    arguments:

    decimalPlaces  [optional] The number of decimal places allowed in the
                              number.  The default is 2.  Specifying 0
                              decimal places is accepted.

    allowSign      [optional] If a sign character (+ or -) is allowed as the
                              leading character of the number.  The default
                              is .false.  Specify .true to allow a sign
                              character.

  3.) For each decimal only edit control, the character event must be
  connected to a method in the Rexx dialog object.

    editObject = self~newEdit(IDC_EDIT)
    editObject~connectCharEvent(onChar)

  4.) In the connected character event handler, the message and its
  arguments must be forwarded on to the onChar() method of the edit control.
  The 6th argument to the event handler is the dialog control object where
  the character event occurred.  That is the object the event must be
  forwared to.

  When the programmer connects the character event to a method named onChar,
  the event handler is simple:

      ::method onChar unguarded
        forward to (arg(6))

  If the programmer chooses a different method name for the event handler,
  then he must be sure to forward to the onChar() method of the edit
  control:

      ::method myOwnMethodName unguarded
        forward  message 'onChar' to (arg(6))

  And of course there is no reason why the event handler can not be coded
  using the explicit arguments:

      ::method myOwnMethodName unguarded
        use arg char, isShift, isCtrl, isAlt, misc, control
        forward  message 'onChar' to (control)



  The accompanying program, CalcMPG.rex, in the ooDialog Extra Samples
  package demonstrates the usage as outlined above.

  Note:  The NumberOnlyEditEx class and NumberOnlyEditEx.cls file is meant
         to be used by programmers to improve their ooDialog programs.

         In addition, the file provides the technique to create any type of
         restricted input edit control.

