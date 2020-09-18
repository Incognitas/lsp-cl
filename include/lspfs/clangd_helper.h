#ifndef CLANGD_HELPER_H
#define CLANGD_HELPER_H

#include <cstdint>
#include <memory>
#include <sdkddkver.h>
#include <vector>
#include <thread>

#include <boost/asio.hpp>
#include <boost/process.hpp>

using ByteArray = std::vector<uint8_t>;

class ClangdHelper
{
public:
  ClangdHelper();
  ~ClangdHelper();
  /**
   * @brief Update working directory in which clangd can be launched when calling \ref ClangdHelper::startProcess()
   *
   * @param[in] newWorkingDir The new current working directory
   */
  bool setWorkingDirectory(const std::string& newWorkingDir) noexcept;
  size_t currentProcessId() const;
  bool startProcess();
  bool running() const;
  bool write(const ByteArray& data);
  bool write(const std::string& data);
  void read();
  
private:
  void handle_stderr_messages(const boost::system::error_code& ec, size_t);
  void handle_header(const boost::system::error_code ec);
  void handle_content(const boost::system::error_code ec);
  void handle_write(const boost::system::error_code& ec, size_t);

  boost::process::child m_childProcess;
  boost::asio::io_service m_ios;
  boost::process::async_pipe m_asyncIn;
  boost::process::async_pipe m_asyncOut;
  boost::process::async_pipe m_asyncErr;
  boost::asio::streambuf m_inbuf;
  boost::asio::streambuf m_outbuf;
  boost::asio::streambuf m_errbuf;
  std::string m_newWorkingDirectory;
};

#endif // CLANGD_HELPER_H
