# This is the main CMakeLists file for testing GenComp time-aware modules with GTest
#
# @author János Végh

# It is just a simple proxy module for the subdirectories

message(HIGHLIGHTED "Configuring GenComp 'Tests' executables")

  add_subdirectory(DEVEL)	# The very basic functionality of hardware modules, including communication
#  add_subdirectory(DEMO)	# The very basic functionality of hardware modules, including communication
