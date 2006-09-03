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
#include <mmsystem.h>
#include <oorexx.h>
#include <stdio.h>
#include <dlgs.h>
#ifdef __CTL3D
#include <ctl3d.h>
#endif
/* set the noglob... so global variables wont be defined twice */
#define NOGLOBALVARIABLES 1
#include "oovutil.h"
#undef NOGLOBALVARIABLES


extern HINSTANCE MyInstance = NULL;

extern DIALOGADMIN * DialogTab[MAXDIALOGS] = {NULL};
extern DIALOGADMIN * topDlg = {NULL};
extern INT StoredDialogs = 0;
extern CRITICAL_SECTION crit_sec = {0};
extern WPARAM InterruptScroll;

extern BOOL SearchMessageTable(ULONG message, WPARAM param, LPARAM lparam, DIALOGADMIN * addressedTo);
extern BOOL DrawBitmapButton(DIALOGADMIN * addr, HWND hDlg, WPARAM wParam, LPARAM lParam, bool MsgEnabled);
extern BOOL DrawBackgroundBmp(DIALOGADMIN * addr, HWND hDlg, WPARAM wParam, LPARAM lParam);
extern BOOL DataAutodetection(DIALOGADMIN * aDlg);
extern LRESULT PaletteMessage(DIALOGADMIN * addr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern BOOL AddDialogMessage(MESSAGE_QUEUE *q, char *message, int args, ULONG param, ULONG id, char *text);
extern BOOL IsNT = true;
extern MESSAGE_QUEUE *CreateMessageQueue();

INT DelDialog(DIALOGADMIN * aDlg);

bool DialogInAdminTable(DIALOGADMIN * Dlg)
{
    register INT i;
    for (i = 0; i < StoredDialogs; i++)
        if (DialogTab[i] == Dlg) break;
    return (i < StoredDialogs);
}

/* dialog procedure
   handles the search for user defined messages and bitmap buttons
   handles messages necessary for 3d controls
   seeks for the addressed dialog to handle the messages */


LRESULT CALLBACK RexxDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   HBRUSH hbrush = NULL;
   HWND hW;
   DIALOGADMIN * addressedTo = NULL;
   bool MsgEnabled = false;
   register INT i=0;

   if (uMsg != WM_INITDIALOG)
   {
       SEEK_DLGADM_TABLE(hDlg, addressedTo);
       if (!addressedTo && topDlg && DialogInAdminTable(topDlg) && topDlg->TheDlg) addressedTo = topDlg;

       if (addressedTo)
       {
          MsgEnabled = IsWindowEnabled(hDlg) && DialogInAdminTable(addressedTo);
          if (MsgEnabled && (uMsg != WM_PAINT) && (uMsg != WM_NCPAINT)                /* do not search message table for WM_PAINT to improve redraw */
              && SearchMessageTable(uMsg, wParam, lParam, addressedTo)) return false;

          switch (uMsg) {
             case WM_PAINT:
                if (addressedTo->BkgBitmap) DrawBackgroundBmp(addressedTo, hDlg, wParam, lParam);
                break;

             case WM_DRAWITEM:
                if ((lParam != 0) && (addressedTo)) return DrawBitmapButton(addressedTo, hDlg, wParam, lParam, MsgEnabled);
                break;

             case WM_CTLCOLORDLG:
                if (addressedTo->BkgBrush) return (LRESULT) addressedTo->BkgBrush;

             case WM_CTLCOLORSTATIC:
             case WM_CTLCOLORBTN:
             case WM_CTLCOLOREDIT:
             case WM_CTLCOLORLISTBOX:
             case WM_CTLCOLORMSGBOX:
             case WM_CTLCOLORSCROLLBAR:
                if (addressedTo->CT_size)   /* Has the dialog item its own user color ?*/
                {
                    id = GetWindowLong((HWND)lParam, GWL_ID);
                    SEARCHBRUSH(addressedTo, i, id, hbrush);
                    if (hbrush)
                    {
                        SetBkColor((HDC)wParam, PALETTEINDEX(addressedTo->ColorTab[i].ColorBk));
                        if (addressedTo->ColorTab[i].ColorFG != -1) SetTextColor((HDC)wParam, PALETTEINDEX(addressedTo->ColorTab[i].ColorFG));
                    }
                }
                if (hbrush)
                   return (LRESULT) hbrush;
                else
                   return DefWindowProc(hDlg, uMsg, wParam, lParam);

             case WM_COMMAND:
             switch( LOWORD(wParam) ) {
                   case IDOK:
                      if (!HIWORD(wParam)) addressedTo->LeaveDialog = 1; /* Notify code must be 0 */
                      return true;
                      break;
                   case IDCANCEL:
                      if (!HIWORD(wParam)) addressedTo->LeaveDialog = 2; /* Notify code must be 0 */
                      return true;
                      break;
             }
             break;

             case WM_QUERYNEWPALETTE:
             case WM_PALETTECHANGED:
                if (addressedTo) return PaletteMessage(addressedTo, hDlg, uMsg, wParam, lParam);
                break;

             case WM_SETTEXT:
             case WM_NCPAINT:
             case WM_NCACTIVATE:
                return false;

             case CREATECHILD:
                hW = CreateDialogIndirectParam(MyInstance, (DLGTEMPLATE *) lParam, (HWND) wParam, RexxDlgProc, addressedTo->Use3DControls); /* pass 3D flag to WM_INITDIALOG */
                ReplyMessage((LRESULT) hW);
                return (LRESULT) hW;
             case INTERRUPTSCROLL:
                addressedTo->StopScroll = wParam;
                return (true);
             case GETSETFOCUS:
                if (!lParam) ReplyMessage((LRESULT)GetFocus());
                else ReplyMessage((LRESULT)SetFocus((HWND)lParam));
                return (true);
             case GETSETCAPTURE:
                if (!wParam) ReplyMessage((LRESULT)GetCapture());
                else if (wParam == 2) ReplyMessage((LRESULT)ReleaseCapture());
                else ReplyMessage((LRESULT)SetCapture((HWND)lParam));
                return (true);
             case GETKEYSTATE:
                ReplyMessage((LRESULT)GetAsyncKeyState(wParam));
                return (true);
          }
       }
   }
   else
   /* the WM_INITDIALOG message is sent by CreateDialog(Indirect)Param before TheDlg is set */
   {
         return true;
   }
   return false;
}


