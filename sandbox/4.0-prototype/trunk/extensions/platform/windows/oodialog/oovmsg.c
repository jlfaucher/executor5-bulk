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
/* Object REXX OODialog                                              oovmsg.c */
/*                                                                            */
/*  OODialog Messaging function                                               */
/*                                                                            */

#include <windows.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#include "oovutil.h"
#include <commctrl.h>


extern CRITICAL_SECTION crit_sec;  /* defined in OOVUTIL.C */
extern bool DialogInAdminTable(DIALOGADMIN * Dlg);


BOOL AddDialogMessage(MESSAGE_QUEUE *q, char *message, int args, ULONG param, ULONG id, char *text)
{
    MQ_ITEM *item;

    // see if we can grab a pooled one.
    if (q->pool != NULL)
    {
        item = q->pool;
        q->pool = item->next;
    }
    else
    {
        item = malloc(sizeof(MQ_ITEM));
        if (item == NULL)
        {
            return FALSE;
        }
    }

    item->message = message;
    item->count = args;
    item->param = param;
    item->item = id;
    if (text != NULL)
    {
        item->text = strdup(text);
    }
    else
    {
        test = NULL;
    }

    // only item in the queue?
    if (q->tail == NULL)
    {
        q->head = item;
        q->tail = item;
        item->next = NULL;
    }
    else
    {
        tail->next = item;
        item->next = NULL;
        tail = item;
    }

    return TRUE;
}

void DeleteMessageQueue(MESSAGE_QUEUE *q)
{
    MQ_ITEM *item = q->head;
    MQ_ITEM *previous;
    while (item != NULL)
    {
        previous = item;
        item = item->next;
        if (previous->text != NULL)
        {
            free(previous->text);
        }
        free(previous);
    }

    item = q->pool;
    while (item != NULL)
    {
        previous = item;
        item = item->next;
        free(previous);
    }

    free(q);
}

MESSAGE_QUEUE *CreateMessageQueue()
{
    MESSAGE_QUEUE *q = malloc(sizeof(MESSAGE_QUEUE));
    if (q != NULL)
    {
        memset(q, '\0', sizeof(MESSAGE_QUEUE));
    }
    return q;
}


RexxArrayObject GetDlgMessage(RexxCallContext *context, DIALOGADMIN * addressedTo, bool remove)
{
   int i = 0, l;
   MSG msg;
   MESSAGE_QUEUE *q = addressedTo->pMessageQueue;

   if (q != NULL)
   {
       MQ_ITEM *item = q->head;
       if (item == NULL)
       {
           // if we're just peeking, don't sleep.  Otherwise, give up control a bit and see
           // if we have anything.
           if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && remove) Sleep(1);
       }

       item = q->head;
       // still nothing?, then get out of here.
       if (item == NULL)
       {
           return NULL;
       }

       // the message is an array object...first item is message, second is argument array
       RexxArrayObject res = context->NewArray(2);
       context->ArrayPut(res, context->NewStringFromAsciiz(item->message), 1);
       RexxArrayObject args = context->NewArray(item-count)
       context->ArrayPut(res, args, 2);

       if (item->count >= 1)
       {
           context->ArrayPut(args, context->NumberToObject(item->param), 1);
       }
       if (item->count >= 2)
       {
           context->ArrayPut(args, context->NumberToObject(item->item), 2);
       }
       if (item->count == 3)
       {
           context->ArrayPut(args, context->NewStringFromAsciiz(item->text), 3);
       }

       // need to remove?
       if (remove)
       {
           q->head = item->next;
           if (item->text != NULL)
           {
               free(item->text);
           }
           item->next = q->pool;
           q->pool = item;
       }

       return res;

   }
   return NULL;
}


