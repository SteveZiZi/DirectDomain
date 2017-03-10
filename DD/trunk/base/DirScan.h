#ifndef INCLUDED_DIR_SCAN
#define INCLUDED_DIR_SCAN


typedef bool (*DIR_SCAN_PROC)(int iiDepth,bool ibIsDir,const char* ipDir,const char* ipName,void* ipOther);

#ifndef WIN32
const char PATH_SEPS[]="/";
#else
const char PATH_SEPS[]="\\";
#endif

class DirScan
{
public:
	DirScan(const char* pPathName);
	~DirScan();
public:
	bool Scan(DIR_SCAN_PROC ipScanProc,void* ipOther,int iiScanMaxDepth=-1);
private:
	bool IsDir(const char* ipPath);
	bool ScanDir(const char* ipDir,int iiDepth,DIR_SCAN_PROC ipScanProc,void* ipOther,int iiScanMaxDepth);
	void ParsePath(const char* pPath);
public:
	char* m_pDir;
	char* m_pExe;
};

#endif

