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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "MemBuf.h"

#include "AnException.h"
#include "EnEx.h"
#include "AutoXMLChar.h"
#include "memptr.h"

using namespace SLib;

MemBuf::MemBuf() :
	m_data (NULL),
	m_data_size (0)
{
	/* ************************************************************************ */
	/* MemBuf's are used during log processing.  For this reason, do not include */
	/* the ee(FL, ...) version of the EnEx call.                                */
	/* ************************************************************************ */
	EnEx ee("MemBuf::MemBuf()");
}

MemBuf::MemBuf(const MemBuf& t) :
	m_data (NULL),
	m_data_size (0)
{
	EnEx ee("MemBuf::MemBuf(const MemBuf& t)");

	// short circuit for source having nothing in it.
	if(t.m_data_size == 0) {
		return;
	}

	reserve(t.m_data_size);
	m_data_size = t.m_data_size;
	memcpy(m_data, t.m_data, m_data_size);
}

MemBuf::MemBuf(const char* c) :
	m_data (NULL),
	m_data_size (0)
{
	EnEx ee("MemBuf::MemBuf(const char* c)");
	if(c == NULL){
		throw AnException(0, FL, "Input is null.");
	}
	size_t csize = strlen(c);
	reserve(csize);
	memcpy(m_data, c, csize);
	m_data_size = csize;
}

MemBuf::MemBuf(const xmlChar* c) :
	m_data (NULL),
	m_data_size (0)
{
	EnEx ee("MemBuf::MemBuf(const xmlChar* c)");
	if(c == NULL){
		throw AnException(0, FL, "Input is null.");
	}
	size_t csize = strlen((const char*)c);
	reserve(csize);
	memcpy(m_data, (const char*)c, csize);
	m_data_size = csize;
}

MemBuf::MemBuf(const size_t s) :
	m_data (NULL),
	m_data_size (0)
{
	EnEx ee("MemBuf::MemBuf(const size_t s)");
	reserve(s);
	m_data_size = s;
}

MemBuf::MemBuf(const twine& c):
	m_data (NULL),
	m_data_size (0)
{
	EnEx ee("MemBuf::MemBuf(const twine& c)");

	reserve( c.size() );
	memcpy(m_data, c(), c.size());
	m_data_size = c.size();

}

MemBuf::~MemBuf() 
{
	EnEx ee("MemBuf::~MemBuf()");
	if(m_data_size > 0 || m_data != NULL){
		free(m_data);
		m_data_size = 0;
		m_data = NULL;
	}
}

MemBuf& MemBuf::operator=(const MemBuf& t)
{
	EnEx ee("MemBuf::operator=(const MemBuf& t)");
	if(&t == this){
		return *this;
	}

	// Clear out anything that we have.
	clear();

	// short circuit for source having nothing in it.
	if(t.m_data_size == 0){
		return *this;
	}

	reserve(t.m_data_size);
	m_data_size = t.m_data_size;
	memcpy(m_data, t.m_data, m_data_size);
	return *this;
}

MemBuf& MemBuf::operator=(const char* c)
{
	EnEx ee("MemBuf::operator=(const char* c)");
	if(c == NULL){
		throw AnException(0, FL, "MemBuf = NULL not allowed.");
	}
	
	// Clear out anything that we have.
	clear();

	size_t tsize = strlen(c);

	reserve(tsize);
	m_data_size = tsize;
	memcpy(m_data, c, m_data_size);
	return *this;
}

MemBuf& MemBuf::operator=(const xmlChar* c)
{
	EnEx ee("MemBuf::operator=(const xmlChar* c)");

	// Clear out anything that we have.
	clear();

	size_t tsize = strlen((const char*)c);

	reserve(tsize);
	m_data_size = tsize;
	memcpy(m_data, c, m_data_size);
	return *this;
}
	
MemBuf& MemBuf::operator=(const twine& t)
{
	EnEx ee("MemBuf::operator=(const twine& t)");
	
	// Clear out anything that we have.
	clear();

	reserve(t.size());
	m_data_size = t.size();
	memcpy(m_data, t(), m_data_size);
	return *this;
}

MemBuf& MemBuf::operator+=(const MemBuf& t)
{
	EnEx ee("MemBuf::operator+=(const MemBuf& t)");
	append(t);
	return *this;
}

MemBuf& MemBuf::operator+=(const char* c)
{
	EnEx ee("MemBuf::operator+=(const char* c)");
	append(c);
	return *this;
}

MemBuf& MemBuf::operator+=(const xmlChar* c)
{
	EnEx ee("MemBuf::operator+=(const xmlChar* c)");
	append((const char*)c);
	return *this;
}

MemBuf& MemBuf::operator+=(const twine& t)
{
	EnEx ee("MemBuf::operator+=(const size_t i)");
	append(t());
	return *this;
}

const char MemBuf::operator[](size_t i) const
{
	EnEx ee("MemBuf::operator[](size_t i)");
	if( (i < 0) || (i >= m_data_size)){
		throw AnException(0,FL,"MemBuf: Out Of Bounds Access");
	}
	return ((char*)m_data)[i];
}

const char* MemBuf::operator()() const
{
	EnEx ee("MemBuf::operator()()");
	return (char*)m_data;
}

char* MemBuf::data(void)
{
	EnEx ee("MemBuf::data(void)");
	return (char*)m_data;
}

MemBuf& MemBuf::set(const char* c)
{
	EnEx ee("MemBuf::set(const char* c)");
	return operator=(c);
}

