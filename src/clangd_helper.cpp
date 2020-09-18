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
    //boost::regex{ "^.*?Starting LSP.*$" },
    boost::bind(&ClangdHelper::handle_stderr_messages, this, _1, _2));

  std::thread t{ [this] {
    while (this->running())
    {
      this->m_ios.run();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
  /*boost::asio::async_read_until(
    this->m_asyncOut,
    this->m_outbuf,
    R"(^.*?Content-Length:\s*\d+$)",
    boost::bind(&ClangdHelper::handle_header, this, _1));*/

  /*boost::asio::async_read(this->m_asyncErr,
                          this->m_errbuf,
                          boost::bind(&ClangdHelper::handle_stderr_messages, this, _1, _2));*/

  boost::asio::async_read(this->m_asyncOut,
                            this->m_outbuf, 
      //"Content-Length: \\d+",
                            boost::bind(&ClangdHelper::handle_header, this, _1));


}

void
ClangdHelper::handle_header(const boost::system::error_code ec)
{
  std::cerr << "plop" << std::endl;
  if (!ec)
  { /*
    size_t content_length_to_read;
    std::string method;
    char sp1, sp2, cr, lf;
    std::istream is(&this->m_outbuf);

    // TODO :handle content length
    // we are expecting a header at this point
    // this means that next reading will be the content length
    
      //std::cout << method << std::endl;
      // handle header from here
      is >> method >> sp1 >> sp2 >> content_length_to_read >> cr >> lf;
      std::cout << "Received : " << method << " >> " << content_length_to_read << std::endl;

      // read full content
      std::cout << "[other] received...";
      boost::asio::async_read(this->m_asyncOut,
                              this->m_outbuf,
                              boost::asio::transfer_exactly(content_length_to_read),
                              boost::bind(&ClangdHelper::handle_content, this, _1)); 
    
   
      */
      
    
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message()
              << std::endl;
  }
}

void
ClangdHelper::handle_content(const boost::system::error_code ec)
{
  if (!ec)
  {
    // now at this point we shall have pure json object
    std::cout << "Content to read !" << std::endl;
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message()
              << "\n";
  }
}