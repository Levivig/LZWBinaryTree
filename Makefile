build:

	@echo "Compiling code.."
	@g++ main.cpp -o lzw -std=c++11 -Wall
	@echo "Done!"
clean:
	@echo "Removing.."
	@rm lzw
	@echo "Done"
