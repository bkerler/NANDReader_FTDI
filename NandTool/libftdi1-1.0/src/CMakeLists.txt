# Includes
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     )

# Version information
set(SNAPSHOT_VERSION "unknown")
execute_process(COMMAND git describe
                OUTPUT_VARIABLE GIT_DESCRIBE_OUTPUT
                RESULT_VARIABLE GIT_DESCRIBE_RESULT
                OUTPUT_STRIP_TRAILING_WHITESPACE
                )
if(${GIT_DESCRIBE_RESULT} STREQUAL 0)
    set(SNAPSHOT_VERSION ${GIT_DESCRIBE_OUTPUT})
endif(${GIT_DESCRIBE_RESULT} STREQUAL 0)
message(STATUS "Detected git snapshot version: ${SNAPSHOT_VERSION}")

configure_file(ftdi_version_i.h.in "${CMAKE_CURRENT_BINARY_DIR}/ftdi_version_i.h" @ONLY)

# Targets
set(c_sources     ${CMAKE_CURRENT_SOURCE_DIR}/ftdi.c ${CMAKE_CURRENT_SOURCE_DIR}/ftdi_stream.c CACHE INTERNAL "List of c sources" )
set(c_headers     ${CMAKE_CURRENT_SOURCE_DIR}/ftdi.h CACHE INTERNAL "List of c headers" )

add_library(ftdi1 SHARED ${c_sources})

math(EXPR VERSION_FIXUP "${MAJOR_VERSION} + 1")    # Compatiblity with previous releases
set_target_properties(ftdi1 PROPERTIES VERSION ${VERSION_FIXUP}.${MINOR_VERSION}.0 SOVERSION 2)

# Static library
add_library(ftdi1-static STATIC ${c_sources})
set_target_properties(ftdi1-static PROPERTIES OUTPUT_NAME "ftdi1")

# Prevent clobbering each other during the build
set_target_properties(ftdi1 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(ftdi1-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# Dependencies
target_link_libraries(ftdi1 ${LIBUSB_LIBRARIES})

# Install
if(${UNIX})

   install( TARGETS ftdi1
            LIBRARY DESTINATION lib${LIB_SUFFIX}
            COMPONENT sharedlibs
            )

   install( TARGETS ftdi1-static
            ARCHIVE DESTINATION lib${LIB_SUFFIX}
            COMPONENT staticlibs
            )

   install( FILES ${c_headers}
            DESTINATION include/${PROJECT_NAME}
            COMPONENT headers
            )

endif(${UNIX})

if(${WIN32})

   install( TARGETS ftdi1
            DESTINATION bin
            COMPONENT sharedlibs
            )

   install( TARGETS ftdi1-static
            DESTINATION bin
            COMPONENT staticlibs
            )

   install( FILES ${c_headers}
            DESTINATION include/${PROJECT_NAME}
            COMPONENT headers
            )

endif(${WIN32})
