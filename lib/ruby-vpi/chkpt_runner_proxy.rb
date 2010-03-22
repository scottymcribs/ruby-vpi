# A template to simplify running multiple tests for an examples.
# This file is meant to be embedded in another Rakefile.
#--
# Copyright 2006 Suraj N. Kurapati
# See the file named LICENSE for details.

# supress errors about nonexistent tasks
task :default

ARGV.each do |t|
  task t
end

# invoke each test runner with the command-line args
at_exit do
  # First argument assumed to be the simulator
  ARGV[1..ARGV.size].each do |runner|
    sh 'rake', '-f', runner, ARGV[0]
  end
end
