set(src_dir "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB lib_src ${src_dir}/*.cpp)
file(GLOB lib_hdr ${src_dir}/*.h)

add_library(minimal SHARED ${lib_src} ${lib_hdr})
target_link_libraries(minimal ringbuffer)

install(TARGETS minimal
	LIBRARY DESTINATION ${INSTALL_LIB_DIR})
install(FILES ${lib_hdr}
	DESTINATION "${INSTALL_INC_DIR}/minimal")

