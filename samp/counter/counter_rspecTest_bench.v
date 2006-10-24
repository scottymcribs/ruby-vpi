/* This is the Verilog side of the bench. */

`include "counter.v"

module counter_rspecTest_bench;

  // instantiate the design under test
    parameter Size = 5;
    reg  clock;
    reg  reset;
    wire [Size - 1 : 0] count;

    counter #(.Size(Size)) counter_rspecTest_bench_design(.clock(clock), .reset(reset), .count(count));

  // connect to the Ruby side of this bench
    initial begin
      clock = 0;
      $ruby_init("ruby", "-w", "-rubygems", "counter_rspecTest_bench.rb", "-f", "s");
    end

    always begin
      #5 clock = ~clock;
    end

    always @(posedge clock) begin
      #1 $ruby_relay;
    end

endmodule
