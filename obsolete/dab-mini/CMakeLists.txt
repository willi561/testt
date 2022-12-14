cmake_minimum_required( VERSION 2.8.11 )
set (objectName dabMini-2.0)
set (CMAKE_CXX_FLAGS "${CMAKE_XCC_FLAGS} -Wall -std=c++11 -flto")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

set(CMAKE_AUTORCC ON)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/usr/local/bin" CACHE PATH "default install path" FORCE )
endif()
#set (CMAKE_INSTALL_PREFIX /usr/local/bin)

#	modify if you want

if(MINGW)
    add_definitions ( -municode)
#else (MINGW)
#	add_definitions (-DSSE_AVAILABLE)
endif()

IF(EXISTS (".git") AND IS_DIRECTORY (".git"))
   execute_process (
      COMMAND git rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
   )
endif ()

if(GIT_COMMIT_HASH)
   add_definitions("-DGITHASH=\"${GIT_COMMIT_HASH}\"")
else ()
   add_definitions ("-DGITHASH=\"       \"")
endif()


########################################################################
# select the release build type by default to get optimization flags
########################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

### make sure our local CMake Modules path comes first
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

######################################################################
#
#####################################################################
#
#	default value
	set(USE_PORTAUDIO true)

if(DEFINED AIRSPY)
	set(AIRSPY true)
endif ()

if(DEFINED SDRPLAY)
	set(SDRPLAY true)
endif ()

if(DEFINED SDRPLAY_V3)
	set(SDRPLAY_V3 true)
endif ()

if(DEFINED RTLSDR)
	set(RTLSDR true)
endif ()

if(DEFINED HACKRF)
	set(HACKRF true)
endif ()

if(DEFINED LIMESDR)
	set(LIMESDR true)
endif ()
if (DEFINED FDK_AAC)
        set (FDK_AAC true)
endif ()

add_definitions (-DPRESET_NAME)
add_definitions (-D__THREADED_BACKEND)	# uncomment for use for an RPI
add_definitions (-D__MSC_THREAD__)
#
########################################################################

	find_package (PkgConfig)
	find_package (LSBId)

	find_package (Qt5Core REQUIRED)
	find_package (Qt5Widgets REQUIRED)
	find_package (Qt5Xml REQUIRED)
	include_directories (
	  ${Qt5Xml_INCLUDE_DIRS}
	)

        find_package(FFTW3f)
        if (NOT FFTW3F_FOUND)
            message(FATAL_ERROR "please install FFTW3")
        endif ()

        find_package(zlib)
	if (NOT ZLIB_FOUND)
            message(FATAL_ERROR "please install libz")
        endif ()
	list(APPEND extraLibs ${ZLIB_LIBRARY})

        find_package(LibSndFile)
        if (NOT LIBSNDFILE_FOUND)
            message(FATAL_ERROR "please install libsndfile")
        endif ()
        list(APPEND extraLibs ${LIBSNDFILE_LIBRARY})

        find_package(LibSampleRate)
        if (NOT LIBSAMPLERATE_FOUND)
            message(FATAL_ERROR "please install libsamplerate")
        endif ()
        list(APPEND extraLibs ${LIBSAMPLERATE_LIBRARY})

	find_library (PTHREADS pthread)
	if (NOT(PTHREADS))
	   message (FATAL_ERROR "please install libpthread")
	else (NOT(PTHREADS))
	   set (extraLibs ${extraLibs} ${PTHREADS})
	endif (NOT(PTHREADS))

