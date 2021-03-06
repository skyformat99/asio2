// compile application on linux system can use below command :
// g++ -std=c++11 -lpthread -lrt -ldl main.cpp -o main.exe -I /usr/local/include -I ../../asio2 -L /usr/local/lib -l boost_system -Wall

#include <clocale>
#include <climits>
#include <csignal>
#include <ctime>
#include <locale>
#include <limits>
#include <thread>
#include <chrono>
#include <iostream>

#if defined(_MSC_VER)
#	pragma warning(disable:4996)
#endif

#include <asio2/asio2.hpp>

class user_udp_server_listener : public asio2::udp_server_listener
{
public:
	virtual void on_send(asio2::session_ptr session_ptr, asio2::buffer_ptr data_ptr, int error) override
	{
	}
	virtual void on_recv(asio2::session_ptr session_ptr, asio2::buffer_ptr data_ptr) override
	{
		std::printf("recv : %.*s\n", (int)data_ptr->size(), (const char*)data_ptr->data());

		session_ptr->send(data_ptr);
	}
	virtual void on_close(asio2::session_ptr session_ptr, int error) override
	{
	}
	virtual void on_listen() override
	{
	}
	virtual void on_accept(asio2::session_ptr session_ptr) override
	{
	}
	virtual void on_shutdown(int error) override
	{
	}
};

volatile bool run_flag = true;

int main(int argc, char *argv[])
{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)
	// Detected memory leaks on windows system,linux has't these function
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// the number is the memory leak line num of the vs output window content.
	//_CrtSetBreakAlloc(1640); 
#endif

	std::signal(SIGINT, [](int signal) { run_flag = false; });

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	while (run_flag)
	{
		asio2::server udp_server("udp://*:9530/?send_buffer_size=256m&recv_buffer_size=256m&pool_buffer_size=1024");
		udp_server.bind_listener(std::make_shared<user_udp_server_listener>());
		if (!udp_server.start())
			std::printf("start udp server failed : %d - %s.\n", asio2::get_last_error(), asio2::get_last_error_desc().c_str());
		else
			std::printf("start udp server successed : %s - %u\n", udp_server.get_listen_address().c_str(), udp_server.get_listen_port());

		//-----------------------------------------------------------------------------------------
		std::thread([&]()
		{
			while (run_flag)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			}
		}).join();

		std::printf(">> ctrl + c is pressed,prepare exit...\n");
	}

	std::printf(">> leave main \n");

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)
	//system("pause");
#endif

	return 0;
};
