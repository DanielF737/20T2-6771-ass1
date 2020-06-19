#include "comp6771/word_ladder.hpp"
#include "absl/container/flat_hash_set.h"
//#include "range/v3/view.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <queue>
#include <iostream>

// Write your implementation here
auto collect_variants(std::vector<std::string> const& words, absl::flat_hash_set<std::string> const& lexicon, absl::flat_hash_set<std::string> const& visited) -> std::vector<std::vector<std::string>>;

namespace word_ladder {
    auto generate(std::string const& from,
                  std::string const& to,
                  absl::flat_hash_set<std::string> const& lexicon)
    -> std::vector<std::vector<std::string>> {
        auto variants = std::vector<std::vector<std::string>>();    //variable to hold results to loop through
        auto queue = std::queue<std::vector<std::string>>();        //Our queue for the BFS
        auto visited = absl::flat_hash_set<std::string>();          //Hash map to store words already accessed to help shorted execution times
        auto results = std::vector<std::vector<std::string>>();     //Hold the return values

        if (from == to) {
            auto retval = std::vector<std::string>{from};
            results.push_back(retval);
            return results;
        }

        visited.insert(from);

        //Initial step: Add all variants from the starting word to the queue
        variants = collect_variants(std::vector<std::string>{from}, lexicon, visited);
        for (auto & variant : variants) {
            queue.push(variant); //Enqueue the variant
        }

        auto found = false; //Flag to stop adding new paths to the queue when one path has been found
        auto length = std::size_t();

        //Run through the queue until we find a valid ladder or the queue ends
        while (!queue.empty()){
            auto current = queue.front();

            visited.insert(current.back());     //Stop us going down redundant paths
            if (current.back()==to) {
                if (!found) {
                    results.push_back(current);
                    found = true;
                    length = current.size();
                } else {
                    if (current.size()==length) {
                        if (results.empty()) {
                            results.push_back(current);
                        } else {
                            //Lexicographical ordering
                            auto inserted = false;
                            for (auto i = results.begin(); i < results.end(); i++){ //Loop through the current results
                                auto k = current.begin();
                                for (auto j = i->begin(); j < i->end(); j++) { //Loop through the inner vectors
                                    if (*k != *j) { //Check alphabetical ordering, if the words arent the same, we are stopping this list either way
                                        if (*k < *j) {  //if the new lest is alphabetically above, insert the new list before and exit the loop
                                            inserted=true;
                                            results.insert(i, current);
                                        }
                                        break;
                                    }
                                    k++;
                                }
                                if (inserted) { //If we have already inserted the new list, exit, else, compare against the next list in results
                                    break;
                                }
                            }
                            if (!inserted) { //If we got to the end (AKA its lexicographically last) stick it on at the end of results
                                results.push_back(current);
                            }
                        }
                    }
                }
            } else {
                if (!found) {
                    variants = collect_variants(current, lexicon, visited);
                    if (!variants.empty()) {
                        for (auto & variant : variants) {
                            queue.push(variant); //Enqueue the variant
                        }
                    }
                }
            }
            queue.pop();
        }
        return results;
    }
} // namespace word_ladder

auto collect_variants(std::vector<std::string> const& words, absl::flat_hash_set<std::string> const& lexicon, absl::flat_hash_set<std::string> const& visited) -> std::vector<std::vector<std::string>> {
    auto variants = std::vector<std::vector<std::string>>();
    auto word = words.back(); //Grab the word currently at the end of the ladder
    auto alphabet = std::vector<char>{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    visited.size();

    for (size_t i = 0; i < word.size(); i++) {                                              //Loop through each character
        for (size_t j = 0; j<26; j++) {                                                     //Loop through the alphabet
            auto new_word = word;                                                           //Create a variable to hold new word
            new_word[i] = alphabet.at(j);                                                   //Change 1 letter in the new word
            if (lexicon.contains(new_word) and !visited.contains(new_word)) {               //check if the last word in the current ladder is in the lexicon
                auto new_ladder=std::vector<std::string>();                                 //Create a new word ladder
                new_ladder=words;                                                           //copy the start of the original word ladder
                new_ladder.push_back(new_word);                                             //add the new word to the end of the word ladder
                variants.push_back(new_ladder);                                             //add the new word ladder to the return
            }
        }
    }

    return variants;
}

//Basic algorithm:
    //get start word from params
    //add every variation of every letter to the queue (separate function)
    //go through queue, check if words are in the lexicon
        //If yes, enqueue all of its variations
            //Make a visited hash set? would reduce time for sure
        //If no, just move on
    //repeat until first case where word is found.
    //stop adding to the queue, search through the rest of the queue for successful paths of the same length.
