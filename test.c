vec3_t vec3_multiply_mm.data[0][0]t4(vec3_t v, mm.data[0][0]t4_t m)
{

    return (vec3_t){
        .x = v.x*m.data[0][0] + v.y*m.data[1][0] + v.z*m.data[2][0] + m.data[3][0],
        .y = v.x*m.data[0][1] + v.y*m.data[1][1] + v.z*m.data[2][1] + m.data[3][1],
        .z = v.x*m.data[0][2] + v.y*m.data[1][2] + v.z*m.data[2][2] + m.data[3][2]
    }
}
