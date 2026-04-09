// Golden/unit tests for the JSON utility functions in test_utils.cpp.
// No model, GPU, or network required.
#include "test_utils.h"
#include <cmath>
#include <string>

using namespace EngineTestUtils;

// ---------------------------------------------------------------------------
// escape_json
// ---------------------------------------------------------------------------

bool test_escape_plain() {
    return escape_json("hello world") == "hello world";
}

bool test_escape_quote() {
    return escape_json("say \"hi\"") == "say \\\"hi\\\"";
}

bool test_escape_backslash() {
    return escape_json("a\\b") == "a\\\\b";
}

bool test_escape_newline() {
    return escape_json("line1\nline2") == "line1\\nline2";
}

bool test_escape_carriage_return() {
    return escape_json("line1\rline2") == "line1\\rline2";
}

bool test_escape_combined() {
    return escape_json("a \"b\"\n") == "a \\\"b\\\"\\n";
}

bool test_escape_empty() {
    return escape_json("") == "";
}

// ---------------------------------------------------------------------------
// json_number
// ---------------------------------------------------------------------------

bool test_number_integer() {
    const std::string j = "{\"count\":42}";
    return std::abs(json_number(j, "count") - 42.0) < 1e-9;
}

bool test_number_float() {
    const std::string j = "{\"score\":3.14}";
    return std::abs(json_number(j, "score") - 3.14) < 1e-9;
}

bool test_number_negative() {
    const std::string j = "{\"delta\":-2.5}";
    return std::abs(json_number(j, "delta") - (-2.5)) < 1e-9;
}

bool test_number_zero() {
    const std::string j = "{\"val\":0}";
    return std::abs(json_number(j, "val") - 0.0) < 1e-9;
}

bool test_number_missing_returns_default() {
    const std::string j = "{\"score\":3.14}";
    return json_number(j, "missing", -99.0) == -99.0;
}

bool test_number_multiple_keys() {
    const std::string j = "{\"a\":1,\"b\":2,\"c\":3}";
    return std::abs(json_number(j, "a") - 1.0) < 1e-9 &&
           std::abs(json_number(j, "b") - 2.0) < 1e-9 &&
           std::abs(json_number(j, "c") - 3.0) < 1e-9;
}

// ---------------------------------------------------------------------------
// json_string
// ---------------------------------------------------------------------------

bool test_string_basic() {
    const std::string j = "{\"name\":\"alice\"}";
    return json_string(j, "name") == "alice";
}

bool test_string_multiple_keys() {
    const std::string j = "{\"first\":\"foo\",\"second\":\"bar\"}";
    return json_string(j, "first") == "foo" &&
           json_string(j, "second") == "bar";
}

bool test_string_missing_key() {
    const std::string j = "{\"name\":\"alice\"}";
    return json_string(j, "missing").empty();
}

bool test_string_empty_value() {
    const std::string j = "{\"val\":\"\"}";
    return json_string(j, "val") == "";
}

bool test_string_escape_quote() {
    const std::string j = "{\"msg\":\"say \\\"hi\\\" now\"}";
    return json_string(j, "msg") == "say \"hi\" now";
}

bool test_string_escape_newline() {
    const std::string j = "{\"text\":\"line1\\nline2\"}";
    return json_string(j, "text") == "line1\nline2";
}

bool test_string_escape_tab() {
    const std::string j = "{\"text\":\"col1\\tcol2\"}";
    return json_string(j, "text") == "col1\tcol2";
}

bool test_string_escape_backslash() {
    const std::string j = "{\"path\":\"C:\\\\Users\"}";
    return json_string(j, "path") == "C:\\Users";
}

// ---------------------------------------------------------------------------
// Roundtrip: escape_json output is valid for json_string parsing
// ---------------------------------------------------------------------------

bool test_roundtrip_string() {
    const std::string original = "He said \"hello\"\nNew line\\end";
    const std::string json = "{\"v\":\"" + escape_json(original) + "\"}";
    return json_string(json, "v") == original;
}

int main() {
    TestUtils::TestRunner runner("JSON Utilities Tests");

    runner.run_test("escape plain text",       test_escape_plain());
    runner.run_test("escape quote",            test_escape_quote());
    runner.run_test("escape backslash",        test_escape_backslash());
    runner.run_test("escape newline",          test_escape_newline());
    runner.run_test("escape carriage return",  test_escape_carriage_return());
    runner.run_test("escape combined",         test_escape_combined());
    runner.run_test("escape empty string",     test_escape_empty());

    runner.run_test("number integer",          test_number_integer());
    runner.run_test("number float",            test_number_float());
    runner.run_test("number negative",         test_number_negative());
    runner.run_test("number zero",             test_number_zero());
    runner.run_test("number missing default",  test_number_missing_returns_default());
    runner.run_test("number multiple keys",    test_number_multiple_keys());

    runner.run_test("string basic",            test_string_basic());
    runner.run_test("string multiple keys",    test_string_multiple_keys());
    runner.run_test("string missing key",      test_string_missing_key());
    runner.run_test("string empty value",      test_string_empty_value());
    runner.run_test("string escape quote",     test_string_escape_quote());
    runner.run_test("string escape newline",   test_string_escape_newline());
    runner.run_test("string escape tab",       test_string_escape_tab());
    runner.run_test("string escape backslash", test_string_escape_backslash());
    runner.run_test("roundtrip escape+parse",  test_roundtrip_string());

    runner.print_summary();
    return runner.all_passed() ? 0 : 1;
}
