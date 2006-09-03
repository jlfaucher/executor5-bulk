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
/*                                                                            */
/* Non-system specific utility functions.                                     */
/*                                                                            */
/******************************************************************************/

#include "oorexx.h"
#include "math.h"
#include <stdio.h>

/*********************************************************************/
/*  Various definitions used by the math functions                   */
/*********************************************************************/
#define SINE        0                  /* trig function defines...   */
#define COSINE      3                  /* the ordering is important, */
#define TANGENT     1                  /* as these get transformed   */
#define COTANGENT   2                  /* depending on the angle     */
#define MAXTRIG     3                  /* value                      */
#define ARCSINE     0                  /* defines for arc trig       */
#define ARCCOSINE   1                  /* functions.  Ordering is    */
#define ARCTANGENT  2                  /* not as important here      */


#define pi  3.14159265358979323846l    /* pi value                   */

#define DEFAULT_PRECISION  9           /* default precision to use   */
#define MAX_PRECISION     16           /* maximum available precision*/


/*********************************************************************/
/* Defines used by SysStemSort -- must match values in okstem.hpp    */
/*********************************************************************/
#define SORT_CASESENSITIVE 0
#define SORT_CASEIGNORE    1

#define SORT_ASCENDING 0
#define SORT_DECENDING 1


/*************************************************************************
* Function:  SysDropFuncs                                                *
*                                                                        *
* Syntax:    call SysDropFuncs                                           *
*                                                                        *
* Return:    This is a NOP for REXXUTIL.  These are not droppable anymore*
*************************************************************************/
RexxMethod0(CSTRING, SysDropFuncs)
{
  return "";                           /* no error on call           */
}


/*************************************************************************
* Function:  SysLoadFuncs                                                *
*                                                                        *
* Syntax:    call SysLoadFuncs                                           *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    null string                                                 *
*************************************************************************/

RexxFunction0(CSTRING, SysLoadFuncs)
{
    // these are not loaded from a separate DLL any more, so this function
    // is now a NOP.
    return "";
}


/*************************************************************************
* Function:  SysAddRexxMacro                                             *
*                                                                        *
* Syntax:    result = SysAddRexxMacro(name, file, <order>)               *
*                                                                        *
* Params:    name   - loaded name of the macro file                      *
*            file   - file containing the loaded macro                   *
*            order  - Either 'B'efore or 'A'fter.  The default is 'B'    *
*                                                                        *
* Return:    return code from RexxAddMacro                               *
*************************************************************************/

RexxFunction3(int, SysAddRexxMacro, CSTRING, name, CSTRING, file, OPTIONAL_CSTRING, option)
{
  RexxUnsignedNumber position;         /* added position             */

  position = RXMACRO_SEARCH_BEFORE;    /* set default search position*/
  if (option != NULL)                  /* have an option?            */
  {
      switch (*option)
      {
          case 'B':     // 'B'efore
          case 'b':
              position = RXMACRO_SEARCH_BEFORE;
              break;

          case 'A':     // 'A'fter
          case 'a':
              position = RXMACRO_SEARCH_AFTER;
              break;

          default:
              context->InvalidRoutine();
              return 0;
      }
  }
                                       /* try to add the macro       */
  return (int)ooRexxAddMacro(name, file, position);
}

/*************************************************************************
* Function:  SysReorderRexxMacro                                         *
*                                                                        *
* Syntax:    result = SysReorderRexxMacro(name, order)                   *
*                                                                        *
* Params:    name   - loaded name of the macro file                      *
*            order  - Either 'B'efore or 'A'fter.                        *
*                                                                        *
* Return:    return code from RexxReorderMacro                           *
*************************************************************************/

RexxFunction2(int, SysReorderRexxMacro, CSTRING, name, CSTRING, option)
{
  RexxUnsignedNumber  position;        /* added position             */

  switch (*option)
  {
      case 'B':     // 'B'efore
      case 'b':
          position = RXMACRO_SEARCH_BEFORE;
          break;

      case 'A':     // 'A'fter
      case 'a':
          position = RXMACRO_SEARCH_AFTER;
          break;

      default:
          context->InvalidRoutine();
          return 0;
  }
  return (int)ooRexxReorderMacro(name, position);
}

