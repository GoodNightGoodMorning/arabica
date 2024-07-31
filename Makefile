app          = arabica
test         = test_$(app)
cc_desktop   = clang
cxx_desktop  = clang++
cc_wasm      = emcc
cxx_wasm     = em++
src_app      = app
src_emulator = arabica
src_test     = test
src          = $(src_app) $(src_emulator) $(src_test)
dir_build    = build
dir_rom      = roms
game         = Tetris_Fran_Dachille_1991.ch8

default: desktop

build_desktop: clean
	mkdir $(dir_build);                                                                                            
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc_desktop)" -DCMAKE_CXX_COMPILER="$(cxx_desktop)" -DBUILD_DESKTOP=ON -GNinja ..; ninja; 

build_webassembly: clean
	mkdir -p $(dir_build)
	cd $(dir_build) && cmake -DCMAKE_C_COMPILER="$(cc_wasm)" -DCMAKE_CXX_COMPILER="$(cxx_wasm)" -DBUILD_WASM=ON -GNinja .. && ninja;

build_test: clean 
	mkdir $(dir_build); 
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc_desktop)" -DCMAKE_CXX_COMPILER="$(cxx_desktop)" -DBUILD_TEST=ON -GNinja ..; ninja; 

desktop: build_desktop
	./$(dir_build)/$(app).out $(dir_rom)/$(game)

webassembly: build_webassembly kill_webserver
	cd build; python3 -m http.server 8080 &
	firefox http://localhost:8080/arabica_wasm.html

test: build_test
	./$(test).app

debug: build_desktop
	gdb -x commands.gdb --args ./$(dir_build)/$(app).out $(dir_rom)/$(game).ch8

clean:
	rm -rf $(dir_build)

fmt:
	for dir in $(src); do                                                                      \
		find $$dir -type f -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i -style=file; \
	done

kill_webserver:
	pkill -f 8080

.PHONY: default build_desktop build_webassembly build_test desktop webassembly test debug clean fmt kill_webserver