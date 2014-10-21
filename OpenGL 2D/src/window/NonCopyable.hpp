#pragma once

class NonCopyable
{
protected:

    NonCopyable() {}

private:

    NonCopyable(const NonCopyable&);

    NonCopyable& operator =(const NonCopyable&);
};