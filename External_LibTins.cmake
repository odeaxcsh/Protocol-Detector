find_package(Threads REQUIRED)

include(ExternalProject)
ExternalProject_Add(
  libtins
  GIT_REPOSITORY https://github.com/mfontanini/libtins.git
  UPDATE_COMMAND ""
  INSTALL_COMMAND   ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON
)

ExternalProject_Get_Property(libtins source_dir)
set(LIBTINS_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${LIBTINS_INCLUDE_DIRS})

ExternalProject_Get_Property(libtins binary_dir)
set(LIBTINS_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}/libtins.so)
set(TINS_LIBRARY tins)

add_library(${TINS_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${TINS_LIBRARY} PROPERTIES
    "IMPORTED_LOCATION" "${LIBTINS_LIBRARY_PATH}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${LIBTINS_INCLUDE_DIRS}")

add_dependencies(${TINS_LIBRARY} libtins)
