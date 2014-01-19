
find_package ( SWIG )
include(UseSWIG)
find_package ( PythonLibs ) 
find_package ( PythonInterp ) 

option(PYTHON_BINDINGS "Build python bindings via swig" ON)

if(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/src)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
    LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../src)

    swig_add_module ( ftdi1 python ftdi1.i )
    swig_link_libraries ( ftdi1 ftdi1 )

    # do not link python modules on debian
    # http://www.debian.org/doc/packaging-manuals/python-policy/ch-module_packages.html
    if ( NOT UNIX OR ( UNIX AND NOT EXISTS "/etc/debian_version" ) )
      swig_link_libraries ( ftdi1 ${PYTHON_LIBRARIES} )
    endif ()
    
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print( '%d.%d' % ( sys.version_info[0], sys.version_info[1] ) )"
                      OUTPUT_VARIABLE PYTHON_VERSION
                      OUTPUT_STRIP_TRAILING_WHITESPACE )

    set ( SITEPACKAGE lib${LIB_SUFFIX}/python${PYTHON_VERSION}/site-packages )

    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/_ftdi1.so DESTINATION ${SITEPACKAGE})
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/ftdi1.py DESTINATION ${SITEPACKAGE})

    if(DOCUMENTATION AND DOXYGEN_FOUND)

        set(CMAKE_SWIG_FLAGS -DDOXYGEN=${DOXYGEN_FOUND})

        # Run doxygen to only generate the xml
        add_custom_command ( OUTPUT ${CMAKE_BINARY_DIR}/doc/xml/ftdi_8c.xml
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile.xml
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${c_headers};${c_sources};${cpp_sources};${cpp_headers}
        )

        # generate .i from doxygen .xml
        add_custom_command ( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ftdi1_doc.i
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doxy2swig.py -n
                ${CMAKE_BINARY_DIR}/doc/xml/ftdi_8c.xml
                ${CMAKE_CURRENT_BINARY_DIR}/ftdi1_doc.i
            DEPENDS ${CMAKE_BINARY_DIR}/doc/xml/ftdi_8c.xml
        )
        add_custom_target ( doc_i DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ftdi1_doc.i )
        add_dependencies( ${SWIG_MODULE_ftdi1_REAL_NAME} doc_i )
        
    endif(DOCUMENTATION AND DOXYGEN_FOUND)



    
  set ( LIBFTDI_PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/${SITEPACKAGE} PARENT_SCOPE )  
  message(STATUS "Building python bindings via swig. Will be installed under ${LIBFTDI_PYTHON_MODULE_PATH}")
else(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
    message(STATUS "Not building python bindings")
endif(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
