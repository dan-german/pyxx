#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "vst.h"
#include "prs.h"
#include <vector>
#include <memory>
#include "utils.h"

using namespace std;
using namespace ast;
using namespace vst;

TEST(vst, preorder) {
    auto ast = Parser("1*2+3*4").expr();
    vec<pair<string, int>> res;
    vst::preorder(ast.get(), [&res](const Node* node, int level) { res.push_back({ string(*node), level }); });
    vec<pair<string, int>> expected {
        { "BOp(BOp(Int(1),*,Int(2)),+,BOp(Int(3),*,Int(4)))", 0 },
        { "BOp(Int(1),*,Int(2))", 1 },
        { "Int(1)", 2 },
        { "Int(2)", 2 },
        { "BOp(Int(3),*,Int(4))", 1 },
        { "Int(3)", 2 },
        { "Int(4)", 2 }
    };
    EXPECT_EQ(res, expected);
}

TEST(vst, postorder) {
    auto ast = Parser("1*2+3*4").expr();
    vec<string> res;
    vst::postorder(ast.get(), [&res](const Node* node) { res.push_back(string(*node)); });
    vec<string> expected {
        "Int(1)",
        "Int(2)",
        "BOp(Int(1),*,Int(2))",
        "Int(3)",
        "Int(4)",
        "BOp(Int(3),*,Int(4))",
        "BOp(BOp(Int(1),*,Int(2)),+,BOp(Int(3),*,Int(4)))",
    };
    EXPECT_EQ(res, expected);
}
