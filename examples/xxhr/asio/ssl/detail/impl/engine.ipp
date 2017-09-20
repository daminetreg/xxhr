//
// ssl/detail/impl/engine.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef XXHR_ASIO_SSL_DETAIL_IMPL_ENGINE_IPP
#define XXHR_ASIO_SSL_DETAIL_IMPL_ENGINE_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)
# include <boost/asio/detail/throw_error.hpp>
# include <boost/asio/error.hpp>
# include <xxhr/asio/ssl/detail/engine.hpp>
# include <boost/asio/ssl/error.hpp>
# include <boost/asio/ssl/verify_context.hpp>
#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

#include <boost/asio/detail/push_options.hpp>

namespace xxhr {
namespace asio {
namespace ssl {
namespace detail {

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)


engine::engine(br_ssl_engine_context* context)
  : ssl_(context)
{
  //if (!ssl_)
  //{
  //  boost::system::error_code ec(
  //      static_cast<int>(::br_ssl_engine_last_error(&context)),
  //      boost::asio::error::get_ssl_category());
  //  boost::asio::detail::throw_error(ec, "engine");
  //}

  accept_mutex().init();

//  ::SSL_set_mode(ssl_, SSL_MODE_ENABLE_PARTIAL_WRITE);
//  ::SSL_set_mode(ssl_, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
//#if defined(SSL_MODE_RELEASE_BUFFERS)
//  ::SSL_set_mode(ssl_, SSL_MODE_RELEASE_BUFFERS);
//#endif // defined(SSL_MODE_RELEASE_BUFFERS)

//  ::BIO* int_bio = 0;
//  ::BIO_new_bio_pair(&int_bio, 0, &ext_bio_, 0);
//  ::SSL_set_bio(ssl_, int_bio, int_bio);

  br_ssl_engine_set_buffer(ssl_, iobuf, sizeof iobuf, 1 /*BIDIRECTIONAL (can send and recv at the same time)*/);

 
}

engine::~engine()
{
//  if (SSL_get_app_data(ssl_))
//  {
//    delete static_cast<verify_callback_base*>(SSL_get_app_data(ssl_));
//    SSL_set_app_data(ssl_, 0);
//  }

//  ::BIO_free(ext_bio_);
//  ::SSL_free(ssl_);
}

br_ssl_engine_context* engine::native_handle()
{
  return ssl_;
}

boost::system::error_code engine::set_verify_mode(
    verify_mode v, boost::system::error_code& ec)
{
//TODO: Check how this can be implemented with bearssl
//  ::SSL_set_verify(ssl_, v, ::SSL_get_verify_callback(ssl_));

  ec = boost::system::error_code();
  return ec;
}

boost::system::error_code engine::set_verify_depth(
    int depth, boost::system::error_code& ec)
{
//TODO: Check how this can be implemented with bearssl
//  ::SSL_set_verify_depth(ssl_, depth);

  ec = boost::system::error_code();
  return ec;
}

//boost::system::error_code engine::set_verify_callback(
//    verify_callback_base* callback, boost::system::error_code& ec)
//{
////TODO: Check how this can be implemented with bearssl
////  if (SSL_get_app_data(ssl_))
////    delete static_cast<verify_callback_base*>(SSL_get_app_data(ssl_));
////
////  SSL_set_app_data(ssl_, callback);
////
////  ::SSL_set_verify(ssl_, ::SSL_get_verify_mode(ssl_),
////      &engine::verify_callback_function);
//
//  ec = boost::system::error_code();
//  return ec;
//}

//int engine::verify_callback_function(int preverified, X509_STORE_CTX* ctx)
//{
////TODO: Check how this can be implemented with bearssl
////  if (ctx)
////  {
////    if (SSL* ssl = static_cast<SSL*>(
////          ::X509_STORE_CTX_get_ex_data(
////            ctx, ::SSL_get_ex_data_X509_STORE_CTX_idx())))
////    {
////      if (SSL_get_app_data(ssl))
////      {
////        verify_callback_base* callback =
////          static_cast<verify_callback_base*>(
////              SSL_get_app_data(ssl));
////
////        verify_context verify_ctx(ctx);
////        return callback->call(preverified != 0, verify_ctx) ? 1 : 0;
////      }
////    }
////  }
////
////  return 0;
//}

engine::want engine::handshake(
    stream_base::handshake_type type, boost::system::error_code& ec)
{
  return perform((type == boost::asio::ssl::stream_base::client)
      ? &engine::do_connect : &engine::do_accept, 0, 0, ec, 0);
}

engine::want engine::shutdown(boost::system::error_code& ec)
{
  return perform(&engine::do_shutdown, 0, 0, ec, 0);
}

engine::want engine::write(const boost::asio::const_buffer& data,
    boost::system::error_code& ec, std::size_t& bytes_transferred)
{
  if (boost::asio::buffer_size(data) == 0)
  {
    ec = boost::system::error_code();
    return engine::want_nothing;
  }

  return perform(&engine::do_write,
      const_cast<void*>(boost::asio::buffer_cast<const void*>(data)),
      boost::asio::buffer_size(data), ec, &bytes_transferred);
}

engine::want engine::read(const boost::asio::mutable_buffer& data,
    boost::system::error_code& ec, std::size_t& bytes_transferred)
{
  if (boost::asio::buffer_size(data) == 0)
  {
    ec = boost::system::error_code();
    return engine::want_nothing;
  }

  return perform(&engine::do_read,
      boost::asio::buffer_cast<void*>(data),
      boost::asio::buffer_size(data), ec, &bytes_transferred);
}

boost::asio::mutable_buffers_1 engine::get_output(
    const boost::asio::mutable_buffer& data)
{
  size_t length;
	auto* buf_sendrec = br_ssl_engine_sendrec_buf(ssl_, &length);
	if (length > buffer_size(data)) {
		length = buffer_size(data);
	}
	memcpy(buffer_cast<void*>(data), buf_sendrec, length);
  ::br_ssl_engine_sendrec_ack(ssl_, length);

  return boost::asio::buffer(data,
      length > 0 ? static_cast<std::size_t>(length) : 0);
}

boost::asio::const_buffer engine::put_input(
    const boost::asio::const_buffer& data)
{
  size_t length{};
  auto* buf_recvrec = ::br_ssl_engine_recvrec_buf(ssl_, &length);
  //XXX: Check not zero (length)
  if (length > buffer_size(data)) {
    length = buffer_size(data);
  }
  //XXX: ?unsigned char or void* ?
  std::memcpy(buf_recvrec, boost::asio::buffer_cast<const unsigned char*>(data), length);
  ::br_ssl_engine_recvrec_ack(ssl_, length);

  return boost::asio::buffer(data +
      (length > 0 ? static_cast<std::size_t>(length) : 0));
}

const boost::system::error_code& engine::map_error_code(
    boost::system::error_code& ec) const
{
  // We only want to map the error::eof code.
  if (ec != boost::asio::error::eof)
    return ec;

  // If there's data yet to be read, it's an error.
  if (br_ssl_engine_current_state(ssl_) & BR_SSL_RECVAPP)
  {
    ec = boost::asio::ssl::error::stream_truncated;
    return ec;
  }

  // Otherwise, the peer should have negotiated a proper shutdown.
  if (::br_ssl_engine_closed(ssl_))
  {
    ec = boost::asio::ssl::error::stream_truncated;
  }

  return ec;
}

boost::asio::detail::static_mutex& engine::accept_mutex()
{
  static boost::asio::detail::static_mutex mutex = BOOST_ASIO_STATIC_MUTEX_INIT;
  return mutex;
}

engine::want engine::perform(int (engine::* op)(void*, std::size_t),
    void* data, std::size_t length, boost::system::error_code& ec,
    std::size_t* bytes_transferred)
{
  int result = (this->*op)(data, length);
  auto state = ::br_ssl_engine_current_state(ssl_);
  auto ssl_error = ::br_ssl_engine_last_error(ssl_);

  if (ssl_error != 0)
  {
    ec = boost::system::error_code(ssl_error,
        boost::asio::error::get_ssl_category());
    return want_nothing;
  }

  if (result > 0 && bytes_transferred)
    *bytes_transferred = static_cast<std::size_t>(result);

  if (state & BR_SSL_SENDAPP)
  {
    ec = boost::system::error_code();
    return want_output_and_retry;
  }
  //else if (pending_output_after > pending_output_before)
  //{
  //  ec = boost::system::error_code();
  //  return result > 0 ? want_output : want_output_and_retry;
  //}
  else if (state & BR_SSL_RECVAPP)
  {
    ec = boost::system::error_code();
    return want_input_and_retry;
  }
  else if (state &  BR_SSL_CLOSED)
  {
    ec = boost::asio::error::eof;
    return want_nothing;
  }
  else
  {
    ec = boost::system::error_code();
    return want_nothing;
  }
}

int engine::do_accept(void*, std::size_t)
{
  boost::asio::detail::static_mutex::scoped_lock lock(accept_mutex());
  server_ctx.eng = *ssl_;
  return ::br_ssl_server_reset(&server_ctx);
}

int engine::do_connect(void*, std::size_t)
{
  client_ctx.eng = *ssl_;
  return ::br_ssl_client_reset(&client_ctx, nullptr, 0);
}

int engine::do_shutdown(void*, std::size_t)
{
  //XXX: Might require a flush ? and send the byte count.
  ::br_ssl_engine_close(ssl_);
  return 0;
}

int engine::do_read(void* data, std::size_t length)
{
  size_t alen;
	auto* buf_recvapp = br_ssl_engine_recvapp_buf(ssl_, &alen);
	if (alen > length) {
		alen = length;
	}
	memcpy(data, buf_recvapp, alen);
	br_ssl_engine_recvapp_ack(ssl_, alen);

  //XXX: Needs better bounding length < INT_MAX ? static_cast<int>(length) : INT_MAX);
  return (int)alen;
}

int engine::do_write(void* data, std::size_t length)
{
	unsigned char *buf;
	size_t alen;

	if (length == 0) {
		return 0;
	}
  //XXX: Needs a check to flush before writing ? Buffer might be full of records to send
	buf = br_ssl_engine_sendapp_buf(ssl_, &alen);
	if (alen > length) {
		alen = length;
	}
	memcpy(buf, data, alen);
	br_ssl_engine_sendapp_ack(ssl_, alen);
  
  //XXX: Needs better bounding length < INT_MAX ? static_cast<int>(length) : INT_MAX);
	return (int)alen;
}

#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // XXHR_ASIO_SSL_DETAIL_IMPL_ENGINE_IPP
