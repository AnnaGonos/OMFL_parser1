#ifndef TEMP_TEST_H
#define TEMP_TEST_H

#include <sstream>
#include <cmath>
#include <vector>

#include <parser.h>

using namespace omfl;

#define ASSERT_TRUE(_condition) { if(!_condition) { int a = 1; a /= 0; } }
#define ASSERT_FALSE(_condition) { if(_condition) { int a = 1; a /= 0; } }
#define ASSERT_EQ(_first, _second) { if(_first != _second) { int a = 1; a /= 0; } }
#define ASSERT_FLOAT_EQ(_first, _second) { if(fabs(_first - _second) > 0.0001f) { int a = 1; a /= 0; } }

#define TEST_IT false

#if TEST_IT == true
	#define START_TESTS int main() {
	#define END_TESTS return 0; }
#else
	#define START_TESTS void test() {
	#define END_TESTS }
#endif

namespace testing {
	template<typename... T>
	std::vector<std::string> Values(T... _args)
	{
		std::initializer_list<std::string> _il = {_args...};
		return std::vector<std::string>(_il);
	}
//	using Values = std::initializer_list<std::string>;
}

#define TEST(x, y)

START_TESTS

TEST(ParserTestSuite, EmptyTest) {
	std::string data = "";

	const auto root = parse(data);

	ASSERT_TRUE(root.valid());
}

TEST(ParserTestSuite, IntTest) {
	std::string data = R"(
		key1 = 100500
		key2 = -22
		key3 = +28)";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_TRUE(root.Get("key1").IsInt());
	ASSERT_TRUE(root.Get("key2").IsInt());
	ASSERT_TRUE(root.Get("key3").IsInt());

	ASSERT_EQ(root.Get("key1").AsInt(), 100500);
	ASSERT_EQ(root.Get("key2").AsInt(), -22);
	ASSERT_EQ(root.Get("key3").AsInt(), 28);
}

TEST(ParserTestSuite, InvalidIntTest) {
	std::string data = R"(
		key1 = true
		key2 = -22.1
		key3 = "ITMO")";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_FALSE(root.Get("key1").IsInt());
	ASSERT_FALSE(root.Get("key2").IsInt());
	ASSERT_FALSE(root.Get("key3").IsInt());

	ASSERT_EQ(root.Get("key1").AsIntOrDefault(1), 1);
	ASSERT_EQ(root.Get("key2").AsIntOrDefault(-2), -2);
	ASSERT_EQ(root.Get("key3").AsIntOrDefault(3), 3);
}

TEST(ParserTestSuite, FloatTest) {
	std::string data = R"(
		key1 = 2.1
		key2 = -3.14
		key3 = -0.001)";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_TRUE(root.Get("key1").IsFloat());
	ASSERT_TRUE(root.Get("key2").IsFloat());
	ASSERT_TRUE(root.Get("key3").IsFloat());

	ASSERT_FLOAT_EQ(root.Get("key1").AsFloat(), 2.1f);
	ASSERT_FLOAT_EQ(root.Get("key2").AsFloat(), -3.14f);
	ASSERT_FLOAT_EQ(root.Get("key3").AsFloat(), -0.001f);
}

TEST(ParserTestSuite, InvalidFloatTest) {
	std::string data = R"(
		key1 = true
		key2 = -2
		key3 = "ITMO")";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_FALSE(root.Get("key1").IsFloat());
	ASSERT_FALSE(root.Get("key2").IsFloat());
	ASSERT_FALSE(root.Get("key3").IsFloat());

	ASSERT_FLOAT_EQ(root.Get("key1").AsFloatOrDefault(2.1), 2.1);
	ASSERT_FLOAT_EQ(root.Get("key2").AsFloatOrDefault(-3.14), -3.14);
	ASSERT_FLOAT_EQ(root.Get("key3").AsFloatOrDefault(-0.001), -0.001);
}

TEST(ParserTestSuite, StringTest) {
	std::string data = R"(
		key = "value"
		key1 = "value1")";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_TRUE(root.Get("key").IsString());
	ASSERT_TRUE(root.Get("key1").IsString());

	ASSERT_EQ(root.Get("key").AsString(), "value");
	ASSERT_EQ(root.Get("key1").AsString(), "value1");
}

