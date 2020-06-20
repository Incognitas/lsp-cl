#include <sdkddkver.h>

#include <lspfs/clangd_helper.h>
#include <cassert>

int
main(int argc, char** argv)
{
  ClangdHelper helper;
  assert(helper.setWorkingDirectory(WORKINGDIRECTORY));
  helper.startProcess();

  
}
