#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <string>
#include <vector>

enum class Wrap_type {
    RPT,
    MRP,
    CTE,
    CTB
};

enum class Filter_type {
    NER,
    LIN,
    NMN,
    LMN,
    NML,
    LML
};

class Texture {
public:
    unsigned int ID;
    // might be better not to associate each texture with a texture unit
    size_t unit;
    
    Texture(const std::string& name);
    void setWrapMode(Wrap_type type);
    void setMinFilter(Filter_type type);
    void setMagFilter(Filter_type type);
    void setBoarderCol(std::vector<float>& rgba);
    void bind(void);

    ~Texture(void);
};

#endif