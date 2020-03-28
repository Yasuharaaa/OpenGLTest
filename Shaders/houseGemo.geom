#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
    vec3 color;
}gs_in[];

out vec3 fColor;
vec3 getNormal(){
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[2].gl_Position);
    return normalize(cross(a, b));
}

void build_house(vec4 position){
    fColor = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main(){
    build_house(gl_in[0].gl_Position);
}