bool SearchMessageTable(ULONG message, WPARAM param, LPARAM lparam, DIALOGADMIN * addressedTo)
{
   register LONG i = 0;
   MESSAGETABLEENTRY * m = addressedTo->MsgTab;

   if (m)
   for (i=0; i<addressedTo->MT_size; i++)
      if (((message & m[i].filterM) == m[i].msg)
      && ( (ULONG)(param & m[i].filterP) == m[i].wParam)
      && ( ((message == WM_NOTIFY) && ((ULONG)(((NMHDR *)lparam)->code & m[i].filterL) == m[i].lParam))
         || ((message != WM_NOTIFY) && ( (ULONG)(lparam & m[i].filterL) == m[i].lParam)) ) )
      {
             if (param || lparam)  /* if one of the params is <> 0, build argument string */
             {
                char msgstr[512];
                CHAR tmp[20];
                char * np = NULL;
                ULONG item;

                    /* do we have a notification where we have to extract some information ? */
                if (message == WM_NOTIFY)
                {
                    /* do we have an end label edit for tree or list view? */
                    if ((((NMHDR *)lparam)->code == TVN_ENDLABELEDIT) && ((TV_DISPINFO *)lparam)->item.pszText)
                    {
                        np = ((TV_DISPINFO *)lparam)->item.pszText;
                        item = (ULONG)((TV_DISPINFO *)lparam)->item.hItem;
                    }
                    else if ((((NMHDR *)lparam)->code == LVN_ENDLABELEDIT) && ((LV_DISPINFO *)lparam)->item.pszText)
                    {
                        np = ((LV_DISPINFO *)lparam)->item.pszText;
                        item = ((LV_DISPINFO *)lparam)->item.iItem;
                    }
                    /* do we have a tree expand/collapse? */
                    else if ((((NMHDR *)lparam)->code == TVN_ITEMEXPANDED) || (((NMHDR *)lparam)->code == TVN_ITEMEXPANDING))
                    {
                        item = (ULONG)((NM_TREEVIEW *)lparam)->itemNew.hItem;
                        if (((NM_TREEVIEW *)lparam)->itemNew.state & TVIS_EXPANDED) np = "EXPANDED";
                        else np = "COLLAPSED";
                    }
                    /* do we have a key_down? */
                    else if ((((NMHDR *)lparam)->code == TVN_KEYDOWN) || (((NMHDR *)lparam)->code == LVN_KEYDOWN)
                        || (((NMHDR *)lparam)->code == TCN_KEYDOWN))
                    {
                        lparam = (ULONG)((TV_KEYDOWN *)lparam)->wVKey;
                    }
                    /* do we have a list drag and drop? */
                    else if ((((NMHDR *)lparam)->code == LVN_BEGINDRAG) || (((NMHDR *)lparam)->code == LVN_BEGINRDRAG))
                    {
                        item = (ULONG)((NM_LISTVIEW *)lparam)->iItem;
                        param = ((NMHDR *)lparam)->idFrom;
                        sprintf(tmp, "%d %d", ((NM_LISTVIEW *)lparam)->ptAction.x, ((NM_LISTVIEW *)lparam)->ptAction.y);
                        np = tmp;
                    }
                    /* do we have a tree drag and drop? */
                    else if ((((NMHDR *)lparam)->code == TVN_BEGINDRAG) || (((NMHDR *)lparam)->code == TVN_BEGINRDRAG))
                    {
                        item = (ULONG)((NM_TREEVIEW *)lparam)->itemNew.hItem;
                        param = ((NMHDR *)lparam)->idFrom;
                        sprintf(tmp, "%d %d", ((NM_TREEVIEW *)lparam)->ptDrag.x, ((NM_TREEVIEW *)lparam)->ptDrag.y);
                        np = tmp;
                    }
                    /* do we have a column click in a report? */
                    else if ((((NMHDR *)lparam)->code == LVN_COLUMNCLICK))
                    {
                        param = ((NMHDR *)lparam)->idFrom;
                        lparam = (ULONG)((NM_LISTVIEW *)lparam)->iSubItem;  /* which column is pressed */
                    }
                }

                if (np)
                    AddDialogMessage(m[i].rexxProgram, 3, param, item, np);
                else
                    AddDialogMessage(m[i].rexxProgram, 2, param, lparam, NULL);
             }
             else
                 AddDialogMessage(m[i].rexxProgram, 0, 0, 0, NULL);
             return 1;
      }
   return 0;
}