TEST(ParserTestSuite, InvalidStringTest) {
	std::string data = R"(
		key = true
		key1 = ["1", "2", "3"])";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_FALSE(root.Get("key").IsString());
	ASSERT_FALSE(root.Get("key1").IsString());

	ASSERT_EQ(root.Get("key").AsStringOrDefault("Hello"), "Hello");
	ASSERT_EQ(root.Get("key1").AsStringOrDefault("World"), "World");
}

TEST(ParserTestSuite, ArrayTest) {
	std::string data = R"(
		key1 = [1, 2, 3, 4, 5, 6])";


	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_TRUE(root.Get("key1").IsArray());

	ASSERT_EQ(root.Get("key1")[0].AsInt(), 1);
	ASSERT_EQ(root.Get("key1")[1].AsInt(), 2);
	ASSERT_EQ(root.Get("key1")[2].AsInt(), 3);

	ASSERT_EQ(root.Get("key1")[100500].AsIntOrDefault(99), 99);
}

TEST(ParserTestSuite, DiffTypesArrayTest) {
	std::string data = R"(
		key1 = [1, true, 3.14, "ITMO", [1, 2, 3], ["a", "b", 28]])";

	const auto root = parse(data);

	ASSERT_TRUE(root.valid());

	ASSERT_TRUE(root.Get("key1").IsArray());

	ASSERT_TRUE(root.Get("key1")[0].IsInt());
	ASSERT_EQ(root.Get("key1")[0].AsInt(), 1);

	ASSERT_TRUE(root.Get("key1")[1].IsBool());
	ASSERT_EQ(root.Get("key1")[1].AsBool(), true);

	ASSERT_TRUE(root.Get("key1")[2].IsFloat());
	ASSERT_FLOAT_EQ(root.Get("key1")[2].AsFloat(), 3.14);

	ASSERT_TRUE(root.Get("key1")[3].IsString());
	ASSERT_EQ(root.Get("key1")[3].AsString(), "ITMO");

	ASSERT_TRUE(root.Get("key1")[4].IsArray());
	ASSERT_EQ(root.Get("key1")[4][0].AsInt(), 1);
	ASSERT_EQ(root.Get("key1")[4][1].AsInt(), 2);
	ASSERT_EQ(root.Get("key1")[4][2].AsInt(), 3);

	ASSERT_TRUE(root.Get("key1")[5].IsArray());
	ASSERT_EQ(root.Get("key1")[5][0].AsString(), "a");
	ASSERT_EQ(root.Get("key1")[5][1].AsString(), "b");
	ASSERT_EQ(root.Get("key1")[5][2].AsInt(), 28);
}

TEST(ParserTestSuite, CommentsTest) {
	std::string data = R"(
		key1 = 100500  # some important value

		# It's more then university)";

	const auto root = parse(data);

	ASSERT_TRUE(root.valid());
	ASSERT_EQ(root.Get("key1").AsInt(), 100500);
}

TEST(ParserTestSuite, SectionTest) {
	std::string data = R"(
		[section1]
		key1 = 1
		key2 = true

		[section1]
		key3 = "value")";

	const auto root = parse(data);
	ASSERT_TRUE(root.valid());

	ASSERT_EQ(root.Get("section1.key1").AsInt(), 1);
	ASSERT_EQ(root.Get("section1.key2").AsBool(), true);
	ASSERT_EQ(root.Get("section1.key3").AsString(), "value");
}

TEST(ParserTestSuite, MultiSectionTest) {
	std::string data = R"(
		[level1]
		key1 = 1
		[level1.level2-1]
		key2 = 2

		[level1.level2-2]
		key3 = 3)";

	const auto root = parse(data);
	ASSERT_TRUE(root.valid());

	ASSERT_EQ(root.Get("level1.key1").AsInt(), 1);
	ASSERT_EQ(root.Get("level1.level2-1.key2").AsInt(), 2);
	ASSERT_EQ(root.Get("level1.level2-2.key3").AsInt(), 3);
}


TEST(ParserTestSuite, GetSectionTest) {
	std::string data = R"(
		[level1.level2.level3]
		key1 = 1)";

	const auto root = parse(data);
	ASSERT_TRUE(root.valid());

	ASSERT_EQ(root.Get("level1").Get("level2").Get("level3").Get("key1").AsInt(), 1);
}


