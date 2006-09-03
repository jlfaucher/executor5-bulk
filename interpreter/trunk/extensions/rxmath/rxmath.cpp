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
/* REXX mathematical function support                             rxmath.c    */
/*                                                                            */
/* AIX  mathematical utility function package                                 */
/*                                                                            */
/******************************************************************************/

/**********************************************************************
*   RXMATH.C                                                          *
*                                                                     *
*   This program extends the REXX language by providing many          *
*   external mathematical functions                                   *
*   These functions are:                                              *
*       RxCalcPi               -- Return Pi to given precision        *
*       RxCalcSqrt             -- Calculate a square root             *
*       RxCalcExp              -- Calculate an exponent               *
*       RxCalcLog              -- Return natural log of a number      *
*       RxCalcLog10            -- Return log base 10 of a number      *
*       RxCalcSinh             -- Hyperbolic sine function            *
*       RxCalcCosh             -- Hyperbolic cosine function          *
*       RxCalcTanh             -- Hyperbolic tangent function         *
*       RxCalcPower            -- raise number to non-integer power   *
*       RxCalcSin              -- Sine function                       *
*       RxCalcCos              -- Cosine function                     *
*       RxCalcTan              -- Tangent function                    *
*       RxCalcCotan            -- Cotangent function                  *
*       RxCalcArcSin           -- ArcSine function                    *
*       RxCalcArcCos           -- ArcCosine function                  *
*       RxCalcArcTan           -- ArcTangent function                 *
*                                                                     *
**********************************************************************/

extern int errno;

/*------------------------------------------------------------------
 * program defines
 *------------------------------------------------------------------*/

#define PROG_DESC "REXX mathematical function package"
#define PROG_VERS "1.1"
#define PROG_SECU " "
#define PROG_COPY "(c) Copyright RexxLanguage Association 2005."
#define PROG_ALRR "All Rights Reserved."

/*------------------------------------------------------------------
 * standard includes
 *------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#include <math.h>
#include <fcntl.h>

/*------------------------------------------------------------------
 * rexx includes
 *------------------------------------------------------------------*/
#include "oorexx.h"
#include <sys/types.h>

#define MAX_DIGITS     9

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

#define DEGREES    'D'                 /* degrees option             */
#define RADIANS    'R'                 /* radians option             */
#define GRADES     'G'                 /* grades option              */

#define DEFAULT_PRECISION  9           /* default precision to use   */
#define MAX_PRECISION     16           /* maximum available precision*/
#define MIN_PRECISION     1            /* minimum available precision*/

bool bErrorFlag = false;               // flags math errors

/* Turn off optimization under Windows. If this is compiler under    */
/* Windows with the MS Visual C++ copiler and optimization is on     */
/* then the function _matherr is not called                          */
#ifdef WIN32
#pragma optimize( "", off )
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
* Function:  MathLoadFuncs                                               *
*                                                                        *
* Syntax:    call MathLoadFuncs                                          *
*                                                                        *
* Purpose:   load the function package                                   *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    null string                                                 *
*************************************************************************/
RexxFunction1(CSTRING, MathLoadFuncs, OPTIONAL_CSTRING, version)
{
   if (version != NULL)
   {
      fprintf(stdout, "%s %s - %s\n",PROG_NAME,PROG_VERS,PROG_DESC);
      fprintf(stdout, "%s\n",PROG_COPY);
      fprintf(stdout, "%s\n",PROG_ALRR);
      fprintf(stdout, "\n");
   }

   // the rest is a nop now that this uses automatic loading.
   return "";
}

/*************************************************************************
* Function:  MathDropFuncs                                               *
*                                                                        *
* Syntax:    call MathDropFuncs                                          *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/

RexxFunction0(CSTRING, MathDropFuncs)
{
    // this is a nop now.
}


/* Helper functions **************************************************/