RexxFunction1(int, CleanupDialogTable, uintptr_t, admin)
{
    EnterCriticalSection(&crit_sec);
    DIALOGADMIN *dlgAdmin = (DIALOGADMIN *)admin;
    if (!dlgAdm)
    {
         LeaveCriticalSection(&crit_sec);
         return -1;
    }

    if (DialogInAdminTable(dlgAdm)) DelDialog(dlgAdm);
    if (dlgAdm->pMessageQueue) DeleteMessageQueue(dlgAdm->pMessageQueue);
    LocalFree(dlgAdm);
    LeaveCriticalSection(&crit_sec);
    return 0;
}

RexxFunction0(uintptr_t, CreateDialogTable)
{
    DIALOGADMIN * current;
    EnterCriticalSection(&crit_sec);
    if (StoredDialogs<MAXDIALOGS)
    {
       current = (DIALOGADMIN *) LocalAlloc(LPTR, sizeof(DIALOGADMIN));
       if (current) current->pMessageQueue = CreateMessageQueue();
       if (!current || !current->pMessageQueue)
       {
          MessageBox(0,"Out of system resources","Error",MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
          LeaveCriticalSection(&crit_sec);
          return 0;
       }
       current->previous = topDlg;
       current->TableEntry = StoredDialogs;
       StoredDialogs++;
       DialogTab[current->TableEntry] = current;
       LeaveCriticalSection(&crit_sec);
       return current;
    }
    else
    {
       MessageBox(0,"To many active Dialogs","Error",MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
    }

    LeaveCriticalSection(&crit_sec);
    return 0;
}


/* install DLL, control program and 3D controls */
bool InstallNecessaryStuff(DIALOGADMIN* dlgAdm, char *library)
{
   if (dlgAdm->previous) ((DIALOGADMIN*)dlgAdm->previous)->OnTheTop = false;
   topDlg = dlgAdm;

   if (library != NULL)
   {
      dlgAdm->TheInstance = LoadLibrary(library);
      if (!dlgAdm->TheInstance)
      {
         MessageBox(0,"Couldn't load DLL","Error",MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
         return false;
      }
   } else
      dlgAdm->TheInstance = MyInstance;
   return true;
}



/* end a dialog and remove installed components */
INT DelDialog(DIALOGADMIN * aDlg)
{
   DIALOGADMIN * current;
   INT ret, i;
   bool wasFGW;

   EnterCriticalSection(&crit_sec);
   wasFGW = (aDlg->TheDlg == GetForegroundWindow());

   ret = aDlg->LeaveDialog;

   /* add this message, so HandleMessages in DIALOG.CMD knows that finsihed shall be set */
   AddDialogMessage(aDlg->pMessageQueue, MSG_TERMINATE, 0, 0, 0, NULL);

   if (!aDlg->LeaveDialog) aDlg->LeaveDialog = 3;    /* signal to exit */

   /* the entry must be removed before the last message is sent so the GetMessage loop can quit */
   if (aDlg->TableEntry == StoredDialogs-1)
       DialogTab[aDlg->TableEntry] = NULL;
   else
   {
       DialogTab[aDlg->TableEntry] = DialogTab[StoredDialogs-1];  /* move last entry to deleted one */
       DialogTab[aDlg->TableEntry]->TableEntry = aDlg->TableEntry;
       DialogTab[StoredDialogs-1] = NULL;              /* and delete last entry */
   }
   StoredDialogs--;

   PostMessage(aDlg->TheDlg, WM_QUIT, 0, 0);      /* to exit GetMessage */

   if (aDlg->TheDlg) DestroyWindow(aDlg->TheDlg);      /* docu states "must not use EndDialog for non-modal dialogs" */
   if (aDlg->menu) DestroyMenu(aDlg->menu);

#ifdef __CTL3D
   if ((!StoredDialogs) && (aDlg->Use3DControls)) Ctl3dUnregister(aDlg->TheInstance);
#endif
   DeleteObject((HICON) SetClassLong(aDlg->TheDlg, GCL_HICON, (LONG)aDlg->SysMenuIcon));
   if ((aDlg->TheInstance) && (aDlg->TheInstance != MyInstance)) FreeLibrary(aDlg->TheInstance);
   current = (DIALOGADMIN *)aDlg->previous;

   /* delete data, message and bitmap table of the dialog */
   if (aDlg->MsgTab)
   {
       for (i=0;i<aDlg->MT_size;i++)
       {
           if (aDlg->MsgTab[i].rexxProgram) LocalFree(aDlg->MsgTab[i].rexxProgram);
       }
       LocalFree(aDlg->MsgTab);
       aDlg->MT_size = 0;
   }
   if (aDlg->DataTab) LocalFree(aDlg->DataTab);
   aDlg->DT_size = 0;

   /* delete color brushs */
   if (aDlg->ColorTab)
   {
       for (i=0;i<aDlg->CT_size;i++)
           if (aDlg->ColorTab[i].ColorBrush) DeleteObject((HBRUSH)aDlg->ColorTab[i].ColorBrush);
       LocalFree(aDlg->ColorTab);
       aDlg->CT_size = 0;
   }

   /* delete bitmaps */
   if (aDlg->BmpTab)
   {
      for (i=0;i<aDlg->BT_size;i++)
         if ((aDlg->BmpTab[i].Loaded & 0x1011) == 1)           /* otherwise stretched bitmap files are not freed */
         {
            if (aDlg->BmpTab[i].bitmapID) LocalFree((void *)aDlg->BmpTab[i].bitmapID);
            if (aDlg->BmpTab[i].bmpFocusID) LocalFree((void *)aDlg->BmpTab[i].bmpFocusID);
            if (aDlg->BmpTab[i].bmpSelectID) LocalFree((void *)aDlg->BmpTab[i].bmpSelectID);
            if (aDlg->BmpTab[i].bmpDisableID) LocalFree((void *)aDlg->BmpTab[i].bmpDisableID);
         }
         else if (aDlg->BmpTab[i].Loaded == 0)
         {
            if (aDlg->BmpTab[i].bitmapID) DeleteObject((HBITMAP)aDlg->BmpTab[i].bitmapID);
            if (aDlg->BmpTab[i].bmpFocusID) DeleteObject((HBITMAP)aDlg->BmpTab[i].bmpFocusID);
            if (aDlg->BmpTab[i].bmpSelectID) DeleteObject((HBITMAP)aDlg->BmpTab[i].bmpSelectID);
            if (aDlg->BmpTab[i].bmpDisableID) DeleteObject((HBITMAP)aDlg->BmpTab[i].bmpDisableID);
         }

      LocalFree(aDlg->BmpTab);
      if (aDlg->ColorPalette) DeleteObject(aDlg->ColorPalette);
      aDlg->BT_size = 0;
   }

       /* The message queue and the admin block are freed in HandleDialogAdmin called from HandleMessages */

   if (!StoredDialogs) topDlg = NULL; else topDlg = current;
   if (topDlg)
   {
       if (DialogInAdminTable(topDlg))
       {
           if (!IsWindowEnabled(topDlg->TheDlg))
               EnableWindow(topDlg->TheDlg, true);
           if (wasFGW)
           {
               SetForegroundWindow(topDlg->TheDlg);
               topDlg->OnTheTop = true;
           }
       }
       else topDlg = NULL;
   }
   LeaveCriticalSection(&crit_sec);
   return ret;
}


/* create an asynchronous dialog and run asynchronous message loop */
DWORD WINAPI WindowLoopThread(void * arg)
{
   MSG msg;
   CHAR buffer[NR_BUFFER];
   DIALOGADMIN * Dlg;
   bool * release;
   ULONG ret;

   Dlg = (DIALOGADMIN*)arg[1];  /*  thread local admin pointer from StartDialog */
   Dlg->TheDlg = CreateDialogParam( Dlg->TheInstance, MAKEINTRESOURCE((int)arg[0]), 0, (DLGPROC) RexxDlgProc, Dlg->Use3DControls);  /* pass 3D flag to WM_INITDIALOG */
   Dlg->ChildDlg[0] = Dlg->TheDlg;

   release = (BOOL *)arg[3];  /* the Release flag is stored as the 4th argument */
   if (Dlg->TheDlg)
   {
      if ((BOOL)arg[2])
         if (!DataAutodetection(Dlg))
         {
            Dlg->TheThread = NULL;
            return 0;
         };

      *release = true;  /* Release wait in StartDialog  */
      do
      {
         if (GetMessage(&msg,NULL, 0,0)) {
           if (!IsDialogMessage (Dlg->TheDlg, &msg))
               DispatchMessage(&msg);
         }
      } while (Dlg && DialogInAdminTable(Dlg) && !Dlg->LeaveDialog);
   } else *release = true;
   EnterCriticalSection(&crit_sec);
   if (DialogInAdminTable(Dlg))
   {
       ret = DelDialog(Dlg);
       Dlg->TheThread = NULL;
   }
   LeaveCriticalSection(&crit_sec);
   return ret;
}


RexxFunction0(RexxStringObject, GetDialogFactor)
{
    ULONG u;
    double x,y;
    u = GetDialogBaseUnits();


    x = LOWORD(u);
    y = HIWORD(u);

    x = x / 4;
    y = y / 8;

    char buffer[256];

    sprintf(buffer, "%4.1f %4.1f", x, y);
    return context->NewStringFromAsciiz(buffer);
}

RexxFunction6(uintptr_t, StartDialog, uintptr_t, admin, CSTRING, library, int, resource, RexxBoolean, autodetect, int, icon, RexxBoolean, modal)
{
    void *argList[4];
    HINSTANCE hI;
    ULONG thID;
    bool Release = false;

    DIALOGADMIN *dlgAdmin = (DIALOGADMIN *)admin;
    if (!dlgAdm)
    {
        return 0;
    }

    EnterCriticalSection(&crit_sec);
    if (!InstallNecessaryStuff(dlgAdm, library)
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

    argList[0] = (void *) id;              /* dialog resource id */
    argList[1] = (void *) dlgAdm;
    argList[2] = (void *) autodetect;  /* auto detection? */
    argList[3] = (void *) &Release;  /* pass along pointer so that variable can be modified */

    dlgAdm->TheThread = CreateThread(NULL, 2000, WindowLoopThread, argList, 0, &thID);

    while ((!Release) && (dlgAdm->TheThread)) {Sleep(1);};  /* wait for dialog start */
    LeaveCriticalSection(&crit_sec);

    if (dlgAdm)
    {
        if (dlgAdm->TheDlg)
        {
           SetThreadPriority(dlgAdm->TheThread, THREAD_PRIORITY_ABOVE_NORMAL);   /* for a faster drawing */
           dlgAdm->OnTheTop = true;
                                   /* modal flag = yes ? */
           if (dlgAdm->previous && modal && IsWindowEnabled(((DIALOGADMIN *)dlgAdm->previous)->TheDlg)) EnableWindow(((DIALOGADMIN *)dlgAdm->previous)->TheDlg, false);

           if (icon > 0)
           {
               dlgAdm->SysMenuIcon = (HICON)SetClassLong(dlgAdm->TheDlg, GCL_HICON, (LONG)LoadIcon(dlgAdm->TheInstance, MAKEINTRESOURCE(icon)));
           }
           else
           {
               hI = LoadLibrary(VISDLL);
               dlgAdm->SysMenuIcon = (HICON)SetClassLong(dlgAdm->TheDlg, GCL_HICON, (LONG)LoadIcon(hI, MAKEINTRESOURCE(99)));
               FreeLibrary(hI);
           }

        }
        dlgAdm->OnTheTop = false;
        if (dlgAdm->previous) ((DIALOGADMIN *)(dlgAdm->previous))->OnTheTop = true;
        if ((dlgAdm->previous) && !IsWindowEnabled(((DIALOGADMIN *)dlgAdm->previous)->TheDlg))
           EnableWindow(((DIALOGADMIN *)dlgAdm->previous)->TheDlg, true);
        return dlgAdm->TheDlg;
    }
    return 0;
}


LONG InternalStopDialog(HWND h)
{
   ULONG i, ret;
   DIALOGADMIN * dadm = NULL;
   MSG msg;
   HANDLE hTh;

   if (!h) dadm = topDlg;
   else SEEK_DLGADM_TABLE(h, dadm);

   if (dadm)
   {
       DWORD ec;
       hTh = dadm->TheThread;
       if (!dadm->LeaveDialog) dadm->LeaveDialog = 3;    /* signal to exit */
       PostMessage(dadm->TheDlg, WM_QUIT, 0, 0);      /* to exit GetMessage */
       DestroyWindow(dadm->TheDlg);      /* to remove system resources */
       if (dadm->TheThread) GetExitCodeThread(dadm->TheThread, &ec);
       i = 0; while (dadm && (ec == STILL_ACTIVE) && dadm->TheThread && (i < 1000))  /* wait for window thread of old dialog to terminate */
       {
           if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
               DispatchMessage(&msg);
           }
           Sleep(1); i++;
           GetExitCodeThread(dadm->TheThread, &ec);
       }

       if (dadm->TheThread && (ec == STILL_ACTIVE))
       {
           TerminateThread(dadm->TheThread, -1);
           ret = 1;
       } else ret = 0;
       if (hTh) CloseHandle(hTh);
       if (dadm == topDlg) topDlg = NULL;

       /* The message queue and the admin block are freed in HandleDialogAdmin called from HandleMessages */

       return ret;
   }
   return -1;
}

_declspec(dllexport) LONG __cdecl OODialogCleanup(bool Process)
{
    if (Process)
    {
        INT i;
        for (i = MAXDIALOGS; i>0; i--)
        {
            if (DialogTab[i-1]) InternalStopDialog(DialogTab[i-1]->TheDlg);
            DialogTab[i-1] = NULL;
        }
        StoredDialogs = 0;
    }
    return (StoredDialogs);
}

RexxFunction1(RexxBoolean, IsDialogActive, uintptr_t, dlg)
{
    DIALOGADMIN *dlgAdm = NULL;

    SEEK_DLGADM_TABLE((HWND)dlg, dlgAdm);

    return dlgAdm == NULL ? 0 : 1;
}


RexxFuction1(uintptr_t, GetDialogHandle, uintptr_t, admin)
{
    dlgAdm = (DIALOGADMIN*)admin;
    if (!dlgAdm) return 0;
    return dlgAdm->TheDlg;
}


RexxFuction0(uintptr_t, GetTopDialog)
{
    return topDlg->TheDlg;
}


RexxFunction2(uintptr_t, GetDialogItem, int, id, uintptr_t, handle)
{
    HWND hD = (HWND)handle;
    if (hD == 0)
    {
        hD = topDlg->TheDlg;
    }

    return GetDlgItem(hD, id);
}


RexxFunction1(int, StopDialog, uintptr_t, handle)
{
    DIALOGADMIN *dlgAdm = NULL;
    HWND h = (HWND)handle;

    if (h)
    {
        SEEK_DLGADM_TABLE(h, dlgAdm);
        if (dlgAdm)
        {
            return DelDialog(dlgAdm);
        }
        return -1;
    }
    else if (!h && topDlg)
    {
        DelDialog(topDlg);
    }
    else
    {
        return -1;
    }
}


RexxFunction2(RexxBoolean, AssociateDialogMenu, uintptr_t, handle, int, id)
{
    DIALOGADMIN *dlgAdm = NULL;
    hWnd = (HWND)handle;

    if (hWnd)
    {
       SEEK_DLGADM_TABLE(hWnd, dlgAdm);
       if (dlgAdm)
       {
           dlgAdm->menu = LoadMenu(dlgAdm->TheInstance, MAKEINTRESOURCE(id));
           if (dlgAdm->menu)
           {
               SetMenu(hWnd, dlgAdm->menu);
               return 0
           }
       }
    }
    return 1;
}


RexxFunction3(RexxBoolean, EnableMenuItem, uintptr_t, admin, int, id, int state)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    if (!dlgAdm || !dlgAdm->menu)
    {
        return 1;
    }

    if (state == 0) opt = MF_ENABLED; else
    if (state == 1) opt = MF_DISABLED; else
    else opt = MF_GRAYED
    return EnableMenuItem(dlgAdm->menu, atoi(argv[2].strptr), MF_BYCOMMAND | opt) == 0xFFFFFFFF) ? 1 : 0;
}


RexxFunction3(RexxBoolean, CheckMenuItem, uintptr_t, admin, int, id, int, state)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    if (!dlgAdm || !dlgAdm->menu)
    {
        return 1;
    }

    int opt = (state = 0) ? MF_UNCHECKED : MF_CHECKED;
    return CheckMenuItem(dlgAdm->menu, id, MF_BYCOMMAND | opt) == 0xFFFFFFFF ? 1 : 0;
}


