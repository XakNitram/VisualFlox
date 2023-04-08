# Resources
* In this simple application, resources don't need to be unloaded. We can take the data in any form we want and just delete it after it's loaded.

### Resource Super Class
```c++
class Resource {
    unique_ptr<float[]>    m_tex_coords;
    unique_ptr<float[]>    m_vertices;
    unique_ptr<float[]>    m_normals;
    unique_ptr<int[]>      m_indices;
    unique_ptr<Texture[]>  m_textures;
    unique_ptr<Material[]> m_materials;
    PrimitiveMode m_primitive_mode;
    bool m_is_indexed;
public:
};
```

### Resource Tree Class
```c++
enum class ResourceType {
    Texture, Material, Vertices, TexCoords, Normals, Indices
};

class ResourceComponent {
    
};

class Resource {
    vector<ResourceComponent> m_components;
};
```
