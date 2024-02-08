# This is the main CMakeLists file for 3rd-party programs
#
# @author János Végh

# It is just a simple proxy module for the subdirectories

message(HIGHLIGHTED "Configuring '3rd party' components")

  add_subdirectory(QCustomPlot)	# A plotting library
  add_subdirectory(Minuit)	# Routines from Minuit/CERN
  add_subdirectory(fancytabbar)  # Inherited from Ripes

