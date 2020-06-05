# Assignment 1 - Word Ladder

**This assignment is in draft mode. That means that there is likely to be some rapid fixes for typos and clarifications over the coming days. We will keep a log at the top of this file of changes we make.**

**Please git clone frequently to get the latest changes.**

## Change Log
 * No items yet

## Overview

In Week 2 we are learning about the C++ STL, and this assignment is your chance to practise those skills.

## Overview

Now that you’ve been introduced to the C++ standard library, it’s time to put that knowledge to use. In the role of client, the low-level details have already been dealt with, and you can focus your attention on solving more pressing problems. Having a library of thoroughly tested and reviewed types designed by field experts vastly broadens the kinds of tasks you can “easily” tackle. In this first assignment, you’ll write the back-end for a program that heavily leverages the standard library to do nifty things. It might sound a little daunting at first; but given the power-tools in your arsenal, you can focus on solving the problem at hand, instead of worrying about how to implement everything from scratch. Let’s hear it for abstraction!

This assignment has several purposes:
1. To explore C++’s value-semantics.
2. To stress the notion of abstraction as a mechanism for managing data and providing functionality without revealing the implementation.
3. To become more familiar with the C++ type-system.
4. To gain familiarity with the C++ standard library and several widely used third-party libraries.

Leveraging `std::vector`, `std::queue`, and `absl::flat_hash_set` are critical for writing a word-ladder builder. A word-ladder is a connection from one word to another, formed by changing one letter at a time, with the constraint that each transformation yields a new valid word. For example, here is a word ladder connecting "code" to "data".

```txt
code -> cade -> cate -> date -> data
```

Many word ladders will have mutliple solutions, such as connecting "work" to "play".
```txt
work fork form foam flam flay play 
work pork perk peak pean plan play 
work pork perk peak peat plat play 
work pork perk pert peat plat play 
work pork porn pirn pian plan play 
work pork port pert peat plat play 
work word wood pood plod ploy play 
work worm form foam flam flay play 
work worn porn pirn pian plan play 
work wort bort boat blat plat play 
work wort port pert peat plat play 
work wort wert pert peat plat play
```

The back-end that you will write takes a start word, a destination word, and a lexicon, which returns valid word-ladders. By using a [breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search), you’re guaranteed to find the shortest such sequence. Here are some more examples.

```cpp
::word_ladder::generate("work", "play", english_words);
// returns {{"work, "fork", "form", "foam", "flam", "flay", "play"}}
::word_ladder::generate("awake", "sleep", english_words);
// returns {{"awake", "aware", "sware", "shire", "shirr", "shier", "sheer", "sheep", "sleep"}}
::word_ladder::generate("airplane", "tricycle", engish_words);
// returns {}
```

## Understanding a word ladder implementation

