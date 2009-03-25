
#include "oorexxapi.h"
#include <stdio.h>

int __cdecl main(int argc, char **argv)
{
    char *programName = "simple.rex";
    if ( argc == 2 )
    {
        programName = argv[1];
    }


    int return_code;            /* interpreter return code */
    CONSTRXSTRING rxargv[1];    /* program argument string */
    RXSTRING retstr;            /* program return value */
    short rc;                   /* converted return code */
    char tmpArg[10];            /* string to get the argument value */

    retstr.strlength = 0;
    retstr.strptr = NULL;

    for ( int i = 1; i <= 5; i++ )
    {
        sprintf(tmpArg, "%d", i);   // use control variable as argument
        MAKERXSTRING(rxargv[0], tmpArg, strlen(tmpArg));

        printf("C++, round # [%d] ... \n", i);

        return_code = RexxStart(1,              /* one argument */
                                rxargv,         /* argument array */
                                programName,    /* Rexx procedure name */
                                NULL,           /* use disk version */
                                "CMD",          /* default address name */
                                RXSUBROUTINE,   /* calling as a subroutine */
                                NULL,           /* no exits used */
                                &rc,            /* converted return code */
                                &retstr);       /* returned result */

        printf("... RexxStart() return code=[%d] | returned result value=[%s] | converted return code=[%d]\n---\n",
               return_code, retstr.strptr == NULL ? "null string" : retstr.strptr, rc);

        if ( retstr.strptr != NULL )
        {
            RexxFreeMemory(retstr.strptr);
        }
    }

    return 0;
}
