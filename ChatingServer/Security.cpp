#include "Security.h"

void security::ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}

// Buffer clean up routine
void security::Cleanup(PSID pEveryoneSID, PSID pAdminSID, PACL pACL, PSECURITY_DESCRIPTOR pSD)
{
	if(pEveryoneSID)
		FreeSid(pEveryoneSID);
	if(pAdminSID)
		FreeSid(pAdminSID);
	if(pACL)
		LocalFree(pACL);
	if(pSD)
		LocalFree(pSD);
}

SECURITY_ATTRIBUTES security::Seguranca()
{
	SECURITY_ATTRIBUTES sa;
	PSECURITY_DESCRIPTOR pSD;
	PACL pAcl;
	EXPLICIT_ACCESS ea;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	TCHAR str[256];

	sa.nLength = sizeof(sa);

	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
		SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (pSD == NULL) {
		ErrorExit(TEXT("Erro LocalAlloc!"));
		return sa;
	}
	if (!InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION)) {
		ErrorExit(TEXT("Erro IniSec!"));
		return sa;
	}

	// Create a well-known SID for the Everyone group.
	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID,
	0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) 
	{
		_stprintf_s(str, 256,TEXT("AllocateAndInitializeSid() error %u"),GetLastError());
		ErrorExit(str);
		Cleanup(pEveryoneSID, pAdminSID, NULL, pSD);
	}

	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));

	// Tipo de permissão
	ea.grfAccessPermissions = GENERIC_WRITE|GENERIC_READ;
	ea.grfAccessMode = SET_ACCESS;
	ea.grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea.Trustee.ptstrName = (LPTSTR) pEveryoneSID;

	if (SetEntriesInAcl(1,&ea,NULL,&pAcl)!=ERROR_SUCCESS) {
		ErrorExit(TEXT("Erro SetAcl!"));
		return sa;
	}

	// Associar a permissão
	if (!SetSecurityDescriptorDacl(pSD,TRUE,pAcl,FALSE)) {
		ErrorExit(TEXT("Erro IniSec!"));
		return sa;
	}

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE;

	return sa;
}