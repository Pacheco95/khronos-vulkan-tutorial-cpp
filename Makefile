.PHONY: $(MAKECMDGOALS)

clean:
	rm -rf cmake-build*
	find res/shaders -type f -iname \*.spv -delete

format:
	find src -type f \
		-name "CMakeLists.txt" -o \
		-name "*.cpp" -o \
		-name "*.hpp" | xargs clang-format -i
