from . import window
from . import event
from . import time

class Clock:
    def tick(frame_rate: int = 0) -> float:
        """Get the time since the last frame in seconds and optionally cap the framerate"""
        ...
        
    def get_fps() -> int:
        """Get the current frames per second of the program"""
        ...
        
def init() -> None:
    """Initialize Kraken Engine"""
    ...

def quit() -> None:
    """Quit Kraken Engine"""
    ...
