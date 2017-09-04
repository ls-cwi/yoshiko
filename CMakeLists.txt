cmake_minimum_required( VERSION 2.8 )

project( Yoshiko )

set( Yoshiko_Hdr

  src/ClusterEditingInstance.h
  src/ClusterEditingReduction.h
  src/ClusterEditingSolutions.h
  src/ClusterReductionInstance.h
  src/config.h
  src/config.h.in
  src/CoreAlgorithm.h
  src/Delta.h
  src/DynProgTable.h
  src/Globals.h
  src/Helpers.h
  src/InducedCostsHeuristic.h
  src/ParameterizedInstance.h
  src/WorkingCopyInstance.h
  src/Yoshiko.h
  
  src/input/ClusterEditingInput.h
  src/input/JENAInput.h
  src/input/SIFInput.h
  src/input/StreamInput.h
   
  src/output/ClusterEditingOutput.h
  src/output/CSVOutput.h
  src/output/GMLOutput.h
  src/output/NullOutput.h
  src/output/PajekOutput.h
  src/output/TableOutput.h
  src/output/TableAppOutput.h
  src/output/XGMMLOutput.h
  
  src/reductionRules/AlmostCliqueRule.h
  src/reductionRules/CliqueRule.h
  src/reductionRules/CriticalCliqueRule.h
  src/reductionRules/FPTReductionRule.h
  src/reductionRules/HeavyEdgeRule3in1.h
  src/reductionRules/MergingRule.h
  src/reductionRules/ParameterDependentReductionRule.h
  src/reductionRules/SimilarNeighboorhoodRule.h

)

set( Yoshiko_Src
  src/ClusterReductionInstance.cpp
  src/CoreAlgorithm.cpp
  src/ParameterizedInstance.cpp
  src/Helpers.cpp
  src/Delta.cpp
  src/InducedCostsHeuristic.cpp
  src/ClusterEditingInstance.cpp
  src/DynProgTable.cpp
  src/WorkingCopyInstance.cpp
  src/ClusterEditingReduction.cpp
  src/Yoshiko.cpp
  src/ClusterEditingSolutions.cpp
  src/Globals.cpp
  src/main.cpp
  
  src/input/ClusterEditingInput.cpp
  src/input/JENAInput.cpp
  src/input/SIFInput.cpp

  src/output/ClusterEditingOutput.cpp
  src/output/CSVOutput.cpp
  src/output/GMLOutput.cpp
  src/output/NullOutput.cpp
  src/output/XGMMLOutput.cpp
  src/output/PajekOutput.cpp
  src/output/TableOutput.cpp
  src/output/TableAppOutput.cpp
  
  src/reductionRules/AlmostCliqueRule.cpp
  src/reductionRules/CliqueRule.cpp
  src/reductionRules/CriticalCliqueRule.cpp
  src/reductionRules/FPTReductionRule.cpp
  src/reductionRules/HeavyEdgeRule3in1.cpp
  src/reductionRules/MergingRule.cpp
  src/reductionRules/ParameterDependentReductionRule.cpp
  src/reductionRules/SimilarNeighboorhoodRule.cpp
  
)

set (YoshikoLib_Src
	src/input/LibraryInput.cpp
	src/LibraryInterface.cpp
	src/LibraryInterface_wrap.cxx
	${Yoshiko_Src}
)

set (YoshikoLib_Hdr
	src/input/LibraryInput.h
	src/JavaInformer.h
	src/LibraryInterface.h
	${Yoshiko_Hdr}
)

set( GitCommand1 "symbolic-ref HEAD 2> /dev/null | cut -b 12-" )
execute_process( COMMAND git symbolic-ref HEAD  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                 COMMAND tail -c -7 COMMAND tr -d "\\n" OUTPUT_VARIABLE GitBranch )
set( GitCommand2 "log --pretty=format:\\\"%h\\\" -1" )
execute_process( COMMAND git log --pretty=format:%h WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                 COMMAND head -n 1  COMMAND tr -d "\\n" OUTPUT_VARIABLE GitHashtag )
                
