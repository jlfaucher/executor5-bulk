/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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

#ifndef oodShellObjects_Included
#define oodShellObjects_Included


#define HINT_ID                         0x00003749
#define DEFAULT_BIF_FLAGS               BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS

#define WRONG_IDL_TYPE_LIST             "a CSIDL_xxx keyword, a full path name, or a pointer to an Item ID List"
#define WRONG_IDL_TYPE_LIST_SHORT       "a CSIDL_xxx keyword or a full path name"
#define NO_ITEMID_MSG                   "the Windows Shell did not return the item ID for %s"
#define NO_IFILEDIALOG_POINTER          "the Windows Common Item Dialog information is no longer valid"
#define FILE_FILTER_ARRAY_MUST_BE_EVEN  "file types filter array items must be even"

#define BFF_TITLE                       "ooDialog Browse for Folder"
#define BFF_BANNER                      "Select the folder needed"
#define BFF_HINT                        "If the needed folder does not exist it can be created"
#define BFF_STARTDIR                    ""

#define CID_EVENTHANDLER_VAR            "CID_EVENTHANDLER_OBJECT_VAR"


/* Struct for the BrowseForFolder CSelf */
typedef struct _bffCSelf
{
    LPITEMIDLIST    root;
    RexxObjectPtr   rexxOwner;
    HWND            hOwner;
    char           *startDir;
    char           *dlgTitle;
    char           *hint;
    char           *banner;
    size_t          countCoInitialized;
    uint32_t        bifFlags;
    uint32_t        coThreadID;
    bool            useHint;
    bool            usePathForHint;
} CBrowseForFolder;
typedef CBrowseForFolder *pCBrowseForFolder;

// Identifies an attribute of the BrowseForFolder object.
typedef enum
{
    DlgTitle,
    DlgBanner,
    DlgHint,
    DlgStartDir,
    BffRoot
} BffAttributeType;

/* COM class for the Rexx CommonDialogEvents class */
class CommonDialogEvents : public IFileDialogEvents,
                           public IFileDialogControlEvents
{
public:
    // IUnknown methods
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CommonDialogEvents, IFileDialogEvents),
            QITABENT(CommonDialogEvents, IFileDialogControlEvents),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long _cRef = InterlockedDecrement(&cRef);
        if ( _cRef == 0 )
        {
            delete this;
        }
        return _cRef;
    }

    // IFileDialogEvents methods
    IFACEMETHODIMP OnFileOk(IFileDialog *);
    IFACEMETHODIMP OnFolderChange(IFileDialog *);
    IFACEMETHODIMP OnFolderChanging(IFileDialog *, IShellItem *) { return S_OK; };
    IFACEMETHODIMP OnHelp(IFileDialog *) { return S_OK; };
    IFACEMETHODIMP OnSelectionChange(IFileDialog *) { return S_OK; };
    IFACEMETHODIMP OnShareViolation(IFileDialog *, IShellItem *, FDE_SHAREVIOLATION_RESPONSE *) { return S_OK; };
    IFACEMETHODIMP OnTypeChange(IFileDialog *pfd);
    IFACEMETHODIMP OnOverwrite(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *) { return S_OK; };

    // IFileDialogControlEvents methods
    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize *, DWORD);
    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize *, DWORD, BOOL) { return S_OK; };
    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize *, DWORD) { return S_OK; };
    IFACEMETHODIMP OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem);

    inline RexxObjectPtr getRexxPFD() { return rexxPFD; }
    inline void setRexxPFD(RexxObjectPtr rxPFD) { rexxPFD = rxPFD; }

    CommonDialogEvents(RexxObjectPtr rxSelf, RexxInstance *c);

private:
    RexxObjectPtr getCommonDialogHwnd(RexxThreadContext *c, IFileDialog *pfd);
    HRESULT checkEventReply(RexxThreadContext *c, RexxObjectPtr reply);

    ~CommonDialogEvents() { };

    RexxObjectPtr  rexxSelf;            // Our Rexx self
    RexxObjectPtr  rexxPFD;             // The Rexx CommonItemDialog object we are receiving events for.
    RexxInstance  *interpreter;         // The Rexx interpreter instance we're running under.
    long cRef;
};


/* Struct for the CommonItemDialog CSelf */
typedef struct _cidCSelf
{
    IFileDialog        *pfd;
    RexxObjectPtr       rexxSelf;
    CommonDialogEvents *pcde;
    uint32_t            cookie;
    uint32_t            comThreadID;
    bool                comInitialized;
} CCommonItemDialog;
typedef CCommonItemDialog *pCCommonItemDialog;

/* Struct for the CommonDialogEvents CSelf */
typedef struct _cdeCSelf
{
    CommonDialogEvents *pcde;
    RexxObjectPtr       rexxSelf;
    bool                inUse;
} CCommonDialogEvents;
typedef CCommonDialogEvents *pCCommonDialogEvents;

// Identifies a type of text that can be set in IFileDialog
typedef enum
{
    CidTitle,
    CidFileName,
    CidFileNameLabel,
    CidDefaultExtension,
    CidOkButtonLabel
} CidTextType;





#endif
