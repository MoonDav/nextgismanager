/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  Remote Connection classes.
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2013,2014 NextGIS
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

#include "wxgis/catalog/gxobject.h"
#include "wxgis/catalog/gxdataset.h"
#include "wxgis/core/json/jsonval.h"
#include "wxgis/catalog/contupdater.h"
#include "wxgis/catalog/gxremoteconn.h"
#include "wxgis/datasource/rasterdataset.h"

#ifdef wxGIS_USE_CURL

#include "wxgis/net/curl.h"

#include <wx/hashmap.h>

/** @class wxGxNGWService

    A NextGIS Web Service GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWService :
    public wxGxObjectContainer,
    public IGxObjectEdit,
    public IGxRemoteConnection,
    public IGxObjectNoFilter
{
    DECLARE_CLASS(wxGxNGWService)
public:
    wxGxNGWService(wxGxObject *oParent, const wxString &soName = wxEmptyString, const CPLString &soPath = "");
    virtual ~wxGxNGWService(void);
    //wxGxObject
    virtual wxString GetCategory(void) const { return wxString(_("NextGIS Web service")); };
    virtual void Refresh(void);
    //IGxRemoteConnection
    virtual bool Connect(void);
    virtual bool Disconnect(void);
    virtual bool IsConnected(void);
    //IGxObjectEdit
    virtual bool Delete(void);
    virtual bool CanDelete(void) { return true; };
    virtual bool Rename(const wxString& NewName);
    virtual bool CanRename(void) { return true; };
    virtual bool Copy(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanCopy(const CPLString &szDestPath) { return true; };
    virtual bool Move(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanMove(const CPLString &szDestPath) { return CanCopy(szDestPath) & CanDelete(); };
    //wxGxObjectContainer
    virtual bool AreChildrenViewable(void) const { return true; };
    virtual bool HasChildren(void);
    virtual bool CanCreate(long nDataType, long DataSubtype);
    //wxGxNGWService
    virtual wxGISCurl GetCurl();
	virtual wxString GetLogin() const;
	virtual wxString GetPassword() const;
	virtual wxString GetURL() const;
protected:
    virtual void LoadChildren(void);
    virtual bool ConnectToNGW();
	virtual void ReadConnectionFile();
protected:
    wxString m_sLogin;
    wxString m_sPassword;
    wxString m_sURL;
    bool m_bChildrenLoaded, m_bIsConnected, m_bIsAuthorized;
    wxString m_sAuthCookie;
};

/** @enum wxGISEnumNGWResourcesType

    The NGW resource type.

    @library{catalog}
 */
 
enum wxGISEnumNGWResourcesType
{
	enumNGWResourceTypeNone,
	enumNGWResourceTypeResourceGroup,
	enumNGWResourceTypePostgisLayer,
	enumNGWResourceTypePostgisConnection,
	enumNGWResourceTypeWMSServerService,
	enumNGWResourceTypeBaseLayers,
	enumNGWResourceTypeWebMap,
	enumNGWResourceTypeWFSServerService,
	enumNGWResourceTypeVectorLayer,
	enumNGWResourceTypeRasterLayer,
	enumNGWResourceTypeVectorLayerStyle,
	enumNGWResourceTypeRasterLayerStyle
};

WX_DECLARE_HASH_MAP(int, wxJSONValue, wxIntegerHash, wxIntegerEqual, wxNGWResourceDataMap);


/** @class wxGxNGWResource

    A NextGIS Web wxGxNGWResource.

    @library{catalog}
*/
class WXDLLIMPEXP_GIS_CLT wxGxNGWResource : public wxGxRemoteId
{
public:
    wxGxNGWResource(const wxJSONValue &Data);
    virtual ~wxGxNGWResource(void);
	virtual wxGxNGWService *GetNGWService() const;
	virtual bool DeleteResource();
	virtual bool RenameResource(const wxString &sNewName);
	virtual bool MoveResource(int nResourceId);
	static wxString MakeKey(const wxString& sInputStr);
protected:
	virtual int GetParentResourceId() const = 0;
	virtual void ReportError(int nHTTPCode, const wxString& sBody);
    virtual bool CanCopyResource(const CPLString &szDestPath);
    virtual bool CanMoveResource(const CPLString &szDestPath);
protected:
    wxGISEnumNGWResourcesType m_eResourceType;
    bool m_bHasChildren;
    wxString m_sDescription;
    wxString m_sDisplayName;
    //wxArrayString m_aInterfaces;
    wxString m_sKeyName;
    int m_nOwnerId;
    wxArrayString m_aPermissions;
    wxArrayString m_aScopes;
    wxGxNGWService *m_pService;
};

