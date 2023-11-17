.PHONY: $(MAKECMDGOALS)

clean:
	rm -rf build*

format:
	find src -type f \
		-name "CMakeLists.txt" -o \
		-name "*.cpp" -o \
		-name "*.hpp" | xargs clang-format -i
