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

#include "LogMsg.h"
using namespace SLib;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static twine staticAppName;
static twine staticMachineName;

LogMsg::LogMsg()
{
	tid = CURRENT_THREAD_ID;
	SetTimestamp();
	SetAppMachine();

	id = 0;
	line = 0;
	channel = 0;
	appName = staticAppName;
	machineName = staticMachineName;
	msg_static = false;
}

LogMsg::LogMsg(const char* f, int l, twine& m)
{
	tid = CURRENT_THREAD_ID;
	SetTimestamp();
	SetAppMachine();

	id = 0;
	channel = 0;
	file = f;
	line = l;
	msg = m;
	appName = staticAppName;
	machineName = staticMachineName;
	msg_static = false;
}

LogMsg::LogMsg(const char* f, int l)
{
	tid = CURRENT_THREAD_ID;
	SetTimestamp();
	SetAppMachine();

	id = 0;
	channel = 0;
	file = f;
	line = l;
	appName = staticAppName;
	machineName = staticMachineName;
	msg_static = false;
}

LogMsg::LogMsg(const LogMsg& c)
{
	id = c.id;
	tid = c.tid;
	timestamp = c.timestamp;
	channel = c.channel;
	file = c.file;
	line = c.line;
	appName = c.appName;
	machineName = c.machineName;
	msg = c.msg;
	msg_static = c.msg_static;
}

LogMsg& LogMsg::operator=(const LogMsg& c)
{
	id = c.id;
	tid = c.tid;
	timestamp = c.timestamp;
	channel = c.channel;
	file = c.file;
	line = c.line;
	appName = c.appName;
	machineName = c.machineName;
	msg = c.msg;
	msg_static = c.msg_static;
	return *this;
}

LogMsg::~LogMsg()
{
	// nothing at the moment
}

void LogMsg::SetTimestamp(void)
{
#ifdef _WIN32
	ftime(&timestamp);
#else
	gettimeofday(&timestamp, NULL);
#endif
}

void LogMsg::SetAppMachine()
{
	// Only do this once and store it in the static variable so that
	// we pay the price only once, and then have it stored after that.
	if(staticMachineName.length() == 0){
#ifdef _WIN32
		staticMachineName.reserve(512);
		DWORD length = 512;
		GetComputerName(staticMachineName.data(), &length);
		staticMachineName.check_size();
#else
		staticMachineName.reserve(512);
		int length = 512;
		gethostname(staticMachineName.data(), length);
		staticMachineName.check_size();
#endif
	}

	if(staticAppName.length() == 0){
#ifdef _WIN32
		staticAppName.reserve(1024);
		DWORD length = 1024;
		GetModuleFileName(NULL, staticAppName.data(), length);
		staticAppName.check_size();
#else
		staticAppName.reserve(1024);
		// See this: http://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe/1024937#1024937
		readlink("/proc/self/exe", staticAppName.data(), 1024); // Linux
		readlink("/proc/curproc/file", staticAppName.data(), 1024); // FreeBSD
		readlink("/proc/self/path/a.out", staticAppName.data(), 1024); // Solaris
		_NSGetExecutablePath(); // Mac OS X
		getexecname(); // Solaris
#endif
	}

}

twine LogMsg::GetTimestamp(void)
{
	char local_tmp[32];
	memset(local_tmp, 0, 32);
	twine ret;

#ifdef _WIN32
	strftime(local_tmp, 32, "%Y/%m/%d %H:%M:%S", localtime(&(timestamp.time)));
	ret.format("%s.%.3d", local_tmp, (int)timestamp.millitm);
#else
	strftime(local_tmp, 32, "%Y/%m/%d %H:%M:%S", localtime(&(timestamp.tv_sec)));
	ret.format("%s.%.3d", local_tmp, (int)timestamp.tv_usec);
#endif

	return ret;
}
