/*
 * COPYRIGHT (C) 2017, zhllxt
 *
 * author   : zhllxt
 * qq       : 37792738
 * email    : 37792738@qq.com
 * 
 */

#ifndef __ASIO2_TCPS_CLIENT_IMPL_HPP__
#define __ASIO2_TCPS_CLIENT_IMPL_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <asio2/tcp/tcps_connection_impl.hpp>

#include <asio2/tcp/tcp_client_impl.hpp>

namespace asio2
{

	template<class _connection_impl_t>
	class tcps_client_impl : public tcp_client_impl<_connection_impl_t>
	{
	public:

		typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

		/**
		 * @construct
		 */
		explicit tcps_client_impl(
			std::shared_ptr<listener_mgr> listener_mgr_ptr,
			std::shared_ptr<url_parser> url_parser_ptr
		)
			: tcp_client_impl<_connection_impl_t>(listener_mgr_ptr, url_parser_ptr)
		{
			this->m_context_ptr = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
		}

		/**
		 * @destruct
		 */
		virtual ~tcps_client_impl()
		{
		}

		virtual bool start(bool async_connect = true) override
		{
			return tcp_client_impl<_connection_impl_t>::start(async_connect);
		}

		virtual void stop() override
		{
			tcp_client_impl<_connection_impl_t>::stop();
		}

		inline std::shared_ptr<boost::asio::ssl::context> get_context_ptr() { return this->m_context_ptr; }

	protected:

		virtual void _prepare_connection() override
		{
			tcp_client_impl<_connection_impl_t>::_prepare_connection();

			if (this->m_connection_impl_ptr)
			{
				std::dynamic_pointer_cast<_connection_impl_t>(this->m_connection_impl_ptr)->set_context(this->m_context_ptr);
			}
		}

	protected:

		/// ssl context 
		std::shared_ptr<boost::asio::ssl::context> m_context_ptr;

	};

}

#endif // !__ASIO2_TCPS_CLIENT_IMPL_HPP__
