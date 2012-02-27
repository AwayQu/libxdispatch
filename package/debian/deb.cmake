
set( CPACK_DEBIAN_PACKAGE_NAME "libxdispatch")
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>=2.13-1) , libgcc1 (>=4.5.1), libstdc++6 (>=4.5.1)")
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "${XDISPATCH_MAINTAINER}")
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${XDISPATCH_SUMMARY}")
set( CPACK_PACKAGE_DESCRIPTION "${XDISPATCH_DESCRIPTION}")
set( CPACK_DEBIAN_PACKAGE_SECTION "Devel")
set( CPACK_DEBIAN_PACKAGE_VERSION "${XDISPATCH_VERSION}-${PACKAGE_COMPILER}")
set( CPACK_DEBIAN_COMPONENT_INSTALL ON) # enable me as soon as component based deb files work with cmake

add_custom_target(
	debian ./package/debian/package.sh xdispatch-${XDISPATCH_VERSION}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

configure_file( 
    ${CMAKE_CURRENT_SOURCE_DIR}/package/debian/changelog.in
    ${CMAKE_CURRENT_SOURCE_DIR}/package/debian/changelog
)
	
message("-- Configured generator 'DEB (Debian)'")
set( XDISPATCH_CPACK ${XDISPATCH_CPACK} DEB )
