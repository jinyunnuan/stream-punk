# include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>   // 用于 INT_MAX 和 INT_MIN

# include "StreamPunk.hpp"
# include "unit_test.hpp"

class LocaleInitializer {
public:
    static void Initialize() {
        static bool initialized = []() {
            try {
                std::locale::global(std::locale("en_US.UTF-8"));
                std::cout.imbue(std::locale());
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Locale init failed: " << e.what() << '\n';
                return false;
            }
            }();
        (void)initialized;
    }
    // C++11 线程安全初始化
    LocaleInitializer() { Initialize(); }
};

#if __cplusplus >= 201703L
inline LocaleInitializer __locale_initializer;  // C++17 内联实例
#else
static LocaleInitializer __locale_initializer;  // 头文件多次包含会有多个实例
#endif

int main(void) {
    int number_failed;

    tcase_set_timeout(tc_core, 0);
    suite_add_tcase(s, tc_core);

    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}