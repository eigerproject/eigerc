#ifndef _EIGERC_CMDOPTS_HPP_
#define _EIGERC_CMDOPTS_HPP_

#include <string>

namespace EigerC {

struct CmdOptions {
    bool help = false, verbose = false, version = false;
    std::string source = "";
};

}  // namespace EigerC

#endif  // _EIGERC_CMDOPTS_HPP_
