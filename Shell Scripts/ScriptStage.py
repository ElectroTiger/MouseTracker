import os

class ScriptStages:
    """Class used to assist running a script in modular stages, saving the previous stage to a file."""
    def __init__(self, stage_filepath, start_stage=-1, end_stage=-1):
        # Initialize Instance Variables
        self.stage_filepath = stage_filepath
        self.current_stage = start_stage
        self.end_stage = end_stage

        # If the stage log directory does not exist, create it.
        if not os.path.exists(os.path.dirname(stage_filepath)):
            os.makedirs(os.path.dirname(stage_filepath))
        # If it does exist, look for the stage file, unless a manual start stage is specified.
        else:
            if start_stage == -1:
                # If it exists, read the stage number from the file.
                if os.path.exists(stage_filepath):
                    with open(stage_filepath) as stage_file:
                        self.current_stage = int(stage_file.read(1))  # TODO: Error check for invalid stage_file contents
                        print("Found stage number", self.current_stage)
                else:
                    self.current_stage = 1

    def stage(self, stage_num, stage_string):
        parent = self

        class StageInner:
            def __init__(self, stage_num, stage_string):
                self.stage_num = stage_num
                self.stage_string = stage_string
                self.should_run = True
                if parent.end_stage == -1:
                    self.should_run = self.stage_num == parent.current_stage
                else:
                    self.should_run = self.stage_num == parent.current_stage and self.stage_num <= parent.end_stage

            def __enter__(self):
                if self.should_run:
                    print("Stage ", self.stage_num, ": ", self.stage_string)
                    # Open file to log stage number
                    with open(parent.stage_filepath, "w") as stage_file:
                        stage_file.truncate()
                        stage_file.write(str(self.stage_num))
                else:
                    print("Skipping Stage ", self.stage_num)
                # Return whether the stage should be run
                return self.should_run

            def __exit__(self, *args):
                # Open file to log updated stage number.
                if (self.should_run):
                    parent.current_stage += 1
                    with open(parent.stage_filepath, "w") as stage_file:
                        stage_file.truncate()
                        stage_file.write(str(parent.current_stage))

        return StageInner(stage_num, stage_string)

