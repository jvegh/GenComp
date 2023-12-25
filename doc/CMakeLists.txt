project(GenCompDoc)

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)

if(CMAKE_COMPILER_IS_GNUCXX)
  if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g1")
  endif(CMAKE_SYSTEM_NAME MATCHES Linux)
endif(CMAKE_COMPILER_IS_GNUCXX)

option(TAC_INTERNAL_DOCUMENTATION "Build internal documentation" OFF)
option(TAC_DOC_USE_MATHJAX "Use MathJax for rendering math in HTML docs" ON)

# Set some Doxygen flags
set(TAC_DOXY_PROJECT_NAME             "Time-Aware Computing")
set(TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX  "/html")
set(TAC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TAC_DOXY_INPUT                     "${TAC_SOURCE_DIR} ${TAC_SOURCE_DIR}/../modules/ ${TAC_SOURCE_DIR}/../modules/include/") #\"${TAC_SOURCE_DIR}/modules/NEURER/include\" \"${TAC_SOURCE_DIR}/modules/MEMBRANE/include\"                                    # \"${TAC_SOURCE_DIR}/modules/NEURER/include\" \"${TAC_SOURCE_DIR}/modules/MEMBRANE/include\"
set(TAC_DOXY_HTML_COLORSTYLE_HUE      "220")
set(TAC_DOXY_TAGFILES                 "")
set(HTML_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}")
set(doxyfile_in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile [PARENT_SCOPE])

if(TAC_INTERNAL_DOCUMENTATION)
  set(TAC_DOXY_INTERNAL                 "YES")
else(TAC_INTERNAL_DOCUMENTATION)
  set(TAC_DOXY_INTERNAL                 "NO")
endif(TAC_INTERNAL_DOCUMENTATION)

if (TAC_DOC_USE_MATHJAX)
  set(TAC_DOXY_USE_MATHJAX "YES")
else ()
  set(TAC_DOXY_USE_MATHJAX "NO")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
)

message(HIGHLIGHTED "Header " ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/TACdoxy_header.html )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy_header.html.in
  ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/TACdoxy_header.html
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy_footer.html.in
  ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/TACdoxy_footer.html
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy_layout.xml.in
  ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/TACdoxy_layout.xml
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy.css.in
  ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/TACdoxy.css
)

set(examples_targets "")
set(snippets_targets "")

add_definitions("-DTAC_MAKING_DOCS")
add_custom_target(all_examples)

#add_subdirectory(examples)
#add_subdirectory(special_examples)
#add_subdirectory(snippets)

add_custom_target(
  doc-TAC-prerequisites
  ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TAC_navtree_hacks.js           ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/images/TAC_Logo.png  ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ftv2pnode.png                    ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ftv2node.png                     ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy.css          ${CMAKE_CURRENT_BINARY_DIR}${TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX}/
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


add_custom_target(doc ALL
  COMMAND doxygen
  COMMAND ${CMAKE_COMMAND} -E rename html TAC-doc
  COMMAND ${CMAKE_COMMAND} -E remove TAC-doc/TAC-doc.tgz
  COMMAND ${CMAKE_COMMAND} -E tar cfz TAC-doc.tgz TAC-doc
  COMMAND ${CMAKE_COMMAND} -E rename TAC-doc.tgz TAC-doc/TAC-doc.tgz
  COMMAND ${CMAKE_COMMAND} -E rename TAC-doc html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

