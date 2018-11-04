/*
*
* WhatsappTray
* Copyright (C) 1998-2018 Sebastian Amann
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#include "stdafx.h"
#include "Helper.h"

#include "Logger.h"

#include <vector>

#include <cassert>

/*
* @brief Get the Path to the exe-file of the application.
*/
std::string Helper::GetApplicationFilePath()
{
	char szPathToExe[MAX_PATH];
	GetModuleFileNameA(NULL, szPathToExe, MAX_PATH);

	return szPathToExe;
}

/*
* @brief Get the Path to the directory where the exe-file of the application is.
*/
std::string Helper::GetApplicationDirectory()
{
	char szPathToExe[MAX_PATH];
	GetModuleFileNameA(NULL, szPathToExe, MAX_PATH);

	std::string applicationFilePath = GetApplicationFilePath();
	std::size_t found = applicationFilePath.find_last_of('\\');
	if (found == std::string::npos) {
		OutputDebugString(TEXT("ERROR: GetApplicationDirectory() - Could not find \\ in the application-path.\n"));
		return "";
	}

	return applicationFilePath.substr(0, found) + "\\";
}

/**
* From https://stackoverflow.com/questions/7141260/compare-stdwstring-and-stdstring/7159944#7159944
*/
//std::wstring Helper::get_wstring(const std::string& s)
//{
//	const char* cs = s.c_str();
//	const size_t wn = std::mbsrtowcs(NULL, &cs, 0, NULL);
//
//	if (wn == size_t(-1)) {
//		Logger::Error("Error in mbsrtowcs(): %d", errno);
//		return L"";
//	}
//
//	std::vector<wchar_t> buf(wn + 1);
//	const size_t wn_again = std::mbsrtowcs(buf.data(), &cs, wn + 1, NULL);
//
//	if (wn_again == size_t(-1)) {
//		Logger::Error("Error in mbsrtowcs(): %d", errno);
//		return L"";
//	}
//
//	assert(cs == NULL); // successful conversion
//
//	return std::wstring(buf.data(), wn);
//}

/**
* Untested!
*/
std::wstring Helper::ToWString(const std::string& inputString)
{
	const char* inputC_String = inputString.c_str();
	size_t size = inputString.length() + 1;
	wchar_t* outputStringBuffer = new wchar_t[size];

	size_t outSize;
	errno_t convertResult = mbstowcs_s(&outSize, outputStringBuffer, size, inputC_String, size - 1);

	if (convertResult != NULL) {
		Logger::Error("Error in mbsrtowcs(): %d", errno);
		return L"";
	}

	std::wstring outputString = outputStringBuffer;
	delete outputStringBuffer;

	return outputString;
}

/**
* Convert a std::wstring to a std::string.
*/
std::string Helper::ToString(const std::wstring& inputString)
{
	const wchar_t* inputC_String = inputString.c_str();
	size_t size = inputString.length() + 1;
	char* outputStringBuffer = new char[size];

	size_t outSize;
	errno_t convertResult = wcstombs_s(&outSize, outputStringBuffer, size, inputC_String, size - 1);

	if (convertResult != NULL) {
		Logger::Error("Error in mbsrtowcs(): %d", errno);
		return "";
	}

	std::string outputString = outputStringBuffer;
	delete outputStringBuffer;

	return outputString;
}