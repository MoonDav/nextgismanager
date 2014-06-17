/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  Create Remote Database connection dialog.
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2011,2013,2014 Dmitry Baryshnikov
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "wxgis/catalogui/remoteconndlgs.h"

#ifdef wxGIS_USE_POSTGRES

#include "wxgis/datasource/sysop.h"
#include "wxgis/core/crypt.h"
#include "wxgis/core/format.h"
#include "wxgis/datasource/postgisdataset.h"

#include <wx/valgen.h>
#include <wx/valtext.h>

//-------------------------------------------------------------------------------
//  wxGISRemoteConnDlg
//-------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxGISRemoteConnDlg, wxDialog)
    EVT_BUTTON(wxID_OK, wxGISRemoteConnDlg::OnOK)
    EVT_BUTTON(ID_TESTBUTTON, wxGISRemoteConnDlg::OnTest)
END_EVENT_TABLE()

wxGISRemoteConnDlg::wxGISRemoteConnDlg(wxXmlNode* pConnectionNode, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    m_bIsFile = false;

    m_sServer = wxString(wxT("localhost"));
    m_sPort = wxString(wxT("5432"));
    m_sDatabase = wxString(wxT("postgres"));
    m_bIsBinaryCursor = true;

    //load values from xconn file
    if (pConnectionNode)
    {
        m_pConnectionNode = pConnectionNode;
        m_sServer = pConnectionNode->GetAttribute(wxT("server"), m_sServer);
        m_sPort = pConnectionNode->GetAttribute(wxT("port"), m_sPort);
        m_sDatabase = pConnectionNode->GetAttribute(wxT("db"), m_sDatabase);
        m_sUser = pConnectionNode->GetAttribute(wxT("user"), m_sUser);
        Decrypt(pConnectionNode->GetAttribute(wxT("pass"), wxEmptyString), m_sPass);
        m_bIsBinaryCursor = GetBoolValue(pConnectionNode, wxT("isbincursor"), m_bIsBinaryCursor);
    }

    //this->SetSizeHints( wxSize( 320,REMOTECONNDLG_MAX_HEIGHT ), wxSize( -1,REMOTECONNDLG_MAX_HEIGHT ) );

    m_bMainSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer* fgSizer1;
    fgSizer1 = new wxFlexGridSizer(3, 2, 0, 0);
    fgSizer1->AddGrowableCol(1);
    fgSizer1->SetFlexibleDirection(wxBOTH);
    fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_SetverStaticText = new wxStaticText(this, wxID_ANY, _("Server:"), wxDefaultPosition, wxDefaultSize, 0);
    m_SetverStaticText->Wrap(-1);
    fgSizer1->Add(m_SetverStaticText, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);

    m_sServerTextCtrl = new wxTextCtrl(this, ID_SERVERTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sServer));
    fgSizer1->Add(m_sServerTextCtrl, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

    m_PortStaticText = new wxStaticText(this, wxID_ANY, _("Port:"), wxDefaultPosition, wxDefaultSize, 0);
    m_PortStaticText->Wrap(-1);
    fgSizer1->Add(m_PortStaticText, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);

    m_PortTextCtrl = new wxTextCtrl(this, ID_PORTTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_sPort));
    fgSizer1->Add(m_PortTextCtrl, 0, wxALL | wxEXPAND, 5);

    m_DatabaseStaticText = new wxStaticText(this, ID_DATABASESTATICTEXT, _("Database:"), wxDefaultPosition, wxDefaultSize, 0);
    m_DatabaseStaticText->Wrap(-1);
    fgSizer1->Add(m_DatabaseStaticText, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);

    m_DatabaseTextCtrl = new wxTextCtrl(this, ID_DATABASETEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sDatabase));
    fgSizer1->Add(m_DatabaseTextCtrl, 0, wxALL | wxEXPAND, 5);

    m_bMainSizer->Add(fgSizer1, 0, wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Account")), wxVERTICAL);

    wxFlexGridSizer* fgSizer2;
    fgSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
    fgSizer2->AddGrowableCol(1);
    fgSizer2->SetFlexibleDirection(wxBOTH);
    fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_UserStaticText = new wxStaticText(this, ID_USERSTATICTEXT, _("User:"), wxDefaultPosition, wxDefaultSize, 0);
    m_UserStaticText->Wrap(-1);
    fgSizer2->Add(m_UserStaticText, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);

    m_UsesTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sUser));
    fgSizer2->Add(m_UsesTextCtrl, 0, wxALL | wxEXPAND, 5);

    m_PassStaticText = new wxStaticText(this, ID_PASSSTATICTEXT, _("Password:"), wxDefaultPosition, wxDefaultSize, 0);
    m_PassStaticText->Wrap(-1);
    fgSizer2->Add(m_PassStaticText, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);

    m_PassTextCtrl = new wxTextCtrl(this, ID_PASSTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD, wxGenericValidator(&m_sPass));
    fgSizer2->Add(m_PassTextCtrl, 0, wxALL | wxEXPAND, 5);

    sbSizer1->Add(fgSizer2, 1, wxEXPAND, 5);

    m_bMainSizer->Add(sbSizer1, 0, wxEXPAND | wxALL, 5);

    m_checkBoxBinaryCursor = new wxCheckBox(this, wxID_ANY, _("Use binary cursor"), wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_bIsBinaryCursor));
    m_bMainSizer->Add(m_checkBoxBinaryCursor, 0, wxALL | wxEXPAND, 5);

    m_TestButton = new wxButton(this, ID_TESTBUTTON, _("Test Connection"), wxDefaultPosition, wxDefaultSize, 0);
    m_bMainSizer->Add(m_TestButton, 0, wxALL | wxEXPAND, 5);

    m_staticline2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    m_bMainSizer->Add(m_staticline2, 0, wxEXPAND | wxALL, 5);

    m_sdbSizer = new wxStdDialogButtonSizer();
    m_sdbSizerOK = new wxButton(this, wxID_OK, _("OK"));
    m_sdbSizer->AddButton(m_sdbSizerOK);
    m_sdbSizerCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
    m_sdbSizer->AddButton(m_sdbSizerCancel);
    m_sdbSizer->Realize();
    m_bMainSizer->Add(m_sdbSizer, 0, wxEXPAND | wxALL, 5);

    this->SetSizerAndFit(m_bMainSizer);
    this->Layout();

    this->Centre(wxBOTH);
}