/*************************************************************************
* Function:  matherr                                                     *
*                                                                        *
* Syntax:    matherr is called by the system if exist whenever  a mathe  *
*            matical function fails.                                     *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/
#ifdef WIN32
int _cdecl _matherr(struct _exception *x )
#elif OPSYS_SUN
int matherr(struct __math_exception *x)    /* return string            */
#elif OPSYS_AIX
int matherr(struct __exception *x)         /* return string            */
#endif
#if defined(WIN32) || defined(OPSYS_SUN) || defined(OPSYS_AIX)
{
    char *message;

    switch (x->type)
    {
        case DOMAIN:
            message = "Argument domain error";
            break;
        case OVERFLOW:
            message = "Overflow range error";
            break;
        case UNDERFLOW:
            message = "Underflow range error";
            break;
        case SING:
            message = "Argument singularity";
            break;
        case TLOSS:
            message = "Total loss of significance";
            break;
        case PLOSS:
            message = "Total loss of significance";
            break;
        default:
            message = "Mathematical error occured";
            break;
    }

    NumericFormatter::setErrorFlag(message);

    return(1);                            /* otherwhile system throws exception */
}
#endif

// simple class for handling numeric values
class NumericFormatter
{
public:
    NumericFormatter(RexxCallContext *c, int p)
    {
        if (p == 0)
        {
            precision DEFAULT_PRECISION;
        }
        else
        {
            precision = min(p, MAX_PRECISION);
        }
        context = c;
        errorFlag = false;
    }

    RexxObjectPtr format(double x)
    {
        if (errorFlag)
        {
            return context->NewStringFromAsciiz("ERROR");
        }
        return context->DoubleToObjectWithPrecision(x, precision);
    }

    static void setErrorMessage(char *msg)
    {
        RexxStringObject message = context->NewStringFromAsciiz(msg);
        context->SetContextVariable("MATHERRNO", message);
    }

    static void setErrorFlag(char *msg)
    {
        errorFlag = true;
        setErrorMessage(msg);
    }

protected:
    int precision;
    RexxCallContext *context;

    static bool errorFlag;
};


class TrigFormatter : public NumericFormatter
{
public:

    TrigFormatter(RexxCallContext *c, int p, char *u) : NumericFormatter(c, p)
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

    double evaluate(double angle, int function)
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


    double evaluate(double angle, int function)
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

