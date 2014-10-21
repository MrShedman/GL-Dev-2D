#pragma once

#ifdef _DEBUG

// In debug mode, perform a test on every OpenGL call
#define glCheck(x) x; sf::priv::glCheckError(__FILE__, __LINE__);

#else

// Else, we don't add any overhead
#define glCheck(call) (call)

#endif

////////////////////////////////////////////////////////////
/// \brief Check the last OpenGL error
///
/// \param file Source file where the call is located
/// \param line Line number of the source file where the call is located
///
////////////////////////////////////////////////////////////
void glCheckError(const char* file, unsigned int line);