#######################################################################
#
#	Here we really start

	include_directories (
	           ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	           ${QT5Widgets_INCLUDES}
	           ${QT_QTCORE_INCLUDE_DIR}
	           ${QT_QTGUI_INCLUDE_DIR}
	           .
	           ..
	           ../includes
	           ../includes/ofdm
	           ../includes/protection
	           ../includes/backend
	           ../includes/backend/audio
	           ../includes/backend/data
	           ../includes/backend/data/journaline
	           ../includes/backend/data/mot
	           ../includes/support
#	           ../includes/support/viterbi-jan
	           ../includes/support/viterbi-spiral
	           ../includes/output
	           ../includes/scopes-qwt6
	           ./devices-dab-mini
	           /usr/include/
	)

	set (${objectName}_HDRS
	     ../dab-processor.h
             ../includes/country-codes.h
	     ../includes/dab-constants.h
             ../includes/ofdm/sample-reader.h
	     ../includes/ofdm/phasereference.h
	     ../includes/ofdm/ofdm-decoder.h
	     ../includes/ofdm/phasetable.h
	     ../includes/ofdm/freq-interleaver.h
	     ../includes/ofdm/fib-decoder.h
	     ../includes/ofdm/dab-config.h
	     ../includes/ofdm/fib-table.h
	     ../includes/ofdm/fic-handler.h
	     ../includes/ofdm/tii_detector.h
	     ../includes/ofdm/timesyncer.h
	     ../includes/protection/protTables.h
	     ../includes/protection/protection.h
	     ../includes/protection/uep-protection.h
	     ../includes/protection/eep-protection.h
	     ../includes/backend/firecode-checker.h
	     ../includes/backend/frame-processor.h
	     ../includes/backend/charsets.h
	     ../includes/backend/galois.h
	     ../incluces/backend/reed-solomon.h
	     ../includes/backend/msc-handler.h
	     ../includes/backend/backend.h
	     ../includes/backend/backend-deconvolver.h
	     ../includes/backend/backend-driver.h
	     ../includes/backend/audio/mp4processor.h
	     ../includes/backend/audio/bitWriter.h
	     ../includes/backend/audio/mp2processor.h
	     ../includes/backend/data/ip-datahandler.h
	     ../includes/backend/data/tdc-datahandler.h
	     ../includes/backend/data/journaline-datahandler.h
	     ../includes/backend/data/journaline/dabdatagroupdecoder.h
	     ../includes/backend/data/journaline/crc_8_16.h
	     ../includes/backend/data/journaline/log.h
	     ../includes/backend/data/journaline/newssvcdec_impl.h
	     ../includes/backend/data/journaline/Splitter.h
	     ../includes/backend/data/journaline/dabdgdec_impl.h
	     ../includes/backend/data/journaline/newsobject.h
	     ../includes/backend/data/journaline/NML.h
	     ../includes/backend/data/virtual-datahandler.h
	     ../includes/backend/data/pad-handler.h
	     ../includes/backend/data/mot/mot-handler.h
	     ../includes/backend/data/mot/mot-object.h
	     ../includes/backend/data/mot/mot-dir.h
	     ../includes/backend/data/data-processor.h
	     ../includes/output/fir-filters.h
	     ../includes/output/audio-base.h
	     ../includes/output/newconverter.h
	     ../includes/support/process-params.h
	     ../includes/support/fft-handler.h
	     ../includes/support/ringbuffer.h
	     ../includes/support/Xtan2.h
	     ../includes/support/dab-params.h
	     ../includes/support/band-handler.h
#	     ../includes/support/text-mapper.h
	     ../includes/support/dab-tables.h
#	     ../includes/support/viterbi-jan/viterbi-handler.h
	     ../includes/support/viterbi-spiral/viterbi-spiral.h
	     ../includes/support/preset-handler.h
	     ../includes/support/presetcombobox.h
	     ../includes/support/smallqlistview.h
	     ../includes/support/smallpushbutton.h
	     ../includes/support/verysmallpushbutton.h
	     ../includes/support/smallcombobox.h
	     ../include/support/smallspinbox.h
	     ../includes/support/findfilenames.h 
             ../includes/support/scheduler.h 
             ../includes/support/schedule-selector.h 
             ../includes/support/element-selector.h 
	     ../includes/support/dl-cache.h
	     ./devices-dab-mini/device-handler.h
	)

	set (${objectName}_SRCS
	     ${${objectName}_SRCS}
	     ./main.cpp
	     ../dab-processor.cpp
             ../src/ofdm/sample-reader.cpp
	     ../src/ofdm/ofdm-decoder.cpp
	     ../src/ofdm/phasereference.cpp
	     ../src/ofdm/phasetable.cpp
	     ../src/ofdm/freq-interleaver.cpp
	     ../src/ofdm/fib-decoder.cpp
	     ../src/ofdm/fic-handler.cpp
	     ../src/ofdm/tii_detector.cpp
	     ../src/ofdm/timesyncer.cpp
	     ../src/protection/protTables.cpp
	     ../src/protection/protection.cpp
	     ../src/protection/eep-protection.cpp
	     ../src/protection/uep-protection.cpp
	     ../src/backend/firecode-checker.cpp
	     ../src/backend/charsets.cpp
	     ../src/backend/galois.cpp
	     ../src/backend/reed-solomon.cpp
	     ../src/backend/msc-handler.cpp
	     ../src/backend/backend.cpp
	     ../src/backend/backend-deconvolver.cpp
	     ../src/backend/backend-driver.cpp
	     ../src/backend/audio/mp4processor.cpp
	     ../src/backend/audio/bitWriter.cpp
	     ../src/backend/audio/mp2processor.cpp
	     ../src/backend/data/ip-datahandler.cpp
	     ../src/backend/data/journaline-datahandler.cpp
	     ../src/backend/data/journaline/crc_8_16.c
	     ../src/backend/data/journaline/log.c
	     ../src/backend/data/journaline/newssvcdec_impl.cpp
	     ../src/backend/data/journaline/Splitter.cpp
	     ../src/backend/data/journaline/dabdgdec_impl.c
	     ../src/backend/data/journaline/newsobject.cpp
	     ../src/backend/data/journaline/NML.cpp
	     ../src/backend/data/tdc-datahandler.cpp
	     ../src/backend/data/pad-handler.cpp
	     ../src/backend/data/mot/mot-handler.cpp
	     ../src/backend/data/mot/mot-object.cpp
	     ../src/backend/data/mot/mot-dir.cpp
	     ../src/backend/data/data-processor.cpp
	     ../src/output/audio-base.cpp
	     ../src/output/newconverter.cpp
	     ../src/output/fir-filters.cpp
	     ../src/support/fft-handler.cpp
	     ../src/support/Xtan2.cpp
	     ../src/support/dab-params.cpp
	     ../src/support/band-handler.cpp
#	     ../src/support/text-mapper.cpp
	     ../src/support/dab-tables.cpp
	     ../src/support/preset-handler.cpp
	     ../src/support/presetcombobox.cpp
	     ../src/support/smallqlistview.cpp
	     ../src/support/smallpushbutton.cpp
	     ../src/support/verysmallpushbutton.cpp
	     ../src/support/smallcombobox.cpp
	     ../src/support/smallspinbox.cpp
#	     ../src/support/viterbi-jan/viterbi-handler.cpp
	     ../src/support/viterbi-spiral/viterbi-spiral.cpp
	     ../src/support/scheduler.cpp 
             ../src/support/schedule-selector.cpp 
             ../src/support/element-selector.cpp 
             ../src/support/findfilenames.cpp 
	     ./devices-dab-mini/device-handler.cpp
	)

	set (${objectName}_MOCS
	     ../dab-processor.h
	     ../includes/output/audio-base.h
	     ../includes/output/audiosink.h
	     ../includes/ofdm/sample-reader.h
	     ../includes/ofdm/ofdm-decoder.h
	     ../includes/ofdm/phasereference.h
	     ../includes/ofdm/fib-decoder.h
	     ../includes/ofdm/fic-handler.h
	     ../includes/ofdm/tii_detector.h
	     ../includes/backend/msc-handler.h
	     ../includes/backend/backend.h
	     ../includes/backend/audio/mp2processor.h
	     ../includes/backend/audio/mp4processor.h
	     ../includes/backend/data/virtual-datahandler.h
	     ../includes/backend/data/pad-handler.h
	     ../includes/backend/data/mot/mot-handler.h
	     ../includes/backend/data/mot/mot-object.h
	     ../includes/backend/data/mot/mot-dir.h
	     ../includes/backend/data/ip-datahandler.h
	     ../includes/backend/data/tdc-datahandler.h
	     ../includes/backend/data/journaline-datahandler.h
	     ../includes/backend/data/data-processor.h
	     ../includes/support/preset-handler.h
	     ../includes/support/presetcombobox.h
	     ../includes/support/smallqlistview.h
	     ../includes/support/smallpushbutton.h
	     ../includes/support/verysmallpushbutton.h
	     ../includes/support/smallcombobox.h
	     ../includes/support/smallspinbox.h
	     ../includes/support/band-handler.h
	     ../includes/support/scheduler.h
             ../includes/support/schedule-selector.h
             ../includes/support/element-selector.h
	)

	set (${objectName}_UIS
	        ${${objectName}_UIS}
	     ./dab-mini.ui
	   )

	set (${objectName}_MOCS
	        ${${objectName}_MOCS} ./radio.h)

	set ($(objectName)_HDRS
	        ${${objectName}_HDRS} ./radio.h)

	set (${objectName}_SRCS
	        ${${objectName}_SRCS} ./radio.cpp)


