import sys
import logging

from tritondse import CompositeData
from tritondse import Config
from tritondse import CoverageStrategy
from tritondse import ProcessState
from tritondse import Program
from tritondse import Seed
from tritondse import SeedFormat
from tritondse import SymbolicExecutor
from tritondse import SymbolicExplorator


logging.basicConfig(level=logging.INFO)


def pre_exec_hook(se: SymbolicExecutor, state: ProcessState):
    logging.info(f"[PRE-EXEC] Processing seed: {se.seed.hash}, \
                    ({repr(se.seed.content.files)})")
    with open("out/" + se.seed.hash, 'wb') as file:
        file.write(se.seed.content.files["stdin"] )


# Load the program (LIEF-based program loader).
prog = Program(sys.argv[1])

# Load the configuration.
config = Config(coverage_strategy=CoverageStrategy.PATH, debug=True,
                pipe_stdout=True, seed_format=SeedFormat.COMPOSITE)

# Create an instance of the Symbolic Explorator
dse = SymbolicExplorator(config, prog)

# Create a starting seed, representing argv.
seed = Seed(CompositeData(files={"stdin": b"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"}))

# Add seed to the worklist.
dse.add_input_seed(seed)

# Add callbacks.
dse.callback_manager.register_pre_execution_callback(pre_exec_hook)

# Start exploration!
dse.explore()
