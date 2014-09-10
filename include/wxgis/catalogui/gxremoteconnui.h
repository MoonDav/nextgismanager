/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  Remote Connection UI classes.
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
#pragma once

#include "wxgis/catalogui/catalogui.h"
#include "wxgis/catalog/gxremoteconn.h"
#include "wxgis/catalogui/gxview.h"

#ifdef wxGIS_USE_POSTGRES

/** @class wxGxRemoteConnectionUI

    A Remote Connection GxObjectUI.

    @library {catalogui}
*/

class WXDLLIMPEXP_GIS_CLU wxGxRemoteConnectionUI :
    public wxGxRemoteConnection,
	public IGxObjectUI,
    public IGxObjectEditUI,
    public IGxObjectWizard,
    public IGxDropTarget,
    public wxGxAutoRenamer
{
    DECLARE_CLASS(wxGxRemoteConnectionUI)
public:
	wxGxRemoteConnectionUI(wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &LargeIconConn = wxNullIcon, const wxIcon &SmallIconConn = wxNullIcon, const wxIcon &LargeIconDisconn = wxNullIcon, const wxIcon &SmallIconDisconn = wxNullIcon);
	virtual ~wxGxRemoteConnectionUI(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void) const {return wxString(wxT("wxGxRemoteConnection.ContextMenu"));};
	virtual wxString NewMenu(void) const {return wxString(wxT("wxGxRemoteConnection.NewMenu"));};
    //IGxRemoteConnection
	virtual bool Connect(void);
	//IGxObjectEditUI
	virtual void EditProperties(wxWindow *parent);
    //IGxObjectWizard
    virtual bool Invoke(wxWindow* pParentWnd);
    //IGxDropTarget
    virtual wxDragResult CanDrop(wxDragResult def);
    virtual bool Drop(const wxArrayString& saGxObjectPaths, bool bMove);
protected:
    //wxGxRemoteConnection
    virtual wxGxRemoteDBSchema* GetNewRemoteDBSchema(const wxString &sName, const CPLString &soPath, wxGISPostgresDataSource *pwxGISRemoteConn);
    virtual wxThread::ExitCode Entry();
    //events
    virtual void OnThreadFinished(wxThreadEvent& event);
protected:
    wxIcon m_oLargeIconConn, m_oSmallIconConn;
    wxIcon m_oLargeIconDisconn, m_oSmallIconDisconn;
    wxIcon m_oLargeIconFeatureClass, m_oSmallIconFeatureClass;
    wxIcon m_oLargeIconTable, m_oSmallIconTable;
    wxIcon m_oLargeIconSchema, m_oSmallIconSchema;
    long m_PendingId;
};

/** @class wxGxRemoteDBSchemaUI

    A Remote Database schema GxObjectUI.

    @library {catalogui}
*/

class WXDLLIMPEXP_GIS_CLU wxGxRemoteDBSchemaUI :
    public wxGxRemoteDBSchema,
	public IGxObjectUI,
    public IGxObjectEditUI,
    public IGxDropTarget
{
    DECLARE_CLASS(wxGxRemoteDBSchemaUI)
    enum
    {
        EXIT_EVENT = wxID_HIGHEST+1
    };
public:
	wxGxRemoteDBSchemaUI(bool bHasGeom, bool bHasGeog, bool bHasRaster, wxGISPostgresDataSource* pwxGISRemoteConn, wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &LargeIcon = wxNullIcon, const wxIcon &SmallIcon = wxNullIcon, const wxIcon &LargeIconFeatureClass = wxNullIcon, const wxIcon &SmallIconFeatureClass = wxNullIcon, const wxIcon &LargeIconTable = wxNullIcon, const wxIcon &SmallIconTable = wxNullIcon);
	virtual ~wxGxRemoteDBSchemaUI(void);
    //wxGxObjectContainer
    virtual bool HasChildren(void);
    //IGxObjectEdit
    virtual bool Delete(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void) const {return wxString(wxT("wxGxRemoteDBSchema.ContextMenu"));};
	virtual wxString NewMenu(void) const {return wxString(wxT("wxGxRemoteDBSchema.NewMenu"));};
	//IGxObjectEditUI
	virtual void EditProperties(wxWindow *parent);
    //IGxDropTarget
    virtual wxDragResult CanDrop(wxDragResult def);
    virtual bool Drop(const wxArrayString& saGxObjectPaths, bool bMove);
    //events
    void OnThreadFinished(wxThreadEvent& event);
protected:
    //wxGxRemoteDBSchema
    virtual wxGxObject* AddTable(const wxString &sTableName, const wxGISEnumDatasetType eType);
    virtual wxThread::ExitCode Entry();
    virtual bool CreateAndRunThread(void);
protected:
    wxIcon m_oLargeIcon, m_oSmallIcon;
    wxIcon m_oLargeIconFeatureClass, m_oSmallIconFeatureClass;
    wxIcon m_oLargeIconTable, m_oSmallIconTable;
    long m_PendingId;
};

#endif // wxGIS_USE_POSTGRES

/** @class wxGxTMSWebServiceUI

    A Tile Map Service GxObjectUI.

    @library {catalogui}
*/