RexxFunction4(RexxBoolean, CheckMenuRadioItem, uintptr_t, admin, int, idstart, int, idend, int, idset)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    if (!dlgAdm || !dlgAdm->menu)
    {
        return 1;
    }
                                                     /* start of group       end                selected item */
    return !CheckMenuRadioItem(dlgAdm->menu, idstart, idend, idset, MF_BYCOMMAND) ? 1 : 0;
}


RexxFunction2(RexxStringObject, GetMenuItem, uintptr_t, admin, int, id)
{
    DIALOGADMIN *dlgAdm = (DIALOGADMIN *)admin;
    if (!dlgAdm || !dlgAdm->menu)
    {
        return 1;
    }

    UINT state;
    char buffer[256];
    buffer[0] = '\0';

    state = GetMenuState(dlgAdm->menu, id, MF_BYCOMMAND);
    if (state == 0xFFFFFFFF) return context->NullString();
    if (state & MF_CHECKED) strcat(buffer, "CHECKED ");
    if (state & MF_DISABLED) strcat(buffer, "DISABLED ");
    if (state & MF_GRAYED) strcat(buffer, "GRAYED ");
    if (state & MF_HILITE) strcat(buffer, "HIGHLIGHTED ");
    return context->NewStringFromAsciiz(buffer);
}


