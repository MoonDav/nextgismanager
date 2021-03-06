/******************************************************************************
 * Project:  wxGIS
 * Purpose:  color functions and classes
 * Author:   Dmitry Baryshnikov (aka Bishop), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2013 Dmitry Baryshnikov
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

#include "wxgis/display/display.h"

#if defined(wxUSE_GUI) && wxUSE_GUI
    #include <wx/colour.h>
#else
enum {
    wxC2S_NAME = 1,   // return colour name, when possible
    wxC2S_CSS_SYNTAX = 2,   // return colour in rgb(r,g,b) syntax
    wxC2S_HTML_SYNTAX = 4    // return colour in #rrggbb syntax
}; 

const unsigned char wxALPHA_TRANSPARENT = 0;
const unsigned char wxALPHA_OPAQUE = 0xff;
#endif
#define ChannelType unsigned char

/** @class wxGISColor

    The color representation class

    @library{display}
*/

class WXDLLIMPEXP_GIS_DSP wxGISColor
{
public:
    wxGISColor();
    wxGISColor(ChannelType red, ChannelType green, ChannelType blue, ChannelType alpha = wxALPHA_OPAQUE);
    wxGISColor(const wxColour &Color);
    wxGISColor(const wxGISColor &Color);
#if defined(wxUSE_GUI) && wxUSE_GUI
    wxColour GetColour() const;
#endif
    wxGISColor& operator=(const wxGISColor& Color);
    double GetRed() const;
    double GetBlue() const;
    double GetGreen() const;
    double GetAlpha() const;
    ChannelType Red() const;
    ChannelType Blue() const;
    ChannelType Green() const;
    ChannelType Alpha() const;
    static wxGISColor MakeRandom(ChannelType low = 205);
    void Set (ChannelType red, ChannelType green, ChannelType blue, ChannelType alpha = wxALPHA_OPAQUE);
    virtual wxString GetAsString(long nFlags = wxC2S_NAME|wxC2S_CSS_SYNTAX) const;
    virtual void SetFromString(const wxString &sColorTxt);
    virtual void SetAlpha(ChannelType alpha);
    virtual void SetRed(ChannelType red);
    virtual void SetGreen(ChannelType green);
    virtual void SetBlue(ChannelType blue);
protected:
    virtual void SetValues(ChannelType red, ChannelType green, ChannelType blue, ChannelType alpha);
protected:
    double m_dfRed;
    double m_dfBlue;
    double m_dfGreen;
    double m_dfAlpha;
    ChannelType m_nRed;
    ChannelType m_nGreen;
    ChannelType m_nBlue;
    ChannelType m_nAlpha;
};