#define INSTANTIATE_TEST_SUITE_P(some_useless_shit, expected_result, values) { \
	for(auto& v : values) { \
		const auto root = parse(v); \
		ASSERT_EQ(root.valid(), expected_result); \
	} }

//std::initializer_list<std::string> il = {"as", "va", "ca"};

bool ValidFormatTestSuite = true;
bool InvalidFormatTestSuite = false;


// Key
INSTANTIATE_TEST_SUITE_P(
	KeyValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key = \"value\"",
		" key      =  \"value\"   ",
		" key=  \"value\"",
		" key123 =  \"value\"",
		" key_1-23-abcd =  \"value\"",
		" 23key_ =  \"value\""
	)
);

INSTANTIATE_TEST_SUITE_P(
	KeyValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"!key = \"value\"",
		"key = \n\"value\"",
		"      =  \"value\"   ",
		"=  \"value\"",
		" key**123 =  \"value\"",
		" . =  \"value\"",
		" .key2.key3 =  \"value\"",
		" key1. =  \"value\""
	)
);
//std::vector<int> v = {1, 2, 4, 5, 6, 7}
// Value
INSTANTIATE_TEST_SUITE_P(
	ValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key = \"value\""
	)
);

INSTANTIATE_TEST_SUITE_P(
	ValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key = ",
		"key = abcd"
	)
);


TEST(FormatTestSuite, KeyRedifenitionTest) {
	std::string data = R"(
		key = 1
		key = 2)";

	ASSERT_FALSE(parse(data).valid());
}

// Integer Value
INSTANTIATE_TEST_SUITE_P(
	IntValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 2",
		"key2 = -22",
		"key3 = +48"
	)
);

INSTANTIATE_TEST_SUITE_P(
	IntValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = 2+",
		"key2 = 2-2",
		"key3 = 4+8",
		"key4 = +"
	)
);

// Float Value
INSTANTIATE_TEST_SUITE_P(
	FloatValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 3.14",
		"key2 = -3.14",
		"key3 = +0.00001"
	)
);

INSTANTIATE_TEST_SUITE_P(
	FloatValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = .0",
		"key2 = 1.",
		"key3 = +.1",
		"key4 = +.",
		"key5 = ."
	)
);

// String Value
INSTANTIATE_TEST_SUITE_P(
	StringValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = \"Hello world\"",
		"key2 = \"1, 2, 3, 4, 5\"",
		"key3 = \"3.14\"",
		"key4 = \"1\t2\t3\"",
		"key5 = \"[1,2,3,4,5]\""
//		"key5 = \"true"		//	this test should not pass: string is:			key5 = "true	  - missing closing '"'
	)
);

INSTANTIATE_TEST_SUITE_P(
	StringValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = \"Hello world",
		"key2 = \"ITMO\"University\"",
		"key3 = \"Bjarne\" \"stroustrup\""
	)
);


// Bool Value
INSTANTIATE_TEST_SUITE_P(
	BoolValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = true",
		"key2 = false"
	)
);

INSTANTIATE_TEST_SUITE_P(
	BoolValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = tru",
		"key2 = alse",
		"key3 = true true",
		"key4 = fal se",
		"key5 = truefalse"
	)
);


// Array Value
INSTANTIATE_TEST_SUITE_P(
	ArrayValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = []",		//	this comflicts with test below
		"key2 = [1,2,3,4,5]",
		"key3 = [1, -3.14, true, \"ITMO\"]",
		"key4 = [[1,2],[2,[3,4,5]]]"
	)
);

INSTANTIATE_TEST_SUITE_P(
	ArrayValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = [",
		"key2 = ]",
		"key3 = [1;2;3]",
		"key4 = [1,2,3,4",
		"key5 = [[1,2],[2,[3,4,5]"
	)
);


// Section
INSTANTIATE_TEST_SUITE_P(
	SectionValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"[section-1]",
		"[section-1.section-2]",
		"[a.b.c.d]"
	)
);

INSTANTIATE_TEST_SUITE_P(
	SectionValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		//"key1 = []",		//	why can't we create empty array?
		"key2 = {section-1}",
		"key3 = [section-1.]",
		"key4 = [.section-1]"
	)
);

