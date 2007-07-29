# = Environment variables
# CFLAGS:: Arguments to the compiler.
# LDFLAGS:: Arguments to the linker.
#--
# Copyright 2006 Suraj N. Kurapati
# See the file named LICENSE for details.

require 'rake/clean'
require 'rake/rdoctask'
require 'rake/gempackagetask'

require 'tempfile'
require 'rbconfig'

PROJECT_LIBS = File.join(File.dirname(__FILE__), 'lib')

$:.unshift PROJECT_LIBS
require 'ruby-vpi'
require 'ruby-vpi/rake'

task :default => :build


# load project information
  include RubyVPI
  PROJECT_SSH_URL  = "snk@rubyforge.org:/var/www/gforge-projects/#{PROJECT_ID}"

  load 'doc/history.rb'
  head             = @history.first
  PROJECT_VERSION  = head['Version']
  PROJECT_BIRTHDAY = head['Date']


# utility

  # Returns a temporary, unique path ready for
  # use. No file exists at the returned path.
  def generate_temp_path
    path = Tempfile.new($$).path
    rm_f path
    path
  end

  # uploads the given sources to the given destination URL
  def upload aDestUrl, *aSources
    sh 'rsync', '-avz', '--delete', aSources, aDestUrl
  end

  # propogate cleaning tasks recursively to lower levels
  %w[clean clobber].each do |t|
    task t do
      files = FileList['**/Rakefile'].exclude('_darcs')
      files.shift # avoid infinite loop on _this_ file

      # allows propogation to lower levels when gem not installed
      ENV['RUBYLIB'] = PROJECT_LIBS

      files.each do |f|
        cd File.dirname(f) do
          sh 'rake', t
        end
      end
    end
  end


# extension

  desc "Builds object files for all simulators."
  task :build

  directory 'obj'
  CLOBBER.include 'obj'

  ccFlags = ENV['CFLAGS']
  ldFlags = ENV['LDFLAGS']

  SIMULATORS.each_pair do |id, sim|
    taskName = "build_#{id}"

    desc "Builds object files for #{sim.name}."
    task taskName => ['obj', 'ext'] do
      src = PROJECT_ID + '.' + Config::CONFIG['DLEXT']
      dst = File.expand_path(File.join('obj', "#{id}.so"))

      unless File.exist? dst
        cd 'ext' do
          ENV['CFLAGS']  = [ccFlags, sim.compiler_args].compact.join(' ')
          ENV['LDFLAGS'] = [ldFlags, sim.linker_args].compact.join(' ')

          sh "rake SIMULATOR=#{id}"
          mv src, dst
          sh 'rake clean'
        end
      end
    end

    task :build => taskName
  end


# documentation

  desc 'Generate user documentation.'
  task :doc do |t|
    cd t.name do
      sh 'rake'
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
    t.title    = "#{PROJECT_NAME}: #{PROJECT_SUMMARY}"
    t.options.concat %w(--charset utf-8 --line-numbers)
    t.rdoc_files.include '{bin,lib/**}/*.rb'
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


# distribution

  desc "Prepare for distribution."
  task :dist => ['ext', 'ref', :doc] do |t|
    cd 'ext' do
      sh 'rake swig'
    end
  end

  desc 'Publish documentation to website.'
  task :web => [:web_ref, :web_doc]

  desc "Publish reference documentation."
  task :web_ref => 'ref' do |t|
    upload PROJECT_SSH_URL, *t.prerequisites
  end

  desc "Publish user documentation."
  task :web_doc => 'doc' do |t|
    upload PROJECT_SSH_URL, *t.prerequisites
  end

  desc 'Connect to website FTP.'
  task :ftp do
    sh 'lftp', "sftp://#{PROJECT_SSH_URL}"
  end

  desc 'Generate release announcement.'
  task :ann => 'doc/history.rb' do |t|
    require t.prerequisites[0]

    $: << File.join(File.dirname(__FILE__), 'doc', 'lib')
    require 'doc_proxy'

    text = [
      PROJECT_DETAIL,
      "* See #{PROJECT_URL} for details.",
      "---",
      @history.first
    ].join "\n\n"

    IO.popen('w3m -T text/html -dump -cols 60', 'w+') do |pipe|
      pipe.write text.to_html
      pipe.close_write
      puts pipe.read
    end
  end


# packaging

  desc "Generate release packages."
  task :release => [:clobber, :dist] do
    sh 'rake package'
  end

  spec = Gem::Specification.new do |s|
    s.name              = PROJECT_ID
    s.rubyforge_project = PROJECT_ID
    s.summary           = PROJECT_SUMMARY
    s.description       = PROJECT_DETAIL
    s.homepage          = PROJECT_URL
    s.version           = PROJECT_VERSION

    s.add_dependency 'rake',       '>= 0.7.0'
    s.add_dependency 'rspec',      '>= 1.0.0'
    s.add_dependency 'rcov',       '>= 0.7.0'
    s.add_dependency 'xx'           # needed by rcov
    s.add_dependency 'ruby-debug', '>= 0.5.2'

    s.requirements << "POSIX threads library"
    s.requirements << "C language compiler"

    s.files       = FileList['**/*'].exclude('_darcs')
    s.autorequire = PROJECT_ID
    s.extensions << 'gem_extconf.rb'
    s.executables = PROJECT_ID
  end

  Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_tar = true
  end


# installation

  desc "Configures the gem during installation."
  task :gem_config_inst do |t|
    # make documentation available to gem_server
      gemDir  = File.dirname(__FILE__)
      gemName = File.basename(gemDir)
      docDir  = File.join('..', '..', 'doc', gemName)

      mkdir_p docDir
      ln_s gemDir, File.join(docDir, 'rdoc')
  end


# testing

  desc "Ensure that examples work with $SIMULATOR"
  task :test => :build do
    # ensures that current sources are tested instead of the installed gem
    ENV['RUBYLIB'] = PROJECT_LIBS

    FileList['samp/**/*.rake'].each do |runner|
      cd File.dirname(runner) do
        sh 'rake', '-f', File.basename(runner), ENV['SIMULATOR'] || 'cver'
      end
    end
  end
