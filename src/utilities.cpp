#include <precomp.h>
#include <utilities.h>

namespace vulkanEng
{
    bool streq(gsl::czstring left, gsl::czstring right)
    {
        return std::strcmp(left, right) == 0;
    }
}