wxGISRemoteConnDlg::wxGISRemoteConnDlg( CPLString pszConnPath, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_bCreateNew = !CPLCheckForFile((char*)pszConnPath.c_str(), NULL);
    m_bIsFile = true;

    m_sConnName = wxString(CPLGetFilename(pszConnPath), wxConvUTF8);
	m_sOutputPath = wxString(CPLGetPath(pszConnPath), wxConvUTF8);
	//set default values
	m_sServer = wxString(wxT("localhost"));
	m_sPort = wxString(wxT("5432"));
	m_sDatabase = wxString(wxT("postgres"));
	m_bIsBinaryCursor = true;

	//load values from xconn file
	if(!m_bCreateNew)
	{
        m_sOriginOutput = pszConnPath;
		wxXmlDocument doc(wxString(pszConnPath,  wxConvUTF8));
		if(doc.IsOk())
		{
			wxXmlNode* pRootNode = doc.GetRoot();
			if(pRootNode)
			{
				m_sServer = pRootNode->GetAttribute(wxT("server"), m_sServer);
				m_sPort = pRootNode->GetAttribute(wxT("port"), m_sPort);
				m_sDatabase = pRootNode->GetAttribute(wxT("db"), m_sDatabase);
				m_sUser = pRootNode->GetAttribute(wxT("user"), m_sUser);
				Decrypt(pRootNode->GetAttribute(wxT("pass"), wxEmptyString), m_sPass);
				m_bIsBinaryCursor = GetBoolValue(pRootNode, wxT("isbincursor"), m_bIsBinaryCursor);
			}
		}
	}

    //this->SetSizeHints( wxSize( 320,REMOTECONNDLG_MAX_HEIGHT ), wxSize( -1,REMOTECONNDLG_MAX_HEIGHT ) );

	m_bMainSizer = new wxBoxSizer( wxVERTICAL );

	m_ConnName = new wxTextCtrl( this, ID_CONNNAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sConnName) );
	m_bMainSizer->Add( m_ConnName, 0, wxALL|wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_bMainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_SetverStaticText = new wxStaticText( this, wxID_ANY, _("Server:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SetverStaticText->Wrap( -1 );
	fgSizer1->Add( m_SetverStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_sServerTextCtrl = new wxTextCtrl( this, ID_SERVERTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sServer) );
	fgSizer1->Add( m_sServerTextCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_PortStaticText = new wxStaticText( this, wxID_ANY, _("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortStaticText->Wrap( -1 );
	fgSizer1->Add( m_PortStaticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_PortTextCtrl = new wxTextCtrl( this, ID_PORTTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_sPort) );
	fgSizer1->Add( m_PortTextCtrl, 0, wxALL|wxEXPAND, 5 );

	m_DatabaseStaticText = new wxStaticText( this, ID_DATABASESTATICTEXT, _("Database:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DatabaseStaticText->Wrap( -1 );
	fgSizer1->Add( m_DatabaseStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_DatabaseTextCtrl = new wxTextCtrl( this, ID_DATABASETEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sDatabase) );
	fgSizer1->Add( m_DatabaseTextCtrl, 0, wxALL|wxEXPAND, 5 );

	m_bMainSizer->Add( fgSizer1, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Account") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_UserStaticText = new wxStaticText( this, ID_USERSTATICTEXT, _("User:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UserStaticText->Wrap( -1 );
	fgSizer2->Add( m_UserStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_UsesTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_sUser) );
	fgSizer2->Add( m_UsesTextCtrl, 0, wxALL|wxEXPAND, 5 );

	m_PassStaticText = new wxStaticText( this, ID_PASSSTATICTEXT, _("Password:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PassStaticText->Wrap( -1 );
	fgSizer2->Add( m_PassStaticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_PassTextCtrl = new wxTextCtrl( this, ID_PASSTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD, wxGenericValidator(&m_sPass) );
	fgSizer2->Add( m_PassTextCtrl, 0, wxALL|wxEXPAND, 5 );

	sbSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );

	m_bMainSizer->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );

    m_checkBoxBinaryCursor = new wxCheckBox(this, wxID_ANY, _("Use binary cursor"), wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_bIsBinaryCursor));
	m_bMainSizer->Add( m_checkBoxBinaryCursor, 0, wxALL|wxEXPAND, 5 );

	m_TestButton = new wxButton( this, ID_TESTBUTTON, _("Test Connection"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bMainSizer->Add( m_TestButton, 0, wxALL|wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_bMainSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK, _("OK") );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL, _("Cancel") );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	m_bMainSizer->Add( m_sdbSizer, 0, wxEXPAND|wxALL, 5 );

    this->SetSizerAndFit(m_bMainSizer);
	this->Layout();

	this->Centre( wxBOTH );
}

wxGISRemoteConnDlg::~wxGISRemoteConnDlg()
{
}

void wxGISRemoteConnDlg::OnOK(wxCommandEvent& event)
{
	if ( Validate() && TransferDataFromWindow() )
	{
		wxString sCryptPass;
		if(!Crypt(m_sPass, sCryptPass))
		{
			wxMessageBox(wxString(_("Crypt password failed!")), wxString(_("Error")), wxICON_ERROR | wxOK );
			return;
		}

        if (m_bIsFile)
        {
		    wxXmlDocument doc;
		    wxXmlNode* pRootNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("connection"));
		    pRootNode->AddAttribute(wxT("server"), m_sServer);
		    pRootNode->AddAttribute(wxT("port"), m_sPort);
		    pRootNode->AddAttribute(wxT("db"), m_sDatabase);
		    pRootNode->AddAttribute(wxT("user"), m_sUser);
		    pRootNode->AddAttribute(wxT("pass"), sCryptPass);

            SetBoolValue(pRootNode, wxT("isbincursor"), m_bIsBinaryCursor);

		    pRootNode->AddAttribute(wxT("type"), wxT("POSTGIS"));//store server type for future

		    doc.SetRoot(pRootNode);

		    wxString sFullPath = m_sOutputPath + wxFileName::GetPathSeparator() + GetName();
		    if(!m_bCreateNew)// && wxGISEQUAL(CPLString(sFullPath.mb_str(wxConvUTF8)), m_sOriginOutput))
		    {
                RenameFile(m_sOriginOutput, CPLString(sFullPath.mb_str(wxConvUTF8)));
		    }

            if(!doc.Save(sFullPath))
		    {
			    wxMessageBox(wxString(_("Connection create failed!")), wxString(_("Error")), wxICON_ERROR | wxOK );
			    return;
		    }

            //m_sOriginOutput = CPLString(sFullPath.mb_str(wxConvUTF8));
        }
        else
        {
            if (m_pConnectionNode->HasAttribute(wxT("server")))
                m_pConnectionNode->DeleteAttribute(wxT("server"));
            m_pConnectionNode->AddAttribute(wxT("server"), m_sServer);

            if (m_pConnectionNode->HasAttribute(wxT("port")))
                m_pConnectionNode->DeleteAttribute(wxT("port"));
            m_pConnectionNode->AddAttribute(wxT("port"), m_sPort);

            if (m_pConnectionNode->HasAttribute(wxT("db")))
                m_pConnectionNode->DeleteAttribute(wxT("db"));
            m_pConnectionNode->AddAttribute(wxT("db"), m_sDatabase);

            if (m_pConnectionNode->HasAttribute(wxT("user")))
                m_pConnectionNode->DeleteAttribute(wxT("user"));
            m_pConnectionNode->AddAttribute(wxT("user"), m_sUser);

            if (m_pConnectionNode->HasAttribute(wxT("pass")))
                m_pConnectionNode->DeleteAttribute(wxT("pass"));
            m_pConnectionNode->AddAttribute(wxT("pass"), sCryptPass);

            if (m_pConnectionNode->HasAttribute(wxT("isbincursor")))
                m_pConnectionNode->DeleteAttribute(wxT("isbincursor"));
            SetBoolValue(m_pConnectionNode, wxT("isbincursor"), m_bIsBinaryCursor);

            if (m_pConnectionNode->HasAttribute(wxT("type")))
                m_pConnectionNode->DeleteAttribute(wxT("type"));
            m_pConnectionNode->AddAttribute(wxT("type"), wxT("POSTGIS"));//store server type for future
        }
		EndModal(wxID_OK);
	}
	else
	{
		wxMessageBox(wxString(_("Some input values are incorrect!")), wxString(_("Error")), wxICON_ERROR | wxOK );
	}
}

void wxGISRemoteConnDlg::OnTest(wxCommandEvent& event)
{
	wxBusyCursor wait;
	if ( Validate() && TransferDataFromWindow() )
	{
		CPLSetConfigOption("PG_LIST_ALL_TABLES", "YES");
		CPLSetConfigOption("PGCLIENTENCODING", "UTF-8");

		wxString sPath = wxString::Format(wxT("%s:host='%s' dbname='%s' port='%s' user='%s' password='%s'"), m_bIsBinaryCursor == true ? wxT("PGB") : wxT("PG"), m_sServer.c_str(), m_sDatabase.c_str(), m_sPort.c_str(), m_sUser.c_str(), m_sPass.c_str());

        CPLString szPath(sPath.ToUTF8());
		wxGISPostgresDataSource oPostgresDataSource(szPath);

		CPLErrorReset();

		if( oPostgresDataSource.Open(false, true ) )
		{
			const char* err = CPLGetLastErrorMsg();
			wxString sErr = wxString::Format(_("Operation '%s' failed! Host '%s', Database name '%s', Port='%s'. OGR error: %s"), wxString(_("Open")), m_sServer.c_str(), m_sDatabase.c_str(), m_sPort.c_str(), wxString(err, wxConvLocal));
			wxMessageBox(sErr, wxString(_("Error")), wxICON_ERROR | wxOK, this );
		}
		else
		{
			wxMessageBox(wxString(_("Connected successufuly!")), wxString(_("Information")), wxICON_INFORMATION | wxOK, this );
		}
	}
	else
	{
		wxMessageBox(wxString(_("Some input values are not correct!")), wxString(_("Error")), wxICON_ERROR | wxOK );
	}
}

CPLString wxGISRemoteConnDlg::GetPath(void)
{
	return CPLString(wxString(m_sOutputPath + wxFileName::GetPathSeparator() + GetName()).mb_str(wxConvUTF8));
}

wxString wxGISRemoteConnDlg::GetName(void)
{
 	if(!m_sConnName.Lower().EndsWith(wxT(".xconn")))
		m_sConnName.Append(wxT(".xconn"));
	return m_sConnName;
}

#endif //wxGIS_USE_POSTGRES
