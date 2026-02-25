// maat.hpp - Maat: The Test Harness
// Version 0.5.0
// Copyright 2026 Mikael Ueno A
// Licenced as-is under the MIT licence.

#ifndef MAAT_TEST_HARNESS__
#define MAAT_TEST_HARNESS__

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

namespace maat
{

    struct test_result
    {
        std::string name;
        bool passed;
    };

    struct test_context
    {
        std::vector<std::string> messages;
        bool failed = false;
    };  

    thread_local test_context* active_context = nullptr;

    inline std::vector<test_result>& all_results()
    {
        static thread_local std::vector<test_result> r;
        return r;
    }

    inline size_t& test_counter()
    {
        static thread_local size_t c = 0;
        return c;
    }


// REQUIRE
// Hard assertation that immediately terminate the 
// execution of the test
//---------------------------------------------------------------------------
    #define REQUIRE(cond, false_msg)                        \
        do                                                  \
        {                                                   \
            if (!(cond))                                    \
            {                                               \
                active_context->messages.push_back(std::string("Req: ") + false_msg); \
                return false;                               \
            }                                               \
        } while (0)


    #define REQ_EQ(lhs, rhs, false_message) REQUIRE(lhs == rhs, false_message)
    #define REQ_NEQ(lhs, rhs, false_message) REQUIRE(lhs != rhs, false_message)


// EXPECT
// Soft assertation that allow the execution of the test to continue
//---------------------------------------------------------------------------
    #define EXPECT(cond, false_msg)                         \
        do                                                  \
        {                                                   \
            if (!(cond))                                    \
            {                                               \
                active_context->messages.push_back(std::string("Exp: ") + false_msg); \
                active_context->failed = true;              \
            }                                               \
        } while (0)

    #define EXP_EQ(lhs, rhs, msg) EXPECT((lhs) == (rhs), msg)
    #define EXP_NEQ(lhs, rhs, msg) EXPECT((lhs) != (rhs), msg)    


// NOTE
// Inserts a message in the test report queue
//---------------------------------------------------------------------------
    #define NOTE(msg) \
        do { if (active_context) active_context->messages.push_back(std::string("Note: ") + msg); } while (0)    


// RUN_TESTS
//---------------------------------------------------------------------------
    #define RUN_TEST(fn)                                   \
        do                                                 \
        {                                                  \
            test_context ctx;                              \
            active_context = &ctx;                         \
            ++test_counter();                              \
            std::cout << test_counter() << ": " << #fn << "... "; \
            bool ok = fn() && !ctx.failed;                 \
            all_results().push_back({ #fn, ok });          \
            std::cout << '\r'                              \
                      << (ok ? "[ ok ] " : "[FAIL] ")      \
                      << test_counter() << ": "            \
                      << #fn << std::endl;                 \
            for (auto const & msg : ctx.messages)          \
                std::cout << "    - " << msg << std::endl; \
            active_context = nullptr;                      \
        } while (0)


// PRINT SUMMARY
//---------------------------------------------------------------------------
    inline void print_summary()
    {
        size_t failed = std::count_if(all_results().begin(), all_results().end(),
            [](auto const & r) { return !r.passed; });

        std::cout << "\nSummary: "
                << (all_results().size() - failed) << " passed, "
                << failed << " failed, "
                << all_results().size() << " total.\n";
    }


// SUBCAT
//---------------------------------------------------------------------------
    #define SUBCAT(msg)                                                     \
        do                                                                  \
        {                                                                   \
            std::string str(msg);                                           \
            std::transform(str.begin(), str.end(), str.begin(), ::toupper); \
            std::cout << "-------" << str << "--------------\n";            \
        }                                                                   \
        while (0)
        
} // ns maat

#endif