#include "lspfs/clangd_helper.h"
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <filesystem>
#include <iostream>

bool
ClangdHelper::running() const
{
  return this->m_childProcess.valid();
}

ClangdHelper::ClangdHelper()
  : m_asyncIn(m_ios)
  , m_asyncOut(m_ios)
  , m_asyncErr(m_ios)
{
}

ClangdHelper::~ClangdHelper() 
{
  if (this->m_childProcess.running())
  {
    this->m_childProcess.terminate();
    this->m_childProcess.wait();
  }
}

size_t
ClangdHelper::currentProcessId() const
{
  return boost::this_process::get_id();
}

bool
ClangdHelper::setWorkingDirectory(const std::string& newWorkingDir) noexcept
{
  const std::filesystem::path fullPath{ std::filesystem::absolute(
    newWorkingDir) };

  if( std::filesystem::exists(fullPath) )
  {
    this->m_newWorkingDirectory = fullPath.string();
    return true;
  }

  return false;
}

bool
ClangdHelper::startProcess()
{

  this->m_childProcess = boost::process::child(
    "clangd.exe",
    boost::process::start_dir(this->m_newWorkingDirectory ),
    boost::process::std_out > this->m_asyncOut,
    boost::process::std_in < this->m_asyncIn, 
    //boost::process::std_err > boost::process::null,
    boost::process::std_err > this->m_asyncErr,
    this->m_ios);

  assert(this->m_childProcess.running());

  boost::asio::async_read(
    this->m_asyncErr,
    this->m_errbuf,
    boost::asio::transfer_at_least(1),
    boost::bind(&ClangdHelper::handle_stderr_messages, this, _1, _2));

  std::thread t{ [this] {
    while (this->running())
    {
      this->m_ios.run();
      //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  } };
  t.detach();
  

  return static_cast<bool>(this->m_childProcess);
}

bool
ClangdHelper::write(const ByteArray& data)
{
  std::ostream os(&this->m_inbuf);
  os.write(reinterpret_cast<const char*>(data.data()), data.size());

  boost::asio::async_write(this->m_asyncIn,
                           this->m_inbuf,
                           boost::bind(&ClangdHelper::handle_write, this, _1, _2));
  return true;
}


bool
ClangdHelper::write(const std::string& data)
{
  std::ostream os(&this->m_inbuf);
  os.write(reinterpret_cast<const char*>(data.data()), data.size());

  boost::asio::async_write(
    this->m_asyncIn, this->m_inbuf, boost::bind(&ClangdHelper::handle_write, this, _1, _2));
  return true;
}


void
ClangdHelper::handle_stderr_messages(const boost::system::error_code& ec,
                                     size_t length)
{
  if (!ec)
  {
    std::cerr << "Received " << length << " bytes" << std::endl;
    std::cout << std::string{ boost::asio::buffers_begin(this->m_errbuf.data()),
                              boost::asio::buffers_end(this->m_errbuf.data()) };

    // now that banner has been read, we just have to remove bytes accordingly
    this->m_errbuf.consume(length);

    boost::asio::async_read(this->m_asyncErr,
                            this->m_errbuf,
                            boost::asio::transfer_at_least(1),
                            // boost::regex{ "^.*?Starting LSP.*$" },
                            boost::bind(&ClangdHelper::handle_stderr_messages, this, _1, _2));
  }
  else
  {
    std::cerr << ec.message() << std::endl;
  }
}

void
ClangdHelper::handle_write(const boost::system::error_code& ec, size_t length)
{
  if (!ec)
  {
    std::cout << "Successfully written " << length << " characters" << std::endl;
  }
  else
  {
    std::cerr << "Error occurred when writing content: " << ec.message()
              << "\n";
  }
}


void
ClangdHelper::read()
{
  boost::asio::async_read_until(this->m_asyncOut,
                                this->m_outbuf,
                                boost::regex("\r\n"),
                                boost::bind(&ClangdHelper::handle_content_length,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));


}

void
ClangdHelper::handle_content_length(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (!ec)
  { 
    size_t content_length_to_read;
    std::string content_header;
    
    
    std::istream is(&this->m_outbuf);

    // read total content length
    is >> content_header >> content_length_to_read;
    std::cout << __FUNCTION__ << " : actual content bytes transferred :" << bytes_transferred
              << std::endl;
    std::cerr << __FUNCTION__ << ":  total length : " << content_length_to_read << std::endl;
    this->m_outbuf.consume(2);
    
    // read full content
    boost::asio::async_read_until(this->m_asyncOut,
                                  this->m_outbuf,
                                  boost::regex("\r\n"),
                                  boost::bind(&ClangdHelper::handle_content_type,
                                              this,
                                              content_length_to_read,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message()
              << std::endl;
  }
}

void
ClangdHelper::handle_content_type(const size_t expectedContentLengthToReadAfter,
                                  const boost::system::error_code& ec,
                                  size_t bytes_transferred)
{
  if (!ec)
  {
    // ignore this particular value for now (the content type)
    std::cout << __FUNCTION__ << " : actual content bytes transferred :" << bytes_transferred
              << std::endl;
    std::cerr << __FUNCTION__ << " - " << expectedContentLengthToReadAfter << " - "
              << m_outbuf.size() << std::endl;
      
    // remove bytes read for current reading
    this->m_outbuf.consume(bytes_transferred); 

    // read the actuel content from now
    boost::asio::async_read(
      this->m_asyncOut,
      this->m_outbuf,
      // as we used async_read_until(...) before, we may have already more data that expected in the
      // buffer so we must substract the already obtained size to the total size to read
      boost::asio::transfer_exactly(expectedContentLengthToReadAfter - m_outbuf.size()),
      boost::bind(&ClangdHelper::handle_content,
                  this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message() << std::endl;
  }
}

void
ClangdHelper::handle_content(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (!ec)
  {
    // now at this point we shall have pure json object
    std::cout << __FUNCTION__ << " : actual content bytes transferred :" << bytes_transferred << std::endl;
    const std::string fullContent{ boost::asio::buffers_begin(this->m_outbuf.data()),
                             boost::asio::buffers_end(this->m_outbuf.data()) };

    std::cout << fullContent
              << std::endl;
    std::cout << "Size : " << fullContent.size() << std::endl;
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message()
              << "\n";
  }
}