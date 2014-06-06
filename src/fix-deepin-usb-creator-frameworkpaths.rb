#!/usr/bin/ruby

libs = `otool -L ./deepin-usb-creator.app/Contents/MacOS/deepin-usb-creator`.split("\n")
libs = libs[1..libs.length]
libs = libs.map { |x| x.split()[0] }
libs = libs.reject { |x| x[0..0] == '/' || x[0..0] == '@' }
libs.each { |x| `install_name_tool -change #{x} @executable_path/../Frameworks/#{x} ./deepin-usb-creator.app/Contents/MacOS/deepin-usb-creator` }
