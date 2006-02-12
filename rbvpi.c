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

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "rbvpi.h"



/* relay logic, used to transfer control between Ruby and Verilog */
	static pthread_t ruby_tid;
	static pthread_mutex_t ruby_lock;
	static pthread_mutex_t veri_lock;

	/**
		Initialize the relay mechanism, which enables Verilog code to transfer control to Ruby code and vice versa.
	*/
	static void relay_init() {
		pthread_mutex_init(&ruby_lock, NULL);
		pthread_mutex_lock(&ruby_lock);
		pthread_mutex_init(&veri_lock, NULL);
		pthread_mutex_lock(&veri_lock);
	}

	/**
		Transfers control to Ruby code.
	*/
	static void relay_ruby(){
		pthread_mutex_unlock(&ruby_lock);
		pthread_mutex_lock(&veri_lock);
	}

	/**
		Transfers control to Verilog code.
	*/
	static void relay_verilog(){
		pthread_mutex_unlock(&veri_lock);
		pthread_mutex_lock(&ruby_lock);
	}

	/**
		Starts the Ruby interpreter.
	*/
	static void* ruby_run_handshake(void* dummy) {
		pthread_mutex_lock(&ruby_lock); /* block */
		ruby_run();
		return NULL;
	}



/* VPI module, used by Ruby code */
	void Init_vpi() {
		VALUE mVPI = rb_define_module("VPI");
		rb_define_singleton_method(mVPI, "relay_verilog", rbvpi_relay_verilog, 0);
	}

	static VALUE rbvpi_relay_verilog(VALUE self) {
		relay_verilog();
		return Qnil;
	}



