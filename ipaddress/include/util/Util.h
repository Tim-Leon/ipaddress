#pragma once
#include "Config.h"
#include <string>
#include <cassert>

namespace ip_address
{
	namespace details
	{

		/**
			* Safely converts a wstirng to string
			* @param [in] wstr wstring to convert.
			* @param [out] result resulting string from convertion
			* @return 0 on success
			*/
		inline int WStringToStringS(const std::wstring& wstr, std::string& result)
		{
			assert(result.size() == 0 && "Resulting string might be overwritten");
			//Check size needed
			const int size = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(),
				const_cast<char*>(result.c_str()), 0, nullptr, nullptr);
			if (size == 0)
				return -1;
			result.resize(size);
			if (const int res = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(),
				const_cast<char*>(result.c_str()), result.size(), nullptr,
				nullptr); res == 0)
			{
				const auto retErr = GetLastError();
				switch (retErr)
				{
					/*All error codes from WideCharToMultiByte */
				case ERROR_INSUFFICIENT_BUFFER:
				case ERROR_INVALID_FLAGS:
				case ERROR_INVALID_PARAMETER:
				case ERROR_NO_UNICODE_TRANSLATION:
					return -1;
				default: return -1;
				}
			}
			return -1;
		}

		/**
		* Unsafely converts a wstirng to string
		* @param [in] wstr wstring to convert.
		* @return converted wstring to string on success
		*/
		inline std::string WStringToString(const std::wstring& wstr)
		{
			std::string result;
			//Check size needed
			const int size = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(),
				const_cast<char*>(result.c_str()), 0, nullptr, nullptr);
			result.resize(size);
			//Convert wstring to string
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.c_str(), wstr.size(), const_cast<char*>(result.c_str()),
				result.size(), nullptr, nullptr);
			return result;
		}

		/**
		* Safely converts a string to wstring
		* @param [in] str string to convert
		* @param [out] result resulting wstring from convertion
		* @return 0 on success
		*/
		inline int StringToWStringS(const std::string& str, std::wstring& result)
		{
			assert(result.size() == 0 && "Resulting wstring might be overwritten");
			//Check size needed
			const int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(),
				const_cast<wchar_t*>(result.c_str()), 0);
			if (size == 0)
				return -1;
			result.resize(size);
			if (const int res = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(),
				const_cast<wchar_t*>(result.c_str()), result.size()); res == 0)
			{
				switch (const auto retErr = GetLastError(); retErr)
				{
					/*All error codes from MultiByteToWideChar */
				case ERROR_INSUFFICIENT_BUFFER:
				case ERROR_INVALID_FLAGS:
				case ERROR_INVALID_PARAMETER:
				case ERROR_NO_UNICODE_TRANSLATION:
					return -1;
				default: return -1;
				}
			}
			return -1;
		}

		/**
		* Unsafely converts a string to wstring
		* @param [in] str string to convert
		* @return wstring on success.
		*/
		inline std::wstring StringToWString(const std::string& str)
		{
			std::wstring result;
			//Check size needed
			const int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(),
				const_cast<wchar_t*>(result.c_str()), 0);
			result.resize(size);
			//Convert string to wstirng
			MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), str.size(), const_cast<wchar_t*>(result.c_str()),
				result.size());
			return result;
		}
	}
}