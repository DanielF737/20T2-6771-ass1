//
//  Copyright UNSW Sydney School of Computer Science and Engineering
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS", BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "comp6771/word_ladder.hpp"

#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "comp6771/testing/range/contain.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/is_sorted.hpp"
#include "range/v3/range/primitives.hpp"

//Test cases to include
	//Mismatched length
	//Input and output same word

TEST_CASE("Correct number of results") {
	SECTION("No results (no valid path)") {
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt");		//Provided Lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("airplane", "tricycle", english_lexicon);
		CHECK(ranges::size(ladders)==0); 	//Check that no results were returned
	}

	SECTION("1 result (1 valid path") {
		auto const english_lexicon = word_ladder::read_lexicon("./smallEnglish.txt"); 	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("word", "bird", english_lexicon);
		CHECK(ranges::size(ladders)==1); 	//Check that exactly 1 results was returned

		auto const expected = std::vector<std::vector<std::string>>{{"word", "ward", "bard", "bird"}};
		CHECK(ladders==expected);
	}

	SECTION("Many results (many valid paths") {
		auto const english_lexicon = word_ladder::read_lexicon("./smallEnglish.txt");	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("aaa", "bbb", english_lexicon);
		CHECK(ranges::size(ladders)==6); 	//Check that exactly 6 results were returned
												//This is known as the lexicon was constructed to return 6 valid ladders of shortest length for this query
	}

	SECTION("Ladder length 2 (starting and destination words are neighbors)") {
		auto const english_lexicon = word_ladder::read_lexicon("./smallEnglish.txt"); 	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("word", "ward", english_lexicon);
		CHECK(ranges::size(ladders)==1); 	//Check that exactly 1 results was returned

		auto const first = ladders.begin();
		CHECK(ranges::size(*first)==2); 	//Check that the ladder is of length 2

		auto const expected = std::vector<std::vector<std::string>>{{"word", "ward"}};
		CHECK(ladders==expected);			//Confirm that it is the correct ladder of length 2
	}

	SECTION("Ladder length >2 (starting and destination words are not neighbors)") {
		auto const english_lexicon = word_ladder::read_lexicon("./smallEnglish.txt"); 	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("aaaaa", "azzza", english_lexicon);
		CHECK(ranges::size(ladders)==6); 	//Check that exactly 6 results were returned

		auto const first = ladders.begin();
		CHECK(ranges::size(*first)==4); 	//Check that the ladder is of length 4 (>2)

		auto const expected = std::vector<std::vector<std::string>>{{"aaaaa", "aaaza", "aazza", "azzza"},
																	{"aaaaa", "aaaza", "azaza", "azzza"},
																	{"aaaaa", "aazaa", "aazza", "azzza"},
																	{"aaaaa", "aazaa", "azzaa", "azzza"},
																	{"aaaaa", "azaaa", "azaza", "azzza"},
																	{"aaaaa", "azaaa", "azzaa", "azzza"}};
		CHECK(ladders==expected);			//Confirm it returns the correct ladders
	}
}

TEST_CASE("Correctness of results") {
	SECTION("Correct words") {
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt"); 	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("work", "play", english_lexicon);
		CHECK(ranges::size(ladders)==12); 	//Check that exactly 6 results were returned

		auto const first = ladders.begin();
		CHECK(ranges::size(*first)==7); 	//Check that the ladder is of length 7

		auto const expected = std::vector<std::vector<std::string>>{{"work", "fork", "form", "foam", "flam", "flay", "play"},
																	{"work", "pork", "perk", "peak", "pean", "plan", "play"},
																	{"work", "pork", "perk", "peak", "peat", "plat", "play"},
																	{"work", "pork", "perk", "pert", "peat", "plat", "play"},
																	{"work", "pork", "porn", "pirn", "pian", "plan", "play"},
																	{"work", "pork", "port", "pert", "peat", "plat", "play"},
																	{"work", "word", "wood", "pood", "plod", "ploy", "play"},
																	{"work", "worm", "form", "foam", "flam", "flay", "play"},
																	{"work", "worn", "porn", "pirn", "pian", "plan", "play"},
																	{"work", "wort", "bort", "boat", "blat", "plat", "play"},
																	{"work", "wort", "port", "pert", "peat", "plat", "play"},
																	{"work", "wort", "wert", "pert", "peat", "plat", "play"}};

		CHECK(ladders==expected);			//Confirm it returns the correct ladders
	}

	SECTION("Lexicographical order") { //A lot of the previous tests also test this, but im gunna leave it one that I actually used while debugging that I kept failing to prove that it actually does it
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt"); 	//Custom lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("awake", "sleep", english_lexicon);
		CHECK(ranges::size(ladders)==2); 	//Check that exactly 6 results were returned

		auto const first = ladders.begin();
		CHECK(ranges::size(*first)==10); 	//Check that the ladder is of length 7

		auto const expected = std::vector<std::vector<std::string>>{{"awake","aware","sware","share","sharn","shawn","shewn","sheen","sheep","sleep"},
																	{"awake","aware","sware","share","shire","shirr","shier","sheer","sheep","sleep"}};

		CHECK(ladders==expected);			//Confirm it returns the correct ladders
	}
}

TEST_CASE("Edge Cases") {
	SECTION("Mismatched starting and destination word lengths") {
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt");		//Provided Lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("a", "aa", english_lexicon);
		CHECK(ranges::size(ladders)==0); 	//Check that no results were returned
	}

	SECTION("Starting and destination words are the same word") {
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt");		//Provided Lexicon
		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty

		auto const ladders = word_ladder::generate("work", "work", english_lexicon);
		CHECK(ranges::size(ladders)==1); 	//Check that no results were returned

		auto const expected = std::vector<std::vector<std::string>>{{"work"}};
		CHECK(ladders == expected);
	}
}