/* VPI callbacks, used by C and Verilog code */
	/**
		Enables Verilog code to transfer control to Ruby code.
	*/
	int rbvpi_callback(PLI_BYTE8* dummy) {
/*		vpiHandle
			operandy_handle = vpi_handle_by_name("my_div32.operandy", NULL)
			, operand1_handle = vpi_handle_by_name("my_div32.operand1", NULL)
			, operand2_handle = vpi_handle_by_name("my_div32.operand2", NULL)
			, signed_op_handle = vpi_handle_by_name("my_div32.signed_op", NULL)
			, status_handle = vpi_handle_by_name("my_div32.status", NULL)
			, result_handle = vpi_handle_by_name("my_div32.result", NULL)
			;

		s_vpi_value val;


		vpi_printf("@@@rbvpi: object my_div32.operandy is named %s\n", vpi_get_str(vpiName, operandy_handle));


		// set the outputs from Verilog as Ruby globals
		val.format = vpiIntVal;

		vpi_get_value(operandy_handle, &val);
		g_operandy = INT2NUM(val.value.integer);

		vpi_get_value(operand1_handle, &val);
		g_operand1 = INT2NUM(val.value.integer);

		vpi_get_value(operand2_handle, &val);
		g_operand2 = INT2NUM(val.value.integer);

		vpi_get_value(signed_op_handle, &val);
		g_signed_op = val.value.integer ? Qtrue : Qfalse;

		vpi_get_value(status_handle, &val);
		g_status = INT2NUM(val.value.integer);

		vpi_get_value(result_handle, &val);
		g_result = INT2NUM(val.value.integer);

		// NOTE: lesson learned: wires in the module's interface are vpiIntVal objects in VPI!
		// NOTE: do *NOT* use vpi_get() to get the value of a wire.. it does NOT work. Instead, use vpi_get_value(), with format = vpiIntVal

		vpi_printf("@@@rbvpi: format of operandy's value: %d\n", val.format);
		vpi_printf("@@@rbvpi: value of operandy's value: %d\n", val.value);
		vpi_printf("@@@rbvpi: value of operandy's value, as integer: %d\n", val.value.integer);
		// vpi_printf("@@@rbvpi: vpi_get(vpiVectorVal) of operandy's value: %d\n", vpi_get(vpiVectorVal, operandy_handle));
		// vpi_printf("@@@rbvpi: vpi_get(vpiIntVal) of operandy's value: %d\n", vpi_get(vpiIntVal, operandy_handle));


		// g_operandy = INT2NUM(vpi_get(vpiIntVal, operandy_handle));
		// g_operand1 = INT2NUM(vpi_get(vpiIntVal, operand1_handle));
		// g_operand2 = INT2NUM(vpi_get(vpiIntVal, operand2_handle));
		// g_signed_op = INT2NUM(vpi_get(vpiIntVal, signed_op_handle));
		// g_status = INT2NUM(vpi_get(vpiIntVal, status_handle));
		// g_result = INT2NUM(vpi_get(vpiIntVal, result_handle));



		// invoke Ruby code
		puts("-> ruby");
		relay_ruby(); // wait for relay_verilog();


		// set the inputs to Verilog from Ruby globals
		val.format = vpiIntVal;

		val.value.integer = NUM2LONG(g_operandy);
		// vpi_printf("@@@rbvpi: operandy = %d, $operandy = long %ld : int %d\n", val.value.integer, NUM2LONG(g_operandy), NUM2INT(g_operandy));
		vpi_put_value(operandy_handle, &val, NULL, vpiNoDelay);

		val.value.integer = NUM2LONG(g_operand1);
		vpi_put_value(operand1_handle, &val, NULL, vpiNoDelay);

		val.value.integer = NUM2LONG(g_operand2);
		vpi_put_value(operand2_handle, &val, NULL, vpiNoDelay);

		val.value.integer = RTEST(g_signed_op);
		vpi_put_value(signed_op_handle, &val, NULL, vpiNoDelay);
*/

		// invoke Ruby code
		puts("-> ruby");
		relay_ruby(); // wait for relay_verilog();


		// transfer control to verilog
		puts("-> verilog");
		return 0;
	}

	/**
		Initializes and starts the Ruby interpreter.
	*/
	static int rbvpi_init(PLI_BYTE8* dummy) {
		relay_init();


		// initialize Ruby interpreter
		ruby_init();
		ruby_init_loadpath();


		// transform the arguments passed to this function by Verilog into arguments for ruby interpeter
		vpiHandle myCall = vpi_handle(vpiSysTfCall, NULL);

		if(myCall) {
			// push all call-arguments into a ruby array, so that I don't have to perform any explicit memory management
			VALUE argAry = rb_ary_new();

			vpiHandle callArgs = vpi_iterate(vpiArgument, myCall);

			if(callArgs) {
				vpiHandle arg;

				while((arg = vpi_scan(callArgs)) != NULL) {
					s_vpi_value argVal;
					argVal.format = vpiObjTypeVal;

					vpi_get_value(arg, &argVal);

					/**(Page 717 in IEEE Std 1364-2001 Version C.)
						When the format ﬁeld is vpiObjTypeVal, the routine shall ﬁll in the value and change the format ﬁeld based on the object type, as follows:
							— For an integer, vpiIntVal
							— For a real, vpiRealVal
							— For a scalar, either vpiScalar or vpiStrength
							— For a time variable, vpiTimeVal with vpiSimTime
							— For a vector, vpiVectorVal
					*/
					switch(argVal.format) {
						/*case vpiIntVal:
							break;

						case vpiRealVal:
							break;

						case vpiScalar:
						case vpiStrength:
							break;

						case vpiTimeVal:
							break;

						case vpiVectorVal:
							break;*/

						case vpiStringVal:
							vpi_printf("ruby-vpi: in $ruby_init(), got vpiStringVal: %s\n", argVal.value.str);

							rb_ary_push(argAry, rb_str_new2(argVal.value.str));
						break;
					}
				}
			}


			// convert the ruby array into an array of C strings
			long argCnt = RARRAY(argAry)->len;
			VALUE* argSrc = RARRAY(argAry)->ptr;
			PLI_BYTE8** argDst = ALLOC_N(PLI_BYTE8*, argCnt);

			long i;
			for(i = 0; i < argCnt; i++) {
				argDst[i] = (PLI_BYTE8*)StringValueCStr(argSrc[i]);
			}

			ruby_options(argCnt, argDst);

			// TODO: check if there is any memory leak here
			free(argDst);
		}


		// create VPI infrastructure for Ruby code
		Init_vpi();
		// TODO: add full VPI support


		// start Ruby interpreter
		// the Ruby code will now bind any additional callbacks via the VPI infrastructure, and relay back to the verilog so that the simulation can begin
		pthread_create(&ruby_tid, 0, ruby_run_handshake, 0);


		return 0;
	}

	static void rbvpi_bind_task(PLI_BYTE8* name, int (*func)(PLI_BYTE8*)) {
		s_vpi_systf_data tf;

		tf.type = vpiSysTask;
		tf.sysfunctype = 0;
		tf.tfname = name;
		tf.calltf = func;
		tf.compiletf = NULL;
		tf.sizetf = NULL;
		tf.user_data = NULL;

		vpi_register_systf(&tf);
	}

	static void rbvpi_startup() {
		rbvpi_bind_task("$ruby_init", rbvpi_init);
		rbvpi_bind_task("$ruby_callback", rbvpi_callback);


		/*s_cb_data cb;

		cb.reason = cbValueChange;
		cb.cb_rtn = ;
		cb.obj = vpi_handle("my_div32.start", NULL);
		cb.time = vpiSuppressTime;
		cb.value = ;
		cb.index = ;
		cb.user_data = ;

		vpi_register_cb(&cb);*/
	}

#ifndef VCS
	void (*vlog_startup_routines[])() = { rbvpi_startup, 0 };
#endif