#########################################################################

	if (FDK_AAC)
	   find_package(Fdk-AAC)
	   if (NOT LIBFDK_AAC_FOUND)
	      message (FATAL_ERROR, "please install libfdk-aac")
	   endif ()
	   include_directories (../specials/fdk-aac)
	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ../includes/backend/audio/fdk-aac.h
	   )
	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ../src/backend/audio/fdk-aac.cpp
	   )
	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ../includes/backend/audio/fdk-aac.h
	   )
           list(APPEND extraLibs ${FDK_AAC_LIBRARIES})
	   add_definitions (-D__WITH_FDK_AAC__)
	elseif (NOT FDK_AAC)
           find_package(Faad)
           if (NOT FAAD_FOUND )
              message(FATAL_ERROR "please install libfaad")
           endif ()
	   set (${objectName}_HDRS
	        ${${objectName}_HDRS}
	        ../includes/backend/audio/faad-decoder.h
	   )
	   set (${objectName}_SRCS
                ${${objectName}_SRCS}
	        ../src/backend/audio/faad-decoder.cpp
	   )
	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ../includes/backend/audio/faad-decoder.h
	   )
	   add_definitions (-D__WITH_FAAD__)
	endif (FDK-AAC)

#	the viterbi decoder
#
	if (VITERBI_NEON)
	   set(VITERBI_SSE false)
	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ../src/support/viterbi-spiral/spiral-neon.h
	   )
	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ../src/support/viterbi-spiral/spiral-neon.c
	   )
	   
	   add_definitions (-DNEON_AVAILABLE)
	elseif(VITERBI_SSE)
	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ../src/support/viterbi-spiral/spiral-sse.h
	   )
	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ../src/support/viterbi-spiral/spiral-sse.c
	   )
	   add_definitions (-DSSE_AVAILABLE)
	else (VITERBI_SSE)
	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ../src/support/viterbi-spiral/spiral-no-sse.h
	   )
	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ../src/support/viterbi-spiral/spiral-no-sse.c
	   )
	endif (VITERBI_NEON)

