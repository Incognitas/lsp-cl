#include <sdkddkver.h>

#include <lspfs/clangd_helper.h>
#include <cassert>
#include <lspfs/messages/initialize_request.h>
#include <lspfs/messages/didopen_notification.h>
#include <iostream>


int
main(int argc, char** argv)
{
  ClangdHelper helper;
  assert(helper.setWorkingDirectory(WORKINGDIRECTORY));
  helper.startProcess();

  {
    lspfs::InitializeRequestMessage m{
      lspfs::InitializeRequestMessage::create()
        .withRootUri("file://D:/Developpement/sandbox_lsp/src")
        .withProcessid(helper.currentProcessId())
        .tracingLevel(lspfs::InitializeRequestMessageBuilder::TraceLevels::VERBOSE)
    };

    std::ostringstream finalData;
    const auto content = m.serialize();
    finalData << "Content-Length: " << content.length() << "\r\n\r\n" << content;
    std::cout << finalData.str() << std::endl;

    helper.write(finalData.str());
    helper.read();
  }
  
  /*{

    DidOpenNotificationMessage m{ "file://D:/Developpement/sandbox_lsp/src/clangd_helper.cpp" };
    const std::string& content = m.serialize();

    std::ostringstream finalData;

    finalData << "Content-Length: " << content.length() << "\r\n\r\n" << content;
    std::cout << finalData.str() << std::endl;

    helper.write(finalData.str());
  }*/

  std::this_thread::sleep_for(std::chrono::seconds(3));

  

  
}
