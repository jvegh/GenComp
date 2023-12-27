# This is the main CMakeLists file for the GenComp 'main' programs
#
# @author János Végh

# It is just a simple proxy module for the subdirectories

message(HIGHLIGHTED "Configuring GenComp 'main' programs")

  add_subdirectory(DEMO)	# Demonstrating the program
  add_subdirectory(DEVEL)	# For the developer only
##  add_subdirectory(MEMBRANE)	# The neuron-membrane related modules

