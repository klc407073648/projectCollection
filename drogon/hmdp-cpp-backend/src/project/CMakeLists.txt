#头文件目录
INCLUDE_DIRECTORIES( ${SERVICE_INCLUDE} ${POCO_INCLUDE} ${YAMLCPP_INCLUDE} ${MYSQL_INCLUDE} /home/drogon/drogon/lib/inc ./include)

#目录源码路径
SET( PROJECT_SRC_DIR ./source )

#代码列表
FILE (GLOB PROJECT_FILES ${PROJECT_SRC_DIR}/*.cpp  ${SERVICE_SRC_DIR}/controllers/*.cc ${SERVICE_SRC_DIR}/common/*.cpp ${SERVICE_SRC_DIR}/constants/*.cpp 
 ${SERVICE_SRC_DIR}/exception/*.cpp  ${SERVICE_SRC_DIR}/models/*.cc  ${SERVICE_SRC_DIR}/services/*.cc ${SERVICE_SRC_DIR}/utils/*.cpp 
 ${SERVICE_SRC_DIR}/dto/*.cpp)
		   
#依赖的LIB
SET(PROJECT_LIB  ${LIB_POCO} ${LIB_YAMLCPP} ${LIB_MYSQL} jsoncpp )

#生成 
ADD_EXECUTABLE( ${PROJECT_NAME} ${PROJECT_FILES} )

#连接生成相关库
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon ${PROJECT_LIB})

if (CMAKE_CXX_STANDARD LESS 17)
    # With C++14, use boost to support any, string_view and filesystem
    message(STATUS "use c++14")
    find_package(Boost 1.61.0 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost ${PROJECT_LIB})
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()