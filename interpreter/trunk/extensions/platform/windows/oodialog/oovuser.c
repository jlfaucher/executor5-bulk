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

#include <windows.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#ifdef __CTL3D
#include <ctl3d.h>
#endif
#include "oovutil.h"
#include <commctrl.h>

extern LRESULT CALLBACK RexxDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern bool InstallNecessaryStuff(DIALOGADMIN * dlgAdm, char *);
extern INT DelDialog(DIALOGADMIN * aDlg);
extern CRITICAL_SECTION crit_sec;
extern bool DialogInAdminTable(DIALOGADMIN * Dlg);
extern void DeleteMessageQueue(MESSAGE_QUEUE *q);


static bool CommCtrlLoaded = false;
//#define USE_DS_CONTROL


#ifndef USE_DS_CONTROL
bool IsNestedDialogMessage(
    DIALOGADMIN * dlgAdm,
    LPMSG  lpmsg    // address of structure with message
   );
#endif

/****************************************************************************************************

           Part for user defined Dialogs

****************************************************************************************************/


LPWORD lpwAlign ( LPWORD lpIn)
{
  ULONG ul;

  ul = (ULONG) lpIn;
  ul +=3;
  ul >>=2;
  ul <<=2;
  return (LPWORD) ul;
}


int nCopyAnsiToWideChar (LPWORD lpWCStr, LPSTR lpAnsiIn)
{
  int nChar = 0;

  do {
    *lpWCStr++ = (WORD) (UCHAR) *lpAnsiIn;  /* first convert to UCHAR, otherwise Ü,Ä,... are >65000 */
    nChar++;
  } while (*lpAnsiIn++);

  return nChar;
}

RexxFunction4(RexxStringObject, GetStdTextSize, CSTRING, text, OPTIONAL_CSTRING, fontName, OPTIONAL_int, pointSize, OPTIONAL_uintptr_t, handle)
{
    HDC hDC;
    SIZE s;
    ULONG bux;
    ULONG buy;
    HFONT hFont=NULL, hSystemFont, oldF;
    HWND hW = NULL;

    CHECKARGL(1);

    if (handle == 0 && topDlg)
    {
        hW = topDlg->TheDlg;
    }
    else
    {
        hW = (HWND)handle;
    }
    if (fontName == NULL)
    {
        fontName = "System";
    }
    if (pointSize == 0)
    {
        pointSize = 8;
    }
    if (hW)
       hDC = GetDC(hW);
    else
       hDC = CreateDC("Display", NULL, NULL, NULL);

    if (hDC)
    {
       hSystemFont = GetStockObject(SYSTEM_FONT);

       if (hW)
          hFont = (HFONT)SendMessage(hW, WM_GETFONT, 0, 0);

       if (hW && !hFont)
          hFont = hSystemFont;
       else
       {
          /* we have no dialog, this is the cas if standard dialogs are called  */
          /* directly from a REXX script */
          /* so if we have a system font, use it ! If not, use an estimated one */
          if (hSystemFont)
             hFont = hSystemFont;
          else
             hFont = CreateFont(pointSize, pointSize, 0, 0, FW_NORMAL,false, false, false, ANSI_CHARSET,
             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fontName);
       }

       if (hFont && (hFont != hSystemFont)) oldF = SelectObject(hDC, hFont);

       GetTextExtentPoint32(hDC, test, strlen(text), &s);

       buy = GetDialogBaseUnits();
       bux = LOWORD(buy);
       buy = HIWORD(buy);

       char buffer[256];
       sprintf(buffer, "%d %d", (s.cx * 4) / bux, (s.cy * 8) / buy);

       if (hFont && (hFont != hSystemFont))
       {
          SelectObject(hDC, oldF);
          DeleteObject(hFont);
       }

       if (hW)
          ReleaseDC(hW, hDC);
       else
          DeleteDC(hDC);
       return context->NewStringFromAsciiz(buffer);
    }
    return context->NullString();
}

RexxFunction0(RexxStringObject, GetScreenSize)
{
    ULONG sx, sy;
    ULONG bux, buy;

    buy = GetDialogBaseUnits();
    bux = LOWORD(buy);
    buy = HIWORD(buy);
    sx = GetSystemMetrics(SM_CXSCREEN);
    sy = GetSystemMetrics(SM_CYSCREEN);

    char buffer[256];
    sprintf(buffer, "%d %d %d %d", (sx * 4) / bux, (sy * 8) / buy, sx, sy);
    return context->NewStringFromAsciiz(buffer);
}


