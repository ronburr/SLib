#ifndef TWINE_H
#define TWINE_H
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
#       endif
#else
#	define DLLEXPORT 
#endif

const int TWINE_NOT_FOUND = -1;
#ifdef CRLF
#undef CRLF
#endif
#ifdef _WIN32
#	define CRLF "\n"
#else
#	define CRLF "\r\n"
#endif

#include <stdlib.h>
#include <stdarg.h>

#include <vector>
using namespace std;

#include <xmlinc.h>

namespace SLib {

/**
  * @memo This is our version of a string class.
  * @doc  This is our version of a string class.
  *       The twine was developed as a replacement for the std::string 
  *       class.  It has much of the same interface ast the std::string
  *       does (not everything though), and is safer and faster in a
  *       multi-threaded environment.
  *       <P>
  *       This class also has several additions that make it easier to use
  *       than std::string such as format(), and the ability to get a 
  *       writable char pointer to the internal storage of this class.
  *       This means less bouncing through temporaries when interfacing
  *       with libraries that want char pointers.
  *       <P>
  *       This class will also do automatic converstion to/from integers
  *       floats and doubles.  If the conversion does not happen the way
  *       you would like it to, simply use the format() method to get 
  *       exactly what you want.
  *       <P>
  *       Note, this class is not designed for simultaneous access by
  *       multiple threads, but it is designed to be safe for multiple 
  *       instances of this class to be used by different threads.
  *       <P>
  *       The philosophy behind this class is to keep a very simple approach
  *       and use the standard C lib char* routines as much as possible.
  *       These routines are time tested and robust, so why not take advantage
  *       of them.
  */
class DLLEXPORT twine
{
	public:

		/** Standard constructor.
		  */
		twine(); 

		/** copy constructor
		  */
		twine(const twine& t);

		/** constructor from a char*
		  */
		twine(const char* c);

		/** constructor from a const xmlChar*
		  */
		twine(const xmlChar* c);

		/** constructor from a single char
		  */
		twine(const char c);

		/** constructor from an xmlNodePtr attribute
		  */
		twine(const xmlNodePtr node, const char* attrName);

		/** Destructor
		  */
		virtual ~twine();

		/** Assignment operation
		  */
		twine& operator=(const twine& t);

		/** Assignment operation
		  */
		twine& operator=(const twine* t);

		/** Assignment operation
		  */
		twine& operator=(const char* c);

		/** Assignment from integer.  This will use the sprintf
		  * "%d" to convert from the given integer.
		  */
		twine& operator=(const int i);
			
		/** Assignment from float. This will use the sprintf
		  * "%f" to convert from the given float.
		  */
		twine& operator=(const float f);

		/** Concatenation
		  */
		twine& operator+=(const twine& t);

		/** Concatenation
		  */
		twine& operator+=(const twine* t);

		/** Concatenation
		  */
		twine& operator+=(const char* c);

		/** Concatenation
		  */
		twine& operator+=(const char c);

		/** Concatenation
		  */
		twine& operator+=(const int i);

		/** Concatenation
		  */
		twine& operator+=(const float f);

		/** Change to an integer.  This uses atoi().
		  */
		int get_int(void) const;

		/** Change to a float. This uses (float)atof().
		  */
		float get_float(void) const;

		/** Change to a double. This uses atof().
		  */
		double get_double() const;

		/** Get a single char from the twine
		  */
		const char operator[](int i) const;

		/** Get the chars - This is easier to type than c_str()
		  */
		const char* operator()() const;

		/** For compatibility with std::string
		  */
		const char* c_str(void) const;

		/** Compares this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(const twine& t) const;

		/** Compares this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(const char* c) const;

		/** Compares count chars of this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(const twine& t, int count) const;

		/** Compares count chars of this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(const char* c, int count) const;

		/** Compares this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(int i) const;

		/** Compares this twine against the input
		  * Returns:
		  * <ul>
		  *    <li>-1 if this object is less than the input.</li>
		  *    <li> 0 if this object is equal to the input.</li>
		  *    <li>+1 if this object is greater than the input.</li>
		  * </ul>
		  */
		int compare(float f) const;

		/** Compares the beginning of this twine against the input
		 * to determine if this twine starts the same way.
		 */
		bool startsWith(const twine& t){
			return compare(t, t.size()) == 0;
		}

		/** Compares the ending of this twine against the input
		 * to determine if this twine ends the same way.
		 */
		bool endsWith(const twine& t) const;

