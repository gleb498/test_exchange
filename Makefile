MAKEFLAGS=-j --no-print-directory

debug:
	cd _debug && ${MAKE}

release:
	cd _release && ${MAKE}
	
config-debug : _debug
	cd $< && cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..

config-release : _release
	cd $< && cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..

clean-release :
	cd _release && make clean

clean-debug :
	cd _debug && make clean

_release:
	mkdir _release

_debug:
	mkdir _debug
