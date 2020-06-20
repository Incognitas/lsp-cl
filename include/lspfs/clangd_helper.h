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

  bool startProcess();
  bool running() const;
  bool write(const ByteArray& data);
  void read();

private:
  void handle_banner(const boost::system::error_code& ec, size_t) const;
  void handle_header(const boost::system::error_code ec);
  void handle_content(const boost::system::error_code ec);
  void handle_write(const boost::system::error_code ec);

  boost::process::child m_childProcess;
  boost::asio::io_service m_ios;
  boost::process::async_pipe m_asyncIn;
  boost::process::async_pipe m_asyncOut;
  boost::asio::streambuf m_inbuf;
  boost::asio::streambuf m_outbuf;
};

#endif // CLANGD_HELPER_H
