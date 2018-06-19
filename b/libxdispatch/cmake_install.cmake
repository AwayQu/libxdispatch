# Install script for directory: /Users/away/code/github/testxdispatch/libxdispatch

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibxdispatchx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Library/Frameworks" TYPE DIRECTORY FILES "/Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "xdispatch.framework/Versions/0.8.0~devel/xdispatch"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Library/Frameworks" TYPE DIRECTORY FILES "/Users/away/code/github/testxdispatch/b/bin/Release/xdispatch.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "xdispatch.framework/Versions/0.8.0~devel/xdispatch"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Library/Frameworks" TYPE DIRECTORY FILES "/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "xdispatch.framework/Versions/0.8.0~devel/xdispatch"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Library/Frameworks" TYPE DIRECTORY FILES "/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "xdispatch.framework/Versions/0.8.0~devel/xdispatch"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Library/Frameworks/xdispatch.framework/Versions/0.8.0~devel/xdispatch")
      endif()
    endif()
  endif()
endif()

