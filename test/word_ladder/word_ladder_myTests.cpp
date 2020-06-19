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
// distributed under the License is distributed on an "AS IS" BASIS,
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
		auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
		auto const ladders = word_ladder::generate("airplane", "tricycle", english_lexicon);

		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty
		CHECK(ranges::size(ladders)==0); 	//Check that no results were returned
	}

	SECTION("1 result (1 valid path") {
		auto const english_lexicon = word_ladder::read_lexicon("./smallEnglish.txt");
		auto const ladders = word_ladder::generate("word", "bird", english_lexicon);


		REQUIRE(!english_lexicon.empty()); 	//Ensure lexicon isnt empty
		CHECK(ranges::size(ladders)==1); 	//Check that no results were returned

		auto const expected = std::vector<std::vector<std::string>>{{"word", "ward", "bard", "bird"}};
		CHECK(ladders==expected);

	}

	SECTION("Many results (many valid paths") {

	}

	SECTION("Ladder length 2 (starting and destination words are neighbors)") {

	}

	SECTION("Ladder length >2 (starting and destination words are not neighbors)") {

	}
}

TEST_CASE("Correctness of results") {
	SECTION("Correct words") {
	}

	SECTION("Lexicographical order") {

	}
}

TEST_CASE("Edge Cases") {
	SECTION("Mismatched starting and destination word lengths") {

	}

	SECTION("Starting and destination words are the same word") {

	}
}
// TEST_CASE("awake -> sleep") {
// 	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
// 	auto const ladders = word_ladder::generate("awake", "sleep", english_lexicon);

// 	CHECK(ranges::size(ladders)>0);
// }

// TEST_CASE("work -> play") {
// 	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
// 	auto const ladders = word_ladder::generate("work", "play", english_lexicon);

// 	CHECK(ranges::size(ladders)>0);
// }

// TEST_CASE("airplane -> tricycle") {
// 	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
// 	auto const ladders = word_ladder::generate("airplane", "tricycle", english_lexicon);

// 	CHECK(ranges::size(ladders)>=0);
// }
