cmake_minimum_required(VERSION 3.5)
project(hmdp CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## 自定义

## 工程库路径
SET(SERVICE_INCLUDE         ${PROJECT_SOURCE_DIR}/hmdp/include)
SET(SERVICE_SRC_DIR         ${PROJECT_SOURCE_DIR}/hmdp/source)

## 第三方库
SET(3PART_INCLUDE           ${PROJECT_SOURCE_DIR}/include/3partlib)
SET(POCO_INCLUDE            ${3PART_INCLUDE}/poco/include)
SET(YAMLCPP_INCLUDE         ${3PART_INCLUDE}/yamlcpp/include)
SET(MYSQL_INCLUDE           ${3PART_INCLUDE}/mysql/include)

# 设置各类应用库的文件路径
SET(3PART_DIR  ${PROJECT_SOURCE_DIR}/lib/3partlib)

# 设置自定义搜索路径
set(CMAKE_INCLUDE_PATH ${3PART_INCLUDE})
set(CMAKE_LIBRARY_PATH ${3PART_DIR})

# 设置第三方库的链接名称
SET(LIB_POCO       
    PocoCrypto
    PocoJSON
    PocoRedis
    PocoEncodings
    PocoJWT
    PocoUtil
    PocoFoundation
    PocoNet
    PocoXML
	PocoDataMySQL
	PocoData
	PocoActiveRecord
	PocoDataSQLite
	PocoNetSSL
	PocoPrometheus
)

SET(LIB_YAMLCPP  yaml-cpp)
SET(LIB_MYSQL mysqlclient)

# 添加外部库的搜索路径
LINK_DIRECTORIES(${3PART_DIR})

## 自定义

SUBDIRS(./project)
#SUBDIRS(./test)