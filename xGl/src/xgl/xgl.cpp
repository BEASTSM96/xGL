/********************************************************************************************
*                                                                                           *
*                                                                                           *
*                                                                                           *
* MIT License                                                                               *
*                                                                                           *
* Copyright (c) 2021 BEAST																	*
*                                                                                           *
* Permission is hereby granted, free of charge, to any person obtaining a copy              *
* of this software and associated documentation files (the "Software"), to deal             *
* in the Software without restriction, including without limitation the rights              *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                 *
* copies of the Software, and to permit persons to whom the Software is                     *
* furnished to do so, subject to the following conditions:                                  *
*                                                                                           *
* The above copyright notice and this permission notice shall be included in all            *
* copies or substantial portions of the Software.                                           *
*                                                                                           *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                  *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE               *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                    *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,             *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE             *
* SOFTWARE.                                                                                 *
*********************************************************************************************
*/

#include "xgl.h"

namespace xGL {


#if defined ( _WIN32 )

	static HMODULE s_Handle = NULL;

	typedef PROC ( *PFNwglGetProcAddress )( LPCSTR args );

	void* LoadFunc( const char* name )
	{
		static PFNwglGetProcAddress wglGetProcAddress = NULL;

		if( !s_Handle )
		{
			s_Handle = LoadLibraryA( "opengl32.lib" );
			wglGetProcAddress = ( PFNwglGetProcAddress )GetProcAddress( s_Handle, "wglGetProcAddress" );
		}

		// https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions

		void* func = ( void* )wglGetProcAddress( name );
		if( func == 0 ||
		  ( func == ( void* )0x1 ) || ( func == ( void* )0x2 ) || ( p == ( void* )0x3 ) ||
		  ( func == ( void* )-1 ) )
		{
			HMODULE module = LoadLibraryA( "opengl32.dll" );
			func = ( void* )GetProcAddress( s_Handle, name );
		}

		return func;
	}

	void Terminate()
	{
		FreeLibrary( s_Handle );
		s_Handle = NULL;
	}

#elif defined ( __LINUX__ )

#include <dlfcn.h>

	static void* s_Handle = XGL_EMPTY;

	void* LoadFunc( const char* name )
	{
		if ( !s_Handle )
		{
			s_Handle = dlopen( " libGL.so.1", RTLD_LAZY | RTLD_LOCAL );
			if( !s_Handle ) 
			{
				s_Handle = dlopen( " libGL.so", RTLD_LAZY | RTLD_LOCAL );
			}
		}

		void* func = dlsym( s_Handle, name );

		return func;
	}

	void Terminate()
	{
		if ( s_Handle )
		{
			dlclose( s_Handle );
			s_Handle = NULL;
		}
	}

#else

	#warning "OpenGL was deprecated in macOS 10.14. To create high performance code on GPUs, use the Metal framework instead."

	#import <mach-o/dyld.h>
	#import <stdlib.h>
	#import <string.h>

	void* LoadFunc( const char* name )
	{
		NSSymbol symbol;
		char* symbolName;

		symbolName = malloc( strlen( name ) + 2 ); 
		strcpy( symbolName + 1, name ); 

		symbolName[ 0 ] = '_';
		symbol = NULL;

		if( NSIsSymbolNameDefined( symbolName ) )
			symbol = NSLookupAndBindSymbol( symbolName );

		free( symbolName ); 

		return symbol ? NSAddressOfSymbol( symbol ) : NULL;
	}

	void Terminate()
	{
	}
	
#endif

}