RexxFunction1(RexxDirectoryObject, DumpAdmin, uintptr_t, admin)
{
    int i;

    dlgAdm = (DIALOGADMIN *)admin;

    RexxDirectoryObject dir = context->NewDirectory();

    context->DirectoryPut(dir,

    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->TableEntry),         "SLOT");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->TheThread),          "THREAD");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->TheDlg),             "DIALOG");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->menu),               "HMENU");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->BkgBrush),           "BKGBRUSH");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->BkgBitmap),          "BKBITMAP");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->OnTheTop),           "TOPMOST");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->AktChild),           "CURRENTCHILD");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->TheInstance),        "DLL");
    context->DirectoryPut(dir,  context->UnsignedInt64ToObject(dlgAdm->pMessageQueue),      "QUEUE");

    RexxArrayObject buttons = context->NewArray(dlgAdm->BT_size);

    context->DirectoryPut(dir,  buttons, "BMPBUTTONS");


    for (i=0; i<dlgAdm->BT_size; i++)
    {
        RexxDirectoryObject button = context->NewDirectory();

        context->DirectoryPut(button, context->NumberToObject(dlgAdm->BmpTab[i].buttonID), "ID");
        context->DirectoryPut(button, context->NumberToObject(dlgAdm->BmpTab[i].bitmapID), "NORMAL");
        context->DirectoryPut(button, context->NumberToObject(dlgAdm->BmpTab[i].bmpFocusID), "FOCUSED");
        context->DirectoryPut(button, context->NumberToObject(dlgAdm->BmpTab[i].bmpSelectID), "SELECTED");
        context->DirectoryPut(button, context->NumberToObject(dlgAdm->BmpTab[i].bmpDisableID), "DISABLED");

        context->ArrayPut(buttons, button, i+1);
    }

    RexxArrayObject messages = context->NewArray(dlgAdm->MT_size);

    context->DirectoryPut(dir,  messages, "MESSAGES");

    for (i=0; i<dlgAdm->MT_size; i++)
    {
        RexxDirectoryObject message = context->NewDirectory();

        context->DirectoryPut(message, context->NumberToObject(dlgAdm->MsgTab[i].msg), "MSG");
        context->DirectoryPut(message, context->NumberToObject(dlgAdm->MsgTab[i].wParam), "PARAM1");
        context->DirectoryPut(message, context->NumberToObject(dlgAdm->MsgTab[i].lParam), "PARAM2");
        context->DirectoryPut(message, context->NewStringFromAsciiz(dlgAdm->MsgTab[i].rexxProgram), "METHOD");

        context->ArrayPut(messages, message, i+1);
    }

    RexxArrayObject dataTable = context->NewArray(dlgAdm->DT_size);

    context->DirectoryPut(dir, dataTable, "DATAITEMS");

    for (i=0; i<dlgAdm->DT_size; i++)
    {
        RexxDirectoryObject item = context->NewDirectory();

        context->DirectoryPut(item, context->NumberToObject(dlgAdm->DataTab[i].id), "ID");
        context->DirectoryPut(item, context->NumberToObject(dlgAdm->DataTab[i].typ), "TYPE");
        context->DirectoryPut(item, context->NumberToObject(dlgAdm->DataTab[i].category), "CATEGORY");

        context->ArrayPut(dataTable, item, i+1);
    }

    RexxArrayObject colorTable = context->NewArray(dlgAdm->CT_size);

    context->DirectoryPut(dir, colorTable, "COLORITEMS");


    for (i=0; i<dlgAdm->CT_size; i++)
    {
        RexxDirectoryObject color = context->NewDirectory();

        context->DirectoryPut(color, context->NumberToObject(dlgAdm->ColorTab[i].itemID), "ID");
        context->DirectoryPut(color, context->NumberToObject(dlgAdm->ColorTab[i].ColorBk), "Background");
        context->DirectoryPut(color, context->NumberToObject(dlgAdm->ColorTab[i].ColorFG), "Foreground");

        context->ArrayPut(colorTable, color, i+1);
    }

    return dir;
}

