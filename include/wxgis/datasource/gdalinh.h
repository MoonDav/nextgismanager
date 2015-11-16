/******************************************************************************
 * Project:  wxGIS
 * Purpose:  inherited from gdal and ogr classes.
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2012-2014 Dmitry Baryshnikov
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

#include "wxgis/datasource/datasource.h"

#undef LT_OBJDIR
#include "gdal_priv.h"
#include "ogrsf_frmts.h"

#if GDAL_VERSION_NUM >= 2000000
    #define OGRCompatibleDataSource GDALDataset
    #define OGRCompatibleClose(x) GDALClose(x)
    #define OGRCompatibleDriver GDALDriver
    #define GetOGRCompatibleDriverByName(x) GetGDALDriverManager()->GetDriverByName( x )
    #define CreateOGRCompatibleDataSource(path, options) Create( path, 0, 0, 0, GDT_Unknown, options );
    #define GetOGRCompatibleDriverName GetDescription
    #define GetOGRCompatibleDatasourceName GetDriverName
#else
    #define OGRCompatibleDataSource OGRDataSource
    #define OGRCompatibleClose(x) OGRDataSource::DestroyDataSource(x)
    #define OGRCompatibleDriver OGRSFDriver
    #define GetOGRCompatibleDriverByName(x) dynamic_cast<OGRSFDriver*>(OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(x))
    #define CreateOGRCompatibleDataSource(path, options) CreateDataSource( path, options )
    #define GetOGRCompatibleDriverName GetName
    #define GetOGRCompatibleDatasourceName GetName
#endif // GDAL_VERSION_NUM

#define wxGISEQUAL(a,b) ( (const char*)a == NULL ? 0 : EQUAL(a,b) )
#define wxGISEQUALN(a,b,n) ( (const char*)a == NULL ? 0 : EQUALN(a,b,n) )

int WXDLLIMPEXP_GIS_DS CPL_STDCALL GDALExecuteProgress( double dfComplete, const char *pszMessage, void *pData);

#define POINT_STR _("Point")
#define LINESTRING_STR _("Line String")
#define POLYGON_STR _("Polygon")
#define MPOINT_STR _("Multi Point")
#define MLINESTRING_STR _("Multi Line String")
#define MPOLYGON_STR _("Multi Polygon")

/** @class wxGISSpatialReference

    An OGRSpatialReference wrapper class.

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISSpatialReference : public wxObject
{
    DECLARE_CLASS(wxGISSpatialReference)
public:
    wxGISSpatialReference(OGRSpatialReference *poSRS = NULL);
	wxGISSpatialReference(const wxString &sWKT);
    bool IsOk() const;

    bool operator == ( const wxGISSpatialReference& obj ) const;
    bool operator != (const wxGISSpatialReference& obj) const { return !(*this == obj); };
    operator OGRSpatialReference*() const;
    OGRSpatialReference* Clone(void) const;
    OGRSpatialReference* operator->(void) const;
    bool IsSame(const wxGISSpatialReference& SpatialReference) const;
	wxString ExportAsWKT() const;
	wxString GetName();
protected:
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;
protected:
	wxString m_sName;
};

extern WXDLLIMPEXP_DATA_GIS_DS(wxGISSpatialReference) wxNullSpatialReference;


/** @class wxGISSpatialReferenceRefData

    A reference data class for wxGISSpatialReference

    @library{datasource}
*/

class wxGISSpatialReferenceRefData : public wxObjectRefData
{
    friend class wxGISSpatialReference;
public:
    wxGISSpatialReferenceRefData(OGRSpatialReference *poSRS = NULL);
    virtual ~wxGISSpatialReferenceRefData(void);
    wxGISSpatialReferenceRefData(const wxGISSpatialReferenceRefData& data);
    bool operator == (const wxGISSpatialReferenceRefData& data) const;
    virtual void Validate();
    virtual bool IsValid(void) const;
protected:
	OGRSpatialReference *m_poSRS;
    bool m_bIsValid;
};

class WXDLLIMPEXP_GIS_DS wxGISGeometry;
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxGISGeometry, wxGISGeometryArray, WXDLLIMPEXP_GIS_DS);
class WXDLLIMPEXP_GIS_DS wxGISFeature;
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxGISFeature, wxGISFeatureArray, WXDLLIMPEXP_GIS_DS);

