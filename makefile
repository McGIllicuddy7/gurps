make: src/*.cpp
	clang++ src/*.cpp -I /opt/homebrew/include/ -std=c++26 -fsanitize=address -g3 -l raylib -L /opt/homebrew/lib/  -O3