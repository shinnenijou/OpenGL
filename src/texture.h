#ifndef TEXTURE_H
#define TEXTURE_H

#include "renderer.h"

class Texture
{
private:
    unsigned int m_rendererID;
    std::string m_filePath;
    unsigned char *m_localBuffer;
    int m_width, m_height, m_BPP;
public:
    Texture(const std::string &path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

};


#endif
