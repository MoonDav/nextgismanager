/******************************************************************************
 * Project:  wxGIS
 * Purpose:  Table class.
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2010-2014 Dmitry Baryshnikov
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

#include "wxgis/datasource/dataset.h"
#include "wxgis/datasource/cursor.h"
#include "wxgis/datasource/filter.h"
#include "wxgis/datasource/spatialtree.h"
#include "wxgis/core/pointer.h"

/** @class wxGISTable

    A GIS Table class.
    This class support basic operations on datasource. No spatial data avaliable, but the OGRFeature is main part of this class, so it's posible to get some spatial information (not recommended).

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISTable :
    public wxGISDataset,
	public wxGISConnectionPointContainer
{
    DECLARE_CLASS(wxGISTable)
    friend class wxGISSpatialTree;
public:
	wxGISTable(const CPLString &sPath, int nSubType, OGRLayer* poLayer = NULL, OGRCompatibleDataSource* poDS = NULL);
	virtual ~wxGISTable(void);
	//wxGISDataset
    virtual wxString GetName(void) const;
	virtual void Close(void);
	//wxGISTable
	virtual bool Open(bool bUpdate = true, bool bShared = true);
	virtual bool Open(int iLayer = 0, bool bUpdate = true, bool bShared = true, bool bCache = true, ITrackCancel* const pTrackCancel = NULL);
    virtual size_t GetSubsetsCount(void) const;
    virtual wxGISDataset* GetSubset(size_t nIndex);
    virtual wxGISDataset* GetSubset(const wxString & sSubsetName);
    //sysop
	virtual char **GetFileList();
	virtual void Cache(ITrackCancel* const pTrackCancel = NULL);
    //rowop
	virtual size_t GetFeatureCount(bool bForce = false, ITrackCancel* const pTrackCancel = NULL);
    virtual bool CanDeleteFeature(void) const;
	virtual OGRErr DeleteFeature(long nFID);
    virtual OGRErr StoreFeature(wxGISFeature &Feature);
	virtual wxGISFeature CreateFeature(void);
    virtual OGRErr SetFeature(const wxGISFeature &Feature);
    virtual OGRErr CommitTransaction(void);
    virtual OGRErr StartTransaction(void);
    virtual OGRErr RollbackTransaction(void);
    //colop
	virtual bool CanDeleteField(void) const;
    virtual OGRErr DeleteField(int nIndex);
    //
    virtual void Reset(void);
    virtual wxGISFeature Next(void);
    virtual wxGISFeature GetFeatureByID(long nFID);
    virtual wxGISFeature GetFeature(long nIndex);
    virtual wxString GetFIDColumn(void) const;
    //
    virtual wxFontEncoding GetEncoding(void) const;
    virtual void SetEncoding(const wxFontEncoding &oEncoding);
    virtual bool HasFID(void) const {return m_bHasFID;};
    virtual bool HasFilter(void) const { return m_bHasFilter; };
    virtual wxArrayString GetFieldNames(void) const;
    virtual wxString GetFieldName(int nIndex) const;
    //
    virtual wxFeatureCursor Search(const wxGISQueryFilter &QFilter = wxGISNullQueryFilter, bool bOnlyFirst = false, ITrackCancel* const pTrackCancel = NULL);
    virtual OGRErr SetFilter(const wxGISQueryFilter &QFilter = wxGISNullQueryFilter);
    virtual OGRErr SetIgnoredFields(const wxArrayString &saIgnoredFields);
	virtual OGRCompatibleDataSource* const GetDataSourceRef(void) const {return m_poDS;};
    virtual OGRLayer* const GetLayerRef(int iLayer = 0) const {return m_poLayer;};
	virtual OGRFeatureDefn* const GetDefinition(void);
	virtual OGRFeatureDefn* const GetDefinition(void) const;
protected:
    virtual void SetInternalValues(void);
 	virtual bool IsContainer() const;
protected:
	OGRCompatibleDataSource* m_poDS;
	OGRLayer* m_poLayer;

    wxFontEncoding m_Encoding;
    bool m_bRecodeToSystem;

    long m_nFeatureCount, m_nCurrentFID;
    bool m_bOLCStringsAsUTF8;
    bool m_bOLCFastFeatureCount;
    bool m_bHasFID;
    bool m_bHasFilter;
};

//#define MAXSTRINGSTORE 1000000

/** @class wxGISTableCached

    A GIS cached Table class.
    The table with caching rows for future fast fetch.

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISTableCached :
    public wxGISTable
{
    DECLARE_CLASS(wxGISTableCached)
public:
	wxGISTableCached(const CPLString &sPath, int nSubType, OGRLayer* poLayer = NULL, OGRCompatibleDataSource* poDS = NULL);
	virtual ~wxGISTableCached(void);
	//wxGISDataset
	virtual void Close(void);
    //wxGISTable
    virtual void Reset(void);
    virtual wxGISFeature Next(void);
    virtual wxGISFeature GetFeatureByID(long nFID);
    virtual wxGISFeature GetFeature(long nIndex);
    //sysop
	virtual void Cache(ITrackCancel* const pTrackCancel = NULL);
    virtual bool IsCaching(void) const;
    //rowop
	virtual size_t GetFeatureCount(bool bForce = false, ITrackCancel* const pTrackCancel = NULL);
	virtual OGRErr DeleteFeature(long nFID);
    virtual OGRErr StoreFeature(wxGISFeature &Feature);
    virtual OGRErr SetFeature(const wxGISFeature &Feature);
    virtual wxFeatureCursor Search(const wxGISQueryFilter &QFilter = wxGISNullQueryFilter, bool bOnlyFirst = false, ITrackCancel* const pTrackCancel = NULL);
protected:
    std::map<long, wxGISFeature> m_omFeatures;
    bool m_bIsCaching;
};

/** @class wxGISTableQuery

    The query result table class

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISTableQuery :
    public wxGISTableCached
{
    DECLARE_CLASS(wxGISTableQuery)
public:
    wxGISTableQuery(const CPLString &sPath, int nSubType, OGRLayer* poLayer = NULL, OGRCompatibleDataSource* poDS = NULL);
    virtual ~wxGISTableQuery(void);
    virtual void Cache(ITrackCancel* const pTrackCancel = NULL);
	virtual size_t GetFeatureCount(bool bForce = false, ITrackCancel* const pTrackCancel = NULL);
};
