#version 330 core
out vec4 f_color;
  
in vec2 v_uv;

uniform sampler2D u_texture;


void main()
{ 
    /* vec3 col;
    float amount = 0.001;
    col.r = texture( u_texture, vec2(v_uv.x,v_uv.y-amount)).r;
    col.g =texture(u_texture, v_uv).g;
    col.b = texture( u_texture, vec2(v_uv.x-amount,v_uv.y)).b;

	col *= (1.0 - amount * 0.5);
    
    f_color = vec4(col,1); */
    f_color = texture(u_texture, v_uv);
}
