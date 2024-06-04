cmake_minimum_required(VERSION 3.27)
project(ft_irc CXX)

set(CMAKE_CXX_STANDARD 98)

file(GLOB ft_irc_src CONFIGURE_DEPENDS "files/srcs/*/*.cpp" "files/srcs/*.cpp" "files/includes/*.hpp")

add_executable(ft_irc ${ft_irc_src})
