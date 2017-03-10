/*for window dll
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� LZ77_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LZ77_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LZ77_EXPORTS
#define LZ77_API extern "C" __declspec(dllexport)
#else
#define LZ77_API extern "C" __declspec(dllimport)
#endif
#pragma comment (linker,"/export:Compress=_Compress@16")
#pragma comment (linker,"/export:Decompress=_Decompress@16")

LZ77_API int __stdcall Compress(int compresscode,BYTE* src, int srclen, BYTE* dest);
LZ77_API BOOL __stdcall Decompress(int compresscode,BYTE* src, int srclen, BYTE* dest);

//extern LZ77_API int nlz77;

//LZ77_API int fnlz77(void);

*/

//////////////////////////////
// LZ77.h
//////////////////////////////

// ʹ�����Լ��Ķ��з��������ڵ㣬����������
// ÿ�����ѹ�� 65536 �ֽ�����
// ���Ż��汾

#ifndef _WIX_LZ77_COMPRESS_HEADER_001_
#define _WIX_LZ77_COMPRESS_HEADER_001_

// �������ڵ��ֽڴ�С
#define _MAX_WINDOW_SIZE	65536

typedef unsigned char BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef int                 BOOL;
#define FALSE   0
#define TRUE    1

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define _ASSERT(expr) ((void)0)

class CCompress
{
public:
	CCompress() 
	{
		m_bNeedInvert=false;
		DWORD test=0xffff;
		unsigned char* pTest=(unsigned char*)(&test);
		if ((*(pTest+2)==0) && (*(pTest+3)==0))
		{
			m_bNeedInvert=true;
		}
	};
	virtual ~CCompress() {};

public:
	virtual int Compress(BYTE* src, int srclen, BYTE* dest) = 0;
	virtual BOOL Decompress(BYTE* src, int srclen, BYTE* dest) = 0;

protected:
	// tools 

	/////////////////////////////////////////////////////////
	// CopyBitsInAByte : ��һ���ֽڷ�Χ�ڸ���λ��
	// ��������ͬ CopyBits �Ĳ���
	// ˵����
	//		�˺����� CopyBits ���ã����������飬��
	//		�ٶ�Ҫ���Ƶ�λ����һ���ֽڷ�Χ��
	void CopyBitsInAByte(BYTE* memDest, int nDestPos, 
				  BYTE* memSrc, int nSrcPos, int nBits);

	////////////////////////////////////////////////////////
	// CopyBits : �����ڴ��е�λ��
	//		memDest - Ŀ��������
	//		nDestPos - Ŀ����������һ���ֽ��е���ʼλ
	//		memSrc - Դ������
	//		nSrcPos - Դ��������һ���ֽڵ�����ʼλ
	//		nBits - Ҫ���Ƶ�λ��
	//	˵����
	//		��ʼλ�ı�ʾԼ��Ϊ���ֽڵĸ�λ����λ���������ң�
	//		����Ϊ 0��1��... , 7
	//		Ҫ���Ƶ������������������غ�
	void CopyBits(BYTE* memDest, int nDestPos, 
				  BYTE* memSrc, int nSrcPos, int nBits);

	//////////////////////////////////////////////////////////////
	// ��DWORDֵ�Ӹ�λ�ֽڵ���λ�ֽ�����
	void InvertDWord(DWORD* pDW);

	/////////////////////////////////////////////////////////////
	// ����byte�ĵ�iBitλΪaBit
	//		iBit˳��Ϊ��λ���0����������
	void SetBit(BYTE* byte, int iBit, BYTE aBit);

	////////////////////////////////////////////////////////////
	// �õ��ֽ�byte��posλ��ֵ
	//		pos˳��Ϊ��λ���0����������
	BYTE GetBit(BYTE byte, int pos);

	////////////////////////////////////////////////////////////
	// ��λָ��*piByte(�ֽ�ƫ��), *piBit(�ֽ���λƫ��)����numλ
	void MovePos(int* piByte, int* piBit, int num);

	/////////////////////////////////////////////////////////
	// ȡlog2(n)��upper_bound
	int UpperLog2(int n);

	/////////////////////////////////////////////////////////
	// ȡlog2(n)��lower_bound
	int LowerLog2(int n);

protected:
	bool m_bNeedInvert;

};

