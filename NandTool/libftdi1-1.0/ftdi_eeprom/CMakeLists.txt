set(FTDI_BUILD_EEPROM False PARENT_SCOPE)

option(FTDI_EEPROM "Build ftdi_eeprom" ON)

if (FTDI_EEPROM)
    include(FindPkgConfig)
    pkg_check_modules(Confuse libconfuse)
    INCLUDE_DIRECTORIES(${Confuse_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Confuse_LIBRARY_DIRS})
    SET(libs ${libs} ${Confuse_LIBRARIES})

    if(Confuse_FOUND)
        set(FTDI_BUILD_EEPROM True PARENT_SCOPE)
        message(STATUS "Building ftdi_eeprom")

        # Version defines
        set(EEPROM_MAJOR_VERSION 0)
        set(EEPROM_MINOR_VERSION 17)
        set(EEPROM_VERSION_STRING ${EEPROM_MAJOR_VERSION}.${EEPROM_MINOR_VERSION})

        include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)
        include_directories(BEFORE ${CMAKE_BINARY_DIR}/ftdi_eeprom)

        configure_file(
          "ftdi_eeprom_version.h.in"
          "${CMAKE_BINARY_DIR}/ftdi_eeprom/ftdi_eeprom_version.h"
        )

        add_executable(ftdi_eeprom main.c)
        target_link_libraries(ftdi_eeprom ftdi1)
        target_link_libraries(ftdi_eeprom ${Confuse_LIBRARIES})

        install(TARGETS ftdi_eeprom DESTINATION bin)

    else(Confuse_FOUND)
        message(STATUS "libConfuse not found, won't build ftdi_eeprom")
    endif(Confuse_FOUND)

else(FTDI_EEPROM)

    message(STATUS "ftdi_eeprom build is disabled")

endif(FTDI_EEPROM)
