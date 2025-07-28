target_include_directories(${PRJ} PUBLIC .)

set(EMBFMT_CPU_PARAMS ${CPU_PARAMS})
add_subdirectory(embfmt)
target_link_libraries(${CMAKE_PROJECT_NAME} embfmt)

set(LWIP_DIR ${MCUXSDK_LWIP})
get_property(include_dirs TARGET ${CMAKE_PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)

set(FLEXPTP_NSD "LWIP")
set(FLEXPTP_SERVO "KALMAN")
set(FLEXPTP_INCLUDES ${include_dirs} ${LWIP_DIR}/src/include)
set(FLEXPTP_CPU_PARAMS ${CPU_PARAMS})
set(FLEXPTP_COMPILE_DEFS ${SYMBOLS})
set(FLEXPTP_HWPORT_SRC ${CMAKE_CURRENT_LIST_DIR}/flexPTP_port/ptp_port_mk64f_lwip.c)
set(FLEXPTP_DONT_LINK_NETWORK_LIB 1)
add_subdirectory(flexPTP)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${FLEXPTP_INCLUDE_EXPORT}) 
target_link_libraries(${CMAKE_PROJECT_NAME} flexptp)