    double evaluateArc(double x, int function)
    {
        double    angle;                     /* working angle              */
        double    nsi;                       /* convertion factor          */
        double    nco;                       /* convertion factor          */
        double    x;                         /* input number               */

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


/* Mathematical function package *************************************/

/********************************************************************/
/* Functions:           RxCalcSqrt(), RxCalcExp(), RxCalcLog(), RxCalcLog10, */
/* Functions:           RxCalcSinH(), RxCalcCosH(), RxCalcTanH()    */
/* Description:         Returns function value of argument.         */
/* Input:               One number.                                 */
/* Output:              Value of the function requested for arg.    */
/*                      Returns 0 if the function executed OK,      */
/*                      40 otherwise.  The interpreter will fail    */
/*                      if the function returns a negative result.  */
/* Notes:                                                           */
/*   These routines take one to two parameters.                     */
/*   The form of the call is:                                       */
/*   result = func_name(x <, prec>)                                 */
/*                                                                  */
/********************************************************************/
RexxFunction2(RexxObjectPtr, RxCalcSqrt, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(sqrt(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, RxCalcExp, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(exp(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, RxCalcLog, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(log(x));
}

RexxFunction2(RexxObjectPtr, RxCalcLog10, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(log10(x));
}


/*==================================================================*/
RexxFunction2(RexxObjectPtr, RxCalcSinH, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(sinh(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, RxCalcCosH, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(cosh(x));
}

/*==================================================================*/
RexxFunction2(RexxObjectPtr, RxCalcTanH, double, x, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(tanh(x));
}

/********************************************************************/
/* Functions:           RxCalcPower()                               */
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
RexxFunction3(RexxObjectPtr, RxCalcPower, double, x, double, y, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    // calculate and return
    return formatter.format(pow(x, y));
}

/********************************************************************/
/* Functions:           RxCalcSin(), RxCalcCos(), RxCalcTan(), RxCalcCotan() */
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
RexxFunction3(RexxObjectPtr, RxCalcSin, double, angle, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluate(angle, SINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, RxCalcCos, double, angle, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluate(angle, COSINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, RxCalcTan, double, angle, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluate(angle, TANGENT);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, RxCalcCotan, double, angle, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluate(angle, COTANGENT);
}

/********************************************************************/
/* Functions:           RxCalcPi()                                  */
/* Description:         Returns value of pi for given precision     */
/* Input:               Precision.   Default is 9                   */
/* Output:              Value of the pi to given precision          */
/* Notes:                                                           */
/*   This routine takes one parameters.                             */
/*   The form of the call is:                                       */
/*   result = RxCalcpi(<precision>)                                    */
/*                                                                  */
/********************************************************************/
RexxFunction1(RexxObjectPTr, SysPi, OPTIONAL_uint32, precision)
{
    NumericFormatter formatter(precision);

    return format(pi);
}

/********************************************************************/
/* Functions:           RxCalcArcSin(), RxCalcArcCos(), RxCalcArcTan()*/
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
RexxFunction3(RexxObjectPtr, RxCalcArcSin, double, x, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCSINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, RxCalcArcCos, double, x, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCCOSINE);
}

/*==================================================================*/
RexxFunction3(RexxObjectPtr, RxCalcArcTan, double, x, OPTIONAL_uint32, precision, OPTIONAL_CSTRING, units)
{
    TrigFormatter formatter(precision);
    if (!formatter.setUnits(units)
    {
        return OREF_NULL;
    }

    // calculate and return
    return formatter.evaluateArc(x, ARCTANGENT);
}

#ifdef __cplusplus
}
#endif


// now build the actual entry list
ooRexxFunctionEntry rxmath_functions[] =
{
    REXX_TYPED_FUNCTION(MathLoadFuncs, MathLoadFuncs)
    REXX_TYPED_FUNCTION(MathDropFuncs, MathDropFuncs)
    REXX_TYPED_FUNCTION(RxCalcPi,      RxCalcPi)
    REXX_TYPED_FUNCTION(RxCalcSqrt,    RxCalcSqrt)
    REXX_TYPED_FUNCTION(RxCalcExp,     RxCalcExp)
    REXX_TYPED_FUNCTION(RxCalcLog,     RxCalcLog)
    REXX_TYPED_FUNCTION(RxCalcLog10,   RxCalcLog10)
    REXX_TYPED_FUNCTION(RxCalcSinH,    RxCalcSinH)
    REXX_TYPED_FUNCTION(RxCalcCosH,    RxCalcCosH)
    REXX_TYPED_FUNCTION(RxCalcTanH,    RxCalcTanH)
    REXX_TYPED_FUNCTION(RxCalcPower,   RxCalcPower)
    REXX_TYPED_FUNCTION(RxCalcSin,     RxCalcSin)
    REXX_TYPED_FUNCTION(RxCalcCos,     RxCalcCos)
    REXX_TYPED_FUNCTION(RxCalcTan,     RxCalcTan)
    REXX_TYPED_FUNCTION(RxCalcCotan,   RxCalcCotan)
    REXX_TYPED_FUNCTION(RxCalcArcSin,  RxCalcArcSin)
    REXX_TYPED_FUNCTION(RxCalcArcCos,  RxCalcArcCos)
    REXX_TYPED_FUNCTION(RxCalcArcTan,  RxCalcArcTan)
};

ooRexxPackageEntry rxmath_package_entry =
{
    STANDARD_PACKAGE_HEADER
    "RXMATH",                            // name of the package
    "1.2",                               // package information
    NULL,                                // no load/unload functions
    NULL,
    rxmath_functions,                    // the exported functions
    NULL                                 // no methods in rxmath.
};

// package loading stub.
OOREXX_GET_PACKAGE(rxmath);

#ifdef WIN32
#pragma optimize( "", on )
#endif
