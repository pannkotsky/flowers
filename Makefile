build:
	cmake --build cmake-build-debug --target flowers -- -j 2

run:
	./cmake-build-debug/flowers
