# Main build specification for Ruby-VPI.
#
# = Environment variables
# CFLAGS:: Arguments to the compiler.
# LDFLAGS:: Arguments to the linker.

=begin
	Copyright 2006 Suraj N. Kurapati

	This file is part of Ruby-VPI.

	Ruby-VPI is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	Ruby-VPI is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=end

require 'rake/clean'
require 'rake/rdoctask'
require 'tempfile'

task :default => :build


# variables
	require 'rbconfig'

	CFLAGS = "#{Config::CONFIG['CFLAGS']} #{ENV['CFLAGS']} -g -DDEBUG"
	LDFLAGS = "#{Config::CONFIG['LDFLAGS']} #{ENV['LDFLAGS']}"


# extension
	desc 'Builds the Ruby-VPI extension.'
	task :build => 'Makefile' do |t|
		sh "make -f #{t.prerequisites[0]}"
	end

	CLEAN.include 'Makefile', 'mkmf.log', '*.o', '*.so'

	file 'Makefile' => [:swig, 'ext/extconf.rb'] do |t|
		ruby "#{t.prerequisites[1]} --with-cflags='#{CFLAGS}' --with-ldflags='#{LDFLAGS}'"
	end


	desc 'Generate Ruby wrapper for VPI.'
	task :swig => 'ext/swig_wrap.cin'

	file 'ext/swig_wrap.cin' => 'ext/swig_vpi.i' do |t|
		sh "swig -ruby -o #{t.name} #{t.prerequisites[0]}"
	end

	file 'ext/swig_vpi.i' => 'ext/swig_vpi.h'

	file 'ext/swig_vpi.h' => 'ext/vpi_user.h' do |t|
		# avoid problems with SWIG-generated wrapper for VPI vprintf functions which use va_list
		ruby %{-pe 'gsub /\\bva_list\\b/, "int"' #{t.prerequisites[0]} > #{t.name}}
	end

	CLEAN.include 'ext/swig_vpi.h', 'ext/swig_wrap.cin'


# documentation
	desc 'Generate documentation.'
	task 'doc' => 'ref' do |t|
		cd t.name do
			sh 'rake'
		end
	end

	task :clobber do |t|
		cd 'doc' do
			sh 'rake', t.name
		end
	end


	directory 'ref'
	CLOBBER.include 'ref'

	desc 'Generate reference documentation.'
	file 'ref' => ['ref/c', 'ref/ruby']


	directory 'ref/ruby'
	CLOBBER.include 'ref/ruby'

	desc 'Generate reference for Ruby.'
	Rake::RDocTask.new 'ref/ruby' do |t|
		t.rdoc_dir = t.name
		t.title = 'Ruby-VPI: Ruby interface to Verilog VPI'
		t.options.concat %w(--charset utf-8 --tab-width 2 --line-numbers)

		t.rdoc_files.include '**/*.rb'
	end


	directory 'ref/c'
	CLOBBER.include 'ref/c'

	desc 'Generate reference for C.'
	file 'ref/c' do |t|
		# doxygen outputs to this temporary destination
		tempDest = 'ext/html'

		cd File.dirname(tempDest) do
			sh "doxygen"
		end

		mv FileList[tempDest + '/*'].to_a, t.name
		rmdir tempDest
	end


	desc 'Publish documentation to website.'
	task :web => ['HISTORY'] do |t|
		buf = Tempfile.new($$).path

		sh "rdoc1.8 -t 'Ruby-VPI: Ruby interface to Verilog VPI' -1 #{t.prerequisites[0]} | sed '/h2>Classes/,$d' > #{buf}"
		sh "scp #{buf} snk@rubyforge.org:/var/www/gforge-projects/ruby-vpi/history.html"
	end
