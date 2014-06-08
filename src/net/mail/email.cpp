/////////////////////////////////////////////////////////////////////////////
// Name:        email.h
// Purpose:     wxEmail: portable email client class
// Author:      Julian Smart
// Modified by:
// Created:     2001-08-21
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wxgis/net/mail/email.h"

#include "wx/string.h"
#include "wxgis/net/mail/email.h"

#ifdef __WINDOWS__
#include "wxgis/net/mail/smapi.h"
#endif

#ifdef __UNIX__
#include "wx/filefn.h"
#include "wx/timer.h"
#include "wx/wfstream.h"
#include "stdlib.h"
#include "unistd.h"
#endif

// Send a message.
// Specify profile, or leave it to wxWidgets to find the current user name
wxEmail::wxEmail(void)
{
}

#ifdef __WINDOWS__
bool wxEmail::Send(wxMailMessage& message, const wxString& profileName, const wxString& WXUNUSED(sendMail))
{
    wxString profile(profileName);
    if (profile.IsEmpty())
        profile = wxGetUserName();

    wxMapiSession session;

    if (!session.MapiInstalled())
        return FALSE;
    if (!session.Logon(profile))
        return FALSE;

    return session.Send(message);
}
#elif defined(__UNIX__)
bool wxEmail::Send(wxMailMessage& message, const wxString& profileName, const wxString& sendMail)
{
    // The 'from' field is optionally supplied by the app; it's not needed
    // by MAPI, and on Unix, will be guessed if not supplied.
    wxString from = message.m_from;
    if ( from.empty() )
    {
        from = wxGetEmailAddress();
    }

    wxString msg;
    msg << wxT("To: ");

    const size_t rcptCount = message.m_to.GetCount();
    for (size_t rcpt = 0; rcpt < rcptCount; rcpt++)
    {
        if ( rcpt )
            msg << wxT(", ");
        msg << message.m_to[rcpt];
    }

    msg << wxT("\nFrom: ") << from << wxT("\nSubject: ") << message.m_subject;
    msg << wxT("\n\n") << message.m_body;

    wxString filename;
    filename.Printf(wxT("/tmp/msg-%ld-%ld-%ld.txt"), (long) getpid(), wxGetLocalTime(), (long) rand());

    {
        wxFileOutputStream stream(filename);
        if (stream.Ok())
        {
            stream.Write(msg, msg.Length());
        }
        else
        {
            return FALSE ;
        }
    }

    // TODO search for a suitable sendmail if sendMail is empty
    wxString sendmail(sendMail);

    wxString cmd;
    cmd << sendmail << wxT(" < ") << filename;

    // TODO: check return code
    wxSystem(cmd.c_str());

    wxRemoveFile(filename);

    return TRUE;
}
#else
#error Send not yet implemented for this platform.
#endif