##########################################################################
#	The devices
#
	if (SDRPLAY)
	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/sdrplay-handler/sdrplay-handler.h
	        ./devices-dab-mini/sdrplay-handler/sdrplayselect.h
	   )

	   include_directories (
	     ./devices-dab-mini/sdrplay-handler
	   )

	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/sdrplay-handler/sdrplay-handler.h
	        ./devices-dab-mini/sdrplay-handler/sdrplayselect.h
	        ./devices-dab-mini/sdrplay-handler/mirsdrapi-rsp.h
           )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/sdrplay-handler/sdrplay-handler.cpp
	        ./devices-dab-mini/sdrplay-handler/sdrplayselect.cpp
	   )

	 add_definitions (-DHAVE_SDRPLAY)
	endif (SDRPLAY)

	if (SDRPLAY_V3)

	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/sdrplay-handler-v3/sdrplay-handler-v3.h
	   )

	   include_directories (
	     ./devices-dab-mini/sdrplay-handler-v3
	     ./devices-dab-mini/sdrplay-handler-v3/includes
	   )

	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/sdrplay-handler-v3/sdrplay-handler-v3.h
	        ./devices-dab-mini/sdrplay-handler-v3/sdrplay-commands.h
	        ./devices-dab-mini/sdrplay-handler-v3/includes/sdrplay_api.h
           )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/sdrplay-handler-v3/sdrplay-handler-v3.cpp
	   )

	 add_definitions (-DHAVE_SDRPLAY_V3)
	endif (SDRPLAY_V3)

	if (AIRSPY)
#           find_package(LibAIRSPY)
#           if (NOT LIBAIRSPY_FOUND)
#               message(FATAL_ERROR "please install airspy library")
#           endif ()
#	   ## include_directories (${AIRSPYLIB_INCLUDE_DIR})

	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/airspy-handler/airspy-handler.h
	   )

	   include_directories (
	     ./devices-dab-mini/airspy-handler
	     ./devices-dab-mini/airspy-handler/libairspy
	   )

	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/airspy-handler/airspy-handler.h
	        ./devices-dab-mini/airspy-handler/airspyfilter.h
	        ./devices-dab-mini/airspy-handler/libairspy/airspy.h
	   )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/airspy-handler/airspy-handler.cpp
	        ./devices-dab-mini/airspy-handler/airspyfilter.cpp
	   )

	   add_definitions (-DHAVE_AIRSPY)
	endif (AIRSPY)

	if (HACKRF)

	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/hackrf-handler/hackrf-handler.h
	   )

	   include_directories (
	     ./devices-dab-mini/hackrf-handler
	     ./devices-dab-mini/hackrf-handler/libhackrf
	   )

	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/hackrf-handler/hackrf-handler.h
	        ./devices-dab-mini/hackrf-handler/libhackrf/hackrf.h
	   )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/hackrf-handler/hackrf-handler.cpp
	   )

	   add_definitions (-DHAVE_HACKRF)
	endif (HACKRF)
