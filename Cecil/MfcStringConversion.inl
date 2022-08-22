/* Copyright (c) 2022 Dreamy Cecil
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

// Inline definition of the MFC-to-CT string conversion method

// Convert MFC's CString to CTString
CTString MfcStringToCT(const CString &str) {
#ifndef UNICODE
  // Return as is
  return str;

#else
  const wchar_t *pstrWide = str;
  INDEX ct = str.GetLength();

  // Create null-terminated ANSI string
  char *pstrANSI = new char[ct + 1];
  pstrANSI[ct] = 0;

  // Copy every byte into wide characters
  while (--ct >= 0) {
    // Special remappings
    if (pstrWide[ct] == (wchar_t)0x0178) {
      pstrANSI[ct] = (char)0x9F;

    } else if (pstrWide[ct] == (wchar_t)0x20AC) {
      pstrANSI[ct] = (char)0x80;

    // Copy as is
    } else {
      pstrANSI[ct] = (char)pstrWide[ct];
    }
  }
  
  CTString strOut(pstrANSI);
  delete[] pstrANSI;
  
  return strOut;
#endif
};
