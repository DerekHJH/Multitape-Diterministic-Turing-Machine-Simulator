.PHONY: run vim
run:
	@g++ turing.cpp -o turing
	@./turing eg.tm 100010001 --verbose
vim:
	@vim turing.cpp
