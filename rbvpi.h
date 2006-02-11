/*
	Copyright 2006 Suraj Kurapati
	Copyright 1999 Kazuhiro HIWADA

	This file is part of Ruby-VPI.

	Ruby-VPI is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Ruby-VPI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	 02110-1301	 USA
*/

#ifndef RBVPI_H
#define RBVPI_H

#include <ruby.h>



/* VPI module, used by Ruby code */
	/**
		Defines the VPI module, used by Ruby code.
	*/
	void Init_vpi();

	/**
		Transfers control from Ruby code to Verilog code.
	*/
	static VALUE rbvpi_relay_verilog(VALUE self);



/* VPI tasks & callbacks, used by C and Verilog code */
	/**
		Binds a C function to a VPI task, so that Verilog code can invoke the C function via the VPI task.

		For example, if we bound a VPI task named "$hello_world" to the C function ruby_world(), then the ruby_world() function would be invoked whenever Verilog code called the "$hello_world" VPI task.

		@param	name	Name of the VPI task. For example, "$hello_world".

		@param	func	The C function which you want to associate with the given VPI task. For example, ruby_world().
	*/
	static void rbvpi_bind_task(char* name, int (*func)(char*));

	// TODO
	// static void rbvpi_bind_callback();

#endif
