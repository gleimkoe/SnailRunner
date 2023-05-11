#ifndef _MAKROS_H
#define _MAKROS_H

#ifdef MAKE_DLL_EXPORTS
#define DLL_DECLSPEC	__declspec(dllexport)	
#else
#define DLL_DECLSPEC	__declspec(dllimport)
#endif

#endif