bool AddTheMessage(DIALOGADMIN * aDlg, size_t message, size_t filt1, size_t param, size_t filt2, size_t lparam, size_t filt3, char * prog)
{
   if (strlen(prog) == 0) return 0;
   if (!(message | param | lparam))
   {
       MessageBox(0,"Message passed is invalid","Error",MB_OK | MB_ICONHAND);
       return 0;
   }
   if (!aDlg->MsgTab)
   {
      aDlg->MsgTab = LocalAlloc(LPTR, sizeof(MESSAGETABLEENTRY) * MAXTABLEENTRIES);
      if (!aDlg->MsgTab)
      {
          MessageBox(0,"No memory available","Error",MB_OK | MB_ICONHAND);
          return 0;
      }

      aDlg->MT_size = 0;
   }

   if (aDlg->MT_size < MAXTABLEENTRIES)
   {
      aDlg->MsgTab[aDlg->MT_size].msg = message;
      aDlg->MsgTab[aDlg->MT_size].filterM = filt1;
      aDlg->MsgTab[aDlg->MT_size].wParam = param;
      aDlg->MsgTab[aDlg->MT_size].filterP = filt2;
      aDlg->MsgTab[aDlg->MT_size].lParam = lparam;
      aDlg->MsgTab[aDlg->MT_size].filterL = filt3;
      aDlg->MsgTab[aDlg->MT_size].rexxProgram = LocalAlloc(LMEM_FIXED, strlen(prog)+1);
      if (aDlg->MsgTab[aDlg->MT_size].rexxProgram) strcpy(aDlg->MsgTab[aDlg->MT_size].rexxProgram, prog);
      aDlg->MT_size ++;
      return 1;
   }
   return 0;
}


/**
 * Convert a character constant into an integer, recognizing either hex or decimal encodings.
 *
 * @param c      A pointer to the constant.  If the string begins with "0X", it is assumed to be hex, otherwise it is treated as a decimal number.
 *
 * @return The integer value of the constant.
 */
size_t xOrD(char *c)
{
    if (ISHEX(c))
    {
        (size_t)return strtoul(c, '\0', 16);
    }
    return (size_t)atol(c);

}

RexxFunction8(int, AddUserMessage, uintptr_t, admin, CSTRING, n0, CSTRING, n1, CSTRING, n2, CSTRING, n3, CSTRING, n4, CSTRING, n5, CSTRING, program)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    return !AddTheMessage(dlgAdm, xOrD(n0), xOrD(n1), xOrD(n2), xOrD(n3), xOrD(n4), xOrD(n5), program);
}

RexxFunction5(int, SendDialogMessage, uintptr_t, handle, int, id, CSTRING, msg, CSTRING, wparam, CSTRING, lparam)
{
    return SendDlgItemMessage((HWND)handle, id, xOrD(msg), xOrD(wparam), xOrD(lparam));
}

