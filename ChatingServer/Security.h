#pragma once

#include <windows.h>
#include <aclapi.h>
#include <tchar.h>
#include <strsafe.h>

namespace security
{
	void ErrorExit(LPTSTR lpszFunction);
	void Cleanup(PSID pEveryoneSID, PSID pAdminSID, PACL pACL, PSECURITY_DESCRIPTOR pSD);
	SECURITY_ATTRIBUTES Seguranca();
};