Finding a word-ladder is a specific instance of a [shortest-path problem](https://en.wikipedia.org/wiki/Shortest_path_problem), where the challenge is to find the shortest path from a starting position to a goal. Shortest-path problems come up in a variety of situations, such as packet routing, robot motion planning, social networks, and studying gene mutations. One approach for finding a shortest path is the classic breadth-first search algorithm. A breadth-first search searches outward from the start in a radial fashion, until it hits the goal. For our word-ladder, this means examining those ladders that represent one hop from the start. A “hop” is a change in letter. One “hop” from the start means one changed letter, two “hops” means two changes in letters, and so on. It’s possible for the same position to change letters across multiple non-adjacent hops. If any of these reach the destination, we’re done. If not, the search now examines all the ladders that add one more hop. By expanding the search at each step, all one-hop ladders are examined before two-hop ladders, and three-hop ladders are only taken into consideration if none of the one-hop or two-hop ladders work out; thus the algorithm is guaranteed to find the shortest successful ladder.

Breadth-first searches are typically implemented using a queue. The queue stores partial ladders that represent possibilities to explore. The ladders are enqueued in order of increasing length. The first elements enqueued are all the one-hop ladders, followed by the two-hop ladders, and so on. Due to FIFO handling, ladders will be dequeued in order of increasing length. The algorithm operates by dequeuing the front ladder from the queue and determining if it reaches the goal. If it does, then you have a complete ladder, and it is the shortest. If not, you take that partial ladder and extend it to reach words that are one more hop away, and enqueue those ladders onto the queue, to be examined later. If you exhaust the queue of possibilities without having found a completed ladder, you can conclude that no ladder exists.

A few of these tasks deserve a bit more explanation. For example, you’ll need to find all the words that differ by one letter from a given word. You might reach for a raw loop to change each letter to all the other letters in the alphabet; or you can explore the libraries that the project uses to solve the problem in a more descriptive manner (note 2). Repeat this for each letter position in the word and you will have discovered all the words that are one letter away.

Another, more subtle issue, is the restriction that you shouldn’t reuse words that have been included in a previous ladder. This is an optimisation that avoids exploring redundant paths. For example, if you previously tried the ladder `cat->cot->cog` and are now processing `cat->cot->con`, you would find that the word `cog` is one letter away from con, which looks like a potential candidate to extend this ladder. However, cog has already been reached in an earlier (and thus shorter) ladder, so there is no point in reconsidering it in a longer ladder. The simplest way to ensure this is to keep track of the words that have been used in any ladder, and ignore them when they resurface. This is also necessary to avoid getting trapped in circular, non-terminal ladders such as `cat->cot->cog->bog->bat->cat`. Since you need linear access to all of the items in a word-ladder when time comes to return it, it makes sense to model an individual word-ladder using `std::vector<std::string>`. Remember that because C++ has value-semantics, you’re able to copy vectors via copy initialisation (e.g. `auto word_ladder_clone = word_ladder;`) and copy assignment (e.g. `word_ladder_clone = word_ladder`).

**If there are multiple shortest paths, your implementation must return all the solutions, sorted in lexicographical order. Thus, the return type for your word-ladder generator will be `std::vector<std::vector<std::string>>`.**


## The Task

This generator might seem like it’s a panacea, but it still benefits from a step-by-step development plan to keep things moving along.

* **Task 1** --- *Familiarise yourself with the libraries available.* You don’t need to deep-dive, but it would be a smart idea to read up on std::vector, std::queue, and std::unordered_set, from the standard library; and to read up on the range-adaptors we’ve listed in the appendix. You shouldn’t worry about their implementation details: focus on the interfaces, and how you use them in practice.
* **Task 2** --- *Test design.* We’ve provided you with a very simple test case to show you how to use the test library. You should add more TEST_CASEs underneath, so you have a suite of checks to make sure you catch any logic errors in your generator. We adopt Google’s Beyoncé rule in this class: “if you liked it, you should have put a test on it”. Test words that are short (one or two letters) and test words that are longer.
* **Task 3** --- *Design your algorithm.* Be sure you understand the breadth-first search algorithm on paper, and what types you will need to use.
* **Task 4** --- *Lexicon handling.* Set up an absl::flat_hash_set object with the large lexicon, read from our data file (there’s a utility function called word_ladder::read_lexicon that will read it in from file for you).

### Assumptions

* You can assume that the start word and end word have the same length (i.e. number of characters)
* You can assume that both the start and the end word are in the lexicon

### Implementation hints

Again, it’s all about leveraging the libraries at your disposal --- you’ll find your job is just to coordinate the activities of your objects to do the search.

* [std::vector](https://en.cppreference.com/w/cpp/container/vector) maintains a contiguous sequence of elements, and has random-access. Benchmarks have shown that its contiguous storage makes it the fastest and smallest container in many situations (even when computer science theory tells us that a linked-list would be better!).
* [std::queue](https://en.cppreference.com/w/cpp/container/queue) offers a FIFO interface over a container, which is probably what you’ll want to use for tracking your partial ladders. Ladders are enqueued (and thus dequeued) in order of length, so as to find the shortest option first.
* [absl::flat_hash_set](https://en.cppreference.com/w/cpp/container/unordered_set) (note 1) is the hash-set that we use for the input lexicon, and is recommended for any internal lexicons you might use.
* [Algorithms](https://en.cppreference.com/w/cpp/algorithm) and ranges are powerful tools for describing what you want your program to do.
* As a minor detail, it doesn’t matter if the start and destination words are contained in the lexicon or not. You can eliminate non-words from the get-go or just allow them to fall through and be searched anyway. During marking, the start and destination words will always be taken from the lexicon.

## Getting Started

If you haven't done so already, clone this repository.

```sh
$ git clone https://gitlab.cse.unsw.edu.au/z5555555/20T2-cs6771-ass1
```

(Note: Replace z5555555 with your zid)

Navigate inside the directory. You can then open vscode with `code .` (not the dot).

### Running the tests

As per tutorial 1, you can follow the instructions there to build and run tests.

## Marking Criteria

This assignment will contribute 15% to your final mark.

The assessment for the assignment recognizes the difficulty of the task, the importance of style, and the importance of appropriate use of programming methods (e.g. using while loops instead of a dozen if statements).

<table class="table table-bordered table-striped">
<tr><td align=right>50%<td><b>Correctness</b><br />
    <P>
The correctness of your program will be determined automatically by tests that we will run against your program. You will not know the full sample of tests used prior to marking. Your program must also find the word ladder(s) for a given input in the time limit specified above. Time limits for performance will be clarified at 10pm on Sunday 7th of June (information will be pushed to this repo).

<tr><td align=right>25%<td><b>Your tests</b><br />
  You are required to write your own tests to ensure your program works.
  You will write tests in <code>word_ladder_test.cpp</code>. At the top of this file you will also include a block comment to explain the rational and approach you took to writing tests. Please read the <a href="https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md">Catch2 tutorial</a> or review lecture/tutorial content to see how to write tests.
  Tests will be marked on several factors. These include, but are not limited to
	<ul>
    <li>Correctness - an incorrect test is worse than useless.</li>
		<li>Coverage - your tests might be great, but if they don't cover the part that ends up failing, they weren't much good to you.</li>
		<li>Brittleness - If you change your implementation, will the tests need to be changed (this generally means avoiding calling functions specific to your implementation where possible - ones that would be private if you were doing OOP).</li>
		<li>Clarity - If your test case failed, it should be immediately obvious what went wrong (this means splitting it up into appropriately sized sub-tests, amongst other things).</li>
	</ul>

<tr><td align=right>20%<td><b>C++ best practices</b><br />Your adherence to good C++ best practice in lecture. This is <b>not</b> saying that if you conform to the style guide you will receive full marks for this section. This 20% is also based on how well you use modern C++ methodologies taught in this course as opposed to using backwards-compatible C methods. Examples include: Not using primitive arrays and not using pointers. We will also penalise you for 

<tr><td align=right>5%<td><b>clang-format</b><br />
In your project folder, run the following commands on all C++ files you submit:<br />
<code>$ clang-format -i /path/to/file.cpp</code>
<p>If the program outputs nothing (i.e. is linted dorrectly), you will receive full marks for this section (5%)</p>

</table>

The following actions will result in a 0/100 mark for Word Ladder, and in some cases a 0 for COMP6771:
* Knowingly providing your work to anyone and it is subsequently submitted (by anyone).
* Submitting any other person's work. This includes joint work.

The lecturer may vary the assessment scheme after inspecting
the assignment submissions but it will remain broadly similar to the description above.


## Originality of Work

The work you submit must be your own work.  Submission of work partially or completely derived from any other person or jointly written with any other person is not permitted.

The penalties for such an offence may include negative marks, automatic failure of the course and possibly other academic discipline. Assignment submissions will be examined both automatically and manually for such submissions.

Relevant scholarship authorities will be informed if students holding scholarships are involved in an incident of plagiarism or other misconduct.

Do not provide or show your assignment work to any other person &mdash; apart from the teaching staff of COMP6771.

If you knowingly provide or show your assignment work to another person for any reason, and work derived from it is submitted, you may be penalized, even if the work was submitted without your knowledge or consent.  This may apply even if your work is submitted by a third party unknown to you.

Note you will not be penalized if your work has the potential to be taken without your consent or knowledge.

## Submission

This assignment is due *Friday 19th of June, 19:59:59*.
Submit the assignment using the following comand while logged into the CSE machines:

```sh
6771 submit ass1
```

This will submit whatever is on the master branch of THIS repository (the one this README.md file is contained in) at the moment of submission.

Please ensure that you can build and run your tests successfully on the CSE machine.

## Late Submission Policy

If your assignment is submitted after this date, each hour it is late reduces the maximum mark it can achieve by 2%.

For example if an assignment worth 76% was submitted 5 hours late, the late submission would have no effect (as maximum mark would be 90%).

If the same assignment was submitted 30 hours late it would be awarded
40%, the maximum mark it can achieve at that time.

## Appendix

* (note 1) We link you to the C++ Reference page for std::unordered_set because absl::flat_hash_set is interface-compatible; the Abseil documentation links to C++ Reference’s std::unordered_set docs anyway, and mostly explains the advantages of absl::flat_hash_set.
* (note 2) You’ll need to consult the lexicon to see if the transformation is a valid word in either case.