void UCreateDlg(WORD ** template, WORD **p, INT NrItems, INT x, INT y, INT cx, INT cy,
                CHAR * class, CHAR * title, CHAR * fontname, INT fontsize, ULONG lStyle)
{
   int   nchar;

   *template = *p = (PWORD) LocalAlloc(LPTR, (NrItems+3)*256);

     /* start to fill in the dlgtemplate information.  addressing by WORDs */
   **p = LOWORD (lStyle);
   (*p)++;
   **p = HIWORD (lStyle);
   (*p)++;
   **p = 0;          // LOWORD (lExtendedStyle)
   (*p)++;
   **p = 0;          // HIWORD (lExtendedStyle)
   (*p)++;
   **p = NrItems;          // NumberOfItems
   (*p)++;
   **p = x;         // x
   (*p)++;
   **p = y;         // y
   (*p)++;
   **p = cx;        // cx
   (*p)++;
   **p = cy;         // cy
   (*p)++;
   /* copy the menu of the dialog */

   /* no menu */
   **p = 0;
   (*p)++;

   /* copy the class of the dialog */
   if ( !(lStyle & WS_CHILD) && (class))
   {
      nchar = nCopyAnsiToWideChar (*p, TEXT(class));
      (*p) += nchar;
   }
   else
   {
     **p = 0;
    (*p)++;
   }
   /* copy the title of the dialog */
   if (title)
   {
      nchar = nCopyAnsiToWideChar (*p, TEXT(title));
      (*p) += nchar;
   }
   else
   {
     **p = 0;
    (*p)++;
   }

   /* add in the wPointSize and szFontName here iff the DS_SETFONT bit on */
   **p = fontsize;         // fontsize
   (*p)++;
   nchar = nCopyAnsiToWideChar (*p, TEXT(fontname));
   (*p) += nchar;

   /* make sure the first item starts on a DWORD boundary */
   (*p) = lpwAlign (*p);
}


RexxFunction10(RexxArrayObject, UsrDefineDialog, int, x, int, y, int, cx, int, cy, CSTRING, dlgclass, CSTRING, title,
    CSTRING, fontname, int, fontsize, CSTRING, opts, int, expected)
{
    bool child;
    ULONG lStyle;
    WORD *p;
    WORD *pbase;
    int i;

    if (strstr(opts, "CHILD")) child = true; else child = false;

    if (child) lStyle = DS_SETFONT | WS_CHILD;
    else lStyle = WS_CAPTION | DS_SETFONT;

 #ifdef USE_DS_CONTROL
    if (child) lStyle |= DS_CONTROL;
 #endif

    if (strstr(opts, "VISIBLE")) lStyle |= WS_VISIBLE;
    if (!strstr(opts, "NOMENU") && !child) lStyle |= WS_SYSMENU;
    if (!strstr(opts, "NOTMODAL") && !child) lStyle |= DS_MODALFRAME;
    if (strstr(opts, "SYSTEMMODAL")) lStyle |= DS_SYSMODAL;
    if (strstr(opts, "THICKFRAME")) lStyle |= WS_THICKFRAME;
    if (strstr(opts, "MINIMIZEBOX")) lStyle |= WS_MINIMIZEBOX;
    if (strstr(opts, "MAXIMIZEBOX")) lStyle |= WS_MAXIMIZEBOX;
    if (strstr(opts, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(opts, "HSCROLL")) lStyle |= WS_HSCROLL;

    if (strstr(opts, "OVERLAPPED")) lStyle |= WS_OVERLAPPED;

    /*                     expected        x          y        cx        cy  */
    UCreateDlg(&pbase, &p, expected, x, y, cx, cy, dlgclass, title, fontname, fontsize, lStyle);
    /*            class         title            fontname         fontsize */
               argv[4].strptr, argv[5].strptr, argv[6].strptr, atoi(argv[7].strptr), lStyle);

    RexxArrayObject result = context->NewArray(2);

    ValueDescriptor type;

    type.value.value_uintptr_t = (uintptr_t)pbase;
    type.type = REXX_VALUE_uintptr_t;

    RexxObject temp = ValueToObject(&type);
    context->ArrayPut(result, 1, temp);


    type.value.value_uintptr_t = (uintptr_t)pbase;
    type.type = REXX_VALUE_uintptr_t;
    temp = context->ValueToObject(&type);
    context->ArrayPut(result, 2, temp);

    return result;
}

     /* Loop getting messages and dispatching them. */
DWORD WINAPI WindowUsrLoopThread(void **arg)
{
   MSG msg;
   CHAR buffer[NR_BUFFER];
   DIALOGADMIN * Dlg;
   ULONG ret=0;
   bool * release;

   Dlg = (DIALOGADMIN*)arg[1];  /*  thread local admin pointer from StartDialog */
   Dlg->TheDlg = CreateDialogIndirectParam(MyInstance, (DLGTEMPLATE *) arg[0], NULL, RexxDlgProc, Dlg->Use3DControls);  /* pass 3D flag to WM_INITDIALOG */
   Dlg->ChildDlg[0] = Dlg->TheDlg;

   release = (bool *)arg[2];  /* the Release flag is stored as the 3rd argument */
   if (Dlg->TheDlg)
   {
      *release = true;
      while (GetMessage(&msg,NULL, 0,0) && DialogInAdminTable(Dlg) && (!Dlg->LeaveDialog)) {
#ifdef USE_DS_CONTROL
           if (Dlg && !IsDialogMessage(Dlg->TheDlg, &msg)
               && !IsDialogMessage(Dlg->AktChild, &msg))
#else
           if (Dlg && (!IsNestedDialogMessage(Dlg, &msg)))
#endif
                   DispatchMessage(&msg);
      }
   }
   else *release = true;
   EnterCriticalSection(&crit_sec);  /* otherwise Dlg might be still in table but DelDialog is already running */
   if (DialogInAdminTable(Dlg))
   {
       DelDialog(Dlg);
       Dlg->TheThread = NULL;
   }
   LeaveCriticalSection(&crit_sec);
   return ret;
}

