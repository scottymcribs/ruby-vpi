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

/**
	An up-counter with synchronous reset.

	@param	Size	The number of bits to use in representing the counter's value.
	@param	clock	The clocking signal.
	@param	reset	Must be asserted for two clock cycles in order to reset this counter.
	@param	count	The value of this counter.
*/
module counter #(parameter Size = 5) (
	input clock
	, input reset
	, output reg [Size - 1:0] count
);

	always @(posedge clock) begin
		if (reset)
			count <= 0;
		else
			count <= count + 1;
	end
endmodule
