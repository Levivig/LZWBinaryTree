build:

	@echo "Compiling code.."
	@g++ -O3 -std=c++11 main.cpp -o lzw -Wall
	@echo "Done!"
clean:
	@echo "Removing.."
	@rm lzw
	@echo "Done"