RexxFunction5(uintptr_t, CreateUserParentDialog, uintptr_t, admin, int, count, uintptr_t, base, int, icon, int, modal)
{
    /* set number of items to dialogtemplate */
    p = (DLGTEMPLATE *)base;
    if (!p)
    {
        MessageBox(0,"Illegal resource buffer","Error",MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
        context->InvalidRoutine();
        return 0;
    }

    p->cdit = (WORD)count;

    EnterCriticalSection(&crit_sec);
    if (!InstallNecessaryStuff(dlgAdm, NULL))
    {
        if (dlgAdm)
        {
            DelDialog(dlgAdm);
            if (dlgAdm->pMessageQueue) DeleteMessageQueue(dlgAdm->pMessageQueue);
            LocalFree(dlgAdm);
        }
        LeaveCriticalSection(&crit_sec);
        return 0;
    }

    void *argList[2];

    //TODO:  make this a structure...
    argList[0] = p;         /* dialog template */
    argList[1] = dlgAdm;
    argList[2] = &Release;  /* pass along pointer so that variable can be modified */
    Release = false;
    dlgAdm->TheThread = hThread = CreateThread(NULL, 2000, WindowUsrLoopThread, argList, 0, &thID);

    while ((!Release) && dlgAdm && (dlgAdm->TheThread)) {Sleep(1);};  /* wait for dialog start */
    LeaveCriticalSection(&crit_sec);

    /* free the memory allocated for template */
    if (p) LocalFree(p);

    if (dlgAdm)
    {
        if (dlgAdm->TheDlg)
        {
           SetThreadPriority(dlgAdm->TheThread, THREAD_PRIORITY_ABOVE_NORMAL);   /* for a faster drawing */
           dlgAdm->OnTheTop = true;

           if (modal)  /* do we have a modal dialog? */
           {
             if (dlgAdm->previous && IsWindowEnabled(((DIALOGADMIN *)dlgAdm->previous)->TheDlg))
                 EnableWindow(((DIALOGADMIN *)dlgAdm->previous)->TheDlg, false);
           }

           if (GetWindowLong(dlgAdm->TheDlg, GWL_STYLE) & WS_SYSMENU)
           {
              if (icon > 0)
              {
                 dlgAdm->SysMenuIcon = (HICON)SetClassLong(dlgAdm->TheDlg, GCL_HICON, (LONG)LoadIcon(dlgAdm->TheInstance, MAKEINTRESOURCE(icon))));
              }
              else
              {
                 hI = LoadLibrary(VISDLL);
                 dlgAdm->SysMenuIcon = (HICON)SetClassLong(dlgAdm->TheDlg, GCL_HICON, (LONG)LoadIcon(hI, MAKEINTRESOURCE(99)));
                 FreeLibrary(hI);
              }
           }
           return dlgAdm->TheDlg;
        }
        else  /* the dialog creation failed, so do a clean up */
        {
           dlgAdm->OnTheTop = false;
           if (dlgAdm->previous) ((DIALOGADMIN *)(dlgAdm->previous))->OnTheTop = true;
           if (dlgAdm->previous && !IsWindowEnabled(((DIALOGADMIN *)dlgAdm->previous)->TheDlg))
                EnableWindow(((DIALOGADMIN *)dlgAdm->previous)->TheDlg, true);
           /* memory cleanup is done in HandleDialogAdmin */
        }
    }
}

RexxFunction5(uintptr_t, CreateUserChildDialog, uintptr_t, admin, int, count, uintptr_t, base, uintptr_t, dialog, int, id)
{
    /* set number of items to dialogtemplate */
    p = (DLGTEMPLATE *) admin;
    p->cdit = (WORD) count;

    /* send a create message. This is out of history so the child dialog has been created in a faster thread */
    HWND hW = (HWND) SendMessage((HWND)dialog, CREATECHILD, (WPARAM) dialog, (LPARAM) base);

    dlgAdm->ChildDlg[id] = hW;

    /* free the memory allocated for template */
    if (p) LocalFree(p);

    return hW;
}


void UAddControl(WORD **p, SHORT kind, INT id, INT x, INT y, INT cx, INT cy, CHAR * txt, ULONG lStyle)
{
   int   nchar;

   **p = LOWORD (lStyle);
   (*p)++;
   **p = HIWORD (lStyle);
   (*p)++;
   **p = 0;          // LOWORD (lExtendedStyle)
   (*p)++;
   **p = 0;          // HIWORD (lExtendedStyle)
   (*p)++;
   **p = x;         // x
   (*p)++;
   **p = y;         // y
   (*p)++;
   **p = cx;         // cx
   (*p)++;
   **p = cy;         // cy
   (*p)++;
   **p = id;         // ID
   (*p)++;

   **p = (WORD)0xffff;
   (*p)++;
   **p = (WORD)kind;
   (*p)++;

   if (txt)
   {
      nchar = nCopyAnsiToWideChar (*p, TEXT(txt));
      (*p) += nchar;
   }
   else
   {
     **p = 0;
    (*p)++;
   }

   **p = 0;  // advance pointer over nExtraStuff WORD
   (*p)++;

   /* make sure the next item starts on a DWORD boundary */
   (*p) = lpwAlign (*p);
}


