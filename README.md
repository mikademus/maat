# Maat: The Test Harness
<div style="float:right"><img src="maat_logo.jpeg" width="200"></div>

*Maat* is the Egyptian goddess of truth, justice, and cosmic order. In the Weighing of the Heart the heart of the deceased is weighed against a feather before Osiris, Thoth and Anubis. The scales do not negotiate. The result is binary. Failed tests are eaten by the Ammit monster.

A test harness should work the same way.

`Maat` is a lightweight, self-contained, single-header C++ test harness with no dependencies and no boilerplate. Drop in the header, write your tests, run them.

## Installation
Copy maat.h into your project and include it:
```~cpp
#include "maat.h"
```
That's it.

## Usage

### Assertions
Maat provides two classes of assertion reflecting the hard and soft judgment of the scales:
`REQUIRE` — hard assertion. On failure the test terminates immediately, as a heart too heavy for the feather ends the ceremony:
```cpp
REQUIRE(ptr != nullptr, "pointer must not be null");
REQ_EQ(result, expected, "values must match");
REQ_NEQ(result, forbidden, "value must differ");
```
`EXPECT` — soft assertion. On failure the verdict is recorded but execution continues, accumulating all failures before the final judgment:
```cpp
EXPECT(value > 0, "value should be positive");
EXP_EQ(result, expected, "values should match");
EXP_NEQ(result, forbidden, "value should differ");
```
`NOTE` — inserts a message into the test report without affecting the result:
```cpp
NOTE("testing edge case: empty input");
```

### Running tests
```cpp
RUN_TEST(my_test_function);
```
Test functions return bool and take no arguments:
```cpp
bool my_test_function()
{
    REQUIRE(true, "this will pass");
    EXPECT(1 + 1 == 2, "arithmetic holds");
    return true;
}
```
Returning false from the test function is equivalent to a failed REQUIRE.

### Print summary
Call `maat::print_summary()` at the end of your test run:
```cpp
maat::print_summary();
```

Output:
```
Summary: 7 passed, 1 failed, 8 total.
```

### A complete example
```cpp
#include "maat.h"
#include <vector>

bool test_vector_basics()
{
    std::vector<int> v;
    REQUIRE(v.empty(), "vector should start empty");
    
    v.push_back(42);
    EXP_EQ(v.size(), 1u, "vector should have one element");
    EXP_EQ(v[0], 42, "element should be 42");
    
    return true;
}

int main()
{
    RUN_TEST(test_vector_basics);    
    maat::print_summary();
}
```

### Output

Tests report live as they run, with the status overwriting the "running" indicator:
```
[ ok ] 1: test_vector_basics
[FAIL] 2: test_something_else
    - Exp: values should match
    - Req: pointer must not be null
```
Failed assertions are reported beneath the test name — `Req:` for hard failures that terminated the test, `Exp:` for soft failures that were accumulated. Ammit is not selective.

### Namespace
All Maat internals live under ::maat. Your test files need not reference the namespace directly — the macros handle everything.

## Licence
Licensed under the MIT License.
