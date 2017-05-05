# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'fast_filter/version'

Gem::Specification.new do |spec|
  spec.name          = 'fast_filter'
  spec.version       = FastFilter::VERSION
  spec.authors       = ['Dave Allie']
  spec.email         = ['dave@tanda.co']

  spec.summary       = 'Native Ruby extension to filter a list quickly.'
  spec.description   = 'Native Ruby extension to filter a list quickly.'
  spec.homepage      = 'https://github.com/TandaHQ/fast_filter'
  spec.license       = 'MIT'

  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.extensions    = ['ext/fast_filter/extconf.rb']

  spec.add_development_dependency 'bundler', '~> 1.12'
  spec.add_development_dependency 'rake', '~> 10.0'
  spec.add_development_dependency 'rake-compiler', '~> 1.0'
  spec.add_development_dependency 'rspec', '~> 3.0'
end