// sends a message using a pointer valued parameter for the lparam value.  This value has a special
// encoding that is processed here.
RexxFunction5(RexxObjectPtr, SendPtrMessage, uintptr_t, handle, int, id, CSTRING, msg, CSTRING, wparam, CSTRING, lparam)
{
    LPARAM lp;
    LONG lBuffer;
    int message = xOrD(msg);
    WPARAM wp = (WPARAM)xOrD(wparam);

    // this parameter is text, so send a pointer to the value
    if (*lparam == 'T')
    {
        lp = (LPARAM)(lparam + 1);
    }
    // this parameter is a pointer to a long value...so save the long value and
    // address it indirectly.
    else if (*lparam == 'L')
    {
        lBuffer = atol(lparam +1);
        lp = (LPARAM)&lBuffer;
    }
    // a buffered Get e.g. to get a text line of an edit control */
    else if (*lparam == 'G')
    {
        // the value here is a buffer length to use for the get.
        int len = atoi(lparam + 1);
        lp = (LPARAM)GlobalAlloc(GMEM_FIXED, len + 1);

        if (lp == NULL)
        {
           return context->NumberToObject(GetLastError());
        }
        // the buffer size needs to be at the start of the buffer
        memcpy((void *)lp, (char *)&len, sizeof(int));

    }
    else
    {
        // convert this directly.
        lp = (LPARAM)xOrD(lparam);
    }

    /* Special handling for this message because it do not wotk for multiple selection lb's */
    if ( message == LB_SETCURSEL )
    {
      // at first check if it is an multiple selection lb
      LONG style;
      style = GetWindowLong(GetDlgItem( (HWND)handle, id ), GWL_STYLE);

      if ( style & LBS_MULTIPLESEL )
        // another special case for this
        if ( *lparam == 'D' )
          // deselect item in muliple selection lb
          ret = SendDlgItemMessage((HWND)handle, id, LB_SETSEL, 0, (LPARAM)wp);
        else
          // select item in muliple selection lb
          ret = SendDlgItemMessage((HWND)handle, id, LB_SETSEL, 1, (LPARAM)wp);
      else
        // select item in single selection lb
        ret = SendDlgItemMessage((HWND)handle, id, message, wp, lp);
    }
    else

      ret = SendDlgItemMessage((HWND)handle, id, message, wp, lp);

   if (*lparam != 'G')
   {
       return context->NumberToObject(ret);
   }
   else
   {
       RexxObjectPtr result = context->NewString((RexxStringPtr)lp, ret);
       GlobalFree((void *)lp);
       return result;
   }
}



RexxFunction5(int, SendWindowMessage, uintptr_t, handle, CSTRING, msg, CSTRING, wparam, CSTRING, lparam)
{
    return SendMessage((HWND)handle, xOrD(msg), xOrD(wparam), xOrD(lparam));
}


RexxFunction2(RexxArrayObject, GetDialogMsg, uintptr_t, admin, RexxBoolean, remove)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    EnterCriticalSection(&crit_sec);
    if (!DialogInAdminTable(dlgAdm))   /* Is the dialog admin valid? */
    {
        LeaveCriticalSection(&crit_sec);
        RexxArrayObject res = context->NewArray(2);
        context->ArrayPut(res, context->NewStringFromAsciiz(MSG_TERMINATE), 1);
        context->ArrayPut(res, context->NewArray(0), 2);

        return res;
    }
    RexxArrayObject res = GetDlgMessage(context, dlgAdm, remove);
    LeaveCriticalSection(&crit_sec);
    // if there was no message in the queue, return .nil
    if (res == NULLOBJECT)
    {
        return context->NilObject();
    }

    return res;
}


RexxFunction3(int, SetLBTabStops, uintptr_t, handle, int, id, RexxArrayObject, stops)
{
    int tabs[20];
    int arraySize = context->ArraySize(stops);

    for (int i = 0; i < arraySize && i < 20 ; i++)
    {
        RexxObjectPtr arrayItem = context->ArrayAt(i + 1);
        if (arrayItem == NULLOBJECT)
        {
            context->InvalidRoutine();
            return 0;
        }

        RexxNumber stop;
        if (!context->ObjectToNumber(arrayItem, &stop))
        {
            context->InvalidRoutine();
            return 0;
        }

        tabs[i] = (int)stop;
    }

    return SendDlgItemMessage((HWND)handle, id, LB_SETTABSTOPS, (WPARAM)(arraySize), (LPARAM)tabs);
}