/*************************************************************************
* Function:  SysDropRexxMacro                                            *
*                                                                        *
* Syntax:    result = SysDropRexxMacro(name)                             *
*                                                                        *
* Params:    name   - name of the macro space function                   *
*                                                                        *
* Return:    return code from RexxDropMacro                              *
*************************************************************************/

RexxFunction1(int, SysDropRexxMacro, CSTRING, name)
{
   return (int)ooRexxDropMacro(name);
}

/*************************************************************************
* Function:  SysQueryRexxMacro                                           *
*                                                                        *
* Syntax:    result = SysQueryRexxMacro(name)                            *
*                                                                        *
* Params:    name   - name of the macro space function                   *
*                                                                        *
* Return:    position of the macro ('B' or 'A'), returns null for errors.*
*************************************************************************/

RexxFunction1(CSTRING, SysQueryRexxMacro, CSTRING, name)
{
  RexxUnsignedNumber position;         /* returned position          */

  if (ooRexxQueryMacro(name, &position) != 0)
  {
      return "";
  }
  // before?
  if (position == RXMACRO_SEARCH_BEFORE)
      return "B";
  else
      return "A";                    /* must be 'A'fter            */
}

/*************************************************************************
* Function:  SysClearRexxMacroSpace                                      *
*                                                                        *
* Syntax:    result = SysClearRexxMacroSpace()                           *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    return code from RexxClearMacroSpace()                      *
*************************************************************************/

RexxFunction0(int, SysClearRexxMacroSpace)
{
  return (int)ooRexxClearMacroSpace();          /* clear the macro space      */
}

/*************************************************************************
* Function:  SysSaveRexxMacroSpace                                       *
*                                                                        *
* Syntax:    result = SysSaveRexxMacroSpace(file)                        *
*                                                                        *
* Params:    file   - name of the saved macro space file                 *
*                                                                        *
* Return:    return code from RexxSaveMacroSpace()                       *
*************************************************************************/

RexxFunction1(int, SysSaveRexxMacroSpace, CSTRING, file)
{
  return (int)ooRexxSaveMacroSpace(0, NULL, file);
}

/*************************************************************************
* Function:  SysLoadRexxMacroSpace                                       *
*                                                                        *
* Syntax:    result = SysLoadRexxMacroSpace(file)                        *
*                                                                        *
* Params:    file   - name of the saved macro space file                 *
*                                                                        *
* Return:    return code from RexxLoadMacroSpace()                       *
*************************************************************************/

RexxFunction1(int, SysLoadRexxMacroSpace, CSTRING, file)
{
  return (int)ooRexxLoadMacroSpace(0, NULL, file);
}


// simple class for handling numeric values
class NumericFormatter
{
public:
    NumericFormatter(RexxCallContext *c, uint32_t p)
    {
        if (p == 0)
        {
            precision = DEFAULT_PRECISION;
        }
        else
        {
            precision = min(p, MAX_PRECISION);
        }
        context = c;
    }

    RexxObjectPtr format(double x)
    {
        return context->DoubleToObjectWithPrecision(x, precision);
    }

protected:
    uint32_t precision;
    RexxCallContext *context;
};

class TrigFormatter : public NumericFormatter
{
public:

    TrigFormatter(RexxCallContext *c, int p) : NumericFormatter(c, p)
    {
        units = DEGREES;
    }

    bool setUnits(char *u)
    {
        if (u != NULL)
        {
            switch (*u)
            {
                case 'D':
                case 'd':
                    units = DEGREES;
                    break;

                case 'R':
                case 'r':
                    units = RADIANS;
                    break;

                case 'G':
                case 'g':
                    units = GRADES;
                    break;

                default:
                    context->InvalidRoutine();
                    return false;
            }
        }
        return true;
    }

