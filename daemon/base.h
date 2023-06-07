/**
 * 	Базовые типы данных
 *
 */

#ifndef BASE_H
#define BASE_H

typedef int				udInt, udBool, *udPInt, **udPPInt;
typedef char			udChar, *udPChar, **udPPChar;
typedef unsigned char	udUChar, *udPUChar, **udPPUChar, udByte, *udPByte, **udPPByte;
typedef unsigned short	udUShort, *udPUShort, **udPPUShort, udWord, *udPWord, **udPPWord;
typedef unsigned		udUInt, *udPUInt, **udPPUInt;
typedef long			udLong, *udPLong, **udPPLong;
typedef unsigned long	udULong, udDWord, *udPULong, **udPPULong, *udPDWord, **udPPDWord;
typedef float			udFloat, *udPFloat, **udPPFloat;
typedef double			udDouble, *udPDouble, **udPPDouble;
typedef void			udVoid, *udPVoid, **udPPVoid;

namespace UdSDK {

typedef class CFile			udFile, *udPFile, **udPPFile;
typedef class CFileUtf32	udFileUtf32, *udPFileUtf32, **udPPFileUtf32;
typedef class CLog			udLog, *udPLog, **udPPLog;
typedef class CConfig		udConfig, *udPConfig, **udPPConfig;
typedef class CConverter	udConverter, *udPConverter, **udPPConverter;
typedef class CTrNode		udNode, *udPNode, **udPPNode;
typedef class CTrIndex		udIndex, *udPIndex, **udPPIndex;
typedef class CTrNodeInfo	udNodeInfo, *udPNodeInfo, **udPPNodeInfo;
typedef class CTrTree		udTree, *udPTree, **udPPTree;
typedef class CTranslator	udTranslator, *udPTranslator, **udPPTranslator;
typedef class CDaemon		udDaemon, *udPDaemon, **udPPDaemon;
typedef class CWarodai		udWarodai, *udPWarodai, **udPPWarodai;

}

// константы символов
#define LF				0x000aL		// \n
#define CR				0x000dL		// \r
#define SPACE			0x0020L		// пробел
#define COMMA			0x002cL		// ,
#define VERTICAL		0x007cL		// |
#define RBRACE_L		0x0028L		// (
#define RBRACE_R		0x0029L		// )
#define LT				0x003cL		// <
#define GT				0x003eL		// >
#define TAB				0x0009L		// tab

#define JPCOMMA			0x3001		// 、
#define JPDOT			0x3002		// 。
#define DITTO			0x3003		// 〃
#define LBLB			0x3010		// 【
#define RBLB			0x3011		// 】
#define LTSB			0x3014		// 〔
#define RTSB			0x3015		// 〕

#define FOR_WIN32
//#define FOR_TRANSLATOR

#endif