class CCompressLZ77 : public CCompress
{
public:
	CCompressLZ77();
	virtual ~CCompressLZ77();
public:
	/////////////////////////////////////////////
	// ѹ��һ���ֽ���
	// src - Դ������
	// srclen - Դ�������ֽڳ���, srclen <= 65536
	// dest - ѹ��������������ǰ����srclen�ֽ��ڴ�
	// ����ֵ > 0 ѹ�����ݳ���
	// ����ֵ = 0 �����޷�ѹ��
	// ����ֵ < 0 ѹ�����쳣����
	int Compress(BYTE* src, int srclen, BYTE* dest);

	/////////////////////////////////////////////
	// ��ѹ��һ���ֽ���
	// src - ����ԭʼ���ݵ��ڴ���, srclen <= 65536
	// srclen - Դ�������ֽڳ���
	// dest - ѹ��������
	// ����ֵ - �ɹ����
	BOOL Decompress(BYTE* src, int srclen, BYTE* dest);

protected:

	BYTE* pWnd;
	// ���ڴ�С���Ϊ 64k �����Ҳ�������
	// ÿ�����ֻѹ�� 64k ���ݣ��������Է�����ļ��м俪ʼ��ѹ
	// ��ǰ���ڵĳ���
	int nWndSize;

	// �Ի���������ÿһ��2�ֽڴ�����
	// ������Ϊ�˽��п�������ƥ��
	// ����ķ�������һ��64k��С��ָ������
	// �����±����ζ�Ӧÿһ��2�ֽڴ���(00 00) (00 01) ... (01 00) (01 01) ...
	// ÿһ��ָ��ָ��һ�����������еĽڵ�Ϊ��2�ֽڴ���ÿһ������λ��
	struct STIDXNODE
	{
		WORD off;		// ��src�е�ƫ��
		WORD off2;		// ���ڶ�Ӧ��2�ֽڴ�Ϊ�ظ��ֽڵĽڵ�
						// ָ�� off �� off2 ����Ӧ�˸�2�ֽڴ�
		WORD next;		// ��SortHeap�е�ָ��
	};
	
	WORD SortTable[65536];  // 256 * 256 ָ��SortHeap���±��ָ��

	// ��Ϊ���ڲ�������û��ɾ���ڵ�Ĳ���������
	// �ڵ������SortHeap ����������
	struct STIDXNODE* SortHeap;
	struct STIDXNODE heap[_MAX_WINDOW_SIZE];
	int HeapPos;	// ��ǰ����λ��

	// ��ǰ���λ��(�ֽ�ƫ�Ƽ�λƫ��)
	int CurByte, CurBit;

protected:
	////////////////////////////////////////
	// ���ѹ����
	// code - Ҫ�������
	// bits - Ҫ�����λ��(��isGamma=TRUEʱ��Ч)
	// isGamma - �Ƿ����Ϊ�ñ���
	void _OutCode(BYTE* dest, DWORD code, int bits, BOOL isGamma);

	///////////////////////////////////////////////////////////
	// �ڻ��������в�������
	// nSeekStart - �Ӻδ���ʼƥ��
	// offset, len - ���ڽ��ս������ʾ�ڻ��������ڵ�ƫ�ƺͳ���
	// ����ֵ- �Ƿ�鵽����Ϊ3��3���ϵ�ƥ���ֽڴ�
	BOOL _SeekPhase(BYTE* src, int srclen, int nSeekStart, int* offset, int* len);

	///////////////////////////////////////////////////////////
	// �õ��Ѿ�ƥ����3���ֽڵĴ���λ��offset
	// ����ƥ����ٸ��ֽ�
	inline int _GetSameLen(BYTE* src, int srclen, int nSeekStart, int offset);

	//////////////////////////////////////////
	// ���������һ���n���ֽ�
	inline void _ScrollWindow(int n);

	// �����������һ��2�ֽڴ�
	inline void _InsertIndexItem(int off);

	// ��ʼ���������ͷ��ϴ�ѹ���õĿռ�
	void _InitSortTable();
};



#endif // _WIX_LZW_COMPRESS_HEADER_001_
