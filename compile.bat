if exist ElysiumEngine\shaders rmdir /s /q ElysiumEngine\shaders
if not exist ElysiumEngine\shaders mkdir ElysiumEngine\shaders

glslc engine\shaders\vertex_core.vert -o ElysiumEngine\shaders\vertex_core.vert.spv
glslc engine\shaders\fragment_core.frag -o ElysiumEngine\shaders\fragment_core.frag.spv