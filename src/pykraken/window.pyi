def create(resolution: tuple[int, int], title: str, scaled: bool) -> None:
    """Create a window"""
    ...

def is_open() -> bool:
    """Check if the window is open"""
    ...
    
def close() -> None:
    """Close the window"""
    ...
    
def clear(color: tuple[int, int, int] = (0, 0, 0)) -> None:
    """Clear the screen"""
    ...
    
def flip() -> None:
    """Flip the render buffer"""
    ...
    