#include <sdkddkver.h>

#pragma warning(push)
#pragma warning(disable : 4267 4189)
#include <boost/asio.hpp>
#include <boost/process.hpp>
#pragma warning(pop)
#include <nlohmann/json.hpp>
#include "clangd_helper.h"

int
main(int argc, char** argv)
{
  ClangdHelper helper;
  helper.startProcess();

  
}