    RexxObjectPtr evaluate(double angle, int function)
    {
        double    nsi;                       /* convertion factor          */
        double    nco;                       /* convertion factor          */
        double    result;                    /* result                     */

        nsi = 1.;                            /* set default conversion     */
        nco = 1.;                            /* set default conversion     */

        switch (units)
        {
            case DEGREES:         {            /* need to convert degrees    */
                    nsi = (angle < 0.) ? -1. : 1.;   /* get the direction          */
                    angle = fmod(fabs(angle), 360.); /* make modulo 360            */
                    if (angle <= 45.)                /* less than 45?              */
                        angle = angle * pi / 180.;
                    else if (angle < 135.)
                    {         /* over on the other side?    */
                        angle = (90. - angle) * pi / 180.;
                        function = MAXTRIG - function; /* change the function        */
                        nco = nsi;                     /* swap around the conversions*/
                        nsi = 1.;
                    }
                    else if (angle <= 225.)
                    {        /* around the other way?      */
                        angle = (180. - angle) * pi / 180.;
                        nco = -1.;
                    }
                    else if (angle < 315.)
                    {         /* close to the origin?       */
                        angle = (angle - 270.) * pi / 180.;
                        function = MAXTRIG - function; /* change the function        */
                        nco = -nsi;
                        nsi = 1.;
                    }
                    else
                        angle = (angle - 360.) * pi / 180.;
                    break;
                }

            case GRADES:              {        /* need to convert degrees    */
                    nsi = (angle < 0.) ? -1. : 1.;   /* get the direction          */
                    angle = fmod(fabs(angle), 400.); /* make modulo 400            */
                    if (angle <= 50.)
                        angle = angle * pi / 200.;
                    else if (angle < 150.)
                    {
                        angle = (100. - angle) * pi / 200.;
                        function = MAXTRIG - function; /* change the function        */
                        nco = nsi;                     /* swap the conversions       */
                        nsi = 1.;
                    }
                    else if (angle <= 250.)
                    {
                        angle = (200. - angle) * pi / 200.;
                        nco = -1.;
                    }
                    else if (angle < 350.)
                    {
                        angle = (angle - 300.) * pi / 200.;
                        function = MAXTRIG - function; /* change the function        */
                        nco = -nsi;
                        nsi = 1.;
                    }
                    else
                        angle = (angle - 400.) * pi / 200.;
                    break;
                }

            // radians are already ok
            case RADIANS:
                break;
        }
        switch (function) {                /* process the function       */
          case SINE:                       /* Sine function              */
            result = nsi * sin(angle);
            break;
          case COSINE:                     /* Cosine function            */
            result = nco * cos(angle);
            break;
          case TANGENT:                    /* Tangent function           */
            result = nsi * nco * tan(angle);
            break;
          case COTANGENT:                  /* cotangent function         */
                                           /* overflow?                  */
            if ((result = tan(angle)) == 0.0)
            {
                context->InvalidRoutine();
                return NULLOBJECT;
            }
            result = nsi * nco / result; /* real result                */
            break;
        }

        // now format based on precision setting
        return format(result);
    }

    RexxObjectPtr evaluateArc(double x, int function)
    {
        double    angle;                     /* working angle              */
        double    nsi;                       /* convertion factor          */
        double    nco;                       /* convertion factor          */

        nsi = 1.;                            /* set default conversion     */
        nco = 1.;                            /* set default conversion     */

        switch (function) {                /* process the function       */
          case ARCSINE:                    /* ArcSine function           */
            angle = asin(x);
            break;
          case ARCCOSINE:                  /* ArcCosine function         */
            angle = acos(x);
            break;
          case ARCTANGENT:                 /* ArcTangent function        */
            angle = atan(x);
            break;
        }
        if (units == DEGREES)              /* have to convert the result?*/
          angle = angle * 180. / pi;       /* make into degrees          */
        else if (units == GRADES)          /* need it in grades?         */
          angle = angle * 200. / pi;       /* convert to base 400        */
        // now format based on precision setting
        return format(angle);
    }

protected:

    typedef enum
    {
        DEGREES,
        RADIANS,
        GRADES
    } Units;


    Units units;              // the type of units to process
};


