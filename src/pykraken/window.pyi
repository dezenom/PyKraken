from typing import Sequence

def create(resolution: Sequence[int, int], title: str, scaled: bool) -> None:
    """Create a window"""
    ...

def is_open() -> bool:
    """Check if the window is open"""
    ...
    
def close() -> None:
    """Close the window"""
    ...
    
def clear(color: Sequence[int, int, int] = (0, 0, 0)) -> None:
    """Clear the screen"""
    ...
    
def flip() -> None:
    """Flip the render buffer"""
    ...
    