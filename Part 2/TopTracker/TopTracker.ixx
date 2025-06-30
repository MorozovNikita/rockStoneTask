module;

#include <algorithm>
#include <iostream>
#include <map>
#include <chrono>
#include <future>
#include <mutex>
#include <condition_variable>

export module TopTracker;

using namespace  std::chrono;

struct UserAction
{
    int id_user;
    int type_action;
};

std::map<time_point<system_clock>, UserAction> action_store;
std::mutex mt;
std::condition_variable cv;

export std::future<bool> add_new_action(const int player_id, const int type_action)
{
    return std::async(std::launch::async, [&]() -> bool {
        std::unique_lock<std::mutex> lock(mt);

        action_store[system_clock::now()] = UserAction(player_id, type_action);

        return true;
    });
}

export void clear_expired_action(const time_point<system_clock> time_action)
{
    std::lock_guard<std::mutex> lock(mt);

    auto iter = action_store.lower_bound(time_action);

    action_store.erase(action_store.begin(), iter);
    
    if (iter != action_store.end())
        std::cout << "We delete actions that happened earlier than: " << system_clock::to_time_t(time_action) << std::endl;
}

export std::map<time_point<system_clock>, UserAction> get_all_actions() 
{
    std::lock_guard<std::mutex> lock(mt);
    return action_store;
}