/** @class wxGISFeature

    An OGRFeature wrapper class.

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISFeature : public wxObject
{
    DECLARE_CLASS(wxGISFeature)
public:
    wxGISFeature(OGRFeature *poFeature = NULL, const wxFontEncoding &oEncoding = wxLocale::GetSystemEncoding(), bool bRecodeToSystem = false);

    bool IsOk() const;

    bool operator == ( const wxGISFeature& obj ) const;
    bool operator != (const wxGISFeature& obj) const { return !(*this == obj); };
    operator OGRFeature*() const;

    //get
    const char* GetFieldAsChar(int nField) const;
    OGRField* GetRawField(int nField) const;
    wxString GetFieldAsString(int nField) const;
    wxString GetFieldAsString(const wxString &sFieldName) const;
    wxDateTime GetFieldAsDateTime(int nField) const;
    wxDateTime GetFieldAsDateTime(const wxString &sFieldName) const;
    double GetFieldAsDouble(int nField) const;
    double GetFieldAsDouble (const wxString &sFieldName) const;
    wxArrayString GetFieldAsStringList(int nField) const;
    wxArrayString GetFieldAsStringList(const wxString &sFieldName) const;
    wxArrayDouble GetFieldAsDoubleList(int nIndex) const;
    wxArrayDouble GetFieldAsDoubleList(const wxString &sFieldName) const;
    wxArrayInt GetFieldAsIntegerList(int nIndex) const;
    wxArrayInt GetFieldAsIntegerList(const wxString &sFieldName) const;
    wxGISGeometry GetGeometry(void) const;
    int GetFieldAsInteger(int nIndex) const;
    int GetFieldAsInteger(const wxString &sFieldName) const;	
	wxString GetStyleString() const;

    int GetFieldIndex(const wxString &sFieldName) const;
    wxString GetFieldName(int nIndex) const;
    int GetFieldCount(void) const;
    long GetFID(void) const;
    //set
    OGRErr SetFID(long nFID);
    void SetField(int nIndex, int nValue);
    void SetField(int nIndex, OGRField* psField);
    void SetField(int nIndex, double dfValue);
    void SetField(int nIndex, const wxString &sValue);
    void SetField(int nIndex, const char* pszStr);
    void SetField (int nIndex, const wxArrayInt &anValues);
    void SetField (int nIndex, const wxArrayDouble &adfValues);
    void SetField(int nIndex, const wxArrayString &asValues);
    void SetField(int nIndex, const wxDateTime &dt);
    void SetField(const wxString &sFieldName, int nValue);
    void SetField(const wxString &sFieldName, double dfValue);
    void SetField(const wxString &sFieldName, const wxString &sValue);
    void SetField (const wxString &sFieldName, const wxArrayInt &anValues);
    void SetField (const wxString &sFieldName, const wxArrayDouble &adfValues);
    void SetField(const wxString &sFieldName, const wxArrayString &asValues);
    void SetField(int nIndex, int nYear, int nMonth, int nDay, int nHour = 0, int nMinute = 0, int nSecond = 0, int nTZFlag = 0);
    void SetField(const wxString &sFieldName, int nYear, int nMonth, int nDay, int nHour = 0, int nMinute = 0, int nSecond = 0, int nTZFlag = 0);
    OGRErr SetGeometry(const wxGISGeometry &Geom);
    OGRErr SetGeometryDirectly(const wxGISGeometry &Geom);
    void SetStyleString(const wxString &sStyle);
    void StealGeometry(void);
    void SetEncoding(const wxFontEncoding &eEnc, bool bRecodeToSystem = false);
protected:
    inline wxString EncodeString(const char* psz, wxFontEncoding oEncoding) const
    {
        if (strlen(psz) > 0)
        {
            if (oEncoding <= wxFONTENCODING_DEFAULT)
            {
                return wxString(psz, wxConvLocal);
            }
            else if (oEncoding == wxFONTENCODING_UTF8)
            {
                return wxString::FromUTF8(psz);
            }
            else if (oEncoding == wxFONTENCODING_UTF7)
            {
                return wxString(psz, wxConvUTF7);
            }
            else if (oEncoding == wxFONTENCODING_UTF16)
            {
                return wxString(psz, wxMBConvUTF16());
            }
            else if (oEncoding == wxFONTENCODING_UTF32)
            {
                return wxString(psz, wxMBConvUTF32());
            }
            else if (oEncoding == wxFONTENCODING_ISO8859_1)
            {
                return wxString(psz, wxConvISO8859_1);
            }
            else if (wxLocale::GetSystemEncoding() == oEncoding)
            {
                return wxString(psz, wxConvLocal);
            }
            else
            {
                wxString sOut = wxString(psz, wxCSConv(oEncoding));
                if (sOut.IsEmpty())
                {
                    sOut = wxString(psz, wxConvLocal);
                }
                return sOut;
            }
        }
        return wxEmptyString;
    };

    inline const char* EncodeString(const wxString &sStr, wxFontEncoding oEncoding) const
    {
        if (!sStr.IsEmpty())
        {
            if (oEncoding <= wxFONTENCODING_DEFAULT)
            {
                return sStr.mb_str(wxConvLocal);
            }
            else if (oEncoding == wxFONTENCODING_UTF8)
            {
                return sStr.mb_str(wxConvUTF8);
            }
            else if (oEncoding == wxFONTENCODING_UTF7)
            {
                return sStr.mb_str(wxConvUTF7);
            }
            else if (oEncoding == wxFONTENCODING_UTF16)
            {
                return sStr.mb_str(wxMBConvUTF16());
            }
            else if (oEncoding == wxFONTENCODING_UTF32)
            {
                return sStr.mb_str(wxMBConvUTF32());
            }
            else if (oEncoding == wxFONTENCODING_ISO8859_1)
            {
                return sStr.mb_str(wxConvISO8859_1);
            }
            else if (wxLocale::GetSystemEncoding() == oEncoding)
            {
                return sStr.mb_str(wxConvLocal);
            }
            else
            {
                const char* pszOut = sStr.mb_str(wxCSConv(oEncoding));
                if (strlen(pszOut) == 0)
                {
                    pszOut = sStr.mb_str(wxConvLocal);
                }
                return pszOut;
            }
        }
        return NULL;
    };



    int GetFieldAsDateTime(int nIndex, int *pnYear, int *pnMonth, int *pnDay, int *pnHour, int *pnMinute, int *pnSecond, int *pnTZFlag) const;
    void SetField(int nIndex, char **papszValues);
    OGRErr SetGeometry(OGRGeometry* pGeom);
protected:
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;
};

/** @class wxGISFeatureRefData

    A reference data class for wxGISFeature

    @library{datasource}
*/