/** @class wxGxNGWResourceWithStyles

    A NextGIS Web Resource with styles (i.e. raster or vector layer).

    @library{catalog}
*/
class WXDLLIMPEXP_GIS_CLT wxGxNGWResourceWithStyles : public wxGxNGWResource
{
public:
    wxGxNGWResourceWithStyles(const wxJSONValue &Data);
    virtual ~wxGxNGWResourceWithStyles(void);
protected:
	virtual bool GetStyles();
	virtual void ClearStyles();
protected:
	wxVector<wxGxNGWResource*> m_apStyles;
};

/** @class wxGxNGWStyle

    A NextGIS Web resource style.

    @library{catalog}
*/
class WXDLLIMPEXP_GIS_CLT wxGxNGWStyle : public wxGxNGWResource
{
public:
    wxGxNGWStyle(const wxJSONValue &Data, int nParentRemoteId);
    virtual ~wxGxNGWStyle(void);
protected:
	virtual int GetParentResourceId() const;
protected:
    int m_nParentRemoteId;
};

/** @class wxGxNGWResourceGroup

    A NextGIS Web Service Resource Group GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWResourceGroup :
    public wxGxObjectContainerUpdater,
    public IGxObjectEdit,
    public IGxObjectNoFilter,
    public wxGxNGWResource
{
    DECLARE_CLASS(wxGxNGWResourceGroup)
public:
    wxGxNGWResourceGroup(wxGxNGWService *pService, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName = _("Resource Group"), const CPLString &soPath = "");
    virtual ~wxGxNGWResourceGroup(void);
    //wxGxObject
	virtual wxString GetCategory(void) const { return wxString(_("Resource Group")); };
    //wxGxObjectContainer
    virtual void Refresh(void);
    //wxGxObjectContainer
    virtual bool AreChildrenViewable(void) const { return true; };
    virtual bool HasChildren(void);
    virtual bool CanCreate(long nDataType, long DataSubtype);	
	virtual bool ValidateDataset( wxGISFeatureDataset* const pSrcDataSet, OGRwkbGeometryType eFilterGeomType, ITrackCancel* const pTrackCancel );
	virtual bool ValidateDataset( wxGISRasterDataset* const pSrcDataSet, ITrackCancel* const pTrackCancel );
	virtual bool IsFieldNameForbidden(const wxString& sTestFieldName) const;
	virtual bool CanStoreMultipleGeometryTypes() const;
    //IGxObjectEdit
	virtual bool Delete(void);
    virtual bool CanDelete(void);
	virtual bool Rename(const wxString& NewName);
    virtual bool CanRename(void);
    virtual bool Copy(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanCopy(const CPLString &szDestPath);
    virtual bool Move(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanMove(const CPLString &szDestPath);
	//wxGxNGWResourceGroup
	virtual wxString CheckUniqName(const wxString &sName, const wxString& sAdd = wxT(" "), int nCounter = 0) const;
	virtual bool CreateResource(const wxString &sName, wxGISEnumNGWResourcesType eType);
	virtual bool CreatePostGISConnection(const wxString &sName, const wxString &sServer, const wxString &sDatabase, const wxString &sUser, const wxString &sPassword);
	virtual bool CreatePostGISLayer(const wxString &sName, int nPGConnId, const wxString &sTable, const wxString &sSchema, const wxString &sFid, const wxString &sGeom);
	virtual bool CreateVectorLayer(const wxString &sName, wxGISDataset * const pInputDataset, OGRwkbGeometryType eFilterGeomType, ITrackCancel* const pTrackCancel);
	virtual bool CreateRasterLayer(const wxString &sName, wxGISDataset * const pInputDataset, ITrackCancel* const pTrackCancel);
protected:
    virtual void LoadChildren(void);
    virtual wxGxObject* AddResource(const wxJSONValue &Data);
	virtual bool CreateResourceGroup(const wxString &sName);
    virtual wxGISEnumNGWResourcesType GetType(const wxJSONValue &Data) const;
	virtual int GetParentResourceId() const;    
	//wxGxObjectContainerUpdater
	virtual wxGxObjectMap GetRemoteObjects();
	virtual void AddObject(int nRemoteId, const wxString &sName);
protected:
	bool m_bHasGeoJSON;
	bool m_bHasPostGIS;
	bool m_bHasWMS;
	wxNGWResourceDataMap m_moJSONData;
};


/** @class wxGxNGWRootResource

    A NextGIS Web Service root resource GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWRootResource :
    public wxGxNGWResourceGroup
{
    DECLARE_CLASS(wxGxNGWRootResource)
public:
    wxGxNGWRootResource(wxGxNGWService *pService, wxGxObject *oParent, const wxString &soName = _("Resources"), const CPLString &soPath = "");
    virtual ~wxGxNGWRootResource(void);
    //wxGxObject
    virtual wxString GetCategory(void) const { return wxString(_("NGW resources")); };
	//IGxObjectEdit
	virtual bool CanDelete(void);
    virtual bool CanRename(void);
    virtual bool CanCopy(const CPLString &szDestPath);
    virtual bool CanMove(const CPLString &szDestPath);
};

/** @class wxGxNGWLayer

    A NextGIS Web Service Layer GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWLayer :
    public wxGxNGWResourceWithStyles,
	public wxGxFeatureDataset
{
    DECLARE_CLASS(wxGxNGWLayer)
public:
    wxGxNGWLayer(wxGxNGWService *pService, wxGISEnumNGWResourcesType eType, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName, const CPLString &soPath = "");
    virtual ~wxGxNGWLayer(void);
    //wxGxObject
    virtual wxString GetCategory(void) const;
	//IGxObjectEdit
	virtual bool Delete(void);
    virtual bool CanDelete(void);
	virtual bool Rename(const wxString& NewName);
    virtual bool CanRename(void);
    virtual bool Copy(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanCopy(const CPLString &szDestPath);
    virtual bool Move(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanMove(const CPLString &szDestPath);
protected:
    //create wxGISDataset without openning it
    virtual wxGISDataset* const GetDatasetFast(void);
	virtual int GetParentResourceId() const;
};

/** @class wxGxNGWRaster

    A NextGIS Web Service Raster GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWRaster :
    public wxGxNGWResourceWithStyles,
	public wxGxRasterDataset
{
    DECLARE_CLASS(wxGxNGWRaster)
public:
    wxGxNGWRaster(wxGxNGWService *pService, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName, const CPLString &soPath = "");
    virtual ~wxGxNGWRaster(void);
    //wxGxObject
    virtual wxString GetCategory(void) const;
	//IGxObjectEdit
	virtual bool Delete(void);
    virtual bool CanDelete(void);
	virtual bool Rename(const wxString& NewName);
    virtual bool CanRename(void);
    virtual bool Copy(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanCopy(const CPLString &szDestPath);
    virtual bool Move(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanMove(const CPLString &szDestPath);
protected:
    //create wxGISDataset without openning it
    virtual wxGISDataset* const GetDatasetFast(void);
	virtual int GetParentResourceId() const;
protected:
	int m_nEPSGCode;
	int m_nBandCount;
};

/** @class wxGxNGWPostGISConnection

    A NextGIS Web Service PostGIS Connection GxObject.

    @library{catalog}
*/

class WXDLLIMPEXP_GIS_CLT wxGxNGWPostGISConnection :
    public wxGxNGWResource,
	public wxGxRemoteConnection
{
    DECLARE_CLASS(wxGxNGWPostGISConnection)
public:
    wxGxNGWPostGISConnection(wxGxNGWService *pService, const wxJSONValue &Data, wxGxObject *oParent, const wxString &soName, const CPLString &soPath = "");
    virtual ~wxGxNGWPostGISConnection(void);
    //wxGxObject
    virtual wxString GetCategory(void) const;    
	//IGxObjectEdit
	virtual bool Delete(void);
    virtual bool CanDelete(void);
	virtual bool Rename(const wxString& NewName);
    virtual bool CanRename(void);
    virtual bool Copy(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanCopy(const CPLString &szDestPath);
    virtual bool Move(const CPLString &szDestPath, ITrackCancel* const pTrackCancel);
    virtual bool CanMove(const CPLString &szDestPath);
protected:
    //create wxGISDataset without openning it
    virtual wxGISDataset* const GetDatasetFast(void);
	virtual int GetParentResourceId() const;
protected:
	wxString m_sUser, m_sPass, m_sDatabase, m_sHost;
};

#endif // wxGIS_USE_CURL