RexxArrayObject0(RexxArrayObject, DumpDialogTable)
{
    int i;
    int count = 0;

    for (i=0; i<MAXDIALOGS; i++)
    {
        if (DialogTab[i] != NULL)
        {
            count++;
        }
    }

    RexxArrayObject tab = NewArray(count);

    count = 0;
    for (i=0; i<MAXDIALOGS; i++)
    {
        if (DialogTab[i] != NULL)
        {
            count++;

            RexxDirectoryObject dlg = context->NewDirectory();
            context->ArrayPut(tab, dlg, count);

            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]), "ADMBLOCK");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->TableEntry), "SLOT");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->TheThread), "THREAD");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->TheDlg), "DIALOG");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->menu), "MENU");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->bkgBrush), "BKGBRUSH");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->bkgBitmap), "BKGBITMAP");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->OnTheTop), "TOPMOST");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->AktChild), "CURRENTCHILD");
            context->DirectoryPut(dlg, context->UnsignedInt64ToObject(DialogTab[i]->TheInstance), "DLL");
            context->DirectoryPut(dlg, context->NewStringFromAsciiz(DialogTab[i]->pMessageQueue), "QUEUE");
            context->DirectoryPut(dlg, context->NumberToObject(DialogTab[i]->BT_size, "BMPBUTTONS");
            context->DirectoryPut(dlg, context->NumberToObject(DialogTab[i]->MT_size, "MESSAGES");
            context->DirectoryPut(dlg, context->NumberToObject(DialogTab[i]->DT_size, "DataItems");
            context->DirectoryPut(dlg, context->NumberToObject(DialogTab[i]->CT_size, "ColorItems");
        }
    }
    return tab;
}



#ifdef __cplusplus
extern "C" {
#endif

bool APIENTRY DllMain(
    HINSTANCE  hinstDLL,    // handle of DLL module
    DWORD  fdwReason,    // reason for calling function
    LPVOID  lpvReserved     // reserved
   )
{
   OSVERSIONINFO version_info={0}; /* for optimization so that GetVersionEx */

   if (fdwReason == DLL_PROCESS_ATTACH) {
      MyInstance = hinstDLL;
      version_info.dwOSVersionInfoSize = sizeof(version_info);  // if not set --> violation error
      GetVersionEx(&version_info);
      if (version_info.dwPlatformId == VER_PLATFORM_WIN32_NT) IsNT = true; else IsNT = false;
      InitializeCriticalSection(&crit_sec);
   }  else if (fdwReason == DLL_PROCESS_DETACH)
   {
       MyInstance = NULL;
       DeleteCriticalSection(&crit_sec);
   }
   return(true);
}

#ifdef __cplusplus
}
#endif

REXX_TYPED_FUNCTION_PROTOTYPE(ChangeBitmapButton);
REXX_TYPED_FUNCTION_PROTOTYPE(DrawBitmapButton);
REXX_TYPED_FUNCTION_PROTOTYPE(SetButtonDisplacement);
REXX_TYPED_FUNCTION_PROTOTYPE(AddBitmapButton);
REXX_TYPED_FUNCTION_PROTOTYPE(GetBitmapButtonWidth);
REXX_TYPED_FUNCTION_PROTOTYPE(GetBitmapButtonHeight);
REXX_TYPED_FUNCTION_PROTOTYPE(GetBitmapButtonX);
REXX_TYPED_FUNCTION_PROTOTYPE(GetBitmapButtonY);
REXX_TYPED_FUNCTION_PROTOTYPE(LoadBitmap);
REXX_TYPED_FUNCTION_PROTOTYPE(ScrollTheWindow);
REXX_TYPED_FUNCTION_PROTOTYPE(GetItemData);
REXX_TYPED_FUNCTION_PROTOTYPE(SetItemData);
REXX_TYPED_FUNCTION_PROTOTYPE(SetAdminData);
REXX_TYPED_FUNCTION_PROTOTYPE(SetStemData);
REXX_TYPED_FUNCTION_PROTOTYPE(AddDataTable);
REXX_TYPED_FUNCTION_PROTOTYPE(GetDataTable);
REXX_TYPED_FUNCTION_PROTOTYPE(SetDataTable);
REXX_TYPED_FUNCTION_PROTOTYPE(FindTheWindow);
REXX_TYPED_FUNCTION_PROTOTYPE(WndGetWindowText);
REXX_TYPED_FUNCTION_PROTOTYPE(WndSetWindowText);
REXX_TYPED_FUNCTION_PROTOTYPE(WndEnableWindow);
REXX_TYPED_FUNCTION_PROTOTYPE(WndSetFocus);

// now build the actual entry list
ooRexxFunctionEntry oodialog_functions[] ={
    REXX_TYPED_FUNCTION( SockDropFuncs      , SockDropFuncs             )
};

ooRexxPackageEntry oodialog_package_entry ={
    STANDARD_PACKAGE_HEADER
    "OODialog",                          // name of the package
    "1.3",                               // package information
    NULL,                                // no load/unload functions
    NULL,
    oodialog_functions,                  // the exported functions
    NULL                                 // no methods in this package
};
