#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(muons4Peace)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


# include boost library
find_package( Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and CRY libraries
#
add_executable(muonsMain muonsMain.cc ${sources} ${headers})
target_link_libraries(muonsMain ${Geant4_LIBRARIES} ${Boost_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory,
# This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MUONS_SCRIPTS
  inputMuons.ini
  simpleRun.mac
  init_vis.mac
  vis.mac
  )

foreach(_script ${MUONS_SCRIPTS})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    )
endforeach()


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS muonsMain DESTINATION bin)