class wxGISFeatureRefData : public wxObjectRefData
{
    friend class wxGISFeature;
public:
    wxGISFeatureRefData(OGRFeature *poFeature = NULL, const wxFontEncoding &oEncoding = wxLocale::GetSystemEncoding(), bool bRecodeToSystem = false)
    {
        m_poFeature = poFeature;
        m_oEncoding = oEncoding;
        m_bRecodeToSystem = bRecodeToSystem;
    }

    virtual ~wxGISFeatureRefData(void)
    {
        if(m_poFeature)
            OGRFeature::DestroyFeature(m_poFeature);
    }

    wxGISFeatureRefData( const wxGISFeatureRefData& data )
        : wxObjectRefData()
    {
        m_poFeature = data.m_poFeature;
        m_oEncoding = data.m_oEncoding;
        m_bRecodeToSystem = data.m_bRecodeToSystem;
    }

    bool operator == (const wxGISFeatureRefData& data) const
    {
        return m_poFeature == data.m_poFeature;
    }

    void SetEncoding(const wxFontEncoding &eEnc, bool bRecodeToSystem = false)
    {
        m_oEncoding = eEnc;
        m_bRecodeToSystem = bRecodeToSystem;
    }

protected:
	OGRFeature *m_poFeature;
    wxFontEncoding m_oEncoding;
    bool m_bRecodeToSystem;
};

/** @class wxGISGeometry

    An OGRGeometry wrapper class.

    @library{datasource}
*/

class WXDLLIMPEXP_GIS_DS wxGISGeometry : public wxObject
{
    DECLARE_CLASS(wxGISGeometry)
public:
    wxGISGeometry(OGRGeometry *poGeom = NULL, bool bOwnGeom = true);

    bool IsOk() const;

    bool operator == ( const wxGISGeometry& obj ) const;
    bool operator != (const wxGISGeometry& obj) const { return !(*this == obj); };
    operator OGRGeometry*() const;

    OGREnvelope GetEnvelope(void) const;
    OGRGeometry* Copy(void) const;
    OGRGeometry* Steal(void);
    OGRPoint* GetCentroid(void);
    wxGISSpatialReference GetSpatialReference() const;
    void SetSpatialReference(const wxGISSpatialReference &SpaRef);

    wxGISGeometry Intersection(const wxGISGeometry &Geom) const;
    wxGISGeometry UnionCascaded() const;
    wxGISGeometry Union(const wxGISGeometry &Geom) const;
    bool Intersects(const wxGISGeometry &Geom) const;
    wxGISGeometry Buffer(double dfBuff, int nQuadSegs = 30) const;
    bool Project(const wxGISSpatialReference &SpaRef);
    bool Project(OGRCoordinateTransformation* const poCT);
    OGRwkbGeometryType GetType() const;
    wxGISGeometry Clone() const;
	bool IsValid() const;
    wxString ToWKT() const;
protected:
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;
};

/** @class wxGISGeometryRefData

    A reference data class for wxGISGeometry

    @library{datasource}
*/

class wxGISGeometryRefData : public wxObjectRefData
{
    friend class wxGISGeometry;
public:
    wxGISGeometryRefData(OGRGeometry *poGeom = NULL, bool bOwnGeom = true)
    {
        m_poGeom = poGeom;
        m_bOwnGeom = bOwnGeom;
    }

    virtual ~wxGISGeometryRefData(void)
    {
        if (m_bOwnGeom && NULL != m_poGeom)
            OGRGeometryFactory::destroyGeometry(m_poGeom);
    }

    wxGISGeometryRefData( const wxGISGeometryRefData& data )
        : wxObjectRefData()
    {
        m_poGeom = data.m_poGeom;
        m_bOwnGeom = data.m_bOwnGeom;
    }

    bool operator == (const wxGISGeometryRefData& data) const
    {
        return m_poGeom == data.m_poGeom;
    }

protected:
	OGRGeometry *m_poGeom;
    bool m_bOwnGeom;
};

extern WXDLLIMPEXP_DATA_GIS_DS(wxGISGeometry) wxNullGeometry;
