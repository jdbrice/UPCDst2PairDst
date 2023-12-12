#!python
import subprocess
import os
import platform

# SConscript('color_SConscript')
# Import( 'env' )

SConscript('color_SConscript')
Import( 'env' )

SConscript( "modules/FemtoDstFormat/SConstruct" )
SConscript( "modules/PairDstFormat/SConstruct" )
SConscript( "modules/upcdst/SConstruct" )

env.Append(CPPPATH    = [ "modules/" ])
env.Append(CXXFLAGS   = ['-std=c++11'])

ROOBARB_LIB = os.environ.get("ROOBARB_LIB", "" )
env.Append(LIBPATH 		= [ ROOBARB_LIB + "/lib" ] )
env.Append(LIBS       = [ "TMVA", "libEG", "libXmlConfig.a", "libRooPlotLib.a", "libTaskEngine.a", "libRootAna.a" ] )

debug = ARGUMENTS.get('debug', 0)
if int(debug):
	env.Append(CXXFLAGS = '-g')

# # ROOT
ROOTCFLAGS      = subprocess.check_output( ['root-config',  '--cflags'] ).rstrip().decode( "utf-8" ).split( " " )
ROOTLIBS        = subprocess.check_output( ["root-config",  "--libs"] ).decode( "utf-8" )
ROOT_SYS        = os.environ.get( "ROOTSYS", "" )

env.Append(CPPPATH   = [ 	ROOT_SYS + "/include/", 
							ROOBARB_LIB + '/XmlConfig',
							ROOBARB_LIB + '/RooPlotLib',
							ROOBARB_LIB + '/TaskEngine',
							ROOBARB_LIB + '/RootAna',
							ROOBARB_LIB + '/',
							"modules/StarClassLibrary" ])
env.Append(CXXFLAGS  = ROOTCFLAGS )
env.Append(LINKFLAGS = ROOTCFLAGS )

env[ "_LIBFLAGS" ] = env[ "_LIBFLAGS" ] + " " + ROOTLIBS + " "
# REMOVE "-pthread" from clang link options in OS X
# assuming darwin=clang but must be a better way...
if "Darwin" in platform.platform() :
	env[ "LINKFLAGS" ].remove( "-pthread" )

env.Program( target="bin/pico.app", source=[ "modules/PairDstFormat/DictionaryFemtoPair.cpp", "modules/FemtoDstFormat/DictionaryFemtoDst.cpp", "modules/upcdst/DictionaryUPCDst.cpp", Glob("modules/upcdst/*.cxx"), "Engine.cpp"] )
