#ifndef WRAPPER_H
#define WRAPPER_H

#if defined(__APPLE__)
    #define ASSERT(x) if(!(x)) __builtin_debugtrap();
#elif defined(_MSC_VER) 
    #define ASSERT(x) if(!(x)) __debugbreak();
#endif

#if defined(NDEBUG)
    #define GLCall(x) x;
#else
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#endif

void GLClearError();

/* NOTE: Platform-dependent */
bool GLLogCall(const char* function, const char* file, int line);

#endif