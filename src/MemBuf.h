#ifndef MEMBUF_H
#define MEMBUF_H
/*
 * Copyright (c) 2001,2002 Steven M. Cherry. All rights reserved.
 *
 * This file is a part of slib - a c++ utility library
 *
 * The slib project, including all files needed to compile 
 * it, is free software; you can redistribute it and/or use it and/or modify 
 * it under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this program.  See file COPYING for details.
 */

#ifdef _WIN32
#	ifndef DLLEXPORT
#		define DLLEXPORT __declspec(dllexport)
#	endif
#else
#	define DLLEXPORT 
#endif

#include <stdlib.h>
#include <stdarg.h>

#include <vector>
using namespace std;

#include "xmlinc.h"
#include "Base64.h"
#include "twine.h"

namespace SLib {

/**
  * This class represents a memory buffer that can be safely allocated, expanded,
  * indexed, and will ensure deletion of it's contents.
  */
class DLLEXPORT MemBuf
{
	public:

		/** Standard constructor.
		  */
		MemBuf(); 

		/** copy constructor
		  */
		MemBuf(const MemBuf& t);

		/** constructor from a char* - we will make a copy of the char* using strlen
		  * to determine the length of the input string.
		  */
		MemBuf(const char* c);

		/** constructor from a const xmlChar* - we will make a copy of the xmlChar* using
		  * strlen to determine the length of the input string.
		  */
		MemBuf(const xmlChar* c);

		/** constructor with a size parameter - we will reserve the amount of space requested
		  * and ensure that it is properly initialized to zero's.
		  */
		MemBuf(const size_t s);

		/** constructor from a twine - we will make a copy of the twine's internal buffer
		  * and use the twine's size() to set the size.
		  */
		MemBuf(const twine& c);

		/** Destructor
		  */
		virtual ~MemBuf();

		/** Assignment operation
		  */
		MemBuf& operator=(const MemBuf& t);

		/** Assignment operation
		  */
		MemBuf& operator=(const char* c);

		/** Assignment operation
		  */
		MemBuf& operator=(const xmlChar* c);

		/** Assignment operation
		  */
		MemBuf& operator=(const twine& c);

		/** Concatenation
		  */
		MemBuf& operator+=(const MemBuf& t);

		/** Concatenation
		  */
		MemBuf& operator+=(const char* c);

		/** Concatenation
		  */
		MemBuf& operator+=(const xmlChar* c);

		/** Concatenation
		  */
		MemBuf& operator+=(const twine& i);

		/** Get a single char from the MemBuf
		  */
		const char operator[](size_t i) const;

		/** Get the chars - This is easier to type than c_str()
		  */
		const char* operator()() const;

		/** Gets a writable pointer to the MemBuf memory.
		  * This, as a non const char pointer
		  * allows writing directly to the MemBuf memory.  This
		  * is allowed for efficiency, but the programmer beware.
		  * Using this is just like using a normal char* and you
		  * MUST be careful about over stepping the array bounds.
		  * <P>
		  * Use the reserve() method to ensure that you have enough
		  * memory to start with.
		  */
		char* data(void);

		/** Sets the chars of the MemBuf from the input.
		  */
		MemBuf& set(const char* c);

		/** Sets the chars of the MemBuf from first n chars of the input.
		  */
		MemBuf& set(const char* c, size_t n);

		/** Appends a const char* to the end of the MemBuf - this will use strlen() to determine size.
		  */
		MemBuf& append(const char* c);
			
		/** Appends a const char* with a given size to the end of the MemBuf
		  */
		MemBuf& append(const char* c, size_t n);
			
		/** Appends a const MemBuf& to the end of the MemBuf
		  */
		MemBuf& append(const MemBuf& t);
			
		/** Erases the range starting at p and going for n bytes.  This will have
		  * no affect on the size of the MemBuf, it will simply write binary 0 to
		  * the buffer starting at p and going for n bytes.
		  */
		MemBuf& erase(size_t p, size_t n);
			
		/** Erases the range starting at p and going to the end.  This will have
		  * no affect on the size of the MemBuf, it will simply write binary 0 to
		  * the buffer starting at p and going to the end.
		  */
		MemBuf& erase(size_t p);
			
		/** Erases the whole MemBuf.  This will write binary 0 to the whole buffer,
		  * without changing the size of the buffer.
		  */
		MemBuf& erase(void);
			
		/** Ensures that the MemBuf size is as requested.  If
		  * enough memory can't be allocated for the given size, an
		  * exception will be thrown.  This is the only allocation
		  * method in the MemBuf.  All other methods that potentially
		  * change the size of the MemBuf will use this.  Therefore
		  * the out of memory exception may be seen from other methods.
		  */
		MemBuf& reserve(size_t size);

		/** Returns the length of the MemBuf.
		  */
		size_t size(void) const;

		/** Returns the length of the MemBuf.
		  */
		size_t length(void) const;

		/** Returns true if the size of the MemBuf is zero.
		  */
		bool empty(void) const;

		/**
		  * Deletes our current memory buffer, and resets our size back to zero.
		  */
		MemBuf& clear(void);
			
		/**
		  * Handles converting the contents of our MemBuf into a base64 encoded
		  * version.
		  */
		MemBuf& encode64();

		/**
		  * Handles converting the contents of our MemBuf into a base64 decoded
		  * version.
		  */
		MemBuf& decode64();

		/**
		  * Zip up the contents of our MemBuf.
		  */
		MemBuf& zip();

		/**
		  * Unzip the contents of our MemBuf.
		  */
		MemBuf& unzip();

	private:

		/** Checks the index against the bounds of the array:
		  */
		void bounds_check(size_t p) const;

		/** our representation is a char array:
		  */
		void* m_data;
		
		/** size of string currently in m_data:
		  */
		size_t m_data_size;

};

// Global operator functions:

/** Buffer concatenation to produce a new MemBuf.
  * This is a global function, not a member function.
  */
inline MemBuf operator+(const MemBuf& lhs, const MemBuf& rhs)
{
	MemBuf ret(lhs);
	ret += rhs();
	return ret;
}

/** String concatenation to produce a new MemBuf.
  * This is a global function, not a member function.
  */
inline MemBuf operator+(const MemBuf& lhs, const char* rhs)
{
	MemBuf ret(lhs);
	ret += rhs;
	return ret;
}

/** String concatenation to produce a new MemBuf.
  * This is a global function, not a member function.
  */
inline MemBuf operator+(const char* lhs, const MemBuf& rhs)
{
	MemBuf ret(lhs);
	ret += rhs();
	return ret;
}

} // End Namespace

#endif // MEMBUF_H Defined