		/** Gets a writable pointer to the twine memory.
		  * This, as a non const char pointer
		  * allows writing directly to the twine memory.  This
		  * is allowed for efficiency, but the programmer beware.
		  * Using this is just like using a normal char* and you
		  * MUST be careful about over stepping the array bounds.
		  * <P>
		  * Use the reserve() method to ensure that you have enough
		  * memory to start with, and then after writing to the
		  * pointer and BEFORE using any other twine methods, use
		  * the check_size() method to re-set the twine state.
		  * <P>
		  * If you do not use the check_size() method you WILL 
		  * cause program failures.
		  */
		char* data(void);

		/** Forces a recheck of the twine size,  Use this after writes.
		  */
		int check_size(void);

		/** Sets the chars of the twine from the input.
		  */
		twine& set(const char* c);

		/** Sets the chars of the twine from first n chars of the input.
		  */
		twine& set(const char* c, int n);

		/** Gets a substring of the twine from start going count
		  * characters.
		  */
		twine substr(int start, int count) const;

		/** Gets a substring of the twine from start to twine size.
		  */
		twine substr(int start) const;

		/** Gets a substring of the twine from start going count
		  * characters.  Returns as a pointer.
		  */
		twine* substrp(int start, int count) const;

		/** Gets a substring of the twine from start to twine size.
		  * Returns as a pointer.
		  */
		twine* substrp(int start) const;

		/** Sets the string contents from the given format.  This
		  * will use the standard printf formatting rules.
		  */
		twine& format(const char* f, ...);

		/** Sets the string contents from the given format.  This
		  * will use the standard printf formatting rules.
		  */
		twine& format(const char* f, va_list ap);

		/** Searches the twine,  Returns position or TWINE_NOT_FOUND.
		  */
		int find(const char* needle) const;

		/** Searches the twine,  Returns position or TWINE_NOT_FOUND.
		  */
		int find(const char c) const;

		/** Searches the twine,  Returns position or TWINE_NOT_FOUND.
		  */
		int find(const twine& t) const;

		/** Searches the twine starting at position p.
		  */
		int find(const char* needle, int p) const;

		/** Searches the twine starting at position p.
		  */
		int find(const char c, int p) const;

		/** Searches the twine starting at position p.
		  */
		int find(const twine& t, int p) const;

		/** Searches the twine in reverse for the target.
		  */
		int rfind(const char c) const;

		/** Searches the twine in reverse starting at p.
		  */
		int rfind(const char c, int p) const;

		/** Searches the twine in reverse for the target.
		  */
		int rfind(const char* needle) const;

		/** Searches the twine in reverse starting at p.
		  */
		int rfind(const char* needle, int p) const;

		/** Searches the twine in reverse for the target.
		  */
		int rfind(const twine& needle) const;

		/** Searches the twine in reverse starting at p.
		  */
		int rfind(const twine& needle, int p) const;

		/** Counts the number of occurrances of a char in the twine.
		  */
		int countof(const char needle) const;

		/** Replaces a range with the input.
		  */
		twine& replace(int start, int count, const char* rep);

		/** Replaces a range with the input.
		  */
		twine& replace(int start, int count, const twine* t);

		/** Replaces a range with the input.
		  */
		twine& replace(int start, int count, const twine& t);

		/** Replaces all instances of char c with char n
		 */
		twine& replace(const char c, const char n);

		/** Appends a const char* to the end of the twine
		  */
		twine& append(const char* c);
			
		/** Appends a const twine& to the end of the twine
		  */
		twine& append(const twine& t) { return append(t()); }
			
		/** Inserts a const char* into the twine at the given position.
		  */
		twine& insert(int p, const char* c);
			
		/** Erases the range starting at p and going for n chars
		  */
		twine& erase(int p, int n);
			
		/** Erases the range starting at p and going to the end.
		  */
		twine& erase(int p);
			
		/** Erases the whole twine.
		  */
		twine& erase(void);
			
		/** Trims whitespace from the end of the twine.
		  */
		twine& rtrim(void);

		/** Trims whitespace from the beginning of the twine.
		  */
		twine& ltrim(void);

		/** Ensures that the twine size is at least min_size.  If
		  * enough memory can't be allocated for the given size, an
		  * exception will be thrown.  This is the only allocation
		  * method in the twine.  All other methods that potentially
		  * change the size of the twine will use this.  Therefore
		  * the out of memory exception may be seen from other methods.
		  */
		twine& reserve(int min_size);

		/** Returns the length of the twine.
		  */
		int size(void) const;

		/** Returns the length of the twine.
		  */
		int length(void) const;

		/** Returns the max length that the twine could hold now.
		  */
		int max_size(void) const;

