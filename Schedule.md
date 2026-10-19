Today — preparation (3 hours, unchanged)

This stays the same. Three hours is the right amount for preparation — more would be padding.
Hour 1: Reread all existing code without touching the keyboard. Write down on paper what each class owns and what each method does
Hour 2: redblobgames movement range article, implement scratch BFS in isolation x
Hour 3: Skim pybind11 docs, confirm CMakeLists builds, confirm clangd is working

Day 1 — complete CombatEngine in C++ (6-7 hours)
    Hours 1-2 — CombatEngine skeleton and reset(): Done(?)
        Create combat_engine.h and combat_engine.cpp with all method signatures
        Implement reset() — construct GameState, place warriors at defined starting positions on the grid
        Implement the grid representation inside GameState — a 2D array of passable cells BFS can query

    Hour 3 — getLegalActions(): Done
        Implement BFS reachable squares
        Implement action ID encoding scheme — write this down explicitly before coding it, since NEAT, step(), and getLegalActions() must all agree on what each ID means
        Test getLegalActions() in isolation by printing its output for a warrior in the middle of an open grid and checking the IDs make sense

    Hour 4 — step(): Done
        Decode action IDs
        Resolve movement and attack
        Check economy after each action, call advanceTurn() when exhausted
        Handle the edge case where a warrior is eliminated mid-turn

    Hour 5 — getStateVector() and computeReward():
        Implement state flattening — HP ratios, normalised positions, turn indicator, round number
        Implement reward function
        Write down the exact size of your state vector — you will need this number for neat_config.txt

    Hours 6-7 — main.cpp test and debugging:
        Two warriors, hardcoded always-attack behaviour, run to completion
        Print full state each turn — positions, HP, whose turn, round number
        Fix everything until output is clean and sensible

End of day goal: complete combat runs in C++ without crashing, output makes sense, state vector size is known and written down.

Day 2 — pybind11, Python environment, and validation (6-7 hours)
    Hours 1-2 — bindings.cpp and build:
        Expose CombatEngine methods to Python
        Expose any GameState and Warrior fields Python needs to read directly
        Build the .so with CMake
        Write a ten-line Python script replicating the main.cpp test — confirm output matches exactly

    Hour 3 — combat_env.py:
        Write the environment wrapper
        Implement reset(), step(), encode_state(), get_legal_actions()
        Keep this thin — it should be mostly calling C++ methods, not reimplementing logic

    Hour 4 — action masking:
        Implement masked argmax in Python
        Test with a hardcoded loop — print legal actions each turn, confirm illegal squares and unavailable actions never appear
        This is worth testing thoroughly now because a masking bug is invisible during NEAT training and only shows up as weird behaviour in the trained agent

    Hour 5 — state encoding validation:
        Print the encoded state vector at each step of a full combat
        Check every value is in the expected range — HP ratios between 0 and 1, positions normalised correctly, no NaN or infinity
        A single out-of-range value in the state vector will cause NEAT to behave erratically and is hard to diagnose later

    Hours 6-7 — stress testing the Python-C++ bridge:
        Run 100 complete combats in a loop from Python, checking for crashes, hangs, or inconsistent results
        Vary starting positions and confirm BFS produces different legal action sets correctly
        Fix anything that isn’t consistent

End of day goal: 100 combats complete through Python without issues, state vector validated, masking confirmed correct.

Day 3 — NEAT training and evaluation (6-8 hours)

Hour 1 — setup:
    Install neat-python, numpy, matplotlib if not already done
    Write neat_config.txt using the state vector size from day 1
    Start with conservative settings — pop_size = 150, num_hidden = 0, 10 combats per genome evaluation

Hours 2-3 — train.py and first run:
    Write eval_genome() and the main population loop
    Run 50 generations with fitness printed per generation
    Watch the fitness curve — it should increase or at least vary. A completely flat line at zero means the reward function isn’t firing, which is the most common first-run failure

Hour 4 — reward function tuning:
    If fitness improved in the first run, skip ahead
    If it flatlined, add print statements inside computeReward() to confirm it’s being called and returning non-zero values
    The most common cause is a combat that never terminates — add a round limit (50 rounds) as a safety valve with a large negative reward for hitting it

Hours 5-6 — longer training run:
    Once the 50 generation run looks healthy, run 300 generations
    Save checkpoints every 50 generations using neat-python’s built-in checkpointer — if something crashes you don’t lose everything
    You don’t need to watch this run closely, use the time to write evaluate.py

Hour 7 — evaluate.py:
    Load the best genome from the checkpoint
    Run it against a fixed party configuration
    Print round-by-round action logs — what action was taken, positions before and after, HP changes
    Look for signs of learned behaviour: does the AI move toward enemies when far away, does it attack when adjacent, does it avoid wasting moves

Hour 8 — review and next steps (if time allows):
    Run the balance checker concept manually — same genome against two different party configurations, compare win rates
    Note what the AI does well and what looks wrong — this becomes your bug and improvement list for after these three days
    Document the state vector format, action ID encoding, and reward function in comments or a README while it’s fresh

End of day goal: NEAT trains successfully over 300 generations, fitness improves meaningfully, saved genome produces observable non-random behaviour in evaluation.