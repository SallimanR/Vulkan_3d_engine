BASE_OUTPUT_DIR="./assets/spirv/"

if [ ! -d "$BASE_OUTPUT_DIR" ]; then
	mkdir "$BASE_OUTPUT_DIR"
fi

/usr/bin/glslc ./assets/shaders/triangle.vert -o ./assets/spirv/triangle.vert.spv
/usr/bin/glslc ./assets/shaders/triangle.frag -o ./assets/spirv/triangle.frag.spv
