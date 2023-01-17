include(GNUInstallDirs)

option(MONITORING "Tools for real-time inspection of the application." OFF)
set(MONITORING_QML_ENTRY_POINT "" CACHE STRING "QML file intended to start the monitoring tool UI.")

set(QTPREFIX "Qt")

find_package(Qt6 COMPONENTS Core Qml Gui Quick QuickControls2 Widgets QUIET)
if (NOT Qt6_FOUND)
    set(QTPREFIX "Qt5")
    find_package(Qt5 COMPONENTS Core Qml Gui Quick QuickControls2 Widgets REQUIRED)
endif()

# Macro for merging common code between static and shared
macro(add_target name type)
    file(GLOB HEADERS include/DOtherSide/*.h include/DOtherSide/Status/*.h)

    file(GLOB SOURCES src/*.cpp src/Status/*.cpp)

    if(MONITORING)
        file(GLOB MONITORING_HEADERS include/DOtherSide/Status/Monitoring/*.h)
        file(GLOB MONITORING_SOURCES src/Status/Monitoring/*.cpp)
    endif()

    if(APPLE)
        file(GLOB MM_FILES src/*.mm src/Status/*.mm)
        #prepend items because .mm files need build priority to override cpp impl
        list(INSERT SOURCES 0 ${MM_FILES})
    endif()

    add_library(${name} ${type} ${SOURCES} ${HEADERS} ${MONITORING_SOURCES} ${MONITORING_HEADERS})

    if (WIN32)
        target_compile_definitions(${name} PRIVATE -DWIN32)
    endif()

    if(NOT MSVC)
      target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    set_target_properties(${name} PROPERTIES CXX_STANDARD 11 AUTOMOC ON)

    target_include_directories(${name} PUBLIC include)

    target_link_libraries(${name} PRIVATE ${QTPREFIX}::Core ${QTPREFIX}::CorePrivate ${QTPREFIX}::Gui ${QTPREFIX}::Widgets ${QTPREFIX}::Qml ${QTPREFIX}::Quick)

    target_compile_definitions(${name} PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>)
    if(DEFINED QML_DEBUG_PORT)
        target_compile_definitions(${name} PRIVATE QML_DEBUG_PORT=${QML_DEBUG_PORT})
    endif()

    if(MONITORING)
        target_compile_definitions(${name} PRIVATE MONITORING)
        target_compile_definitions(${name} PRIVATE MONITORING_QML_ENTRY_POINT="${MONITORING_QML_ENTRY_POINT}")
    endif()

    if (${Qt5QuickControls2_FOUND})
        target_link_libraries(${name} PRIVATE ${QTPREFIX}::QuickControls2)
    endif()
endmacro()

# Add shared version
if(ENABLE_DYNAMIC_LIBS)
    add_target(${PROJECT_NAME} SHARED)

    set(soversion "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}")

    set_target_properties(${PROJECT_NAME}
    PROPERTIES
        SOVERSION "${soversion}"
        VERSION "${CMAKE_PROJECT_VERSION}"
    )
endif(ENABLE_DYNAMIC_LIBS)

# Add static version
if(ENABLE_STATIC_LIBS)
    add_target(${PROJECT_NAME}Static STATIC)
endif(ENABLE_STATIC_LIBS)

# Install directive for header files
install(FILES
    include/DOtherSide/DOtherSide.h
    include/DOtherSide/DOtherSideTypes.h
    DESTINATION include/DOtherSide
)

# Install directive for binaries
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install Pkg-Config definition
configure_file(DOtherSide.pc.in DOtherSide.pc @ONLY)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/DOtherSide.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
