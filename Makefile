app          = arabica
test         = test_$(app)
cc           = clang
cxx          = clang++
src_app      = app
src_emulator = arabica
src_test     = test
src          = $(src_app) $(src_emulator) $(src_test)
dir_build    = build
dir_rom      = roms
game         = Tetris_Fran_Dachille_1991.ch8

default: build_sdl execute

fmt:
	for dir in $(src); do                                                                      \
		find $$dir -type f -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i -style=file; \
	done

build_sdl: clean
	mkdir $(dir_build);                                                                                            
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_APP=ON -GNinja ..; ninja; 

build_wasm: clean
	mkdir -p $(dir_build)
	cd $(dir_build) && cmake -DCMAKE_C_COMPILER=emcc -DCMAKE_CXX_COMPILER=em++ -DBUILD_WASM=ON -GNinja .. && ninja

execute:
	./$(dir_build)/$(app).out $(dir_rom)/$(game)

clean:
	rm -rf $(dir_build)

test: clean 
	mkdir $(dir_build);                                                                                                \
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_TEST=ON -GNinja ..; ninja; \
	./$(test).out

debug: clean build_sdl
	gdb -x commands.gdb --args ./$(dir_build)/$(app).out $(dir_rom)/$(game).ch8

web: build_wasm
	cd build; python3 -m http.server 8080 &
	firefox http://localhost:8080/arabica_wasm.html

kill_webserver:
	pkill -f 8080

.PHONY: default fmt build_sdl build_wasm execute clean debug