MemBuf& MemBuf::set(const char* c, size_t n)
{
	EnEx ee("MemBuf::set(const char* c, size_t n)");

	// Clear out anything that we have
	clear();

	reserve(n);
	memcpy(m_data, c, n);
	m_data_size = n;
	return *this;

}
	
MemBuf& MemBuf::append(const char* c)
{
	EnEx ee("MemBuf::append(const char* c)");
	if(c == NULL){
		return *this; // nothing to append
	}

	size_t oldSize = m_data_size;
	size_t csize = strlen(c);
	reserve(m_data_size + csize);

	memcpy((char*)m_data + oldSize, c, csize);

	return *this;
}

MemBuf& MemBuf::append(const char* c, size_t csize)
{
	EnEx ee("MemBuf::append(const char* c, size_t csize)");
	if(c == NULL || csize == 0){
		return *this; // nothing to append
	}

	size_t oldSize = m_data_size;
	reserve(m_data_size + csize);

	memcpy((char*)m_data + oldSize, c, csize);

	return *this;
}

MemBuf& MemBuf::append(const MemBuf& c)
{
	EnEx ee("MemBuf::append(const MemBuf& c)");
	if(c.size() == 0){
		return *this; // nothing to append
	}

	size_t oldSize = m_data_size;
	reserve(m_data_size + c.size());

	memcpy((char*)m_data + oldSize, c.m_data, c.size());

	return *this;
}

MemBuf& MemBuf::erase(size_t p, size_t n)
{
	EnEx ee("MemBuf::erase(size_t p, size_t n)");

	bounds_check(p);
	bounds_check(p+n-1);

	memset((char*)m_data + p, 0, n);

	return *this;
}

MemBuf& MemBuf::erase(size_t p)
{
	EnEx ee("MemBuf::erase(size_t p)");
	bounds_check(p);

	// 0 1 2 3 4 5 6 7 8 9
	// A B C D E F G H I J
	// m_data_size = 10
	// p = 3
	// count = 7

	memset((char*)m_data + p, 0, m_data_size - p);

	return *this;
}

MemBuf& MemBuf::erase(void)
{
	EnEx ee("MemBuf::erase(void)");
	if(m_data_size == 0){
		return *this; // nothing to do
	}

	memset(m_data, 0, m_data_size);

	return *this;
}

MemBuf& MemBuf::clear(void)
{
	EnEx ee("MemBuf::clear(void)");

	if(m_data_size == 0){
		return *this; // nothing to do
	}
	free(m_data);
	m_data = NULL;
	m_data_size = 0;

	return *this;
}

MemBuf& MemBuf::reserve(size_t min_size) 
{
	EnEx ee("MemBuf::reserve(size_t min_size)");
	if(m_data_size == 0){
		m_data = malloc(min_size + 10);
		if(m_data == NULL){
			throw AnException(0, FL, "MemBuf: Error Allocating Memory");
		}
		m_data_size = min_size;
		memset(m_data, 0, min_size + 10);
		return *this;
	}
	if(min_size < m_data_size) {
		return *this;
	} else {
		char *ptr = (char *)realloc(m_data, min_size+10);
		if(ptr == NULL){
			throw AnException(0, FL, "MemBuf: Error reallocating memory.");
		}
		m_data = ptr;
		size_t oldSize = m_data_size;
		m_data_size = min_size;
		erase( oldSize ); // ensure the new segment of memory is zeroed 

		return *this;
	}
}

size_t MemBuf::size(void) const 
{ 
	EnEx ee("MemBuf::size(void)");
	return m_data_size; 
}

size_t MemBuf::length(void) const 
{ 
	EnEx ee("MemBuf::length(void)");
	return m_data_size; 
}

bool MemBuf::empty(void) const 
{ 
	EnEx ee("MemBuf::empty(void)");
	return (m_data_size == 0); 
}
	
void MemBuf::bounds_check(size_t p) const
{
	EnEx ee("MemBuf::bounds_check(size_t p)");
	if( (p < 0) || (p >= m_data_size)){
		throw AnException(0, FL, "MemBuf: Index out of bounds. p(%d) m_data_size(%d)", p, m_data_size);
	}
}

MemBuf& MemBuf::encode64()
{
	EnEx ee("MemBuf::encode64()");

	// Run the conversion
	size_t len;
	memptr< char > tmpspace = Base64::encode( (char*)m_data, size(), &len);
	if(tmpspace == (char*)NULL){
		throw AnException(0, FL, "Error base64 encoding.");
	}
	
	// Erase what we had in the buffer:
	clear();

	// Re-assign the encoded buffer to ourself:
	m_data = tmpspace.release();
	m_data_size = len;

	return *this;
}

MemBuf& MemBuf::decode64()
{
	EnEx ee("MemBuf::decode64()");

	// Run the conversion
	size_t len;
	memptr< char > tmpspace = Base64::decode( (char*)m_data, size(), &len);
	if(tmpspace == (char*)NULL){
		throw AnException(0, FL, "Error base64 decoding.");
	}

	// Erase what we had in the buffer:
	clear();

	// Re-assign the decoded buffer to ourself:
	m_data = tmpspace.release();
	m_data_size = len;

	return *this;
}

MemBuf& MemBuf::zip()
{
	EnEx ee("MemBuf::zip()");


	return *this;
}

MemBuf& MemBuf::unzip()
{
	EnEx ee("MemBuf::unzip()");


	return *this;
}