class WXDLLIMPEXP_GIS_CLU wxGxTMSWebServiceUI :
	public wxGxTMSWebService,
	public IGxObjectUI,
    public IGxObjectEditUI
{
    DECLARE_CLASS(wxGxTMSWebServiceUI)
public:
	wxGxTMSWebServiceUI(wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &icLargeIcon = wxNullIcon, const wxIcon &icSmallIcon = wxNullIcon, const wxIcon &icLargeIconDsbl = wxNullIcon, const wxIcon &icSmallIconDsbl = wxNullIcon);
	virtual ~wxGxTMSWebServiceUI(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void) const {return wxString(wxT("wxGxTMSWebService.ContextMenu"));};
	virtual wxString NewMenu(void) const {return wxEmptyString;};
	//IGxObjectEditUI
	virtual void EditProperties(wxWindow *parent);
protected:
    wxIcon m_icLargeIcon, m_icSmallIcon;
    wxIcon m_icLargeIconDsbl, m_icSmallIconDsbl;
};

#ifdef wxGIS_USE_CURL
/** @class wxGxNGWServiceUI

    A NextGIS Web Service GxObjectUI.

    @library {catalogui}
*/

class WXDLLIMPEXP_GIS_CLU wxGxNGWServiceUI :
    public wxGxNGWService,
	public IGxObjectUI,
    public IGxObjectEditUI,
    public IGxObjectWizard,
    public IGxDropTarget,
    public wxGxAutoRenamer
{
    DECLARE_CLASS(wxGxNGWServiceUI)
public:
    wxGxNGWServiceUI(wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &icLargeIcon = wxNullIcon, const wxIcon &icSmallIcon = wxNullIcon, const wxIcon &icLargeIconDsbl = wxNullIcon, const wxIcon &icSmallIconDsbl = wxNullIcon);/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  Remote Connection UI classes.
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

    virtual ~wxGxNGWServiceUI(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void) const {return wxString(wxT("wxGxNGWServiceUI.ContextMenu"));};
    virtual wxString NewMenu(void) const { return wxString(wxT("wxGxNGWServiceUI.NewtMenu")); };
	//IGxObjectEditUI
	virtual void EditProperties(wxWindow *parent);
    //IGxObjectWizard
    virtual bool Invoke(wxWindow* pParentWnd);
    //IGxDropTarget
    virtual wxDragResult CanDrop(wxDragResult def);
    virtual bool Drop(const wxArrayString& saGxObjectPaths, bool bMove);
protected:
    virtual void LoadChildren(void);
protected:
    wxIcon m_icLargeIcon, m_icSmallIcon;
    wxIcon m_icLargeIconDsbl, m_icSmallIconDsbl;
};

/** @class wxGxNGWResourceGroupUI

    A NextGIS Web Service Resource Group GxObjectUI.

    @library {catalog}
*/

class WXDLLIMPEXP_GIS_CLU wxGxNGWResourceGroupUI :
    public wxGxNGWResourceGroup,
    public IGxObjectUI
{
    DECLARE_CLASS(wxGxNGWResourceGroupUI)
public:
    wxGxNGWResourceGroupUI(wxGxNGWService *pService, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &icLargeIcon = wxNullIcon, const wxIcon &icSmallIcon = wxNullIcon);
    virtual ~wxGxNGWResourceGroupUI(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
    virtual wxString ContextMenu(void) const { return wxString(wxT("wxGxNGWResourceGroup.ContextMenu")); };
    virtual wxString NewMenu(void) const { return wxString(wxT("wxGxNGWResourceGroup.NewtMenu")); };
protected:
    virtual void AddResource(const wxJSONValue &Data);
protected:
    wxIcon m_icLargeIcon, m_icSmallIcon;
	wxIcon m_icFolderLargeIcon, m_icFolderSmallIcon;
	wxIcon m_icPGLayerLargeIcon, m_icPGLayerSmallIcon;
	wxIcon m_icPGConnLargeIcon, m_icPGConnSmallIcon;
	wxIcon m_icNGWLayerLargeIcon, m_icNGWLayerSmallIcon;
};


/** @class wxGxNGWRootResourceUI

    A NextGIS Web Service Root resource GxObjectUI.

    @library {catalog}
*/

class WXDLLIMPEXP_GIS_CLU wxGxNGWRootResourceUI :
    public wxGxNGWResourceGroupUI
{
    DECLARE_CLASS(wxGxNGWRootResourceUI)
public:
    wxGxNGWRootResourceUI(wxGxNGWService *pService, wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &icLargeIcon = wxNullIcon, const wxIcon &icSmallIcon = wxNullIcon);
    virtual ~wxGxNGWRootResourceUI(void);
    //IGxObjectUI
    virtual wxIcon GetLargeImage(void);
    virtual wxIcon GetSmallImage(void);
    virtual wxString ContextMenu(void) const { return wxString(wxT("wxGxNGWRootResource.ContextMenu")); };
    virtual wxString NewMenu(void) const { return wxString(wxT("wxGxNGWRootResource.NewtMenu")); };
};

/** @class wxGxNGWLayerUI

    A NextGIS Web Service Layer GxObjectUI.

    @library {catalog}
*/

class WXDLLIMPEXP_GIS_CLU wxGxNGWLayerUI :
    public wxGxNGWLayer,
    public IGxObjectUI
{
    DECLARE_CLASS(wxGxNGWLayerUI)
public:
    wxGxNGWLayerUI(wxGxNGWService *pService, wxGISEnumNGWResourcesType eType, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "", const wxIcon &icLargeIcon = wxNullIcon, const wxIcon &icSmallIcon = wxNullIcon);
    virtual ~wxGxNGWLayerUI(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
    virtual wxString ContextMenu(void) const { return wxString(wxT("wxGxNGWLayerUI.ContextMenu")); };
    virtual wxString NewMenu(void) const { return wxString(wxT("wxGxNGWLayerUI.NewtMenu")); };
protected:
    wxIcon m_icLargeIcon, m_icSmallIcon;
};
#endif // wxGIS_USE_CURL
