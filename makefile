make: src/*.cpp
	clang++ src/*.cpp -I /opt/homebrew/include/ -std=c++17 -fsanitize=address -g3 -L /opt/homebrew/lib/  -I /usr/local/include/ -L /usr/local/lib/ -l raylib
