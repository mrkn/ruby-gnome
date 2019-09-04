#!/usr/bin/env ruby
#
# Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "pathname"

source_dir = Pathname(__FILE__).dirname
base_dir = source_dir.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + "lib"
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + "lib"
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gobject_introspection"
package_id = "gobject-introspection-1.0"

require "mkmf-gnome2"

["glib2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

llvm_config = with_config("llvm-config", "llvm-config")
unless find_executable0(llvm_config)
  $stderr.puts "Unable to find llvm-config"
  abort
end

open("|'#{llvm_config}' --version --includedir --libdir --cxxflags --ldflags --libs") do |io|
  LLVM_VERSION = io.gets.chomp

  LLVM_INCLUDEDIR = io.gets.chomp
  $INCFLAGS << " -I#{LLVM_INCLUDEDIR}"

  LLVM_LIBDIR = io.gets.chomp
  $LIBPATH << LLVM_LIBDIR

  LLVM_CXXFLAGS = io.gets.chomp
  $CXXFLAGS << " " << LLVM_CXXFLAGS

  LLVM_LDFLAGS = io.gets.chomp
  $LDFLAGS << " " << LLVM_LDFLAGS

  LLVM_LIBS = io.gets.chomp
  $libs << " " << LLVM_LIBS
end

unless required_pkg_config_package(package_id,
                                   :alt_linux => "gobject-introspection-devel",
                                   :debian => "libgirepository1.0-dev",
                                   :redhat => "gobject-introspection-devel",
                                   :homebrew => "gobject-introspection",
                                   :arch_linux => "gobject-introspection",
                                   :macports => "gobject-introspection",
                                   :msys2 => "gobject-introspection")
  exit(false)
end

# TODO: Remove this when we dropped support for GObject Introspection < 1.60
make_version_header("GI", package_id, ".")

gi_headers = ["girepository.h"]
have_func("g_interface_info_find_signal", gi_headers)

enum_type_prefix = "gobject-introspection-enum-types"
include_paths = PKGConfig.cflags_only_I(package_id)
headers = include_paths.split.inject([]) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "gi{repository,types}.h"))
end
glib_mkenums(enum_type_prefix, headers, "G_TYPE_", ["girepository.h"])

create_pkg_config_file("Ruby/GObjectIntrospection",
                       package_id, ruby_gnome2_version,
                       "ruby-gobject-introspection.pc")

ensure_objs

$defs << "-DRUBY_GOBJECT_INTROSPECTION_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