// Comment
INSTANTIATE_TEST_SUITE_P(
	CommentValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 1 # comment",
		"# some text"
	)
);

INSTANTIATE_TEST_SUITE_P(
	CommentValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"# comment \n newline comment"
	)
);

INSTANTIATE_TEST_SUITE_P(
	SectionValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"[]",
		"{section-1}",
		"[section-1.]",
		"[.section-1]"
	)
);

// Comment
INSTANTIATE_TEST_SUITE_P(
	CommentValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 1 # comment",
		"# some text"
	)
);

INSTANTIATE_TEST_SUITE_P(
	CommentValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"# comment \n newline comment"
	)
);

// Key
INSTANTIATE_TEST_SUITE_P(
	KeyValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key = \"value\"",
		" key      =  \"value\"   ",
		" key=  \"value\"",
		" key123 =  \"value\"",
		" key_1-23-abcd =  \"value\"",
		" 23key_ =  \"value\""
	)
);

INSTANTIATE_TEST_SUITE_P(
	KeyValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"!key = \"value\"",
		"key = \n\"value\"",
		"      =  \"value\"   ",
		"=  \"value\"",
		" key**123 =  \"value\"",
		" . =  \"value\"",
		" .key2.key3 =  \"value\"",
		" key1. =  \"value\""
	)
);

// Value
INSTANTIATE_TEST_SUITE_P(
	ValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key = \"value\""
	)
);

INSTANTIATE_TEST_SUITE_P(
	ValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key = ",
		"key = abcd"
	)
);


TEST(FormatTestSuite, KeyRedifenitionTest) {
	std::string data = R"(
		key = 1
		key = 2)";

	ASSERT_FALSE(parse(data).valid());
}

// Integer Value
INSTANTIATE_TEST_SUITE_P(
	IntValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 2",
		"key2 = -22",
		"key3 = +48"
	)
);

INSTANTIATE_TEST_SUITE_P(
	IntValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = 2+",
		"key2 = 2-2",
		"key3 = 4+8",
		"key4 = +"
	)
);

// Float Value
INSTANTIATE_TEST_SUITE_P(
	FloatValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = 3.14",
		"key2 = -3.14",
		"key3 = +0.00001"
	)
);

INSTANTIATE_TEST_SUITE_P(
	FloatValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = .0",
		"key2 = 1.",
		"key3 = +.1",
		"key4 = +.",
		"key5 = ."
	)
);

// String Value
INSTANTIATE_TEST_SUITE_P(
	StringValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = \"Hello world\"",
		"key2 = \"1, 2, 3, 4, 5\"",
		"key3 = \"3.14\"",
		"key4 = \"1\t2\t3\"",
		"key5 = \"[1,2,3,4,5]\""
	)
);

INSTANTIATE_TEST_SUITE_P(
	StringValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = \"Hello world",
		"key2 = \"ITMO\"University\"",
		"key3 = \"Bjarne\" \"stroustrup\""
	)
);


// Bool Value
INSTANTIATE_TEST_SUITE_P(
	BoolValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = true",
		"key2 = false"
	)
);

INSTANTIATE_TEST_SUITE_P(
	BoolValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = tru",
		"key2 = alse",
		"key3 = true true",
		"key4 = fal se",
		"key5 = truefalse"
	)
);


// Array Value
INSTANTIATE_TEST_SUITE_P(
	ArrayValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"key1 = []",
		"key2 = [1,2,3,4,5]",
		"key3 = [1, -3.14, true, \"ITMO\"]",
		"key4 = [[1,2],[2,[3,4,5]]]"
	)
);

INSTANTIATE_TEST_SUITE_P(
	ArrayValueValidation,
	InvalidFormatTestSuite,
	testing::Values(
		"key1 = [",
		"key2 = ]",
		"key3 = [1;2;3]",
		"key4 = [1,2,3,4",
		"key5 = [[1,2],[2,[3,4,5]"
	)
);


// Section
INSTANTIATE_TEST_SUITE_P(
	SectionValueValidation,
	ValidFormatTestSuite,
	testing::Values(
		"[section-1]",
		"[section-1.section-2]",
		"[a.b.c.d]"
	)
);

END_TESTS

#endif // TEMP_TEST_H