#
	if (LIMESDR)
	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/lime-handler/lime-handler.h
	   )

	   include_directories (
	     ./devices-dab-mini/lime-handler
	   )

	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/lime-handler/lime-handler.h
	        ./devices-dab-mini/lime-handler/LimeSuite.h
	        ./devices-dab-mini/lime-handler/LMS7002M_parameters.h
	   )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/lime-handler/lime-handler.cpp
	   )

	   add_definitions (-DHAVE_LIME)
	endif (LIMESDR)

	if (RTLSDR)
	   set (${objectName}_MOCS
	        ${${objectName}_MOCS}
	        ./devices-dab-mini/rtlsdr-handler/rtlsdr-handler.h
	   )

	   include_directories (
	        ./devices-dab-mini/rtlsdr-handler/
	   )

	   set (${objectName}_HDRS
	        ${${objectName}_HDRS}
	        ./devices-dab-mini/rtlsdr-handler/rtlsdr-handler.h
	        ./devices-dab-mini/rtlsdr-handler/rtl-sdr.h
	        ./devices-dab-mini/rtlsdr-handler/rtl-sdr_export.h
	   )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	        ./devices-dab-mini/rtlsdr-handler/rtlsdr-handler.cpp
	   )

	   add_definitions (-DHAVE_RTLSDR)
	endif()

#
	if (TRY_EPG)
	   set ($(objectName)_HDRS
	        ${${objectName}_HDRS}
	             ./includes/backend/data/epg/epgdec.h
	   )

	   set (${objectName}_SRCS
	        ${${objectName}_SRCS}
	             ./src/backend/data/epg/epgdec.cpp
	   )
	   add_definitions (-DTRY_EPG)
	endif (TRY_EPG)

	if (USE_PORTAUDIO)
           find_package(Portaudio)
           if (NOT PORTAUDIO_FOUND)
               message(FATAL_ERROR "please install portaudio V19")
           endif ()
           list(APPEND extraLibs ${PORTAUDIO_LIBRARIES})
	   set (${objectName}_HDRS
	        ${${objectName}_HDRS}
	       ../includes/output/audiosink.h
	   )

	   set (${objectName}_MOCS
	       ${${objectName}_MOCS}
	      ../includes/output/audiosink.h
	   )

	   set (${objectName}_SRCS
	      ${${objectName}_SRCS}
	      ../src/output/audiosink.cpp
	   )
	endif (USE_PORTAUDIO)

	QT5_WRAP_UI (UIS ${${objectName}_UIS})

	include_directories (
	          ${SDRPLAY_INCLUDES}
	          ${QT5Widgets_INCLUDES}
	          ${QT_QTCORE_INCLUDE_DIR}
	          ${QT_QTGUI_INCLUDE_DIR}
	          ${FFTW_INCLUDE_DIRS}
	          ${PORTAUDIO_INCLUDE_DIRS}
	          ${FAAD_INCLUDE_DIRS}
	)

	QT5_WRAP_CPP (MOCS ${${objectName}_MOCS})

	add_executable (${objectName}
	                ${${objectName}_SRCS}
	                ${UIS}
	                ${RSCS}
	                ${TRS}
	                ${MOCS}
	                resources.qrc
	)

	target_link_libraries (${objectName}
	                       Qt5::Widgets
	                       Qt5::Xml
	                       ${RTLTCP_lib}
	                       ${FFTW3F_LIBRARIES}
	                       ${extraLibs}
	                       ${FAAD_LIBRARIES}
	                       ${CMAKE_DL_LIBS}
	)
	if (QT_AUDIO)
	   target_link_libraries (${objectName} Qt5::Multimedia)
	endif (QT_AUDIO)

	INSTALL (TARGETS ${objectName} DESTINATION  ${CMAKE_INSTALL_PREFIX})
#	INSTALL (TARGETS ${objectName} DESTINATION ./linux-bin)

########################################################################
# Create uninstall target
########################################################################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