void UAddNamedControl(WORD **p, CHAR * className, INT id, INT x, INT y, INT cx, INT cy, CHAR * txt, ULONG lStyle)
{
   int   nchar;

   **p = LOWORD (lStyle);
   (*p)++;
   **p = HIWORD (lStyle);
   (*p)++;
   **p = 0;          // LOWORD (lExtendedStyle)
   (*p)++;
   **p = 0;          // HIWORD (lExtendedStyle)
   (*p)++;
   **p = x;         // x
   (*p)++;
   **p = y;         // y
   (*p)++;
   **p = cx;         // cx
   (*p)++;
   **p = cy;         // cy
   (*p)++;
   **p = id;         // ID
   (*p)++;

   nchar = nCopyAnsiToWideChar (*p, TEXT(className));
   (*p) += nchar;

   if (txt)
   {
      nchar = nCopyAnsiToWideChar (*p, TEXT(txt));
      (*p) += nchar;
   }
   else
   {
     **p = 0;
    (*p)++;
   }

   **p = 0;  // advance pointer over nExtraStuff WORD
   (*p)++;

   /* make sure the next item starts on a DWORD boundary */
   (*p) = lpwAlign (*p);
}


RexxFunction8(uintptr_t, AddRadioButtonControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, text, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD | BS_AUTORADIOBUTTON;

    if (strstr(style, "OWNER")) lStyle |= BS_OWNERDRAW;
    if (strstr(style, "NOTIFY")) lStyle |= BS_NOTIFY;
    if (strstr(style, "LEFTTEXT")) lStyle |= BS_LEFTTEXT;
    if (strstr(style, "BITMAP")) lStyle |= BS_BITMAP;
    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;

    UAddControl(&dlg, 0x0080, id, x, y, cx, cy, text);
    return dlg;
}


RexxFunction8(uintptr_t, AddCheckBoxControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, text, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD | BS_AUTOCHECKBOX;

    if (strstr(style, "OWNER")) lStyle |= BS_OWNERDRAW;
    if (strstr(style, "NOTIFY")) lStyle |= BS_NOTIFY;
    if (strstr(style, "LEFTTEXT")) lStyle |= BS_LEFTTEXT;
    if (strstr(style, "BITMAP")) lStyle |= BS_BITMAP;
    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;

    UAddControl(&dlg, 0x0080, id, x, y, cx, cy, text);
    return dlg;
}


RexxFunction8(uintptr_t, AddButtonControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, text, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;
    if (strstr(style, "3STATE")) lStyle |= BS_AUTO3STATE; else
    if (strstr(style, "DEFAULT")) lStyle |= BS_DEFPUSHBUTTON; else lStyle |= BS_PUSHBUTTON;

    if (strstr(style, "OWNER")) lStyle |= BS_OWNERDRAW;
    if (strstr(style, "NOTIFY")) lStyle |= BS_NOTIFY;
    if (strstr(style, "LEFTTEXT")) lStyle |= BS_LEFTTEXT;
    if (strstr(style, "BITMAP")) lStyle |= BS_BITMAP;
    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;

    UAddControl(&dlg, 0x0080, id, x, y, cx, cy, text);
    return dlg;
}

