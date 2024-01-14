vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO CookieDose/ipaddress
    REF v1.0.0
    SHA512 1
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH "${SOURCE_PATH}"
    PREFER_NINJA
)
#OPTIONS
#-IPADDRESS_BUILD_UNIT_TEST=OFF
#-IPADDRESS_BUILD_EXAMPLES=OFF
#-IPADDRESS_BUILD_BENCHMARK=OFF

vcpkg_install_cmake()

vcpkg_fixup_cmake_targets()

#file(
#  INSTALL "${SOURCE_PATH}/LICENSE"
#  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
#  RENAME copyright)

vcpkg_copy_pdbs()


# Handle copyright
#file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)