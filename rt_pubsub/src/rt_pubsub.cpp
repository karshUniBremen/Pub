#include "rtpubsub.hpp"
#include <exception>
#include <stdexcept>

//==================================== Publisher ====================================================


rt_pub *rt_pub::instance = 0;


rt_pub::rt_pub(int const signal_type, int const var_id) :
		sig_type(signal_type), var_id(var_id) {
}

rt_pub::~rt_pub() {
}

rt_pub* rt_pub::init(int const signal_type, int const index) {
	if (rt_pub::instance == nullptr) {
		rt_pub::instance = new rt_pub { signal_type, index };
	}
	return rt_pub::instance;
}

rt_pub* rt_pub::getInstance() {
	return rt_pub::instance;
}

void rt_pub::add_subscriber(const pid_t pid) {
	this->subscriber_pid_list.push_back(pid);
}

void rt_pub::remove_subscriber(const pid_t pid) {
	for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++) {
		if (*iter == pid) {
			subscriber_pid_list.erase(iter);
		}
	}
}

void rt_pub::notify() {
	union sigval sig_val { };
	sig_val.sival_int = (int) (this->var_id);
	for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++) {
		sigqueue(*iter, (int) this->sig_type, sig_val);
	}
}


//==================================== Subscriber ====================================================

rt_sub *rt_sub::instance = 0;

rt_sub::rt_sub(int const signal_type, const sig_handler signal_handler)
: sig_type{signal_type}, handler{signal_handler},newmask {},oldmask {},action {}
{
	try
	{
		if (this->handler == nullptr)
		{
			throw std::runtime_error("handler can't be null");
		}
		sigemptyset(&newmask);
		sigaddset(&newmask, sig_type);
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		action.sa_flags = SA_SIGINFO;
		action.sa_sigaction = this->handler;
		if (sigaction(sig_type, &action, nullptr) == -1)
		{
			throw std::runtime_error("Error during signal setup");
		}
	}
	catch (std::exception const &e)
	{
		std::clog << "Exception: " << e.what() << "\n";
	}
}

rt_sub::~rt_sub() {
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	signal(this->sig_type, SIG_DFL);
	sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
}

/**
 * @brief
 * @param signal_type
 * @param signal_handler
 * @return
 */
rt_sub* rt_sub::init(int const signal_type, const sig_handler signal_handler) {
	if (rt_sub::instance == nullptr) {
		rt_sub::instance = new rt_sub{signal_type, signal_handler};
	}
	return rt_sub::instance;
}

/**
 * @brief
 * @return
 */
rt_sub* rt_sub::getInstance() {
	return rt_sub::instance;
}
/**
 * @brief
 * @return (void)
 */
void rt_sub::block() {
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
}

/**
 * @brief
 * @return (void)
 */
void rt_sub::unblock() {
	sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
}

/**
 * @brief
 * @return (void)
 */
void rt_sub::unblock_n_await() {
	sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	sigsuspend (&oldmask);
}

/**
 * @brief
 * @return (void)
 */
void rt_sub::await() {
	sigsuspend (&oldmask);
}
