/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/******************************************************************************/
/*                                                                            */
/* Main routine for interpreter platform independent error message support    */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"                    /* incl general definitions        */
#include "StringClass.hpp"
#include "RexxMessageNumbers.h"        /* include  definition of errorcodes */
#include "Interpreter.hpp"

                                       /* define macros to bulid entries in */
                                       /* the msgEntry table for msg lookup */
#define MAJOR(code)   code, code##_msg,/* Major error codes                 */
#define MINOR(code)   code, code##_msg,/* Minor error codes (sub-codes)     */

ErrorMessage RexxInterpreter::messageMappingTable[] =
{
#include "RexxMessageTable.h"          /* include actual table definition   */
};

wholenumber_t RexxInterpreter::messageNumber(
    RexxString *errorcode)             /* REXX error code as string         */
/******************************************************************************/
/* Function:  Parse out the error code string into the messagecode valuey     */
/******************************************************************************/
{
  stringchar_t *decimalPoint;          /* location of decimalPoint in errorcode*/
  wholenumber_t primary;               /* Primary part of error code, major */
  wholenumber_t secondary;             /* Secondary protion (minor code)    */
  stringsize_t count;

                                       /* make sure we get errorcode as str */
  errorcode = (RexxString *)errorcode->stringValue();
                                       /* scan to decimal Point or end of   */
                                       /* error code.                       */
  for (decimalPoint = errorcode->getStringData(), count = 0; *decimalPoint && *decimalPoint != '.'; decimalPoint++, count++);
                                       /* get the primary portion of code   */
  if (!(new_string(errorcode->getStringData(), count)->numberValue(&primary, 9)))
  {
                                       /* Yes, raise an error.              */
      reportException(Error_Expression_result_raise);
  }
  // and normalize the number
  primary *= 1000;
                                       /* did major code compute to long    */
                                       /* and within range                  */
  if (primary < 1 || primary >= 100000) {
                                       /* Nope raise an error.              */
    reportException(Error_Expression_result_raise);
  }

  if (*decimalPoint) {                 /* Was there a decimal point specified?*/
                                       /* Yes, compute its decimal value.   */
    if (!new_string(decimalPoint + 1, errorcode->getLength() - count -1)->numberValue(&secondary, 9) ||
                                       /* is the subcode invalid or too big?*/
        secondary < 0  || secondary >= 1000) {
                                       /* Yes, raise an error.              */
      reportException(Error_Expression_result_raise);
    }
  }
  else {
    secondary = 0;
  }
  return primary + secondary;          /* add two protions together, return */
}

RexxString * RexxInterpreter::messageHeader(
    wholenumber_t code )               /* message code to extract           */
/****************************************************************************/
/* Function:  Return a message header derived from the in-store error table */
/****************************************************************************/
{
 ErrorMessage  *p;                     /* table scan pointer                */
 int            msgid;                 /* message number                    */
 char           dataArea[20];          /* buf addr to return message        */
                                       /* loop through looking for the      */
                                       /* error code                        */
 for (p = messageMappingTable; p->code != 0; p++)
 {
   if (p->code == code) {              /* found the target code?            */
     msgid = p->msgid;                 /* get msg number associated w/ error*/
                                       /* format as a message header        */
     sprintf(dataArea, "REX%4.4dE: ", msgid);
     return new_string(dataArea);     /* return as a string object         */
   }
 }
 return OREF_NULL;                     /* no message retrieved              */
}

RexxString *RexxInterpreter::messageSubstitution(
    RexxString *message,               /* REXX error message                */
    RexxArray  *additional )           /* substitution information          */
