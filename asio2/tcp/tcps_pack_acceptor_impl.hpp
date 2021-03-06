/*
 * COPYRIGHT (C) 2017, zhllxt
 *
 * author   : zhllxt
 * qq       : 37792738
 * email    : 37792738@qq.com
 *
 */

#ifndef __ASIO2_TCPS_PACK_ACCEPTOR_IMPL_HPP__
#define __ASIO2_TCPS_PACK_ACCEPTOR_IMPL_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <asio2/tcp/tcps_acceptor_impl.hpp>
#include <asio2/tcp/tcps_pack_session_impl.hpp>

namespace asio2
{

	template<class _session_impl_t>
	class tcps_pack_acceptor_impl : public tcps_acceptor_impl<_session_impl_t>
	{
	public:

		typedef _session_impl_t session_impl_t;
		typedef typename _session_impl_t::pool_t pool_t;

		using parser_callback = std::size_t(std::shared_ptr<buffer<uint8_t>> data_ptr);

		/**
		 * @construct
		 */
		tcps_pack_acceptor_impl(
			io_service_pool_ptr ioservice_pool_ptr,
			std::shared_ptr<listener_mgr> listener_mgr_ptr,
			std::shared_ptr<url_parser> url_parser_ptr,
			std::shared_ptr<pool_s> send_buf_pool_ptr,
			std::shared_ptr<pool_t> recv_buf_pool_ptr,
			std::shared_ptr<boost::asio::ssl::context> context_ptr = nullptr,
			std::function<parser_callback> pack_parser = nullptr
		)
			: tcps_acceptor_impl<_session_impl_t>(
				ioservice_pool_ptr,
				listener_mgr_ptr,
				url_parser_ptr,
				send_buf_pool_ptr,
				recv_buf_pool_ptr,
				context_ptr
				)
			, m_pack_parser(pack_parser)
		{
		}

		/**
		 * @destruct
		 */
		virtual ~tcps_pack_acceptor_impl()
		{
		}

	protected:
		virtual std::shared_ptr<_session_impl_t> _prepare_session() override
		{
			// get a session shared_ptr from session manager
			try
			{
				// the params of get_session is final passed to session constructor
				std::shared_ptr<_session_impl_t> session_ptr = m_session_mgr_ptr->get_session(
					m_ioservice_pool_ptr->get_io_service_ptr(),
					m_listener_mgr_ptr,
					m_url_parser_ptr,
					m_send_buf_pool_ptr,
					m_recv_buf_pool_ptr,
					m_context_ptr,
					m_pack_parser
				);

				return session_ptr;
			}
			// handle exception,may be is the exception "Too many open files" (exception code : 24)
			catch (boost::system::system_error & e)
			{
				set_last_error(e.code().value());

				PRINT_EXCEPTION;
			}

			return nullptr;
		}

	protected:
		
		std::function<parser_callback>       m_pack_parser;

	};


}

#endif // !__ASIO2_TCPS_PACK_ACCEPTOR_IMPL_HPP__
