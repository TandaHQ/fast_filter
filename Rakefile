require 'bundler/gem_tasks'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

Rake::ExtensionTask.new 'fast_filter' do |ext|
  ext.lib_dir = 'lib/fast_filter'
end
RSpec::Core::RakeTask.new(:spec)

task default: :spec