/********************************************************************/
/* Functions:           SysSqrt(), SysExp(), SysLog(), SysLog10,    */
/* Functions:           SysSinH(), SysCosH(), SysTanH()             */
/* Description:         Returns function value of argument.         */
/* Input:               One number.                                 */
/* Output:              Value of the function requested for arg.    */
/*                      Returns 0 if the function executed OK,      */
/*                      40 otherwise.  The interpreter will fail    */
/*                      if the function returns a negative result.  */
/* Notes:                                                           */
/*   These routines take one to two parameters.                     */
/*   The form of the call is:                                       */
/*   result = func_name(x <, prec> <,angle>)                        */
/*                                                                  */
/********************************************************************/
RexxFunction2(RexxObjectPtr, SysSqrt, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(sqrt(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysExp, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(exp(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysLog, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(log(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysLog10, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(log10(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysSinH, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(sinh(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysCosH, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(cosh(x));
}


/*==================================================================*/
RexxFunction2(RexxObjectPtr, SysTanH, double, x, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(tanh(x));
}

/********************************************************************/
/* Functions:           SysPower()                                  */
/* Description:         Returns function value of arguments.        */
/* Input:               Two numbers.                                */
/* Output:              Value of the x to the power y.              */
/*                      Returns 0 if the function executed OK,      */
/*                      -1 otherwise.  The interpreter will fail    */
/*                      if the function returns a negative result.  */
/* Notes:                                                           */
/*   This routine takes two to three parameters.                    */
/*   The form of the call is:                                       */
/*   result = func_name(x, y <, prec>)                              */
/*                                                                  */
/********************************************************************/
RexxFunction3(RexxObjectPtr, SysPower, double, x, double, y, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    // calculate and return
    return formatter.format(pow(x, y));
}

/********************************************************************/
/* Functions:           RxSin(), RxCos(), RxTan(), RxCotan()        */
/* Description:         Returns trigonometric angle value.          */
/* Input:               Angle in radian or degree or grade          */
/* Output:              Trigonometric function value for Angle.     */
/*                      Returns 0 if the function executed OK,      */
/*                      -1 otherwise.  The interpreter will fail    */
/*                      if the function returns a negative result.  */
/* Notes:                                                           */
/*   These routines take one to three parameters.                   */
/*   The form of the call is:                                       */
/*   x = func_name(angle <, prec> <, [R | D | G]>)                  */
/*                                                                  */
/********************************************************************/
RexxFunction3(RexxObjectPtr, SysSin, double, angle, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluate(angle, SINE);
}

RexxFunction3(RexxObjectPtr, SysCos, double, angle, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluate(angle, COSINE);
}

RexxFunction3(RexxObjectPtr, SysTan, double, angle, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluate(angle, TANGENT);
}

RexxFunction3(RexxObjectPtr, SysCotan, double, angle, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluate(angle, COTANGENT);
}


/********************************************************************/
/* Functions:           SysPi()                                     */
/* Description:         Returns value of pi for given precision     */
/* Input:               Precision.   Default is 9                   */
/* Output:              Value of the pi to given precision          */
/* Notes:                                                           */
/*   This routine takes one parameters.                             */
/*   The form of the call is:                                       */
/*   result = syspi(<precision>)                                    */
/*                                                                  */
/********************************************************************/
RexxFunction1(RexxObjectPtr, SysPi, OPTIONAL_uint32_t, precision)
{
    NumericFormatter formatter(context, precision);

    return formatter.format(pi);
}

/********************************************************************/
/* Functions:           SysArcSin(), SysArcCos(), SysArcTan()       */
/* Description:         Returns angle from trigonometric value.     */
/* Input:               Angle in radian or degree or grade          */
/* Output:              Angle for matching trigonometric value.     */
/*                      Returns 0 if the function executed OK,      */
/*                      -1 otherwise.  The interpreter will fail    */
/*                      if the function returns a negative result.  */
/* Notes:                                                           */
/*   These routines take one to three parameters.                   */
/*   The form of the call is:                                       */
/*   a = func_name(arg <, prec> <, [R | D | G]>)                    */
/*                                                                  */
/********************************************************************/
RexxFunction3(RexxObjectPtr, SysArcSin, double, x, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCSINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, SysArcCos, double, x, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCCOSINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, SysArcTan, double, x, OPTIONAL_uint32_t, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(context, precision);
    if (!formatter.setUnits(units))
    {
        return NULLOBJECT;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCTANGENT);
}

/*************************************************************************
* Function:  SysDumpVariables                                            *
*                                                                        *
* Syntax:    result = SysDumpVariables([filename])                       *
*                                                                        *
* Params:    filename - name of the file where variables are appended to *
*                       (dump is written to stdout if omitted)           *
*                                                                        *
* Return:    0 - dump completed OK                                       *
*            -1 - failure during dump                                     *
*************************************************************************/

RexxFunction1(int, SysDumpVariables, OPTIONAL_CSTRING, filename)
{
  FILE     *outFile = NULL;
  bool      fCloseFile = false;
  DWORD     dwBytesWritten = 0;

  if (filename != NULL)
  {
    /* open output file for append */
    outFile = fopen(filename, "a");
    if (outFile)
    {
       fCloseFile = true;
    }
  }
  else
  {
      outFile = stdin;
  }

  // get the set of context variables
  RexxSupplierObject vars = context->GetAllContextVariables();

  while (context->SupplierAvailable(vars))
  {
      RexxObjectPtr name = context->SupplierIndex(vars);
      RexxObjectPtr value = context->SupplierValue(vars);

      char *varname = context->ObjectToStringValue(name);
      size_t nameLength = strlen(varname);

      // is this a stem variable?
      if (varname[nameLength - 1] == '.')
      {
          // get the value for the stem variable
          RexxObjectPtr stemValue = context->GetStemValue((RexxStemObject)value);
          // write this out
          fprintf(outFile, "Name=%s, Value=%s\n", varname, context->ObjectToStringValue(stemValue));

          RexxSupplierObject tails = context->GetAllStemElements((RexxStemObject)value);

          while (context->SupplierAvailable(tails))
          {
              RexxObjectPtr tailName = context->SupplierIndex(tails);
              RexxObjectPtr tailValue = context->SupplierValue(tails);

              // write this out
              fprintf(outFile, "Name=%s%s, Value=%s\n", varname, context->ObjectToStringValue(tailName),
                  context->ObjectToStringValue(tailValue));

              // we're done with these references
              context->ReleaseLocalReference(tailName);
              context->ReleaseLocalReference(tailValue);
              // and move to the next supplier value
              context->SupplierNext(tails);
          }

          context->ReleaseLocalReference(tails);

      }
      else
      {
          // write this out
          fprintf(outFile, "Name=%s, Value=%s\n", varname, context->ObjectToStringValue(value));
      }

      // we're done with these references
      context->ReleaseLocalReference(name);
      context->ReleaseLocalReference(value);

      context->SupplierNext(vars);
  }

  if (fCloseFile)
  {
      fclose(outFile);
  }

  return 0;
}

extern "C" RexxReturnCode RexxEntry ooRexxStemSort(RexxCallContext *context, RexxStringObject stemname, int order, int type,
    size_t start, size_t end, size_t firstcol, size_t lastcol);

/*************************************************************************
* Function:  SysStemSort                                                 *
*                                                                        *
* Syntax:    result = SysStemSort(stem, order, type, start, end,         *
*                                 firstcol, lastcol)                     *
*                                                                        *
* Params:    stem - name of stem to sort                                 *
*            order - 'A' or 'D' for sort order                           *
*            type - 'C', 'I', 'N' for comparision type                   *
*            start - first index to sort                                 *
*            end - last index to sort                                    *
*            firstcol - first column to use as sort key                  *
*            lastcol - last column to use as sort key                    *
*                                                                        *
* Return:    0 - sort was successful                                     *
*            -1 - sort failed                                            *
*************************************************************************/

RexxFunction7(int, SysStemSort, RexxStringObject, stemname, OPTIONAL_CSTRING, order, OPTIONAL_CSTRING, type,
    OPTIONAL_RexxUnsignedNumber, start, OPTIONAL_RexxUnsignedNumber, end,
    OPTIONAL_RexxUnsignedNumber, firstcol, OPTIONAL_RexxUnsignedNumber, lastcol)
{
    size_t        first = 1;
    size_t        last = SIZE_MAX;
    size_t        firstCol = 0;
    size_t        lastCol = SIZE_MAX;
    int           sortType = SORT_CASESENSITIVE;
    int           sortOrder = SORT_ASCENDING;

    /* check other parameters */
    if (order != NULL) /* sort order */
    {
      switch (*order)
      {
        case 'A':
        case 'a':
          sortOrder = SORT_ASCENDING;
          break;
        case 'D':
        case 'd':
          sortOrder = SORT_DECENDING;
          break;
        default:
            context->InvalidRoutine();
            return NULL;
      }
    }

    if (type != NULL) /* sort type */
    {
      switch (*type)
      {
        case 'C':
        case 'c':
          sortType = SORT_CASESENSITIVE;
          break;
        case 'I':
        case 'i':
          sortType = SORT_CASEIGNORE;
          break;
        default:
            context->InvalidRoutine();
            return NULL;
      }
    }

    /* the sorting is done in the interpreter */
    if (!ooRexxStemSort(context, stemname, sortOrder, sortType, first, last, firstCol, lastCol))
    {
        return -1;
    }

    return 0;
}


/*************************************************************************
* Function:  SysStemDelete                                               *
*                                                                        *
* Syntax:    result = SysStemDelete(stem, startitem [,itemcount])        *
*                                                                        *
* Params:    stem - name of stem where item will be deleted              *
*            startitem - index of item to delete                         *
*            itemcount - number of items to delete if more than 1        *
*                                                                        *
* Return:    0 - delete was successful                                   *
*            -1 - delete failed                                          *
*************************************************************************/

RexxFunction3(int, SysStemDelete, RexxStemObject, toStem, size_t, start, OPTIONAL_size_t, count)

{
  if (argumentOmitted(3))
  {
      count = 1;
  }

  RexxUnsignedNumber items;

  RexxObjectPtr temp = context->GetStemArrayElement(toStem, 0);
  if (temp == NULLOBJECT || !context->ObjectToUnsignedNumber(temp, &items))
  {
      context->InvalidRoutine();
      return 0;
  }

  // make sure the deletion site is within the bounds
  if (start + count - 1 > items)
  {
      context->InvalidRoutine();
      return 0;
  }

  /* now copy the remaining indices up front */
  for (size_t index = start;  index + count <= items; index++)
  {
      // copy from the old index to the new index
      RexxObjectPtr value = context->GetStemArrayElement(toStem, index + count);
      // is this a sparse array?
      if (value == NULLOBJECT)
      {
          // return this as a failure
          return -1;
      }
      context->SetStemArrayElement(toStem, index, value);
  }

      /* now delete the items at the end */
  for (index = items - count + 1; index <= items; index++)
  {
      context->DropStemArrayElement(toStem, index);
  }

  context->SetStemArrayElement(toStem, 0, context->NumberToObject(items - count));
  return 0;
}


/*************************************************************************
* Function:  SysStemInsert                                               *
*                                                                        *
* Syntax:    result = SysStemInsert(stem, position, value)               *
*                                                                        *
* Params:    stem - name of stem where item will be inserted             *
*            position - index where new item will be inserted            *
*            value - new item value                                      *
*                                                                        *
* Return:    0 - insert was successful                                   *
*            -1 - insert failed                                          *
*************************************************************************/

RexxFunction3(int, SysStemInsert, RexxStemObject, toStem, size_t, position, RexxObjectPtr, newValue)
{
    RexxUnsignedNumber count;

    RexxObjectPtr temp = context->GetStemArrayElement(toStem, 0);
    if (temp == NULLOBJECT || !context->ObjectToUnsignedNumber(temp, &count))
    {
        context->InvalidRoutine();
        return 0;
    }

    /* check wether new position is within limits */
    if (position == 0 || (position > count + 1))
    {
        context->InvalidRoutine();
        return 0;
    }
    for (size_t index = count; index >= position; index--)
    {
        // copy from the old index to the new index
        RexxObjectPtr value = context->GetStemArrayElement(toStem, index);
        // is this a sparse array?
        if (value == NULLOBJECT)
        {
            // return this as a failure
            return -1;
        }
        context->SetStemArrayElement(toStem, index + 1, value);
    }

    // now set the new value
    context->SetStemArrayElement(toStem, position, newValue);
    return 0;
}


/*************************************************************************
* Function:  SysStemCopy                                                 *
*                                                                        *
* Syntax:    result = SysStemCopy(fromstem, tostem, from, to, count      *
*                                 [,insert])                             *
*                                                                        *
* Params:    fromstem - name of source stem                              *
*            tostem - - name of target stem                              *
*            from  - first index in source stem to copy                  *
*            to - position where items are copied/inserted in target stem*
*            count - number of items to copy/insert                      *
*            insert - 'I' to indicate insert instead of 'O' overwrite    *
*                                                                        *
* Return:    0 - stem copy was successful                                *
*            -1 - stem copy failed                                       *
*************************************************************************/

RexxFunction6(int, SysStemCopy, RexxStemObject, fromStem, RexxStemObject, toStem,
    OPTIONAL_size_t, from, OPTIONAL_size_t, to, OPTIONAL_size_t, count,
    OPTIONAL_CSTRING, option)
{
    bool inserting = false;

    /* get copy type */
    if (option != NULL)
    {
        switch (*option)
        {
            case 'I':
            case 'i':
                inserting = true;
                break;
            case 'O':
            case 'o':
                inserting = false;
                break;
            default:
                {
                    context->InvalidRoutine();
                    return 0;
                }
        }
    }

    RexxUnsignedNumber fromCount;

    RexxObjectPtr temp = context->GetStemArrayElement(fromStem, 0);
    if (temp == NULLOBJECT || !context->ObjectToUnsignedNumber(temp, &fromCount))
    {
        context->InvalidRoutine();
        return 0;
    }

    // default from location is the first element
    if (argumentOmitted(3))
    {
        from = 1;
    }

    if (argumentOmitted(4))
    {
        to = 1;
    }

    // was a count explicitly specified?
    if (argumentExists(5))
    {
        // this must be in range
        if ((count > (fromCount - from + 1)) || (fromCount == 0))
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    else
    {
        // default is to copy everything from the starting position.
        count = (size_t)fromCount - from + 1;
    }

    RexxUnsignedNumber toCount = 0;
    // but if it is set, then use that value
    temp = context->GetStemArrayElement(toStem, 0);
    if (temp != NULLOBJECT && !context->ObjectToUnsignedNumber(temp, &toCount))
    {
        context->InvalidRoutine();
        return 0;
    }

    // copying out of range?  Error
    if (to > toCount + 1)
    {
        context->InvalidRoutine();
        return 0;
    }

    if (inserting)
    {
        /* if we are about to insert the items we have to make room */
        for (size_t index = toCount; index >= to; index--)
        {
            // copy from the old index to the new index
            RexxObjectPtr value = context->GetStemArrayElement(toStem, index);
            // is this a sparse array?
            if (value == NULLOBJECT)
            {
                // return this as a failure
                return -1;
            }
            context->SetStemArrayElement(toStem, index, value);
        }


        // set the new count value in the target
        toCount += count;
        context->SetStemArrayElement(toStem, 0, context->NumberToObject(toCount));
    }
    /* now do the actual copying from the source to target */
    for (size_t index = 0; index < count; index++)
    {
        // just retrieve and copy
        RexxObjectPtr value = context->GetStemArrayElement(fromStem, from + index);
        // is this a sparse array?
        if (value == NULLOBJECT)
        {
            // return this as a failure
            return -1;
        }
        context->SetStemArrayElement(toStem, to + index, value);
    }

    // do we need to update the size?
    if (to + count - 1 > toCount)
    {
        context->SetStemArrayElement(toStem, 0, context->NumberToObject(to + count - 1));
    }
    return 0;
}


/*************************************************************************
* Function:  SysUtilVersion                                              *
*                                                                        *
* Syntax:    Say SysUtilVersion                                          *
*                                                                        *
* Return:    REXXUTIL.DLL Version                                        *
*************************************************************************/

RexxFunction0(RexxStringObject, SysUtilVersion)
{
    char buffer[256];
                                       /* format into the buffer     */
    sprintf(buffer, "%d.%d.%d", ORX_VER, ORX_REL, ORX_MOD);
    return context->NewStringFromAsciiz(buffer);
}
