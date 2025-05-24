def get_elapsed_time() -> float:
    """Get the elapsed time since the program started, in seconds"""
    ...
    
def delay(ms: int) -> None:
    """
    Delay the program by a duration in milliseconds
    """
    ...
    
class Clock:
    """
    A class to manage time in the game loop.
    """
    
    def tick(frame_rate: int = 0) -> float:
        """
        Get the time since the last frame in seconds and optionally cap the framerate
        """
        ...
        
    def get_fps() -> int:
        """
        Get the current frames per second of the program
        """
        ...