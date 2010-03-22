# Path to a Ruby script that will be invoked before the Verilog
# simulation begins.  This script shall then load the appropriate
# tests to exercise the desired designs in the Verilog simulation.
TEST_LOADER = <%= aOutputInfo.loaderPath.inspect %>

chkpt_dir = "/home/rob/chkpt"
common_dir = File.join(chkpt_dir, "common/rtl/*.v")
analog_dir = File.join(chkpt_dir, "analog/bw_clk/rtl/*.v")

include_dir = File.join(chkpt_dir, "include")
include_flag = '+incdir+' + include_dir

# Include the standard defines
define_file = File.join(include_dir, "xst_defines.h")
defcontents = File.new(define_file, "r").read
defines = []
defcontents.each_line { |line|
  line.chop
  
  if line =~ /`define\s+(\w+)(\s+(.+))?/
    if $2
      defines << "#{$1}=\"#{$2}\""
    else
      defines << "#{$1}"
    end
  end
}

define_flag = ""
if !defines.empty?
  define_flag = '+define+' + defines.join("+")
end
# Array of paths and shell globs (see the Dir.glob method's documentation for
# details) to source files and directories that contain source files.  These
# source files will be loaded by the simulator before the simulation begins.
sim_sources = FileList[
  '<%= fileName %>',
  "#{chkpt_dir}/u1.v"
]
sim_sources.concat(Dir.glob(common_dir))
sim_sources.concat(Dir.glob(analog_dir))

SIMULATOR_SOURCES = sim_sources

# Command-line arguments for the simulator.  These arguments can be
# specified as a string or an array of strings, as demonstrated below:
#
#   :cver => "this is a single string argument",
#   :cver => ["these", "are", "separate", "arguments"],
#   :cver => %w[these are also separate arguments],
#
SIMULATOR_ARGUMENTS = {
  :cver => [include_flag, define_flag, "-q"],
#<% RubyVPI::SIMULATORS.each do |sim| %>
#  # <%= sim.name %>
#  :<%= sim.id %> => "<%= aModuleInfo.name if sim.id == :vsim %>",
#
#<% end %>
  
}

# This task is invoked before the simulator runs.  It
# can be used to make preprations, such as converting
# Verilog header files into Ruby, for the simulation.
task :setup do
  # To learn how to write Rake tasks, please see:
  # http://docs.rubyrake.org/read/chapter/4#page16
end

# This command loads the Ruby-VPI runner template,
# which starts the simulator with the above parameters.
require 'ruby-vpi/runner'
