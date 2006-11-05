/*
  Copyright 2006 Suraj N. Kurapati

  This file is part of Ruby-VPI.

  Ruby-VPI is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  Ruby-VPI is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Ruby-VPI; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
/**\file
  Interface between C and Verilog code.
*/

#ifndef VLOG_H
#define VLOG_H

  #include "verilog.h"


  /**
    Callback for the $ruby_relay VPI task. Transfers control from Verilog to Ruby code.
  */
  verilog_tf_funcSig(vlog_ruby_relay);

  /**
    Callback for the $ruby_init VPI task. Initializes and starts the Ruby interpreter with the same arguments as those given to the VPI task-call.

    For example, to start the Ruby interpreter with the "-w" option and with the "hello_world.rb" script, the Verilog code would have a statement like this:
      - <tt>$ruby_init("-w", "hello_world.rb");</tt>
  */
  verilog_tf_funcSig(vlog_ruby_init);

  /**
    Binds a C function to a VPI task, so that Verilog code can invoke the C function by calling the VPI task.

    For example, if we bound a VPI task named "$hello_world" to the C function hello_world(), then the hello_world() function would be invoked whenever Verilog code called the "$hello_world" VPI task.

    @param	apTaskName	Name of the VPI task. For example, "$hello_world".

    @param	apTaskDef	The C function which you want to associate with the given VPI task. For example, hello_world().
  */
  void vlog_bind_task(PLI_BYTE8* apTaskName, verilog_tf_funcPtr(apTaskDef));

#endif