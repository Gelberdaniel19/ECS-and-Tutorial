pong:
	@echo "Compiling"
	@g++ -o out pong.cpp -lncurses
	@echo "Running"
	@./out
	@echo "Cleaning"
	@rm out

tutorial:
	@echo "Compiling"
	@g++ -o out tutorial.cpp
	@echo "Running"
	@./out
	@echo "Cleaning"
	@rm out
