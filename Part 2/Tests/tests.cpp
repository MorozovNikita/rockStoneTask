import TopTracker;

#include <iostream>
#include <chrono>
#include <cassert>
#include <thread>

using namespace  std::chrono;

void primitive_test()
{
    std::cout << "STARTING primitive_test" << std::endl;

    add_new_action(1, 1);
    add_new_action(2, 1);

    assert(get_all_actions().size() == 2);

    auto test_time = system_clock::now();

    add_new_action(3, 2);
    add_new_action(4, 1);
    add_new_action(5, 2);

    assert(get_all_actions().size() == 5);

    clear_expired_action(test_time);

    assert(get_all_actions().size() == 3);

    std::cout << "END primitive_test" << std::endl;
}

void async_test()
{
std::cout << "STARTING async_test" << std::endl;

    auto addAction = [](const int id, const int type) {
        add_new_action(id, type);
    };
    
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 5; ++i) {
        threads.emplace_back([&, i] {
            for(int j = 0; j < 3; ++j) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                addAction(i * 10 + j, i * 10 + j);
            }
        });
    }
    
    for(auto& t : threads) {
        t.join();
    }

    for (const auto &[t, action] : get_all_actions())
    {
        std::cout << "TIME: " << t.time_since_epoch().count() 
                  << "\nID: " << action.id_user
                  << "\nTYPE: " << action.type_action << std::endl;
    }

    std::cout << "END async_test" << std::endl;
}

int main() {
    primitive_test();
    async_test();
}