set( YOSHIKO_VERSION \"${GitBranch}-${GitHashtag}\" )

set( LIBLEMON_ROOT "$ENV{HOME}/lemon" CACHE PATH "Additional search directory for lemon library" )

find_package(JNI)

if( NOT CPLEX_INC_DIR OR NOT CPLEX_LIB_DIR OR NOT CONCERT_INC_DIR OR NOT CONCERT_LIB_DIR )
	set( CPLEX_AUTODETECTION_PREFIXES "$ENV{HOME}/Applications/IBM/ILOG; $ENV{HOME}/ILOG;/opt/ibm/ILOG;$ENV{HOME}/Applications/IBM/ILOG" CACHE STRING "CPLEX automatic lookup directories" )
	foreach( CplexInstallPrefix ${CPLEX_AUTODETECTION_PREFIXES} )
		file( GLOB_RECURSE CplexLibraryPaths ${CplexInstallPrefix}/*/cplex/lib/*/libcplex.a )
		if( CplexLibraryPaths )
			list( LENGTH CplexLibraryPaths NumPath )
			if( NumPath GREATER 1 )
				message( WARNING "Found multiple CPLEX installations in ${CplexInstallPrefix}, automatically selecting the first one, please set {CPLEX|CONCERT}_{INC|LIB}_DIR manually if this is not the right selection" )
			else()
				message( "Found CPLEX's installation in ${CPlexInstallPrefix}" )
			endif()
			list( GET CplexLibraryPaths 0 CplexLibraryPath )

			set( MyRegex "${CplexInstallPrefix}/(.*)/cplex/?/lib/(.*)/libcplex.a" )
			string( REGEX MATCH "${MyRegex}" ValidatedCplexLibPath ${CplexLibraryPath} )
			if( NOT ValidatedCplexLibPath )
				message( WARNING "Unable to recognize ${CplexLibraryPath} as a valid CPLEX path" )
			else()
				string( REGEX REPLACE "${MyRegex}" "\\1" CplexHome ${ValidatedCplexLibPath} )
				string( REGEX REPLACE "${MyRegex}" "\\2" CplexArch ${ValidatedCplexLibPath} )

				set( CPLEX_INC_DIR   "${CplexInstallPrefix}/${CplexHome}/cplex/include"            CACHE PATH "CPLEX's include dir" )
				set( CPLEX_LIB_DIR   "${CplexInstallPrefix}/${CplexHome}/cplex/lib/${CplexArch}"   CACHE PATH "CPLEX's library dir" )
				set( CONCERT_INC_DIR "${CplexInstallPrefix}/${CplexHome}/concert/include"          CACHE PATH "Concert's include dir" )
        set( CONCERT_LIB_DIR "${CplexInstallPrefix}/${CplexHome}/concert/lib/${CplexArch}" CACHE PATH "Concert's library dir" )

				break()
			endif()
		else()
			message( "Unable to find CPLEX and Concert installation in ${CplexInstallPrefix}" )
		endif()
	endforeach()

	if( NOT CPLEX_INC_DIR OR NOT CPLEX_LIB_DIR OR NOT CONCERT_INC_DIR OR NOT CONCERT_LIB_DIR )
		message( "Unable to autodeted CPLEX's and Concert's installation directories, please set {CPLEX|CONCERT}_{INC|LIB}_DIR manually" )
	else()
		message( "Autodetected CPLEX's include dir   : ${CPLEX_INC_DIR}" )
		message( "Autodetected CPLEX's library dir   : ${CPLEX_LIB_DIR}" )
		message( "Autodetected Concert's include dir : ${CONCERT_INC_DIR}" )
		message( "Autodetected Concert's library dir : ${CONCERT_LIB_DIR}" )
	endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release Profile" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wno-long-long -Wno-unused-parameter  -Wno-unknown-pragmas -g -ggdb -O0 -DDEBUG -DVERBOSE")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -Wno-long-long -Wno-unused-parameter  -Wno-unknown-pragmas -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_PROFILE "-Wall -Wextra -Wno-long-long -Wno-unused-parameter  -Wno-unknown-pragmas -O3 -g -ggdb -DNDEBUG")
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -z noexecstack -fno-strict-aliasing" )
add_definitions( -DIL_STD ) #???
configure_file( src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h )


if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ " )
  set( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libstdc++" )
endif()

include_directories( ${CPLEX_INC_DIR} ${CONCERT_INC_DIR} ${JNI_INCLUDE_DIRS} "${LIBLEMON_ROOT}/include" "src" "src/externlib" )
link_directories( ${CPLEX_LIB_DIR} ${CONCERT_LIB_DIR}  "${LIBLEMON_ROOT}/lib" )

set( CommonLibs
	m
	ilocplex
	cplex
	concert
	pthread
	emon
)

add_library(yoshikoLibrary SHARED ${YoshikoLib_Src} ${YoshikoLib_Hdr} )
add_executable( yoshiko ${Yoshiko_Src} ${Yoshiko_Hdr})

target_link_libraries( yoshiko ${CommonLibs} )
target_link_libraries( yoshikoLibrary ${CommonLibs})