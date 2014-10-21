#pragma once

struct ContextSettings
{
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param depth        Depth buffer bits
    /// \param stencil      Stencil buffer bits
    /// \param antialiasing Antialiasing level
    /// \param major        Major number of the context version
    /// \param minor        Minor number of the context version
    ///
    ////////////////////////////////////////////////////////////
	explicit ContextSettings(unsigned int depth = 0, unsigned int stencil = 0, unsigned int major = 2, unsigned int minor = 0, bool core = false) :
    depthBits        (depth),
    stencilBits      (stencil),
    majorVersion     (major),
    minorVersion     (minor),
	core			 (core)
    {
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int depthBits;         ///< Bits of the depth buffer
    unsigned int stencilBits;       ///< Bits of the stencil buffer
    unsigned int majorVersion;      ///< Major number of the context version to create
    unsigned int minorVersion;      ///< Minor number of the context version to create
	bool		 core;
};