/******************************************************************************/
/* Function:  Perform any required message substitutions on the secondary     */
/*            error message.                                                  */
/******************************************************************************/
{
  size_t      substitutions;           /* number of substitutions           */
  stringsize_t subposition;            /* substitution position             */
  stringsize_t i;                      /* loop counter                      */
  stringsize_t selector;               /* substitution position             */
  RexxString *newmessage;              /* resulting new error message       */
  RexxString *front;                   /* front message part                */
  RexxString *back;                    /* back message part                 */
  RexxObject *value;                   /* substituted message value         */
  RexxString *stringValue;             /* converted substitution value      */
  bool     isDBCS;

  substitutions = additional->size();  /* get the substitution count        */
  newmessage = OREF_NULLSTRING;        /* start with a null string          */
                                       /* loop through and substitute values*/
  for (i = 1; i <= substitutions; i++) {
                                       /* search for a substitution         */
    isDBCS = RexxInterpreter::currentSettings->exmode; /* save EXMODE setting */
    RexxInterpreter::currentSettings->exmode = false;  /* don't use DBCSpos */
    subposition = message->pos(OREF_AND, 0);
    RexxInterpreter::currentSettings->exmode = isDBCS; /* restore EXMODE setting */
    if (subposition == 0)              /* not found?                        */
      break;                           /* get outta here...                 */
                                       /* get the leading part              */
    front = message->extract(0, subposition - 1);
                                       /* pull off the remainder            */
    back = message->extract(subposition + 1, message->getLength() - (subposition + 1));
                                       /* get the descriptor position       */
    selector = message->getChar(subposition);
                                       /* not a good number?                */
    if (selector < '0' || selector > '9')
                                       /* use a default message             */
      stringValue = new_string("<BAD MESSAGE>"); /* must be stringValue, not value, otherwise trap */
    else {
      selector -= '0';                 /* convert to a number               */
      if (selector > substitutions)    /* out of our range?                 */
        stringValue = OREF_NULLSTRING; /* use a null string                 */
      else {                           /* get the indicated selector value  */
        value = additional->get(selector);
        if (value != OREF_NULL) {      /* have a value?                     */
                                       /* set the reentry flag              */
          currentActivity->setRequestingString(true);
          try
          {
                                       /* force to character form           */
              stringValue = value->stringValue();
          }
          catch (ActivityException)
          {
              stringValue = value->defaultName();
          }
          currentActivity->setRequestingString(false);
        }
        else
                                       /* use a null string                 */
          stringValue = OREF_NULLSTRING;
      }
    }
                                       /* accumulate the front part         */
    newmessage = newmessage->concat(front->concat(stringValue));
    message = back;                    /* replace with the remainder        */
  }
                                       /* add on any remainder              */
  newmessage = newmessage->concat(message);
  return newmessage;                   /* return the message                */
}


/**
 * Decode a condition directory into a easier to use
 * structure form for a native code user.  This breaks
 * the directory into its component pieces, including
 * converting values into primitive form using just a single
 * API call.
 *
 * @param conditionObj
 *               A directory object containing the condition information.
 * @param pRexxCondData
 *               The condition data structure that is populated with the
 *               condition information.
 */
void RexxInterpreter::decodeConditionData(RexxDirectory *conditionObj, ConditionData *condData)
{
    memset(condData, 0, sizeof(ConditionData));
    condData->code = RexxInterpreter::messageNumber((RexxString *)conditionObj->at(OREF_CODE));

    condData->rc = RexxInterpreter::messageNumber((RexxString *)conditionObj->at(OREF_RC));
    condData->conditionName = (RexxStringObject)conditionObj->at(OREF_CONDITION);

    RexxObject *temp = conditionObj->at(OREF_NAME_MESSAGE);
    if (temp != TheNilObject)
    {
        condData->message = (RexxStringObject)temp;
    }

    temp = conditionObj->at(OREF_ERRORTEXT);
    if (temp != TheNilObject)
    {
        condData->errortext = (RexxStringObject)temp;
    }

    temp = conditionObj->at(OREF_DESCRIPTION);
    if (temp != TheNilObject)
    {
        condData->description = (RexxStringObject)temp;
    }

    condData->position = ((RexxInteger *)(conditionObj->at(OREF_POSITION)))->wholeNumber();

    temp = conditionObj->at(OREF_PROGRAM);
    if (temp != TheNilObject)
    {
        condData->program = (RexxStringObject)temp;
    }

    temp = conditionObj->at(OREF_ADDITIONAL);
    if (temp != TheNilObject)
    {
        condData->additional = (RexxArrayObject)temp;
    }
}
