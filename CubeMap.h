#ifndef CUBEMAP_HH
#define CUBEMAP_HH

#include <string>
#include <vector>

class CubeMap {
public:
    unsigned int ID;
    // should cube maps have texture units?
    //size_t unit;

    CubeMap(const std::vector<std::string>& faces);
    void bind(void);

    ~CubeMap(void);
};

#endif