		/** Returns the max length that the twine could hold now.
		  */
		int capacity(void) const;

		/** Returns true if the size of the twine is zero.
		  */
		bool empty(void) const;
			
		/**
		  * Uppercases the whole twine.
		  */
		twine& ucase(void);

		/**
		  * Uppercases the character at i in the twine.
		  */
		twine& ucase(int i);
		
		/**
		* Splits the current twine into a vector of twines based
		* on the given split string.
		*/
		vector < twine > split(twine spliton);

		/** Determines if the given twine pointer is null, or if the
		 * twine itself is empty.
		 */
		static bool isNullOrEmpty(const twine* t){
			if(t == NULL) return true;
			return t->empty();
		}

		/** Determines if the given twine pointer is null, or if the
		 * twine itself is empty.
		 */
		static bool isNullOrEmpty(const twine& t){
			return t.empty();
		}

	/* ******************************************************************* */
	/* This group of functions helps to make life easier working with      */
	/* the interfaces of libxml.                                           */
	/* ******************************************************************* */

		/** Assignment operation that will "shift" memory into this
		 * twine class.  We will take ownership of the memory shifted into us
		 * and will ensure that it is properly freed when required.
		  */
		twine& operator<< (xmlChar* c);

		/** Get the chars as xmlChar*
		  */
		operator const xmlChar*() const;

		/** Pick up an attribute from an xmlNodePtr and store it in this twine
		 * for further use.
		 */
		twine& getAttribute(xmlNodePtr node, const char* attrName);

	private:

		/** Checks the index against the bounds of the array:
		  */
		void bounds_check(int p) const;

		/** our representation is a char array:
		  */
		char* m_data;
		
		/** size of allocated memory in m_data:
		  */
		int m_allocated_size;

		/** size of string currently in m_data:
		  */
		int m_data_size;

};

// Global operator functions:

/** String concatenation to produce a new twine.
  * This is a global function, not a member function.
  */
inline twine operator+(const twine& lhs, const twine& rhs)
{
	twine ret(lhs);
	ret += rhs();
	return ret;
}

/** String concatenation to produce a new twine.
  * This is a global function, not a member function.
  */
inline twine operator+(const twine& lhs, const char* rhs)
{
	twine ret(lhs);
	ret += rhs;
	return ret;
}

/** String concatenation to produce a new twine.
  * This is a global function, not a member function.
  */
inline twine operator+(const char* lhs, const twine& rhs)
{
	twine ret(lhs);
	ret += rhs();
	return ret;
}

/** String concatenation to produce a new twine.
  * This is a global function, not a member function.
  */
inline twine operator+(const twine& lhs, const char rhs)
{
	twine ret(lhs);
	ret += rhs;
	return ret;
}

/** String concatenation to produce a new twine.
  * This is a global function, not a member function.
  */
inline twine operator+(const char lhs, const twine& rhs)
{
	twine ret(lhs);
	ret += rhs();
	return ret;
}

/** Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator==(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) == 0;
}

/** Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator==(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) == 0;
}

/** Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator==(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) == 0;
}

/** Non-Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator!=(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) != 0;
}

/** Non-Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator!=(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) != 0;
}

/** Non-Equivalence operation.
  * This is a global function, not a member function.
  */
inline bool operator!=(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) != 0;
}

/** Less Than operation.
  * This is a global function, not a member function.
  */
inline bool operator<(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) < 0;
}

/** Less Than operation.
  * This is a global function, not a member function.
  */
inline bool operator<(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) < 0;
}

/** Less Than operation.
  * This is a global function, not a member function.
  */
inline bool operator<(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) > 0;
}

/** Greater Than operation.
  * This is a global function, not a member function.
  */
inline bool operator>(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) > 0;
}

/** Greater Than operation.
  * This is a global function, not a member function.
  */
inline bool operator>(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) > 0;
}

/** Greater Than operation.
  * This is a global function, not a member function.
  */
inline bool operator>(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) < 0;
}

/** Less Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator<=(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) <= 0;
}

/** Less Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator<=(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) <= 0;
}

/** Less Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator<=(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) >= 0;
}

/** Greater Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator>=(const twine& lhs, const twine& rhs)
{
	return lhs.compare(rhs()) >= 0;
}

/** Greater Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator>=(const twine& lhs, const char* rhs)
{
	return lhs.compare(rhs) >= 0;
}

/** Greater Than or Equals operation.
  * This is a global function, not a member function.
  */
inline bool operator>=(const char* lhs, const twine& rhs)
{
	return rhs.compare(lhs) <= 0;
}


} // End Namespace

#endif // TWINE_H Defined
