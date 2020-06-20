#include "clangd_helper.h"
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <iostream>

bool
ClangdHelper::running() const
{
  return this->m_childProcess.valid();
}

ClangdHelper::ClangdHelper()
  : m_asyncIn(m_ios)
  , m_asyncOut(m_ios)
{
}

// struct MatchCond
//{
//  MatchCond() = default;
//
//  template<typename Iterator>
//  std::pair<Iterator, bool> operator()(Iterator begin, Iterator end) const
//  {
//    const bool retval = std::regex_match(
//      std::string{ begin, end }, MatchCond::ms_regex, std::regex::icase);
//    return std::make_pair(end, retval);
//  }
//
// private:
//  inline static std::regex ms_regex{ "^.*?Starting LSP.*$" };
//};

bool
ClangdHelper::startProcess()
{

  this->m_childProcess = boost::process::child(
    "clangd.exe",
    boost::process::std_out > this->m_asyncOut,
    boost::process::std_in<this->m_asyncIn, boost::process::std_err> this
      ->m_asyncOut,
    this->m_ios);

  assert(this->m_childProcess.running());

  boost::asio::async_read_until(
    this->m_asyncOut,
    this->m_outbuf,
    /*MatchCond{}*/ boost::regex{ "^.*?Starting LSP.*$" },
    boost::bind(&ClangdHelper::handle_banner, this, _1, _2));

  std::thread t{ [this] { this->m_ios.run(); } };
  t.detach();
  this->m_childProcess.wait();

  return static_cast<bool>(this->m_childProcess);
}

bool
ClangdHelper::write(const ByteArray& data)
{
  std::ostream os(&this->m_inbuf);
  os.write(reinterpret_cast<const char*>(data.data()), data.size());

  boost::asio::async_write(this->m_asyncIn,
                           this->m_inbuf,
                           boost::bind(&ClangdHelper::handle_write, this, _1));
  return true;
}

void
ClangdHelper::handle_banner(const boost::system::error_code& ec,
                            size_t length) const
{
  std::cout << "Banner length read:" << length << "\n";
  if (!ec)
  {
    /*std::cout << "Banner : "
              << std::string{ boost::asio::buffers_begin(this->m_outbuf.data()),
       boost::asio::buffers_end(this->m_outbuf.data()) }
              << std::endl;*/
  }
  else
  {
    std::cerr << ec.message() << std::endl;
  }
}

void
ClangdHelper::handle_write(const boost::system::error_code ec)
{
  if (!ec)
  {
    this->read();
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
  boost::asio::async_read_until(
    this->m_asyncOut,
    this->m_outbuf,
    "\r\n",
    boost::bind(&ClangdHelper::handle_header, this, _1));
}

void
ClangdHelper::handle_header(const boost::system::error_code ec)
{
  static size_t content_length_to_read;
  if (!ec)
  {
    std::string method;
    char sp1, sp2, cr, lf;
    std::istream is(&this->m_outbuf);

    // we are expecting a header at this point
    // if we have reached directly '\r' then this means that next reading will
    // be the content length
    if (is.peek() != '\r')
    {
      // handle header from here
      is >> method >> sp1 >> sp2 >> content_length_to_read >> cr >> lf;
      std::cout << "Received : " << method << " >> " << content_length_to_read;
      // read next line
      boost::asio::async_read_until(
        this->m_asyncOut,
        this->m_outbuf,
        "\r\n",
        boost::bind(&ClangdHelper::handle_header, this, _1));
    }
    else
    {
      boost::asio::async_read(
        this->m_asyncOut,
        this->m_outbuf,
        boost::asio::transfer_exactly(content_length_to_read),
        boost::bind(&ClangdHelper::handle_content, this, _1));
    }
  }
  else
  {
    std::cerr << "Error occurred when reading content: " << ec.message()
              << "\n";
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