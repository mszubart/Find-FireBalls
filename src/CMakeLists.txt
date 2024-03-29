# FFB's main source dir
set(the_target "ffb")

add_definitions (-Wall -pedantic -D_GLIBCXX -DBOOST_THREAD_USE_LIB --std=gnu++0x)

include_directories ( ${Boost_INCLUDE_DIRS} )
file(GLOB cpp_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
add_executable(${the_target} ${cpp_sources})
target_link_libraries(${the_target} ${OpenCV_LIBS} ${Boost_THREAD_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# For monolithic executable uncomment below (I wish you luck on x64)
#set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc -monolithic")

if(WIN32)
	file(GLOB OCV_DLLS "${OpenCV_BIN_PATH}/*.dll")
	set(DESTINATION ${EXECUTABLE_OUTPUT_PATH})

	foreach(DLL_FILENAME ${OCV_DLLS} ${PTHREAD_DLLS})
		add_custom_command(
			TARGET ${the_target}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${DLL_FILENAME} ${DESTINATION}
			VERBATIM
		)
	endforeach(DLL_FILENAME)
endif(WIN32)
