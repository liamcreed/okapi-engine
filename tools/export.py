import bpy
import struct

# Function to export mesh data to a binary file
def export_binary_mesh(mesh, filepath):
    vertices = []
    normals = []
    uvs = []
    indices = []
    
    for v in mesh.vertices:
        vertices.extend(v.co)
        normals.extend(v.normal)
        for loop in v.link_loops:
            uvs.extend(mesh.uv_layers.active.data[loop.index].uv)
    
    for poly in mesh.polygons:
        indices.extend(poly.vertices)
    
    # Extract texture data
    materials = mesh.materials
    textures = []
    for material in materials:
        texture = material.texture_slots[0].texture if material.texture_slots and material.texture_slots[0] else None
        if texture and texture.type == 'IMAGE':
            image = texture.image
            texture_data = {
                "width": image.size[0],
                "height": image.size[1],
                "channel_count": 4,  # Assuming RGBA format
                "data": image.pixels[:],
            }
            textures.append(texture_data)
    
    # Pack data into binary format
    vertex_data = struct.pack(f'{len(vertices)}f', *vertices)
    normal_data = struct.pack(f'{len(normals)}f', *normals)
    uv_data = struct.pack(f'{len(uvs)}f', *uvs)
    index_data = struct.pack(f'{len(indices)}I', *indices)
    
    # Write data to binary file
    with open(filepath, 'wb') as f:
        f.write(vertex_data)
        f.write(normal_data)
        f.write(uv_data)
        f.write(index_data)
        for texture_data in textures:
            f.write(struct.pack('I', texture_data['width']))
            f.write(struct.pack('I', texture_data['height']))
            f.write(struct.pack('I', texture_data['channel_count']))
            f.write(struct.pack(f'{len(texture_data["data"])}f', *texture_data['data']))

# Get the selected mesh
selected_mesh = bpy.context.active_object.data

# Call the export function with the mesh and desired file path
export_binary_mesh(selected_mesh, "path/to/output.bin")