RexxFunction8(uintptr_t, AddEntryLineControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;
    if (strstr(style, "PASSWORD")) lStyle |= ES_PASSWORD;
    if (strstr(style, "MULTILINE")) lStyle |= ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL;
    if (strstr(style, "AUTOSCROLLH")) lStyle |= ES_AUTOHSCROLL;
    if (strstr(style, "AUTOSCROLLV")) lStyle |= ES_AUTOVSCROLL;
    if (strstr(style, "HSCROLL")) lStyle |= WS_HSCROLL;
    if (strstr(style, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(style, "READONLY")) lStyle |= ES_READONLY;
    if (strstr(style, "WANTRETURN")) lStyle |= ES_WANTRETURN;
    if (strstr(style, "KEEPSELECTION")) lStyle |= ES_NOHIDESEL;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    if (strstr(style, "CENTER")) lStyle |= ES_CENTER;
    else if (strstr(style, "RIGHT")) lStyle |= ES_RIGHT;
    else lStyle |= ES_LEFT;
    if (strstr(style, "UPPER")) lStyle |= ES_UPPERCASE;
    if (strstr(style, "LOWER")) lStyle |= ES_LOWERCASE;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOBORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;

    UAddControl(&dlg, 0x0081, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}


RexxFunction8(uintptr_t, AddTextControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, text, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;
    if (!strstr(argv[6].strptr,"HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(argv[6].strptr,"BORDER")) lStyle |= WS_BORDER;
    if (strstr(argv[6].strptr,"GROUP")) lStyle |= WS_GROUP;
    if (strstr(argv[6].strptr,"DISABLED")) lStyle |= WS_DISABLED;

    if (strstr(argv[6].strptr,"CENTER")) lStyle |= SS_CENTER;
    else if (strstr(argv[6].strptr,"RIGHT")) lStyle |= SS_RIGHT;
    else if (strstr(argv[6].strptr,"SIMPLE")) lStyle |= SS_SIMPLE;
    else if (strstr(argv[6].strptr,"LEFTNOWRAP")) lStyle |= SS_LEFTNOWORDWRAP;
    else lStyle |= SS_LEFT;

    UAddControl(&dlg, 0x0082, id, x, y, cx, cy, text, lStyle);
    return dlg;
}


RexxFunction7(uintptr_t, AddListBoxControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "COLUMNS")) lStyle |= LBS_USETABSTOPS;
    if (strstr(style, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(style, "HSCROLL")) lStyle |= WS_HSCROLL;
    if (strstr(style, "SORT")) lStyle |= LBS_STANDARD;
    if (strstr(style, "NOTIFY")) lStyle |= LBS_NOTIFY;
    if (strstr(style, "MULTI")) lStyle |= LBS_MULTIPLESEL;
    if (strstr(style, "MCOLUMN")) lStyle |= LBS_MULTICOLUMN;
    if (strstr(style, "PARTIAL")) lStyle |= LBS_NOINTEGRALHEIGHT;
    if (strstr(style, "SBALWAYS")) lStyle |= LBS_DISABLENOSCROLL;
    if (strstr(style, "KEYINPUT")) lStyle |= LBS_WANTKEYBOARDINPUT;
    if (strstr(style, "EXTSEL")) lStyle |= LBS_EXTENDEDSEL;
    if (!strstr(style, "NOBORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;

    UAddControl(&dlg, 0x0083, id, x, y, cx, cy, NULL, lStytle);
    return dlg;
}


RexxFunction7(uintptr_t, AddComboBoxControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;

    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOHSCROLL")) lStyle |= CBS_AUTOHSCROLL;
    if (strstr(style, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(style, "SORT")) lStyle |= CBS_SORT;
    if (strstr(style, "SIMPLE")) lStyle |= CBS_SIMPLE;
    else if (strstr(style, "LIST")) lStyle |= CBS_DROPDOWNLIST;
    else lStyle |= CBS_DROPDOWN;
    if (!strstr(style, "NOBORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "PARTIAL")) lStyle |= CBS_NOINTEGRALHEIGHT;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;

    UAddControl(&dlg, 0x0085, id, x, y, cx, cy, NULL, lStytle);
    return dlg;
}


RexxFunction8(uintptr_t, AddGroupBoxControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, text, CSTRING, style)
{
    ULONG lStyle = 0;

lStyle = WS_CHILD | BS_GROUPBOX;
if (!strstr(argv[6].strptr,"HIDDEN")) lStyle |= WS_VISIBLE;
if (strstr(argv[6].strptr,"BORDER")) lStyle |= WS_BORDER;
if (strstr(argv[6].strptr,"GROUP")) lStyle |= WS_GROUP;
if (strstr(argv[6].strptr,"DISABLED")) lStyle |= WS_DISABLED;

    UAddControl(&dlg, 0x0080, id, x, y, cx, cy, text, lStyle);
    return dlg;
}


RexxFunction8(uintptr_t, AddFrameControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, int, type, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;

    if (type == 0) lStyle |= SS_WHITERECT; else
    if (type == 1) lStyle |= SS_GRAYRECT; else
    if (type == 2) lStyle |= SS_BLACKRECT; else
    if (type == 3) lStyle |= SS_WHITEFRAME; else
    if (type == 4) lStyle |= SS_GRAYFRAME; else
    lStyle |= SS_BLACKFRAME;

    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    lStyle = WS_CHILD;

    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (!strstr(style, "NOHSCROLL")) lStyle |= CBS_AUTOHSCROLL;
    if (strstr(style, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(style, "SORT")) lStyle |= CBS_SORT;
    if (strstr(style, "SIMPLE")) lStyle |= CBS_SIMPLE;
    else if (strstr(style, "LIST")) lStyle |= CBS_DROPDOWNLIST;
    else lStyle |= CBS_DROPDOWN;
    if (!strstr(style, "NOBORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "PARTIAL")) lStyle |= CBS_NOINTEGRALHEIGHT;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;

    UAddControl(&dlg, 0x0082, id, x, y, cx, cy, NULL, lStytle);
    return dlg;
}


RexxFunction7(uintptr_t, AddScrollBarControl, uintptr_t, dlg, int, it, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = 0;

    lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "HORIZONTAL")) lStyle |= SBS_HORZ; else lStyle |= SBS_VERT;
    if (strstr(style, "TOPLEFT")) lStyle |= SBS_TOPALIGN;
    if (strstr(style, "BOTTOMRIGHT")) lStyle |= SBS_BOTTOMALIGN;
    if (strstr(style, "TAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;

    UAddControl(&dlg, 0x0084, id, x, y, cx, cy, NULL, lStytle);
    return dlg;
}



/******************* New 32 Controls ***********************************/


LONG EvaluateListStyle(CHAR * styledesc)
{
    LONG lStyle = 0;

    if (!strstr(styledesc,"NOBORDER")) lStyle |= WS_BORDER;
    if (strstr(styledesc,"VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(styledesc,"HSCROLL")) lStyle |= WS_HSCROLL;
    if (!strstr(styledesc,"NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(styledesc,"EDIT")) lStyle |= LVS_EDITLABELS;
    if (strstr(styledesc,"SHOWSELALWAYS")) lStyle |= LVS_SHOWSELALWAYS;
    if (strstr(styledesc,"ALIGNLEFT")) lStyle |= LVS_ALIGNLEFT;
    if (strstr(styledesc,"ALIGNTOP")) lStyle |= LVS_ALIGNTOP;
    if (strstr(styledesc,"AUTOARRANGE")) lStyle |= LVS_AUTOARRANGE;
    if (strstr(styledesc,"ICON")) lStyle |= LVS_ICON;
    if (strstr(styledesc,"SMALLICON")) lStyle |= LVS_SMALLICON;
    if (strstr(styledesc,"LIST")) lStyle |= LVS_LIST;
    if (strstr(styledesc,"REPORT")) lStyle |= LVS_REPORT;
    if (strstr(styledesc,"NOHEADER")) lStyle |= LVS_NOCOLUMNHEADER;
    if (strstr(styledesc,"NOWRAP")) lStyle |= LVS_NOLABELWRAP;
    if (strstr(styledesc,"NOSCROLL")) lStyle |= LVS_NOSCROLL;
    if (strstr(styledesc,"NOSORTHEADER")) lStyle |= LVS_NOSORTHEADER;
    if (strstr(styledesc,"SHAREIMAGES")) lStyle |= LVS_SHAREIMAGELISTS;
    if (strstr(styledesc,"SINGLESEL")) lStyle |= LVS_SINGLESEL;
    if (strstr(styledesc,"ASCENDING")) lStyle |= LVS_SORTASCENDING;
    if (strstr(styledesc,"DESCENDING")) lStyle |= LVS_SORTDESCENDING;
    return lStyle;
}


RexxFunction7(uintptr_t, AddTreeControl, uintptr_t, dlg, int, id, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "VSCROLL")) lStyle |= WS_VSCROLL;
    if (strstr(style, "HSCROLL")) lStyle |= WS_HSCROLL;
    if (strstr(style, "NODRAG")) lStyle |= TVS_DISABLEDRAGDROP;
    if (strstr(style, "EDIT")) lStyle |= TVS_EDITLABELS;
    if (strstr(style, "BUTTONS")) lStyle |= TVS_HASBUTTONS;
    if (strstr(style, "LINES")) lStyle |= TVS_HASLINES;
    if (strstr(style, "ATROOT")) lStyle |= TVS_LINESATROOT;
    if (strstr(style, "SHOWSELALWAYS")) lStyle |= TVS_SHOWSELALWAYS;
    UAddNamedControl(&dlg, WC_TREEVIEW, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}


RexxFunction7(uintptr_t, AddListControl, uintptr_t, dlg, int, id, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;


    lStyle |= EvaluateListStyle(style);
    UAddNamedControl(&dlg, WC_LISTVIEW, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}


RexxFunction7(uintptr_t, AddProgressControl, uintptr_t, dlg, int, id, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;


    if (strstr(style, "VERTICAL")) lStyle |= PBS_VERTICAL;
    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (strstr(style, "SMOOTH")) lStyle |= PBS_SMOOTH;
    UAddNamedControl(&dlg, PROGRESS_CLASS, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}


RexxFunction7(uintptr_t, AddSliderControl, uintptr_t, dlg, int, id, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;


    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "AUTOTICKS")) lStyle |= TBS_AUTOTICKS;
    if (strstr(style, "NOTICKS")) lStyle |= TBS_NOTICKS;
    if (strstr(style, "VERTICAL")) lStyle |= TBS_VERT;
    if (strstr(style, "HORIZONTAL")) lStyle |= TBS_HORZ;
    if (strstr(style, "TOP")) lStyle |= TBS_TOP;
    if (strstr(style, "BOTTOM")) lStyle |= TBS_BOTTOM;
    if (strstr(style, "LEFT")) lStyle |= TBS_LEFT;
    if (strstr(style, "RIGHT")) lStyle |= TBS_RIGHT;
    if (strstr(style, "BOTH")) lStyle |= TBS_BOTH;
    if (strstr(style, "ENABLESELRANGE")) lStyle |= TBS_ENABLESELRANGE;
    UAddNamedControl(&dlg, TRACKBAR_CLASS, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}


RexxFunction7(uintptr_t, AddTabControl, uintptr_t, dlg, int, id, int, x, int, y, int, cx, int, cy, CSTRING, style)
{
    ULONG lStyle = WS_CHILD;
    if (!strstr(style, "HIDDEN")) lStyle |= WS_VISIBLE;
    if (strstr(style, "GROUP")) lStyle |= WS_GROUP;
    if (strstr(style, "DISABLED")) lStyle |= WS_DISABLED;


    if (strstr(style, "BORDER")) lStyle |= WS_BORDER;
    if (!strstr(style, "NOTAB")) lStyle |= WS_TABSTOP;
    if (strstr(style, "BUTTONS")) lStyle |= TCS_BUTTONS;
    else lStyle |= TCS_TABS;
    if (strstr(style, "FIXED")) lStyle |= TCS_FIXEDWIDTH;
    if (strstr(style, "FOCUSNEVER")) lStyle |= TCS_FOCUSNEVER;
    if (strstr(style, "FOCUSONDOWN")) lStyle |= TCS_FOCUSONBUTTONDOWN;
    if (strstr(style, "ICONLEFT")) lStyle |= TCS_FORCEICONLEFT;
    if (strstr(style, "LABELLEFT")) lStyle |= TCS_FORCELABELLEFT;
    if (strstr(style, "MULTILINE")) lStyle |= TCS_MULTILINE;
    else lStyle |= TCS_SINGLELINE;
    if (strstr(style, "ALIGNRIGHT")) lStyle |= TCS_RIGHTJUSTIFY;
    if (strstr(style, "CLIPSIBLINGS")) lStyle |= WS_CLIPSIBLINGS;  /* used for property sheet to prevent wrong display */
    UAddNamedControl(&dlg, WS_TABCONTROL, id, x, y, cx, cy, NULL, lStyle);
    return dlg;
}



extern bool SHIFTkey = false;

#ifndef USE_DS_CONTROL
bool IsNestedDialogMessage(
    DIALOGADMIN * dlgAdm,
    LPMSG  lpmsg    // address of structure with message
   )
{
   HWND hW, hParent, hW2;
   bool prev = false;

   if ((!dlgAdm->ChildDlg) || (!dlgAdm->AktChild))
      return IsDialogMessage(dlgAdm->TheDlg, lpmsg);

   switch (lpmsg->message)
   {
      case WM_KEYDOWN:
         switch (lpmsg->wParam)
            {
            case VK_SHIFT: SHIFTkey = true;
               break;

            case VK_TAB:

               if (IsChild(dlgAdm->AktChild, lpmsg->hwnd)) hParent = dlgAdm->AktChild; else hParent = dlgAdm->TheDlg;

               hW = GetNextDlgTabItem(hParent, lpmsg->hwnd, SHIFTkey);
               hW2 = GetNextDlgTabItem(hParent, NULL, SHIFTkey);

               /* see if we have to switch to the other dialog */
               if (hW == hW2)
               {
                  if (hParent == dlgAdm->TheDlg)
                     hParent = dlgAdm->AktChild;
                  else
                     hParent = dlgAdm->TheDlg;

                  hW = GetNextDlgTabItem(hParent, NULL, SHIFTkey);
                  return SendMessage(hParent, WM_NEXTDLGCTL, (WPARAM)hW, (LPARAM)true);

               } else return SendMessage(hParent, WM_NEXTDLGCTL, (WPARAM)hW, (LPARAM)true);

                return true;

            case VK_LEFT:
            case VK_UP:
               prev = true;
            case VK_RIGHT:
            case VK_DOWN:

               if (IsChild(dlgAdm->AktChild, lpmsg->hwnd)) hParent = dlgAdm->AktChild; else hParent = dlgAdm->TheDlg;

               hW = GetNextDlgGroupItem(hParent, lpmsg->hwnd, prev);
               hW2 = GetNextDlgGroupItem(hParent, NULL, prev);

               /* see if we have to switch to the other dialog */
               if (hW == hW2)
               {
                  if (hParent == dlgAdm->TheDlg)
                     hParent = dlgAdm->AktChild;
                  else
                     hParent = dlgAdm->TheDlg;

                   return IsDialogMessage(hParent, lpmsg);

               } else
                return IsDialogMessage(hParent, lpmsg);

                return true;

            case VK_CANCEL:
            case VK_RETURN:
               return IsDialogMessage(dlgAdm->TheDlg, lpmsg);

            default:
               hParent = (HWND) GetWindowLong(lpmsg->hwnd, GWL_HWNDPARENT);
               if (!hParent) return false;
               return IsDialogMessage(hParent, lpmsg);
           }
         break;

      case WM_KEYUP:
         if (lpmsg->wParam == VK_SHIFT) SHIFTkey = false;
         break;
   }
   hParent = (HWND) GetWindowLong(lpmsg->hwnd, GWL_HWNDPARENT);
   if (hParent)
      return IsDialogMessage(hParent, lpmsg);
   else return IsDialogMessage(dlgAdm->TheDlg, lpmsg);
}
#endif


RexxFunction1(RexxObjectPtr, CreateMenu, int count)
{
    WORD *p, *template;
    HANDLE hMem;
    hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, (count + 1)*128);

    template = p = (PWORD) GlobalLock(hMem);

    if (p == NULL)
    {
        return context->NilObject();
    }

    /* writing menu header */
 #if EXTENDED_MENU
    *p = 1;    /* for extended menu */
    p++;
    *p = 0;
    offsetptr = p;
    p++;
    *p = 0;   /* DWORD helpid */
    p++;
    *p = 0;
    p++;
    p = lpwAlign (p);
    *offsetptr = (ULONG)p - (ULONG)offsetptr - sizeof(WORD);
 #else
    *p = 0;    /* for normal menu */
    p++;
    *p = 0;
    p++;
 #endif

    RexxArrayObject result = context->NewArray(3);

    ValueDescriptor type;

    type.value.value_uintptr_t = (uintptr_t)hMem;
    type.type = REXX_VALUE_uintptr_t;

    RexxObject temp = ValueToObject(&type);
    context->ArrayPut(result, 1, temp);

    type.value.value_uintptr_t = (uintptr_t)template;
    type.type = REXX_VALUE_uintptr_t;

    temp = ValueToObject(&type);
    context->ArrayPut(result, 2, temp);


    type.value.value_uintptr_t = (uintptr_t)p;
    type.type = REXX_VALUE_uintptr_t;
    temp = context->ValueToObject(&type);
    context->ArrayPut(result, 3, temp);

    return result;
}


RexxFunction4(uintptr_t, AddMenuItem, uintptr_t, base, int, id, CSTRING, name, CSTRING, style)
{
    ULONG lStyle = MFS_ENABLED | MFS_UNCHECKED;
    ULONG lType = MFT_STRING;
    WORD lResInfo = 0;
    int nchar;

    WORD *p = (WORD *)base;

#if EXTENDED_MENU
    if (strstr(style, "CHECK")) lStyle |= MFS_CHECKED;
    if (strstr(style, "GRAY")) lStyle |= MFS_GRAYED;
    if (strstr(style, "DISABLE")) lStyle |= MFS_DISABLED;
    if (strstr(style, "HILITE")) lStyle |= MFS_HILITE;

    if (strstr(style, "POPUP")) lResInfo |= 0x01;
    if (strstr(style, "END")) lResInfo |= 0x80;

    if (strstr(style, "SEPARATOR")) lType = MFT_SEPARATOR;
    if (strstr(style, "RIGHT")) lType |= MFT_RIGHTJUSTIFY;
    if (strstr(style, "RADIO")) lType |= MFT_RADIOCHECK;

    This is not finsihed!

    dp = p;
    *dp = lType;
    dp++;
    *dp = lStyle;
    dp++;
    p = dp;
    *p = id;                       /* menu id */
    p++;
    *p = lResInfo;
    p++;

    /* copy the name of the item */
    if (strlen(name) > 96)
    {
        *p = 0;
        p++;
    }
    else
    {
        nchar = nCopyAnsiToWideChar (p, TEXT(name));
        p += nchar;
    }
    p = lpwAlign (p);
    if (lResInfo == 0x01)
    {
        *p = 0;
        p++;
        *p = 0;
        p++;
        p = lpwAlign (p);
    }
#else
    if (strstr(style, "CHECKED")) lStyle |= MF_CHECKED;
    if (strstr(style, "GRAYED")) lStyle |= MF_GRAYED;
    if (strstr(style, "DISABLED")) lStyle |= MF_DISABLED;
    if (strstr(style, "POPUP")) lStyle |= MF_POPUP;
    if (strstr(style, "END")) lStyle |= MF_END;
    if (strstr(style, "SEPARATOR")) lStyle |= MF_SEPARATOR;


    *p = (WORD)lStyle;
    p++;

    if (!(lStyle & MF_POPUP))
    {
        *p = id;                       /* menu id */
        p++;
    }

    /* copy the name of the item */
    if (strlen(name) > 96)
    {
        *p = 0;
        p++;
    }
    else
    {
        nchar = nCopyAnsiToWideChar (p, TEXT(name));
        p += nchar;
    }
#endif
    return p;
}

RexxFunction3(int, ModifyMenu, uintptr_t, dlg, uintptr_t, mem, uintptr_t, base)
{
    HWND hWnd;
    PVOID *p;

    hWnd = (HWND)dlg;

    if (hWnd)
    {
       SEEK_DLGADM_TABLE(hWnd, dlgAdm);
       if (!dlgAdm)
       {
           return 1;
       }

       hMem = (HANDLE)mem;
       p = (PVOID *)base;

       if (p)
       {
           INT rc;
           dlgAdm->menu = LoadMenuIndirect(p);
           rc = GetLastError();

           if (dlgAdm->menu)
               SetMenu(hWnd, dlgAdm->menu);

           /* free the memory allocated for template */
           GlobalUnlock(hMem);
           GlobalFree(hMem);
           if (!dlgAdm->menu)
           {
               return rc;
           }
           return 0;
       }
    }
    return 1;
}
