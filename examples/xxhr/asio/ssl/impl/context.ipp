//
// ssl/impl/context.ipp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2005 Voipster / Indrek dot Juhani at voipster dot com
// Copyright (c) 2005-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef XXHR_ASIO_SSL_IMPL_CONTEXT_IPP
#define XXHR_ASIO_SSL_IMPL_CONTEXT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)
# include <cstring>
# include <boost/asio/detail/throw_error.hpp>
# include <boost/asio/error.hpp>
# include <xxhr/asio/ssl/context.hpp>
# include <boost/asio/ssl/error.hpp>
#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

#include <boost/asio/detail/push_options.hpp>

namespace xxhr {
namespace asio {
namespace ssl {


/////////// XXX: Remove
/*
 * The hardcoded trust anchors. These are the two DN + public key that
 * correspond to the self-signed certificates cert-root-rsa.pem and
 * cert-root-ec.pem.
 *
 * C code for hardcoded trust anchors can be generated with the "brssl"
 * command-line tool (with the "ta" command).
 */

static const unsigned char TA0_DN[] = {
	0x30, 0x1C, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
	0x02, 0x43, 0x41, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x03,
	0x13, 0x04, 0x52, 0x6F, 0x6F, 0x74
};

static const unsigned char TA0_RSA_N[] = {
	0xB6, 0xD9, 0x34, 0xD4, 0x50, 0xFD, 0xB3, 0xAF, 0x7A, 0x73, 0xF1, 0xCE,
	0x38, 0xBF, 0x5D, 0x6F, 0x45, 0xE1, 0xFD, 0x4E, 0xB1, 0x98, 0xC6, 0x60,
	0x83, 0x26, 0xD2, 0x17, 0xD1, 0xC5, 0xB7, 0x9A, 0xA3, 0xC1, 0xDE, 0x63,
	0x39, 0x97, 0x9C, 0xF0, 0x5E, 0x5C, 0xC8, 0x1C, 0x17, 0xB9, 0x88, 0x19,
	0x6D, 0xF0, 0xB6, 0x2E, 0x30, 0x50, 0xA1, 0x54, 0x6E, 0x93, 0xC0, 0xDB,
	0xCF, 0x30, 0xCB, 0x9F, 0x1E, 0x27, 0x79, 0xF1, 0xC3, 0x99, 0x52, 0x35,
	0xAA, 0x3D, 0xB6, 0xDF, 0xB0, 0xAD, 0x7C, 0xCB, 0x49, 0xCD, 0xC0, 0xED,
	0xE7, 0x66, 0x10, 0x2A, 0xE9, 0xCE, 0x28, 0x1F, 0x21, 0x50, 0xFA, 0x77,
	0x4C, 0x2D, 0xDA, 0xEF, 0x3C, 0x58, 0xEB, 0x4E, 0xBF, 0xCE, 0xE9, 0xFB,
	0x1A, 0xDA, 0xA3, 0x83, 0xA3, 0xCD, 0xA3, 0xCA, 0x93, 0x80, 0xDC, 0xDA,
	0xF3, 0x17, 0xCC, 0x7A, 0xAB, 0x33, 0x80, 0x9C, 0xB2, 0xD4, 0x7F, 0x46,
	0x3F, 0xC5, 0x3C, 0xDC, 0x61, 0x94, 0xB7, 0x27, 0x29, 0x6E, 0x2A, 0xBC,
	0x5B, 0x09, 0x36, 0xD4, 0xC6, 0x3B, 0x0D, 0xEB, 0xBE, 0xCE, 0xDB, 0x1D,
	0x1C, 0xBC, 0x10, 0x6A, 0x71, 0x71, 0xB3, 0xF2, 0xCA, 0x28, 0x9A, 0x77,
	0xF2, 0x8A, 0xEC, 0x42, 0xEF, 0xB1, 0x4A, 0x8E, 0xE2, 0xF2, 0x1A, 0x32,
	0x2A, 0xCD, 0xC0, 0xA6, 0x46, 0x2C, 0x9A, 0xC2, 0x85, 0x37, 0x91, 0x7F,
	0x46, 0xA1, 0x93, 0x81, 0xA1, 0x74, 0x66, 0xDF, 0xBA, 0xB3, 0x39, 0x20,
	0x91, 0x93, 0xFA, 0x1D, 0xA1, 0xA8, 0x85, 0xE7, 0xE4, 0xF9, 0x07, 0xF6,
	0x10, 0xF6, 0xA8, 0x27, 0x01, 0xB6, 0x7F, 0x12, 0xC3, 0x40, 0xC3, 0xC9,
	0xE2, 0xB0, 0xAB, 0x49, 0x18, 0x3A, 0x64, 0xB6, 0x59, 0xB7, 0x95, 0xB5,
	0x96, 0x36, 0xDF, 0x22, 0x69, 0xAA, 0x72, 0x6A, 0x54, 0x4E, 0x27, 0x29,
	0xA3, 0x0E, 0x97, 0x15
};

static const unsigned char TA0_RSA_E[] = {
	0x01, 0x00, 0x01
};

static const unsigned char TA1_DN[] = {
	0x30, 0x1C, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
	0x02, 0x43, 0x41, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x03,
	0x13, 0x04, 0x52, 0x6F, 0x6F, 0x74
};

static const unsigned char TA1_EC_Q[] = {
	0x04, 0x71, 0x74, 0xBA, 0xAB, 0xB9, 0x30, 0x2E, 0x81, 0xD5, 0xE5, 0x57,
	0xF9, 0xF3, 0x20, 0x68, 0x0C, 0x9C, 0xF9, 0x64, 0xDB, 0xB4, 0x20, 0x0D,
	0x6D, 0xEA, 0x40, 0xD0, 0x4A, 0x6E, 0x42, 0xFD, 0xB6, 0x9A, 0x68, 0x25,
	0x44, 0xF6, 0xDF, 0x7B, 0xC4, 0xFC, 0xDE, 0xDD, 0x7B, 0xBB, 0xC5, 0xDB,
	0x7C, 0x76, 0x3F, 0x41, 0x66, 0x40, 0x6E, 0xDB, 0xA7, 0x87, 0xC2, 0xE5,
	0xD8, 0xC5, 0xF3, 0x7F, 0x8D
};

static const br_x509_trust_anchor TAs[2] = {
	{
		{ (unsigned char *)TA0_DN, sizeof TA0_DN },
		BR_X509_TA_CA,
		{
			BR_KEYTYPE_RSA,
			{ .rsa = {
				(unsigned char *)TA0_RSA_N, sizeof TA0_RSA_N,
				(unsigned char *)TA0_RSA_E, sizeof TA0_RSA_E,
			} }
		}
	},
	{
		{ (unsigned char *)TA1_DN, sizeof TA1_DN },
		BR_X509_TA_CA,
		{
			BR_KEYTYPE_EC,
			{ .ec = {
				BR_EC_secp256r1,
				(unsigned char *)TA1_EC_Q, sizeof TA1_EC_Q,
			} }
		}
	}
};

#define TAs_NUM   2
////////// XXX : END OF REMOVE








#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)

context::context(context::method m)
  : handle_(0)
{
  switch (m)
  {
  case context::tlsv1:
  case context::tlsv11:
  case context::tlsv12:
    //TODO:
    break;
  case context::tlsv1_client:
  case context::tlsv11_client:
  case context::tlsv12_client:
    //TODO: Use folders of trusts anchors
    ::br_ssl_client_init_full(&sc, &xc, TAs, TAs_NUM);
    handle_ = &sc;
    break;
  case context::tlsv1_server:
  case context::tlsv11_server:
  case context::tlsv12_server:
		//br_ssl_server_init_full_ec(&sc, CHAIN, CHAIN_LEN,	BR_KEYTYPE_RSA, &SKEY);
    break;
  }

  auto err = ::br_ssl_engine_last_error(&handle_->eng);
  if (err != 0)
  {
    boost::system::error_code ec(
        err,
        boost::asio::error::get_ssl_category());
    boost::asio::detail::throw_error(ec, "context");
  }

  ////set_options(no_compression);
}

context::context(boost::asio::io_service&, context::method m)
   : handle_(0)
{
  context tmp(m);
  handle_ = tmp.handle_;
  tmp.handle_ = 0;
}

#if defined(BOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
context::context(context&& other)
{
  handle_ = other.handle_;
  other.handle_ = 0;
}

context& context::operator=(context&& other)
{
  context tmp(BOOST_ASIO_MOVE_CAST(context)(*this));
  handle_ = other.handle_;
  other.handle_ = 0;
  return *this;
}
#endif // defined(BOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

context::~context()
{
}

context::native_handle_type context::native_handle()
{
  return handle_;
}

context::impl_type context::impl()
{
  return handle_;
}

void context::clear_options(context::options o)
{
  boost::system::error_code ec;
  clear_options(o, ec);
  boost::asio::detail::throw_error(ec, "clear_options");
}

boost::system::error_code context::clear_options(
    context::options o, boost::system::error_code& ec)
{
  //TODO: Implement
  ec = boost::system::error_code();
  return ec;
}

void context::set_options(context::options o)
{
  boost::system::error_code ec;
  set_options(o, ec);
  boost::asio::detail::throw_error(ec, "set_options");
}

boost::system::error_code context::set_options(
    context::options o, boost::system::error_code& ec)
{
  //TODO: Implement
  ec = boost::system::error_code();
  return ec;
}

void context::set_verify_mode(verify_mode v)
{
  boost::system::error_code ec;
  set_verify_mode(v, ec);
  boost::asio::detail::throw_error(ec, "set_verify_mode");
}

boost::system::error_code context::set_verify_mode(
    verify_mode v, boost::system::error_code& ec)
{
  //TODO: ::SSL_CTX_set_verify(handle_, v, ::SSL_CTX_get_verify_callback(handle_));

  ec = boost::system::error_code();
  return ec;
}

void context::set_verify_depth(int depth)
{
  boost::system::error_code ec;
  set_verify_depth(depth, ec);
  boost::asio::detail::throw_error(ec, "set_verify_depth");
}

boost::system::error_code context::set_verify_depth(
    int depth, boost::system::error_code& ec)
{
  //TODO: ::SSL_CTX_set_verify_depth(handle_, depth);

  ec = boost::system::error_code();
  return ec;
}

void context::load_verify_file(const std::string& filename)
{
  boost::system::error_code ec;
  load_verify_file(filename, ec);
  boost::asio::detail::throw_error(ec, "load_verify_file");
}

boost::system::error_code context::load_verify_file(
    const std::string& filename, boost::system::error_code& ec)
{
  //TODO: implement 
  ec = boost::system::error_code();
  return ec;
}

void context::add_certificate_authority(const const_buffer& ca)
{
  boost::system::error_code ec;
  add_certificate_authority(ca, ec);
  boost::asio::detail::throw_error(ec, "add_certificate_authority");
}

boost::system::error_code context::add_certificate_authority(
    const const_buffer& ca, boost::system::error_code& ec)
{
  //TODO: implement 
  return ec;
}

void context::set_default_verify_paths()
{
  boost::system::error_code ec;
  set_default_verify_paths(ec);
  boost::asio::detail::throw_error(ec, "set_default_verify_paths");
}

boost::system::error_code context::set_default_verify_paths(
    boost::system::error_code& ec)
{
  //TODO: implement 
  ec = boost::system::error_code();
  return ec;
}

void context::add_verify_path(const std::string& path)
{
  boost::system::error_code ec;
  add_verify_path(path, ec);
  boost::asio::detail::throw_error(ec, "add_verify_path");
}

boost::system::error_code context::add_verify_path(
    const std::string& path, boost::system::error_code& ec)
{
  //TODO: implement 
  ec = boost::system::error_code();
  return ec;
}

void context::use_certificate(
    const const_buffer& certificate, file_format format)
{
  boost::system::error_code ec;
  use_certificate(certificate, format, ec);
  boost::asio::detail::throw_error(ec, "use_certificate");
}

boost::system::error_code context::use_certificate(
    const const_buffer& certificate, file_format format,
    boost::system::error_code& ec)
{
  //TODO : Implement

  if (format == context_base::asn1)
  {
    return ec;
  }
  else if (format == context_base::pem)
  {
    ec = boost::system::error_code();
    return ec;
  }
  else
  {
    ec = boost::asio::error::invalid_argument;
    return ec;
  }

  ec = boost::system::error_code(
      static_cast<int>(::br_ssl_engine_last_error(&handle_->eng)),
      boost::asio::error::get_ssl_category());
  return ec;
}

void context::use_certificate_file(
    const std::string& filename, file_format format)
{
  boost::system::error_code ec;
  use_certificate_file(filename, format, ec);
  boost::asio::detail::throw_error(ec, "use_certificate_file");
}

boost::system::error_code context::use_certificate_file(
    const std::string& filename, file_format format,
    boost::system::error_code& ec)
{
  //TODO: Implement
  ec = boost::system::error_code();
  return ec;
}

void context::use_certificate_chain(const const_buffer& chain)
{
  boost::system::error_code ec;
  use_certificate_chain(chain, ec);
  boost::asio::detail::throw_error(ec, "use_certificate_chain");
}

boost::system::error_code context::use_certificate_chain(
    const const_buffer& chain, boost::system::error_code& ec)
{
  //TODO: Implement

  return ec;
}

void context::use_certificate_chain_file(const std::string& filename)
{
  boost::system::error_code ec;
  use_certificate_chain_file(filename, ec);
  boost::asio::detail::throw_error(ec, "use_certificate_chain_file");
}

boost::system::error_code context::use_certificate_chain_file(
    const std::string& filename, boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

void context::use_private_key(
    const const_buffer& private_key, context::file_format format)
{
  boost::system::error_code ec;
  use_private_key(private_key, format, ec);
  boost::asio::detail::throw_error(ec, "use_private_key");
}

boost::system::error_code context::use_private_key(
    const const_buffer& private_key, context::file_format format,
    boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

void context::use_private_key_file(
    const std::string& filename, context::file_format format)
{
  boost::system::error_code ec;
  use_private_key_file(filename, format, ec);
  boost::asio::detail::throw_error(ec, "use_private_key_file");
}

void context::use_rsa_private_key(
    const const_buffer& private_key, context::file_format format)
{
  boost::system::error_code ec;
  use_rsa_private_key(private_key, format, ec);
  boost::asio::detail::throw_error(ec, "use_rsa_private_key");
}

boost::system::error_code context::use_rsa_private_key(
    const const_buffer& private_key, context::file_format format,
    boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

boost::system::error_code context::use_private_key_file(
    const std::string& filename, context::file_format format,
    boost::system::error_code& ec)
{
  //TODO: Implement
  ec = boost::system::error_code();
  return ec;
}

void context::use_rsa_private_key_file(
    const std::string& filename, context::file_format format)
{
  boost::system::error_code ec;
  use_rsa_private_key_file(filename, format, ec);
  boost::asio::detail::throw_error(ec, "use_rsa_private_key_file");
}

boost::system::error_code context::use_rsa_private_key_file(
    const std::string& filename, context::file_format format,
    boost::system::error_code& ec)
{
  //TODO: Implement
  ec = boost::system::error_code();
  return ec;
}

void context::use_tmp_dh(const const_buffer& dh)
{
  boost::system::error_code ec;
  use_tmp_dh(dh, ec);
  boost::asio::detail::throw_error(ec, "use_tmp_dh");
}

boost::system::error_code context::use_tmp_dh(
    const const_buffer& dh, boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

void context::use_tmp_dh_file(const std::string& filename)
{
  boost::system::error_code ec;
  use_tmp_dh_file(filename, ec);
  boost::asio::detail::throw_error(ec, "use_tmp_dh_file");
}

boost::system::error_code context::use_tmp_dh_file(
    const std::string& filename, boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

boost::system::error_code context::do_use_tmp_dh(
    BIO* bio, boost::system::error_code& ec)
{
  //TODO: Implement
  return ec;
}

//boost::system::error_code context::do_set_verify_callback(
//    detail::verify_callback_base* callback, boost::system::error_code& ec)
//{
//  //TODO: Implement
//  ec = boost::system::error_code();
//  return ec;
//}
//
//boost::system::error_code context::do_set_password_callback(
//    detail::password_callback_base* callback, boost::system::error_code& ec)
//{
//  //TODO: Implement
//  ec = boost::system::error_code();
//  return ec;
//}

#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

} // namespace ssl
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // XXHR_ASIO_SSL_IMPL_CONTEXT_IPP
