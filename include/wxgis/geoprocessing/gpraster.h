/******************************************************************************
 * Project:  wxGIS (GIS Toolbox)
 * Purpose:  raster dataset functions.
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2011,2014 Dmitry Baryshnikov
*   Copyright (C) 2014 NextGIS
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

#include "wxgis/geoprocessing/geoprocessing.h"
#include "wxgis/datasource/featuredataset.h"
#include "wxgis/datasource/rasterdataset.h"
#include "wxgis/catalog/gxfilters.h"

/** @enum wxGISEnumForceBandColorInterpretation

    Typs of band color interpretation.
*/

enum wxGISEnumForceBandColorInterpretation
{
	enumGISForceBandsToNone = 0,     /**< No force */
    enumGISForceBandsToGray = 1,	 /**< Force all bands to gray */
    enumGISForceBandsToRGB  = 3,     /**< Force 1 band to red, 2 - green, 3 - blue */
	enumGISForceBandsToRGBA = 4      /**< Force 1 band to red, 2 - green, 3 - blue, 4 - alpha */
};

typedef std::vector<double> Color;
typedef std::vector< Color > Colors;


/** \fn bool SubrasterByVector(wxGISFeatureDatasetSPtr pSrcFeatureDataSet, wxGISRasterDatasetSPtr pSrcRasterDataSet, CPLString &szDstFolderPath)
 *  \brief Get subruster clipped by vector geometry.
 *  \param pSrcFeatureDataSet The clip geometry source dataset
 *  \param pSrcRasterDataSet The clipped raster
 *  \param szDstFolderPath The subraster store path
 *  \return False if any error or true
 */
//bool WXDLLIMPEXP_GIS_DS SubrasterByVector( wxGISFeatureDatasetSPtr pSrcFeatureDataSet, wxGISRasterDatasetSPtr pSrcRasterDataSet, CPLString &szDstFolderPath, wxGxRasterFilter* pFilter = NULL, GDALDataType eOutputType = GDT_Unknown, int nBandCount = 0, int *panBandList = NULL, bool bUseCounter = true, int nCounterBegin = -1, int nFieldNo = -1, double dfOutResX = -1, double dfOutResY = -1, bool bCopyNodata = false, bool bSkipSourceMetadata = false, char** papszOptions = NULL, ITrackCancel* pTrackCancel = NULL );

//bool CreateSubRaster( wxGISRasterDatasetSPtr pSrcRasterDataSet, OGREnvelope &Env, const OGRGeometry *pGeom, GDALDriver* pDriver, CPLString &szDstPath, GDALDataType eOutputType = GDT_Unknown, int nBandCount = 0, int *panBandList = NULL, double dfOutResX = -1, double dfOutResY = -1, bool bCopyNodata = false, bool bSkipSourceMetadata = false, char** papszOptions = NULL, ITrackCancel* pTrackCancel = NULL );
//
WXDLLIMPEXP_GIS_GP bool ExportFormat(wxGISRasterDataset* const pSrsDataSet, const CPLString &sPath, const wxString &sName, wxGxObjectFilter* const pFilter, const wxGISSpatialFilter &SpaFilter, char ** papszOptions, ITrackCancel* const pTrackCancel = NULL);
WXDLLIMPEXP_GIS_GP bool ExportFormatEx(wxGISRasterDataset* const pSrsDataSet, const CPLString &sPath, const wxString &sName, wxGxObjectFilter* const pFilter, char ** papszOptions, const OGREnvelope &DstWin, GDALDataType eOutputType = GDT_Unknown, const wxArrayInt & anBands = wxArrayInt(), wxGISEnumForceBandColorInterpretation eForceBandColorTo = enumGISForceBandsToNone, bool bCopyNodata = false, bool bSkipSourceMetadata = false, ITrackCancel* const pTrackCancel = NULL);
WXDLLIMPEXP_GIS_GP bool ComputeStatistics(wxGISRasterDataset* const pSrsDataSet, bool bApprox, ITrackCancel* const pTrackCancel = NULL);
WXDLLIMPEXP_GIS_GP bool MakeBorderTransparent(wxGISRasterDataset* const pSrcDataSet, const wxArrayInt & anBands, int nAphaBand, double dfTransparentColor = 0, ITrackCancel* const pTrackCancel = NULL);

/** @fn CopyBandInfo( GDALRasterBand * const poSrcBand, GDALRasterBand * const poDstBand, bool bCanCopyStatsMetadata, bool bCopyScale, bool bCopyNoData )
  * 
  * A bit of a clone of VRTRasterBand::CopyCommonInfoFrom(), but we need more and more custom behaviour in the context of gdal_translate ...
  * 
  * @library{pg}
  */
void CopyBandInfo( GDALRasterBand * const poSrcBand, GDALRasterBand * const poDstBand, bool bCanCopyStatsMetadata, bool bCopyScale, bool bCopyNoData );
void AttachMetadata( GDALDataset * pDS, char **papszMetadataOptions );
bool ComputeStatistics(GDALDataset* poGDALDataset, bool bApprox, ITrackCancel* const pTrackCancel = NULL);
void ProcessLine(void *pabyLine, GDALDataType eType, int iStart, int iEnd, int nBands, double dfNearDist, int nMaxNonBlack, Colors &poColors, int *panLastLineCounts, int bDoHorizontalCheck, int bDoVerticalCheck, int bBottomUp);

inline void SetPixelValue(void* pBuff, GDALDataType eType, int nPos, double dfVal)
{
    switch (eType)
    {
    case GDT_Byte:
        ((GByte *)pBuff)[nPos] = (GByte)dfVal;
        break;
    case GDT_Float32:
        ((float *)pBuff)[nPos] = (float)dfVal;
        break;
    case GDT_Float64:
        ((double *)pBuff)[nPos] = dfVal;
        break;
    case GDT_Int32:
        ((GInt32 *)pBuff)[nPos] = (GInt32)dfVal;
        break;
    case GDT_UInt16:
        ((GUInt16 *)pBuff)[nPos] = (GUInt16)dfVal;
        break;
    case GDT_UInt32:
        ((GUInt32 *)pBuff)[nPos] = (GUInt32)dfVal;
        break;
    case GDT_CInt16:
        ((GInt16 *)pBuff)[nPos * 2] = (GInt16)dfVal;
        break;
    case GDT_CInt32:
        ((GInt32 *)pBuff)[nPos * 2] = (GInt32)dfVal;
        break;
    case GDT_CFloat32:
        ((float *)pBuff)[nPos * 2] = (float)dfVal;
        break;
    case GDT_CFloat64:
        ((double *)pBuff)[nPos * 2] = (double)dfVal;
        break;
    default:
        break;
    }
}

